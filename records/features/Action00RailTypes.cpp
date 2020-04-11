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
#include "Action00RailTypes.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_railtype_label          = "railtype_label";
constexpr const char* str_toolbar_caption_id      = "toolbar_caption_id";
constexpr const char* str_dropdown_text_id        = "dropdown_text_id";
constexpr const char* str_window_caption_id       = "window_caption_id";
constexpr const char* str_autoreplace_text_id     = "autoreplace_text_id";
constexpr const char* str_new_engine_text_id      = "new_engine_text_id";
constexpr const char* str_compatible_railtypes    = "compatible_railtypes";
constexpr const char* str_powered_railtypes       = "powered_railtypes";
constexpr const char* str_railtype_flags          = "railtype_flags";
constexpr const char* str_curve_speed_multiplier  = "curve_speed_multiplier";
constexpr const char* str_station_graphics        = "station_graphics";
constexpr const char* str_construction_costs      = "construction_costs";
constexpr const char* str_speed_limit             = "speed_limit";
constexpr const char* str_acceleration_model      = "acceleration_model";
constexpr const char* str_minimap_colour          = "minimap_colour";
constexpr const char* str_introduction_date       = "introduction_date";
constexpr const char* str_required_railtypes      = "required_railtypes";
constexpr const char* str_introduced_railtypes    = "introduced_railtypes";
constexpr const char* str_sort_order              = "sort_order";
constexpr const char* str_rail_type_name_id       = "rail_type_name_id";
constexpr const char* str_maintenance_cost_factor = "maintenance_cost_factor";
constexpr const char* str_alternate_railtypes     = "alternate_railtypes";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_railtype_label,          0x08'00 },
    { str_toolbar_caption_id,      0x09'00 },
    { str_dropdown_text_id,        0x0A'00 },
    { str_window_caption_id,       0x0B'00 },
    { str_autoreplace_text_id,     0x0C'00 },
    { str_new_engine_text_id,      0x0D'00 },
    { str_compatible_railtypes,    0x0E'00 },
    { str_powered_railtypes,       0x0F'00 },
    { str_railtype_flags,          0x10'00 },
    { str_curve_speed_multiplier,  0x11'00 },
    { str_station_graphics,        0x12'00 },
    { str_construction_costs,      0x13'00 },
    { str_speed_limit,             0x14'00 },
    { str_acceleration_model,      0x15'00 },
    { str_minimap_colour,          0x16'00 },
    { str_introduction_date,       0x17'00 },
    { str_required_railtypes,      0x18'00 },
    { str_introduced_railtypes,    0x19'00 },
    { str_sort_order,              0x1A'00 },
    { str_rail_type_name_id,       0x1B'00 },
    { str_maintenance_cost_factor, 0x1C'00 },
    { str_alternate_railtypes,     0x1D'00 },
};


constexpr GRFLabelDescriptor           desc_08 = { 0x08, str_railtype_label };
constexpr IntegerDescriptorT<uint16_t> desc_09 = { 0x09, str_toolbar_caption_id,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0A = { 0x0A, str_dropdown_text_id,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0B = { 0x0B, str_window_caption_id,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0C = { 0x0C, str_autoreplace_text_id,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0D = { 0x0D, str_new_engine_text_id,      PropFormat::Hex };
constexpr GRFLabelListDescriptor       desc_0E = { 0x0E, str_compatible_railtypes };
constexpr GRFLabelListDescriptor       desc_0F = { 0x0F, str_powered_railtypes };
constexpr IntegerDescriptorT<uint8_t>  desc_10 = { 0x10, str_railtype_flags,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_11 = { 0x11, str_curve_speed_multiplier,  PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_12 = { 0x12, str_station_graphics,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_13 = { 0x13, str_construction_costs,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_14 = { 0x14, str_speed_limit,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_15 = { 0x15, str_acceleration_model,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_16 = { 0x16, str_minimap_colour,          PropFormat::Hex };
constexpr LongDateDescriptor           desc_17 = { 0x17, str_introduction_date };
constexpr GRFLabelListDescriptor       desc_18 = { 0x18, str_required_railtypes };
constexpr GRFLabelListDescriptor       desc_19 = { 0x19, str_introduced_railtypes };
constexpr IntegerDescriptorT<uint8_t>  desc_1A = { 0x1A, str_sort_order,              PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1B = { 0x1B, str_rail_type_name_id,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1C = { 0x1C, str_maintenance_cost_factor, PropFormat::Hex };
constexpr GRFLabelListDescriptor       desc_1D = { 0x1D, str_alternate_railtypes };


} // namespace {


bool Action00RailTypes::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_railtype_label.read(is);                   break;
        case 0x09: m_09_toolbar_caption_id      = read_uint16(is); break;
        case 0x0A: m_0A_dropdown_text_id        = read_uint16(is); break;
        case 0x0B: m_0B_window_caption_id       = read_uint16(is); break;
        case 0x0C: m_0C_autoreplace_text_id     = read_uint16(is); break;
        case 0x0D: m_0D_new_engine_text_id      = read_uint16(is); break;
        case 0x0E: m_0E_compatible_railtypes.read(is);             break;
        case 0x0F: m_0F_powered_railtypes.read(is);                break;
        case 0x10: m_10_railtype_flags          = read_uint8(is);  break;
        case 0x11: m_11_curve_speed_multiplier  = read_uint8(is);  break;
        case 0x12: m_12_station_graphics        = read_uint8(is);  break;
        case 0x13: m_13_construction_costs      = read_uint16(is); break;
        case 0x14: m_14_speed_limit             = read_uint16(is); break;
        case 0x15: m_15_acceleration_model      = read_uint8(is);  break;
        case 0x16: m_16_minimap_colour          = read_uint8(is);  break;
        case 0x17: m_17_introduction_date.read(is); break;
        case 0x18: m_18_required_railtypes.read(is);               break;
        case 0x19: m_19_introduced_railtypes.read(is);             break;
        case 0x1A: m_1A_sort_order              = read_uint8(is);  break;
        case 0x1B: m_1B_rail_type_name_id       = read_uint16(is); break;
        case 0x1C: m_1C_maintenance_cost_factor = read_uint16(is); break;
        case 0x1D: m_1D_alternate_railtypes.read(is);              break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00RailTypes::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_railtype_label.write(os); break;
        case 0x09: write_uint16(os, m_09_toolbar_caption_id); break;
        case 0x0A: write_uint16(os, m_0A_dropdown_text_id); break;
        case 0x0B: write_uint16(os, m_0B_window_caption_id); break;
        case 0x0C: write_uint16(os, m_0C_autoreplace_text_id); break;
        case 0x0D: write_uint16(os, m_0D_new_engine_text_id); break;
        case 0x0E: m_0E_compatible_railtypes.write(os); break;
        case 0x0F: m_0F_powered_railtypes.write(os); break;
        case 0x10: write_uint8(os, m_10_railtype_flags); break;
        case 0x11: write_uint8(os, m_11_curve_speed_multiplier); break;
        case 0x12: write_uint8(os, m_12_station_graphics); break;
        case 0x13: write_uint16(os, m_13_construction_costs); break;
        case 0x14: write_uint16(os, m_14_speed_limit); break;
        case 0x15: write_uint8(os, m_15_acceleration_model); break;
        case 0x16: write_uint8(os, m_16_minimap_colour); break;
        case 0x17: m_17_introduction_date.write(os); break;
        case 0x18: m_18_required_railtypes.write(os); break;
        case 0x19: m_19_introduced_railtypes.write(os); break;
        case 0x1A: write_uint8(os, m_1A_sort_order); break;
        case 0x1B: write_uint16(os, m_1B_rail_type_name_id); break;
        case 0x1C: write_uint16(os, m_1C_maintenance_cost_factor); break;
        case 0x1D: m_1D_alternate_railtypes.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00RailTypes::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_railtype_label, os, indent); break;
        case 0x09: desc_09.print(m_09_toolbar_caption_id, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_dropdown_text_id, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_window_caption_id, os, indent); break;
        case 0x0C: desc_0C.print(m_0C_autoreplace_text_id, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_new_engine_text_id, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_compatible_railtypes, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_powered_railtypes, os, indent); break;
        case 0x10: desc_10.print(m_10_railtype_flags, os, indent); break;
        case 0x11: desc_11.print(m_11_curve_speed_multiplier, os, indent); break;
        case 0x12: desc_12.print(m_12_station_graphics, os, indent); break;
        case 0x13: desc_13.print(m_13_construction_costs, os, indent); break;
        case 0x14: desc_14.print(m_14_speed_limit, os, indent); break;
        case 0x15: desc_15.print(m_15_acceleration_model, os, indent); break;
        case 0x16: desc_16.print(m_16_minimap_colour, os, indent); break;
        case 0x17: desc_17.print(m_17_introduction_date, os, indent); break;
        case 0x18: desc_18.print(m_18_required_railtypes, os, indent); break;
        case 0x19: desc_19.print(m_19_introduced_railtypes, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_sort_order, os, indent); break;
        case 0x1B: desc_1B.print(m_1B_rail_type_name_id, os, indent); break;
        case 0x1C: desc_1C.print(m_1C_maintenance_cost_factor, os, indent); break;
        case 0x1D: desc_1D.print(m_1D_alternate_railtypes, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00RailTypes::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_railtype_label, is); break;
            case 0x09'00: desc_09.parse(m_09_toolbar_caption_id, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_dropdown_text_id, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_window_caption_id, is); break;
            case 0x0C'00: desc_0C.parse(m_0C_autoreplace_text_id, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_new_engine_text_id, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_compatible_railtypes, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_powered_railtypes, is); break;
            case 0x10'00: desc_10.parse(m_10_railtype_flags, is); break;
            case 0x11'00: desc_11.parse(m_11_curve_speed_multiplier, is); break;
            case 0x12'00: desc_12.parse(m_12_station_graphics, is); break;
            case 0x13'00: desc_13.parse(m_13_construction_costs, is); break;
            case 0x14'00: desc_14.parse(m_14_speed_limit, is); break;
            case 0x15'00: desc_15.parse(m_15_acceleration_model, is); break;
            case 0x16'00: desc_16.parse(m_16_minimap_colour, is); break;
            case 0x17'00: desc_17.parse(m_17_introduction_date, is); break;
            case 0x18'00: desc_18.parse(m_18_required_railtypes, is); break;
            case 0x19'00: desc_19.parse(m_19_introduced_railtypes, is); break;
            case 0x1A'00: desc_1A.parse(m_1A_sort_order, is); break;
            case 0x1B'00: desc_1B.parse(m_1B_rail_type_name_id, is); break;
            case 0x1C'00: desc_1C.parse(m_1C_maintenance_cost_factor, is); break;
            case 0x1D'00: desc_1D.parse(m_1D_alternate_railtypes, is); break;
            default:      throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}

