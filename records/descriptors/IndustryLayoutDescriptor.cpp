///////////////////////////////////////////////////////////////////////////////
// Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)
//
// This file is part of yagl.
//
// yagl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// yagl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with yagl. If not, see <https://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#include "IndustryLayoutDescriptor.h"
#include "StreamHelpers.h"
#include "EnumDescriptor.h"


namespace {


constexpr const char* str_reference    = "layout_reference";
constexpr const char* str_industry_num = "industry_num";
constexpr const char* str_layout_num   = "layout_num";
constexpr const char* str_layout       = "layout";
constexpr const char* str_clearance    = "clearance";
constexpr const char* str_new_tile     = "new_tile";
constexpr const char* str_old_tile     = "old_tile";


const EnumDescriptorT<IndustryTile::Type> desc_type 
{ 
    0x03, "type", 
    {
        { static_cast<uint8_t>(IndustryTile::Type::OldTile),   str_old_tile },       
        { static_cast<uint8_t>(IndustryTile::Type::NewTile),   str_new_tile },  
        { static_cast<uint8_t>(IndustryTile::Type::Clearance), str_clearance }, 
    }
};


// Used to determine type of layout when parsing.
const std::map<std::string, uint16_t> g_indices =
{
    { str_reference, 0x01 },
    { str_layout,    0x02 },
};


} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool IndustryTile::read(std::istream& is)
{
    m_x_off = read_uint8(is);
    m_y_off = read_uint8(is);
 
    // List termination.
    // Nasty! Sign extension of the signed int8 causes test against 0x80 to fail.
    if (m_x_off == 0x00 && uint8_t(m_y_off) == 0x80)
        return true;
 
    m_tile = read_uint8(is); 
    switch (m_tile)
    {
        case 0xFF:
            m_type = Type::Clearance;
            break;
        case 0xFE:
            m_type = Type::NewTile;
            m_tile = read_uint16(is);
            break;
        default:
            m_type = Type::OldTile;
    }

    return false;
}


void IndustryTile::write(std::ostream& os) const
{
    write_uint8(os, m_x_off);
    write_uint8(os, m_y_off);
 
    switch (m_type)
    {
        case Type::Clearance:
            write_uint8(os, 0xFF);
            break;
        case Type::NewTile:
            write_uint8(os, 0xFE);
            write_uint16(os, m_tile);
            break;
        case Type::OldTile:
            write_uint8(os, uint8_t(m_tile));
    }
}


void IndustryTile::print(std::ostream& os, uint16_t indent) const
{
    switch (m_type)
    {
        case Type::Clearance: 
            os << pad(indent) << str_clearance << "("; 
            os << int16_t(m_x_off) << ", " << int16_t(m_y_off) << ");\n"; 
            break; 
        case Type::NewTile:   
            os << pad(indent) << str_new_tile << "(";
            os << int16_t(m_x_off) << ", " << int16_t(m_y_off) << ", " << to_hex(m_tile) << ");\n"; 
            break;
        case Type::OldTile:          
            os << pad(indent) << str_old_tile << "(";
            os << int16_t(m_x_off) << ", " << int16_t(m_y_off) << ", " << to_hex(m_tile) << ");\n"; 
            break;
    }
}


void IndustryTile::parse(TokenStream& is)
{
    desc_type.parse(m_type, is);
    is.match(TokenType::OpenParen);

    switch (m_type)
    {
        case Type::Clearance:
            m_x_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            m_y_off = is.match_uint8(); 
            break;

        case Type::NewTile:   
        case Type::OldTile:          
            m_x_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            m_y_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            m_tile = is.match_uint16(); 
            break;
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void IndustryLayout::read(std::istream& is)
{
    m_is_reference = (is.peek() == 0xFE);
    if (m_is_reference)
    {
        read_uint8(is);
        m_industry_num = read_uint8(is);
        m_layout_num   = read_uint8(is);
    }
    else
    {
        while (true)
        {
            IndustryTile tile{};
            bool is_terminator = tile.read(is);
            if (is_terminator)
                break;
            m_tiles.push_back(tile);
        }
    }
}


void IndustryLayout::write(std::ostream& os) const
{
    if (m_is_reference)
    {
        write_uint8(os, 0xFE);
        write_uint8(os, m_industry_num);
        write_uint8(os, m_layout_num);
    }
    else
    {
        for (const auto& tile: m_tiles)
        {
            tile.write(os);
        }

        // Write the terminator.
        write_uint8(os, 0x00);
        write_uint8(os, 0x80);
    }
}


void IndustryLayout::print(std::ostream& os, uint16_t indent) const
{
    if (m_is_reference)
    {
        os << pad(indent) << str_reference << "(" << to_hex(m_industry_num) << ", " << to_hex(m_layout_num) << ");\n";
    }
    else
    {
        os << pad(indent) << str_layout << "\n"; 
        os << pad(indent) << "{\n"; 

        for (const auto& tile: m_tiles)
        {
            tile.print(os, indent + 4);
        }

        os << pad(indent) << "}\n"; 
    }
}


void IndustryLayout::parse(TokenStream& is)
{
    std::string name = is.match(TokenType::Ident);

    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        switch (index)
        {
            case 0x01: 
                m_is_reference = true;
                is.match(TokenType::OpenParen);
                m_industry_num = is.match_uint8();
                is.match(TokenType::Comma);
                m_layout_num = is.match_uint8();
                is.match(TokenType::CloseParen);
                is.match(TokenType::SemiColon);
                break;
            
            case 0x02: 
                m_is_reference = false;
                is.match(TokenType::OpenBrace);
                while (is.peek().type != TokenType::CloseBrace)
                {
                    IndustryTile tile{};
                    tile.parse(is);
                    m_tiles.push_back(tile);
                }
                is.match(TokenType::CloseBrace);
                break;
            
            default:   
                throw RUNTIME_ERROR("Unknown layout type");
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void IndustryLayouts::read(std::istream& is)
{
    uint8_t num_layouts = read_uint8(is);

    // Size not used for anything.
    read_uint32(is);

    m_layouts.resize(num_layouts);
    for (uint8_t i = 0; i < num_layouts; ++i)
    {
        m_layouts[i].read(is);
    }
}


void IndustryLayouts::write(std::ostream& os) const
{
    write_uint8(os, uint8_t(m_layouts.size()));

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: m_layouts)
    {
        layout.write(ss);
    }
    write_uint32(os, uint32_t(ss.str().length()));
    
    for (const auto& layout: m_layouts)
    {
        layout.write(os);
    }
}


void IndustryLayouts::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "{\n"; 
    for (const auto& layout: m_layouts)
    {
        layout.print(os, indent + 4);
    }

    os << pad(indent) << "}"; 
}


void IndustryLayouts::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        IndustryLayout layout = {};
        layout.parse(is);
        m_layouts.push_back(layout);
    }

    is.match(TokenType::CloseBrace);
}


