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
#include "Descriptors.h"


namespace {


constexpr const char* str_reference    = "reference";
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
        { 0, str_old_tile },  // OldTile     
        { 1, str_new_tile },  // NewTile
        { 2, str_clearance }, // Clearance
    }
};


// Used to determine type of layout when parsing.
const std::map<std::string, uint16_t> g_indices =
{
    { str_reference, 0x01 },
    { str_layout,    0x02 },
};


} // namespace {


// This is the same as an industry tile. I think.
void IndustryTile::read(std::istream& is)
{
    x_off = read_uint8(is);
    y_off = read_uint8(is);
 
    // List termination.
    // Nasty! Sign extension of the signed int8 causes test against 0x80 to fail.
    if (x_off == 0x00 && uint8_t(y_off) == 0x80)
        return;
 
    tile = read_uint8(is); 
    switch (tile)
    {
        case 0xFF:
            type = Type::Clearance;
            break;
        case 0xFE:
            type = Type::NewTile;
            tile = read_uint16(is);
            // TODO offset adjustment required depending on GRF version.
            break;
        default:
            type = Type::OldTile;
    }
}


void IndustryTile::write(std::ostream& os) const
{
    write_uint8(os, x_off);
    write_uint8(os, y_off);
 
    //if (x_off == 0x00 && uint8_t(y_off) == 0x80)
    //    return;
 
    switch (type)
    {
        case Type::Clearance:
            write_uint8(os, 0xFF);
            break;
        case Type::NewTile:
            write_uint8(os, 0xFE);
            write_uint16(os, tile);
            break;
        case Type::OldTile:
            write_uint8(os, uint8_t(tile));
    }
}


void IndustryTile::print(std::ostream& os, uint16_t indent) const
{
    switch (type)
    {
        case Type::Clearance: 
            os << pad(indent) << str_clearance << "("; 
            os << int16_t(x_off) << ", " << int16_t(y_off) << ");\n"; 
            break; 
        case Type::NewTile:   
            os << pad(indent) << str_new_tile << "(";
            os << int16_t(x_off) << ", " << int16_t(y_off) << ", " << to_hex(tile) << ");\n"; 
            break;
        case Type::OldTile:          
            os << pad(indent) << str_old_tile << "(";
            os << int16_t(x_off) << ", " << int16_t(y_off) << ", " << to_hex(tile) << ");\n"; 
            break;
    }
}


void IndustryTile::parse(TokenStream& is)
{
    // Enum descriptor needed...
    desc_type.parse(type, is);
    is.match(TokenType::OpenParen);

    switch (type)
    {
        case Type::Clearance:
            x_off = is.match_integer(); 
            is.match(TokenType::Comma);
            y_off = is.match_integer(); 
            break;

        case Type::NewTile:   
        case Type::OldTile:          
            x_off = is.match_integer(); 
            is.match(TokenType::Comma);
            y_off = is.match_integer(); 
            is.match(TokenType::Comma);
            tile = is.match_integer(); 
            break;
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


void IndustryLayout::read(std::istream& is)
{
    is_reference = (is.peek() == 0xFE);
    if (is_reference)
    {
        read_uint8(is);
        uint8_t industry_num = read_uint8(is);
        uint8_t layout_num   = read_uint8(is);
    }
    else
    {
        while (true)
        {
            IndustryTile tile;
            tile.read(is);
            // This is the terminator.
            if (tile.x_off == 0x00 && uint8_t(tile.y_off) == 0x80)
                break;
            tiles.push_back(tile);
        }
    }
}


void IndustryLayout::write(std::ostream& os) const
{
    if (is_reference)
    {
        write_uint8(os, 0xFE);
        write_uint8(os, industry_num);
        write_uint8(os, layout_num);
    }
    else
    {
        for (const auto& tile: tiles)
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
    if (is_reference)
    {
        os << pad(indent) << str_reference << "{ " << to_hex(industry_num) << ", " << to_hex(layout_num) << " }\n";
    }
    else
    {
        os << pad(indent) << str_layout << "\n"; 
        os << pad(indent) << "{\n"; 

        for (const auto& tile: tiles)
        {
            tile.print(os, indent + 4);
        }

        os << pad(indent) << "}\n"; 
    }
}


void IndustryLayout::parse(TokenStream& is)
{
    std::string name = is.match(TokenType::Ident);
    is.match(TokenType::OpenBrace);

    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        switch (index)
        {
            case 0x01: 
                industry_num = is.match_integer();
                is.match(TokenType::Comma);
                layout_num = is.match_integer();
                break;
            
            case 0x02: 
                while (is.peek().type != TokenType::CloseBrace)
                {
                    IndustryTile tile;
                    tile.parse(is);
                    tiles.push_back(tile);
                }
                break;
            
            default:   
                throw RUNTIME_ERROR("Unknown layout type");
        }
    }

    is.match(TokenType::CloseBrace);
}


void IndustryLayouts::read(std::istream& is)
{
    uint8_t num_layouts = read_uint8(is);
    // Size not used for anything.
    read_uint32(is);
    layouts.resize(num_layouts);
    for (uint8_t i = 0; i < num_layouts; ++i)
    {
        layouts[i].read(is);
    }
}


void IndustryLayouts::write(std::ostream& os) const
{
    write_uint8(os, uint8_t(layouts.size()));

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: layouts)
    {
        layout.write(ss);
    }

    write_uint32(os, uint32_t(ss.str().length()));
    for (const auto& layout: layouts)
    {
        layout.write(os);
    }
}


void IndustryLayouts::print(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << "layouts\n"; 
    os << "\n" << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4);
    }

    os << pad(indent) << "};\n"; 
}


void IndustryLayouts::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        IndustryLayout layout;
        layout.parse(is);
        layouts.push_back(layout);
    }

    is.match(TokenType::CloseBrace);
}


