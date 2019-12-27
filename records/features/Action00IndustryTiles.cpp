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
constexpr const char* str_acceptance_cargo1      = "acceptance_cargo1";
constexpr const char* str_acceptance_degree1     = "acceptance_degree1";
constexpr const char* str_acceptance_cargo2      = "acceptance_cargo2";
constexpr const char* str_acceptance_degree2     = "acceptance_degree2";
constexpr const char* str_acceptance_cargo3      = "acceptance_cargo3";
constexpr const char* str_acceptance_degree3     = "acceptance_degree3";
constexpr const char* str_land_shape_flags       = "land_shape_flags";
constexpr const char* str_callback_flags         = "callback_flags";
constexpr const char* str_animation_frames       = "animation_frames";
constexpr const char* str_animation_type         = "animation_type";
constexpr const char* str_animation_speed        = "animation_speed";
constexpr const char* str_callback_25_triggers   = "callback_25_triggers";
constexpr const char* str_special_flags          = "special_flags";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_substitute_building_id, 0x08'00 },
    { str_industry_tile_override, 0x09'00 },
    { str_acceptance_cargo1,      0x0A'00 },
    { str_acceptance_degree1,     0x0A'01 },
    { str_acceptance_cargo2,      0x0B'00 },
    { str_acceptance_degree2,     0x0B'01 },
    { str_acceptance_cargo3,      0x0C'00 },
    { str_acceptance_degree3,     0x0C'01 },
    { str_land_shape_flags,       0x0D'00 },
    { str_callback_flags,         0x0E'00 },
    { str_animation_frames,       0x0F'00 },
    { str_animation_type,         0x0F'01 },
    { str_animation_speed,        0x10'00 },
    { str_callback_25_triggers,   0x11'00 },
    { str_special_flags,          0x12'00 },
};


constexpr IntegerDescriptorT<uint8_t> desc_08  = { 0x08, str_substitute_building_id, PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_09  = { 0x09, str_industry_tile_override, PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0A0 = { 0x0A, str_acceptance_cargo1,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0A1 = { 0x0A, str_acceptance_degree1,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0B0 = { 0x0B, str_acceptance_cargo2,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0B1 = { 0x0B, str_acceptance_degree2,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0C0 = { 0x0C, str_acceptance_cargo3,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0C1 = { 0x0C, str_acceptance_degree3,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0D  = { 0x0D, str_land_shape_flags,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0E  = { 0x0E, str_callback_flags,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0F0 = { 0x0F, str_animation_frames,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_0F1 = { 0x0F, str_animation_type,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_10  = { 0x10, str_animation_speed,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_11  = { 0x11, str_callback_25_triggers,   PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t> desc_12  = { 0x12, str_special_flags,          PropFormat::Hex };


} // namespace {


bool Action00IndustryTiles::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_substitute_building_id = read_uint8(is); break;
        case 0x09: m_09_industry_tile_override = read_uint8(is); break;
        case 0x0A: m_0A_acceptance_cargo1      = read_uint8(is);
                   m_0A_acceptance_degree1     = read_uint8(is); break;
        case 0x0B: m_0B_acceptance_cargo2      = read_uint8(is);
                   m_0B_acceptance_degree2     = read_uint8(is); break;
        case 0x0C: m_0C_acceptance_cargo3      = read_uint8(is);
                   m_0C_acceptance_degree3     = read_uint8(is); break;
        case 0x0D: m_0D_land_shape_flags       = read_uint8(is); break;
        case 0x0E: m_0E_callback_flags         = read_uint8(is); break;
        case 0x0F: m_0F_animation_frames       = read_uint8(is);
                   m_0F_animation_type         = read_uint8(is); break;
        case 0x10: m_10_animation_speed        = read_uint8(is); break;
        case 0x11: m_11_callback_25_triggers   = read_uint8(is); break;
        case 0x12: m_12_special_flags          = read_uint8(is); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}   


bool Action00IndustryTiles::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint8(os, m_08_substitute_building_id); break;
        case 0x09: write_uint8(os, m_09_industry_tile_override); break;
        case 0x0A: write_uint8(os, m_0A_acceptance_cargo1);       
                   write_uint8(os, m_0A_acceptance_degree1); break;
        case 0x0B: write_uint8(os, m_0B_acceptance_cargo2);       
                   write_uint8(os, m_0B_acceptance_degree2); break;
        case 0x0C: write_uint8(os, m_0C_acceptance_cargo3);       
                   write_uint8(os, m_0C_acceptance_degree3); break;
        case 0x0D: write_uint8(os, m_0D_land_shape_flags); break;
        case 0x0E: write_uint8(os, m_0E_callback_flags); break;
        case 0x0F: write_uint8(os, m_0F_animation_frames);       
                   write_uint8(os, m_0F_animation_type); break;
        case 0x10: write_uint8(os, m_10_animation_speed); break;
        case 0x11: write_uint8(os, m_11_callback_25_triggers); break;
        case 0x12: write_uint8(os, m_12_special_flags); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }
}


bool Action00IndustryTiles::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_substitute_building_id, os, indent); break;
        case 0x09: desc_09.print(m_09_industry_tile_override, os, indent); break;
        case 0x0A: desc_0A0.print(m_0A_acceptance_cargo1, os, indent); os << "\n";
                   desc_0A1.print(m_0A_acceptance_degree1, os, indent); break;
        case 0x0B: desc_0B0.print(m_0B_acceptance_cargo2, os, indent); os << "\n";
                   desc_0B1.print(m_0B_acceptance_degree2, os, indent); break;
        case 0x0C: desc_0C0.print(m_0C_acceptance_cargo3, os, indent); os << "\n";
                   desc_0C1.print(m_0C_acceptance_degree3, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_land_shape_flags, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_callback_flags, os, indent); break;
        case 0x0F: desc_0F0.print(m_0F_animation_frames, os, indent); os << "\n";
                   desc_0F1.print(m_0F_animation_type, os, indent); break;
        case 0x10: desc_10.print(m_10_animation_speed, os, indent); break;
        case 0x11: desc_11.print(m_11_callback_25_triggers, os, indent); break;
        case 0x12: desc_12.print(m_12_special_flags, os, indent); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    os << "\n";
    return true;
}


bool Action00IndustryTiles::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_substitute_building_id, is); break;
            case 0x09'00: desc_09.parse(m_09_industry_tile_override, is); break;
            case 0x0A'00: desc_0A0.parse(m_0A_acceptance_cargo1, is); break;
            case 0x0A'01: desc_0A1.parse(m_0A_acceptance_degree1, is); break;
            case 0x0B'00: desc_0B0.parse(m_0B_acceptance_cargo2, is); break;
            case 0x0B'01: desc_0B1.parse(m_0B_acceptance_degree2, is); break;
            case 0x0C'00: desc_0C0.parse(m_0C_acceptance_cargo3, is); break;
            case 0x0C'01: desc_0C1.parse(m_0C_acceptance_degree3, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_land_shape_flags, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_callback_flags, is); break;
            case 0x0F'00: desc_0F0.parse(m_0F_animation_frames, is); break;
            case 0x0F'01: desc_0F1.parse(m_0F_animation_type, is); break;
            case 0x10'00: desc_10.parse(m_10_animation_speed, is); break;
            case 0x11'00: desc_11.parse(m_11_callback_25_triggers, is); break;
            case 0x12'00: desc_12.parse(m_12_special_flags, is); break;
            default:      throw RUNTIME_ERROR("Unknown property");
        }

        return true;
    }

    throw RUNTIME_ERROR("Unknown property");
}
