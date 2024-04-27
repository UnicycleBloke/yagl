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
#include "Action00RoadStops.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_class_label            = "class_label";
constexpr const char* str_road_stop_type         = "road_stop_type";
constexpr const char* str_road_stop_name_text_id = "road_stop_name_text_id";
constexpr const char* str_class_name_text_id     = "class_name_text_id";
constexpr const char* str_draw_mode              = "draw_mode";
constexpr const char* str_random_trigger_cargoes = "random_trigger_cargoes"; 
constexpr const char* str_animation_information  = "animation_information";
constexpr const char* str_animation_speed        = "animation_speed";
constexpr const char* str_animation_triggers     = "animation_triggers"; 
constexpr const char* str_callback_flags         = "callback_flags";
constexpr const char* str_general_flags          = "general_flags";
constexpr const char* str_cost_multipliers       = "cost_multipliers";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_class_label,            0x08 },
    { str_road_stop_type,         0x09 },
    { str_road_stop_name_text_id, 0x0A },
    { str_class_name_text_id,     0x0B },
    { str_draw_mode,              0x0C },
    { str_random_trigger_cargoes, 0x0D }, 
    { str_animation_information,  0x0E },
    { str_animation_speed,        0x0F },
    { str_animation_triggers,     0x10 }, 
    { str_callback_flags,         0x11 },
    { str_general_flags,          0x12 },
    { str_cost_multipliers,       0x15 },
};


constexpr GRFLabelDescriptor desc_08 = { 0x08, str_class_label };
constexpr UInt8Descriptor    desc_09 = { 0x09, str_road_stop_type,         UIntFormat::Hex };
constexpr UInt16Descriptor   desc_0A = { 0x0A, str_road_stop_name_text_id, UIntFormat::Hex };
constexpr UInt16Descriptor   desc_0B = { 0x0B, str_class_name_text_id,     UIntFormat::Hex };
constexpr UInt8Descriptor    desc_0C = { 0x0C, str_draw_mode,              UIntFormat::Hex };
constexpr UInt32Descriptor   desc_0D = { 0x0D, str_random_trigger_cargoes, UIntFormat::Hex }; 
constexpr UInt16Descriptor   desc_0E = { 0x0E, str_animation_information,  UIntFormat::Hex };
constexpr UInt8Descriptor    desc_0F = { 0x0F, str_animation_speed,        UIntFormat::Hex };
constexpr UInt16Descriptor   desc_10 = { 0x10, str_animation_triggers,     UIntFormat::Hex }; 
constexpr UInt8Descriptor    desc_11 = { 0x11, str_callback_flags,         UIntFormat::Hex };
constexpr UInt32Descriptor   desc_12 = { 0x12, str_general_flags,          UIntFormat::Hex };
constexpr UInt16Descriptor   desc_15 = { 0x15, str_cost_multipliers,       UIntFormat::Hex };

} // namespace {


bool Action00RoadStops::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_class_label.read(is);            break;
        case 0x09: m_09_road_stop_type.read(is);         break;
        case 0x0A: m_0A_road_stop_name_text_id.read(is); break;
        case 0x0B: m_0B_class_name_text_id.read(is);     break;
        case 0x0C: m_0C_draw_mode.read(is);              break;
        case 0x0D: m_0D_random_trigger_cargoes.read(is); break; 
        case 0x0E: m_0E_animation_information.read(is);  break;
        case 0x0F: m_0F_animation_speed.read(is);        break;
        case 0x10: m_10_animation_triggers.read(is);     break; 
        case 0x11: m_11_callback_flags.read(is);         break;
        case 0x12: m_12_general_flags.read(is);          break;
        case 0x15: m_15_cost_multipliers.read(is);       break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadStops::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_class_label.write(os);            break;
        case 0x09: m_09_road_stop_type.write(os);         break;
        case 0x0A: m_0A_road_stop_name_text_id.write(os); break;
        case 0x0B: m_0B_class_name_text_id.write(os);     break;
        case 0x0C: m_0C_draw_mode.write(os);              break;
        case 0x0D: m_0D_random_trigger_cargoes.write(os); break; 
        case 0x0E: m_0E_animation_information.write(os);  break;
        case 0x0F: m_0F_animation_speed.write(os);        break;
        case 0x10: m_10_animation_triggers.write(os);     break; 
        case 0x11: m_11_callback_flags.write(os);         break;
        case 0x12: m_12_general_flags.write(os);          break;
        case 0x15: m_15_cost_multipliers.write(os);       break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadStops::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_class_label, os, indent);            break;
        case 0x09: desc_09.print(m_09_road_stop_type, os, indent);         break;
        case 0x0A: desc_0A.print(m_0A_road_stop_name_text_id, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_class_name_text_id, os, indent);     break;
        case 0x0C: desc_0C.print(m_0C_draw_mode, os, indent);              break;
        case 0x0D: desc_0D.print(m_0D_random_trigger_cargoes, os, indent); break; 
        case 0x0E: desc_0E.print(m_0E_animation_information, os, indent);  break;
        case 0x0F: desc_0F.print(m_0F_animation_speed, os, indent);        break;
        case 0x10: desc_10.print(m_10_animation_triggers, os, indent);     break; 
        case 0x11: desc_11.print(m_11_callback_flags, os, indent);         break;
        case 0x12: desc_12.print(m_12_general_flags, os, indent);          break;
        case 0x15: desc_15.print(m_15_cost_multipliers, os, indent);       break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadStops::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_class_label, is);            break;
            case 0x09: desc_09.parse(m_09_road_stop_type, is);         break;
            case 0x0A: desc_0A.parse(m_0A_road_stop_name_text_id, is); break;
            case 0x0B: desc_0B.parse(m_0B_class_name_text_id, is);     break;
            case 0x0C: desc_0C.parse(m_0C_draw_mode, is);              break;
            case 0x0D: desc_0D.parse(m_0D_random_trigger_cargoes, is); break; 
            case 0x0E: desc_0E.parse(m_0E_animation_information, is);  break;
            case 0x0F: desc_0F.parse(m_0F_animation_speed, is);        break;
            case 0x10: desc_10.parse(m_10_animation_triggers, is);     break; 
            case 0x11: desc_11.parse(m_11_callback_flags, is);         break;
            case 0x12: desc_12.parse(m_12_general_flags, is);          break;
            case 0x15: desc_15.parse(m_15_cost_multipliers, is);       break;
            default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", feature(), property);
}
