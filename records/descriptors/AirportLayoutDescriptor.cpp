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
    type = static_cast<Type>(tile);
    switch (type)
    {
        case Type::Clearance:
            break;
        case Type::NewTile:
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
            write_uint8(os, tile);
    }
}


void AirportTile::print(std::ostream& os, uint16_t indent) const
{
    switch (type)
    {
        case Type::Clearance: 
            os << pad(indent) << "clearance("; 
            os << to_hex(x_off, true) << ", " << to_hex(y_off, true) << ");\n"; 
            break; 
        case Type::NewTile:   
            os << pad(indent) << "new_tile(";
            os << to_hex(x_off, true) << ", " << to_hex(y_off, true) << ", " << to_hex(tile, true) << ");\n"; 
            break;
        case Type::OldTile:          
            os << pad(indent) << "old_tile(";
            os << to_hex(x_off, true) << ", " << to_hex(y_off, true) << ", " << to_hex(tile, true) << ");\n"; 
            break;
    }
}


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
        os << pad(indent) << "layout<" << rotation_name(rotation) << ">\n"; 
    }
    else
    {
        os << pad(indent) << "layout\n"; 
    }
    
    os << pad(indent) << "{\n"; 

    for (const auto& tile: tiles)
    {
        tile.print(os, indent + 4);
    }

    os << pad(indent) << "}\n"; 
}


const char* AirportLayout::rotation_name(Rotation rotation) const
{
    switch (rotation)
    {
        case Rotation::North: return "North";
        case Rotation::South: return "South";
        case Rotation::East:  return "East";
        case Rotation::West:  return "West";
    }

    // Should never get here.
    return "<unknown>";
}


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
    write_uint8(os, layouts.size());

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: layouts)
    {
        layout.write(ss, type);
    }

    write_uint32(os, ss.str().length());
    for (const auto& layout: layouts)
    {
        layout.write(os, type);
    }
}


void AirportLayouts::print(std::ostream& os, uint16_t indent, AirportType type) const
{
    os << pad(indent) << "layouts\n"; 
    os << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4, type);
    }
    os << pad(indent) << "};\n"; 
}


void AirportLayouts::parse(TokenStream& is, AirportType type)
{
    throw RUNTIME_ERROR("AirportLayouts::parse not implemented");
}


