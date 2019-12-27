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
#include "Action00Airports.h"
#include "Descriptors.h"
#include "StreamHelpers.h"


namespace {


struct AirportLayoutsDescriptor : PropertyDescriptor
{
    void print(const Action00Airports::AirportLayouts& layouts, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Airports::AirportLayouts& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


constexpr const char* str_sprite_id               = "sprite_id";
constexpr const char* str_airport_override_id     = "airport_override_id";
constexpr const char* str_airport_layouts         = "airport_layouts";
constexpr const char* str_first_year_available    = "first_year_available";
constexpr const char* str_last_year_available     = "last_year_available";
constexpr const char* str_compatible_ttd_airport  = "compatible_ttd_airport";
constexpr const char* str_catchment_area          = "catchment_area";
constexpr const char* str_noise_level             = "noise_level";
constexpr const char* str_airport_name_id         = "airport_name_id";
constexpr const char* str_maintenance_cost_factor = "maintenance_cost_factor";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_airport_override_id,       0x08'00 },
    { str_airport_layouts,           0x0A'00 },
    { str_first_year_available,      0x0C'00 },
    { str_last_year_available,       0x0C'01 },
    { str_compatible_ttd_airport,    0x0D'00 },
    { str_catchment_area,            0x0E'00 },
    { str_noise_level,               0x0F'00 },
    { str_airport_name_id,           0x10'00 },
    { str_maintenance_cost_factor,   0x11'00 },
};


constexpr IntegerDescriptorT<uint8_t>   desc_08  = { 0x08, str_airport_override_id,        PropFormat::Hex };
constexpr AirportLayoutsDescriptor      desc_0A  = { 0x0A, str_airport_layouts };
constexpr IntegerDescriptorT<uint16_t>  desc_0C0 = { 0x0C, str_first_year_available,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t>  desc_0C1 = { 0x0C, str_last_year_available,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>   desc_0D  = { 0x0D, str_compatible_ttd_airport,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>   desc_0E  = { 0x0E, str_catchment_area,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>   desc_0F  = { 0x0F, str_noise_level,                PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t>  desc_10  = { 0x10, str_airport_name_id,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t>  desc_11  = { 0x11, str_maintenance_cost_factor,    PropFormat::Hex };


} // namespace {


// This is the same as an industry tile. I think.
void Action00Airports::AirportTile::read(std::istream& is)
{
    x_off = read_uint8(is);
    y_off = read_uint8(is);
 
    // List termination.
    // Nasty! Sign extension of the signed int8 causes test against 0x80 to fail.
    if (x_off == 0x00 && uint8_t(y_off) == 0x80)
        return;
 
    tile = read_uint8(is); 
    type = static_cast<Type>(tile);
    // TODO assert in enum.
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


void Action00Airports::AirportTile::write(std::ostream& os) const
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


void Action00Airports::AirportTile::print(std::ostream& os, uint16_t indent) const
{
    switch (type)
    {
        case Type::Clearance: 
            os << pad(indent) << "clearance(" << x_off << ", " << y_off << ");\n"; 
            break; 
        case Type::NewTile:   
            os << pad(indent) << "new_tile(" << x_off << ", " << y_off << ", " << to_hex(tile, true) << ");\n"; 
            break;
        case Type::OldTile:
            os << pad(indent) << "old_tile(" << x_off << ", " << y_off << ", " << to_hex(tile, true) << ");\n"; 
            break;
    }
}


void Action00Airports::AirportLayout::read(std::istream& is)
{
    rotation = static_cast<Rotation>(read_uint8(is));
    // TODO assert in enumeration.
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


void Action00Airports::AirportLayout::write(std::ostream& os) const
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


void Action00Airports::AirportLayout::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "layout<" << rotation_name(rotation) << ">\n"; 
    os << pad(indent) << "{\n"; 
    os << pad(indent) << "}\n"; 

    for (const auto& tile: tiles)
    {
        tile.print(os, indent + 4);
    }
}


const char* Action00Airports::AirportLayout::rotation_name(Rotation rotation) const
{
    switch (rotation)
    {
        case Rotation::North: return "North";
        case Rotation::South: return "South";
        case Rotation::East:  return "East";
        case Rotation::West:  return "West";
    }
}


void Action00Airports::AirportLayouts::read(std::istream& is)
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


void Action00Airports::AirportLayouts::write(std::ostream& os) const
{
    write_uint8(os, layouts.size());

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: layouts)
    {
        layout.write(ss);
    }

    write_uint32(os, ss.str().length());
    for (const auto& layout: layouts)
    {
        layout.write(os);
    }
}


void Action00Airports::AirportLayouts::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "layouts\n"; 
    os << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4);
    }
    os << pad(indent) << "};\n"; 
}


void Action00Airports::AirportLayouts::parse(TokenStream& is)
{
    throw RUNTIME_ERROR("Action00Airports::AirportLayouts::parse not implemented");
}


bool Action00Airports::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_airport_override_id     = read_uint8(is); break;
        case 0x0A: m_0A_airport_layouts.read(is); break;
        case 0x0C: m_0C_first_year_available    = read_uint16(is);
                   m_0C_last_year_available     = read_uint16(is); break;
        case 0x0D: m_0D_compatible_ttd_airport  = read_uint8(is); break;
        case 0x0E: m_0E_catchment_area          = read_uint8(is); break;
        case 0x0F: m_0F_noise_level             = read_uint8(is); break;
        case 0x10: m_10_airport_name_id         = read_uint16(is); break;
        case 0x11: m_11_maintenance_cost_factor = read_uint16(is); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}   


bool Action00Airports::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint8(os,  m_08_airport_override_id); break;
        case 0x0A: m_0A_airport_layouts.write(os); break;
        case 0x0C: write_uint16(os, m_0C_first_year_available);
                   write_uint16(os, m_0C_last_year_available); break;
        case 0x0D: write_uint8(os,  m_0D_compatible_ttd_airport); break;
        case 0x0E: write_uint8(os,  m_0E_catchment_area); break;
        case 0x0F: write_uint8(os,  m_0F_noise_level); break;
        case 0x10: write_uint16(os, m_10_airport_name_id); break;
        case 0x11: write_uint16(os, m_11_maintenance_cost_factor); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}


bool Action00Airports::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_airport_override_id, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_airport_layouts, os, indent); break;
        case 0x0C: desc_0C0.print(m_0C_first_year_available, os, indent); os << "\n";
                   desc_0C1.print(m_0C_last_year_available, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_compatible_ttd_airport, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_catchment_area, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_noise_level, os, indent); break;
        case 0x10: desc_10.print(m_10_airport_name_id, os, indent); break;
        case 0x11: desc_11.print(m_11_maintenance_cost_factor, os, indent); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    os << "\n";
    return true;
}


bool Action00Airports::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_airport_override_id, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_airport_layouts, is); break;
            case 0x0C'00: desc_0C0.parse(m_0C_first_year_available, is); break;
            case 0x0C'01: desc_0C1.parse(m_0C_last_year_available, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_compatible_ttd_airport, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_catchment_area, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_noise_level, is); break;
            case 0x10'00: desc_10.parse(m_10_airport_name_id, is); break;
            case 0x11'00: desc_11.parse(m_11_maintenance_cost_factor, is); break;
            default:      throw RUNTIME_ERROR("Unknown property");
        }

        return true;
    }

    throw RUNTIME_ERROR("Unknown property");
}
