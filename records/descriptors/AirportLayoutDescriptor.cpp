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

constexpr const char* str_layout    = "layout";
constexpr const char* str_clearance = "clearance";
constexpr const char* str_new_tile  = "new_tile";
constexpr const char* str_old_tile  = "old_tile";

} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This is the same as an industry tile. I think.
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


void AirportTile::print(std::ostream& os, uint16_t indent) const
{
    switch (type)
    {
        case Type::Clearance: 
            os << pad(indent) << str_clearance << "("; 
            os << to_hex(x_off) << ", " << to_hex(y_off) << ");\n"; 
            break; 
        case Type::NewTile:   
            os << pad(indent) << str_new_tile << "(";
            os << to_hex(x_off) << ", " << to_hex(y_off) << ", " << to_hex(tile) << ");\n"; 
            break;
        case Type::OldTile:          
            os << pad(indent) << str_old_tile << "(";
            os << to_hex(x_off) << ", " << to_hex(y_off) << ", " << to_hex(tile) << ");\n"; 
            break;
    }
}


void AirportTile::parse(TokenStream& is)
{
    std::string ident = is.match(TokenType::Ident);
    is.match(TokenType::OpenParen);
    if (ident == str_clearance)
    {
        type = Type::Clearance;
        x_off = is.match_uint8();
        is.match(TokenType::Comma);
        y_off = is.match_uint8();
    }
    else if (ident == str_new_tile)
    {
        type = Type::NewTile;
        x_off = is.match_uint8();
        is.match(TokenType::Comma);
        y_off = is.match_uint8();
        is.match(TokenType::Comma);
        tile  = is.match_uint16();

    }
    else if (ident == str_old_tile)
    {
        type = Type::OldTile;
        x_off = is.match_uint8();
        is.match(TokenType::Comma);
        y_off = is.match_uint8();
        is.match(TokenType::Comma);
        tile  = is.match_uint16();
    }
    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AirportLayout::read(std::istream& is, AirportType type)
{
    if (type == AirportType::Airport)
    {
        rotation = static_cast<Rotation>(read_uint8(is));
    }
    
    while (true)
    {
        AirportTile tile;
        tile.read(is);
        // This is the terminator.
        if (tile.x_off == 0x00 && uint8_t(tile.y_off) == 0x80)
            break;
        tiles.push_back(tile);
    }
}


void AirportLayout::write(std::ostream& os, AirportType type) const
{
    if (type == AirportType::Airport)
    {
        write_uint8(os, static_cast<uint8_t>(rotation));
    }

    for (const auto& tile: tiles)
    {
        tile.write(os);
    }
    // Write the terminator.
    write_uint8(os, 0x00);
    write_uint8(os, 0x80);
}


void AirportLayout::print(std::ostream& os, uint16_t indent, AirportType type) const
{
    if (type == AirportType::Airport)
    {
        os << pad(indent) << str_layout << "<" << desc_rotation.value(rotation) << ">\n"; 
    }
    else
    {
        os << pad(indent) << str_layout << "\n"; 
    }
    
    os << pad(indent) << "{\n"; 
    for (const auto& tile: tiles)
    {
        tile.print(os, indent + 4);
    }
    os << pad(indent) << "}\n"; 
}


void AirportLayout::parse(TokenStream& is, AirportType type)
{
    is.match_ident(str_layout);
    if (type == AirportType::Airport)
    {
        is.match(TokenType::OpenAngle);
        desc_rotation.parse(rotation, is);
        is.match(TokenType::CloseAngle);
    }

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        AirportTile tile;
        tile.parse(is);
        tiles.push_back(tile);
    }
    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AirportLayouts::read(std::istream& is, AirportType type)
{
    uint8_t num_layouts = read_uint8(is);

    // Size not used for anything.
    read_uint32(is);
    
    layouts.resize(num_layouts);
    for (uint8_t i = 0; i < num_layouts; ++i)
    {
        layouts[i].read(is, type);
    }
}


void AirportLayouts::write(std::ostream& os, AirportType type) const
{
    write_uint8(os, uint8_t(layouts.size()));

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: layouts)
    {
        layout.write(ss, type);
    }
    write_uint32(os, uint32_t(ss.str().length()));
    
    for (const auto& layout: layouts)
    {
        layout.write(os, type);
    }
}


void AirportLayouts::print(std::ostream& os, uint16_t indent, AirportType type) const
{
    os << "\n" << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4, type);
    }
    os << pad(indent) << "}"; 
}


void AirportLayouts::parse(TokenStream& is, AirportType type)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        AirportLayout layout;
        layout.parse(is, type);
        layouts.push_back(layout);
    }
    is.match(TokenType::CloseBrace);
}


