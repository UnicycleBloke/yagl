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
#include "AirportLayoutDescriptor.h"
#include "StreamHelpers.h"
#include "EnumDescriptor.h"


namespace {


constexpr const char* str_layout    = "layout";
constexpr const char* str_clearance = "clearance";
constexpr const char* str_new_tile  = "new_tile";
constexpr const char* str_old_tile  = "old_tile";


const EnumDescriptorT<AirportTile::Type> desc_type 
{ 
    0x03, "type", 
    {
        { static_cast<uint8_t>(AirportTile::Type::OldTile),   str_old_tile },       
        { static_cast<uint8_t>(AirportTile::Type::NewTile),   str_new_tile },  
        { static_cast<uint8_t>(AirportTile::Type::Clearance), str_clearance }, 
    }
};


using Rotation = AirportLayout::Rotation;
const EnumDescriptorT<Rotation> desc_rotation 
{
    0x00, "rotation", 
    {
        { static_cast<uint8_t>(Rotation::North), "North" },
        { static_cast<uint8_t>(Rotation::South), "South" },
        { static_cast<uint8_t>(Rotation::East),  "East" },
        { static_cast<uint8_t>(Rotation::West),  "West" },
    }    
};




} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AirportTile::read(std::istream& is)
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
            break;
        default:
            type = Type::OldTile;
    }
}


void AirportTile::write(std::ostream& os) const
{
    write_uint8(os, x_off);
    write_uint8(os, y_off);
 
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


void AirportTile::print(std::ostream& os, uint16_t indent) const
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


void AirportTile::parse(TokenStream& is)
{
    desc_type.parse(type, is);
    is.match(TokenType::OpenParen);

    switch (type)
    {
        case Type::Clearance:
            x_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            y_off = is.match_uint8(); 
            break;

        case Type::NewTile:   
        case Type::OldTile:          
            x_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            y_off = is.match_uint8(); 
            is.match(TokenType::Comma);
            tile = is.match_uint16(); 
            break;
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// The layout consists of a single byte giving the rotation of the layout
// (0: north, 2 : east, 4 : south, 6 : west) followed by a list of the above tile 
// definitions, terminated by two bytes : 0, 80h 
void AirportLayout::read(std::istream& is)
{
    rotation = static_cast<Rotation>(read_uint8(is));

    while (true)
    {
        AirportTile tile;
        tile.read(is);
        if (tile.x_off == 0x00 && uint8_t(tile.y_off) == 0x80)
            break;
        tiles.push_back(tile);
    }
}


void AirportLayout::write(std::ostream& os) const
{
    write_uint8(os, static_cast<uint8_t>(rotation));

    for (const auto& tile: tiles)
    {
        tile.write(os);
    }

    // Write the terminator.
    write_uint8(os, 0x00);
    write_uint8(os, 0x80);
}


void AirportLayout::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_layout << "<" << desc_rotation.value(rotation) << ">\n";    
    os << pad(indent) << "{\n"; 

    for (const auto& tile: tiles)
    {
        tile.print(os, indent + 4);
    }

    os << pad(indent) << "}\n"; 
}


void AirportLayout::parse(TokenStream& is)
{
    is.match_ident(str_layout);
    is.match(TokenType::OpenAngle);
    desc_rotation.parse(rotation, is);
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        AirportTile tile{};
        tile.parse(is);
        tiles.push_back(tile);
    }
    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AirportLayouts::read(std::istream& is)
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


void AirportLayouts::write(std::ostream& os) const
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


void AirportLayouts::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4);
    }

    os << pad(indent) << "}"; 
}


void AirportLayouts::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        AirportLayout layout;
        layout.parse(is);
        layouts.push_back(layout);
    }
    
    is.match(TokenType::CloseBrace);
}


