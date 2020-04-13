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
#include "Action00IndustryTiles.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


namespace {


constexpr const char* str_substitute_building_id = "substitute_building_id";
constexpr const char* str_industry_tile_override = "industry_tile_override";
constexpr const char* str_tile_acceptance1       = "tile_acceptance1";
constexpr const char* str_tile_acceptance2       = "tile_acceptance2";
constexpr const char* str_tile_acceptance3       = "tile_acceptance3";
constexpr const char* str_land_shape_flags       = "land_shape_flags";
constexpr const char* str_callback_flags         = "callback_flags";
constexpr const char* str_animation_info         = "animation_info";
constexpr const char* str_animation_speed        = "animation_speed";
constexpr const char* str_callback_25_triggers   = "callback_25_triggers";
constexpr const char* str_special_flags          = "special_flags";
constexpr const char* str_cargo_acceptance_list  = "cargo_acceptance_list";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_substitute_building_id, 0x08 },
    { str_industry_tile_override, 0x09 },
    { str_tile_acceptance1,       0x0A },
    { str_tile_acceptance2,       0x0B },
    { str_tile_acceptance3,       0x0C },
    { str_land_shape_flags,       0x0D },
    { str_callback_flags,         0x0E },
    { str_animation_info,         0x0F },
    { str_animation_speed,        0x10 },
    { str_callback_25_triggers,   0x11 },
    { str_special_flags,          0x12 },
    { str_cargo_acceptance_list,  0x13 },
};


constexpr UInt8Descriptor            desc_08  = { 0x08, str_substitute_building_id, UIntFormat::Hex };
constexpr UInt8Descriptor            desc_09  = { 0x09, str_industry_tile_override, UIntFormat::Hex };
constexpr UInt16Descriptor           desc_0A  = { 0x0A, str_tile_acceptance1,       UIntFormat::Hex };
constexpr UInt16Descriptor           desc_0B  = { 0x0B, str_tile_acceptance2,       UIntFormat::Hex };
constexpr UInt16Descriptor           desc_0C  = { 0x0C, str_tile_acceptance3,       UIntFormat::Hex };
constexpr UInt8Descriptor            desc_0D  = { 0x0D, str_land_shape_flags,       UIntFormat::Hex };
constexpr UInt8Descriptor            desc_0E  = { 0x0E, str_callback_flags,         UIntFormat::Hex };
constexpr UInt16Descriptor           desc_0F  = { 0x0F, str_animation_info,         UIntFormat::Hex };
constexpr UInt8Descriptor            desc_10  = { 0x10, str_animation_speed,        UIntFormat::Hex };
constexpr UInt8Descriptor            desc_11  = { 0x11, str_callback_25_triggers,   UIntFormat::Hex };
constexpr UInt8Descriptor            desc_12  = { 0x12, str_special_flags,          UIntFormat::Hex };
constexpr CargoAcceptanceDescriptor  desc_13  = { 0x13, str_cargo_acceptance_list };


} // namespace {


bool Action00IndustryTiles::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_substitute_building_id.read(is); break;
        case 0x09: m_09_industry_tile_override.read(is); break;
        case 0x0A: m_0A_tile_acceptance1.read(is);       break;
        case 0x0B: m_0B_tile_acceptance2.read(is);       break;
        case 0x0C: m_0C_tile_acceptance3.read(is);       break;
        case 0x0D: m_0D_land_shape_flags.read(is);       break;
        case 0x0E: m_0E_callback_flags.read(is);         break;
        case 0x0F: m_0F_animation_info.read(is);         break;
        case 0x10: m_10_animation_speed.read(is);        break;
        case 0x11: m_11_callback_25_triggers.read(is);   break;
        case 0x12: m_12_special_flags.read(is);          break;
        case 0x13: m_13_cargo_acceptance_list.read(is);  break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00IndustryTiles::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_substitute_building_id.write(os); break;
        case 0x09: m_09_industry_tile_override.write(os); break;
        case 0x0A: m_0A_tile_acceptance1.write(os);       break;
        case 0x0B: m_0B_tile_acceptance2.write(os);       break;
        case 0x0C: m_0C_tile_acceptance3.write(os);       break;
        case 0x0D: m_0D_land_shape_flags.write(os);       break;
        case 0x0E: m_0E_callback_flags.write(os);         break;
        case 0x0F: m_0F_animation_info.write(os);         break;
        case 0x10: m_10_animation_speed.write(os);        break;
        case 0x11: m_11_callback_25_triggers.write(os);   break;
        case 0x12: m_12_special_flags.write(os);          break;
        case 0x13: m_13_cargo_acceptance_list.write(os);  break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00IndustryTiles::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_substitute_building_id, os, indent); break;
        case 0x09: desc_09.print(m_09_industry_tile_override, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_tile_acceptance1, os, indent);       break;
        case 0x0B: desc_0B.print(m_0B_tile_acceptance2, os, indent);       break;
        case 0x0C: desc_0C.print(m_0C_tile_acceptance3, os, indent);       break;
        case 0x0D: desc_0D.print(m_0D_land_shape_flags, os, indent);       break;
        case 0x0E: desc_0E.print(m_0E_callback_flags, os, indent);         break;
        case 0x0F: desc_0F.print(m_0F_animation_info, os, indent);         break;
        case 0x10: desc_10.print(m_10_animation_speed, os, indent);        break;
        case 0x11: desc_11.print(m_11_callback_25_triggers, os, indent);   break;
        case 0x12: desc_12.print(m_12_special_flags, os, indent);          break;
        case 0x13: desc_13.print(m_13_cargo_acceptance_list, os, indent);  break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00IndustryTiles::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_substitute_building_id, is); break;
            case 0x09: desc_09.parse(m_09_industry_tile_override, is); break;
            case 0x0A: desc_0A.parse(m_0A_tile_acceptance1, is);       break;
            case 0x0B: desc_0B.parse(m_0B_tile_acceptance2, is);       break;
            case 0x0C: desc_0C.parse(m_0C_tile_acceptance3, is);       break;
            case 0x0D: desc_0D.parse(m_0D_land_shape_flags, is);       break;
            case 0x0E: desc_0E.parse(m_0E_callback_flags, is);         break;
            case 0x0F: desc_0F.parse(m_0F_animation_info, is);         break;
            case 0x10: desc_10.parse(m_10_animation_speed, is);        break;
            case 0x11: desc_11.parse(m_11_callback_25_triggers, is);   break;
            case 0x12: desc_12.parse(m_12_special_flags, is);          break;
            case 0x13: desc_13.parse(m_13_cargo_acceptance_list, is);  break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
