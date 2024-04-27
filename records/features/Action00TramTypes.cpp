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
#include "Action00TramTypes.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_tramtype_label          = "tramtype_label";
constexpr const char* str_toolbar_caption_id      = "toolbar_caption_id";
constexpr const char* str_dropdown_text_id        = "dropdown_text_id";
constexpr const char* str_window_caption_id       = "window_caption_id";
constexpr const char* str_autoreplace_text_id     = "autoreplace_text_id";
constexpr const char* str_new_engine_text_id      = "new_engine_text_id";
constexpr const char* str_powered_tramtypes       = "powered_tramtypes";
constexpr const char* str_tramtype_flags          = "tramtype_flags";
constexpr const char* str_construction_costs      = "construction_costs";
constexpr const char* str_speed_limit             = "speed_limit";
constexpr const char* str_minimap_colour          = "minimap_colour";
constexpr const char* str_introduction_date       = "introduction_date";
constexpr const char* str_required_tramtypes      = "required_tramtypes";
constexpr const char* str_introduced_tramtypes    = "introduced_tramtypes";
constexpr const char* str_sort_order              = "sort_order";
constexpr const char* str_tram_type_name_id       = "tram_type_name_id";
constexpr const char* str_maintenance_cost_factor = "maintenance_cost_factor";
constexpr const char* str_alternate_tramtypes     = "alternate_tramtypes";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_tramtype_label,          0x08 },
    { str_toolbar_caption_id,      0x09 },
    { str_dropdown_text_id,        0x0A },
    { str_window_caption_id,       0x0B },
    { str_autoreplace_text_id,     0x0C },
    { str_new_engine_text_id,      0x0D },
    { str_powered_tramtypes,       0x0F },
    { str_tramtype_flags,          0x10 },
    { str_construction_costs,      0x13 },
    { str_speed_limit,             0x14 },
    { str_minimap_colour,          0x16 },
    { str_introduction_date,       0x17 },
    { str_required_tramtypes,      0x18 },
    { str_introduced_tramtypes,    0x19 },
    { str_sort_order,              0x1A },
    { str_tram_type_name_id,       0x1B },
    { str_maintenance_cost_factor, 0x1C },
    { str_alternate_tramtypes,     0x1D },
};


constexpr GRFLabelDescriptor     desc_08 = { 0x08, str_tramtype_label };
constexpr UInt16Descriptor       desc_09 = { 0x09, str_toolbar_caption_id,      UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0A = { 0x0A, str_dropdown_text_id,        UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0B = { 0x0B, str_window_caption_id,       UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0C = { 0x0C, str_autoreplace_text_id,     UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0D = { 0x0D, str_new_engine_text_id,      UIntFormat::Hex };
constexpr GRFLabelListDescriptor desc_0F = { 0x0F, str_powered_tramtypes };
constexpr UInt8Descriptor        desc_10 = { 0x10, str_tramtype_flags,          UIntFormat::Hex };
constexpr UInt16Descriptor       desc_13 = { 0x13, str_construction_costs,      UIntFormat::Hex };
constexpr UInt16Descriptor       desc_14 = { 0x14, str_speed_limit,             UIntFormat::Hex };
constexpr UInt8Descriptor        desc_16 = { 0x16, str_minimap_colour,          UIntFormat::Hex };
constexpr LongDateDescriptor     desc_17 = { 0x17, str_introduction_date };
constexpr GRFLabelListDescriptor desc_18 = { 0x18, str_required_tramtypes };
constexpr GRFLabelListDescriptor desc_19 = { 0x19, str_introduced_tramtypes };
constexpr UInt8Descriptor        desc_1A = { 0x1A, str_sort_order,              UIntFormat::Hex };
constexpr UInt16Descriptor       desc_1B = { 0x1B, str_tram_type_name_id,       UIntFormat::Hex };
constexpr UInt16Descriptor       desc_1C = { 0x1C, str_maintenance_cost_factor, UIntFormat::Hex };
constexpr GRFLabelListDescriptor desc_1D = { 0x1D, str_alternate_tramtypes };


} // namespace {


bool Action00TramTypes::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_tramtype_label.read(is);          break;
        case 0x09: m_09_toolbar_caption_id.read(is);      break;
        case 0x0A: m_0A_dropdown_text_id.read(is);        break;
        case 0x0B: m_0B_window_caption_id.read(is);       break;
        case 0x0C: m_0C_autoreplace_text_id.read(is);     break;
        case 0x0D: m_0D_new_engine_text_id.read(is);      break;
        case 0x0F: m_0F_powered_tramtypes.read(is);       break;
        case 0x10: m_10_tramtype_flags.read(is);          break;
        case 0x13: m_13_construction_costs.read(is);      break;
        case 0x14: m_14_speed_limit.read(is);             break;
        case 0x16: m_16_minimap_colour.read(is);          break;
        case 0x17: m_17_introduction_date.read(is);       break;
        case 0x18: m_18_required_tramtypes.read(is);      break;
        case 0x19: m_19_introduced_tramtypes.read(is);    break;
        case 0x1A: m_1A_sort_order.read(is);              break;
        case 0x1B: m_1B_tram_type_name_id.read(is);       break;
        case 0x1C: m_1C_maintenance_cost_factor.read(is); break;
        case 0x1D: m_1D_alternate_tramtypes.read(is);     break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00TramTypes::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_tramtype_label.write(os);          break;
        case 0x09: m_09_toolbar_caption_id.write(os);      break;
        case 0x0A: m_0A_dropdown_text_id.write(os);        break;
        case 0x0B: m_0B_window_caption_id.write(os);       break;
        case 0x0C: m_0C_autoreplace_text_id.write(os);     break;
        case 0x0D: m_0D_new_engine_text_id.write(os);      break;
        case 0x0F: m_0F_powered_tramtypes.write(os);       break;
        case 0x10: m_10_tramtype_flags.write(os);          break;
        case 0x13: m_13_construction_costs.write(os);      break;
        case 0x14: m_14_speed_limit.write(os);             break;
        case 0x16: m_16_minimap_colour.write(os);          break;
        case 0x17: m_17_introduction_date.write(os);       break;
        case 0x18: m_18_required_tramtypes.write(os);      break;
        case 0x19: m_19_introduced_tramtypes.write(os);    break;
        case 0x1A: m_1A_sort_order.write(os);              break;
        case 0x1B: m_1B_tram_type_name_id.write(os);       break;
        case 0x1C: m_1C_maintenance_cost_factor.write(os); break;
        case 0x1D: m_1D_alternate_tramtypes.write(os);     break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00TramTypes::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_tramtype_label, os, indent);          break;
        case 0x09: desc_09.print(m_09_toolbar_caption_id, os, indent);      break;
        case 0x0A: desc_0A.print(m_0A_dropdown_text_id, os, indent);        break;
        case 0x0B: desc_0B.print(m_0B_window_caption_id, os, indent);       break;
        case 0x0C: desc_0C.print(m_0C_autoreplace_text_id, os, indent);     break;
        case 0x0D: desc_0D.print(m_0D_new_engine_text_id, os, indent);      break;
        case 0x0F: desc_0F.print(m_0F_powered_tramtypes, os, indent);       break;
        case 0x10: desc_10.print(m_10_tramtype_flags, os, indent);          break;
        case 0x13: desc_13.print(m_13_construction_costs, os, indent);      break;
        case 0x14: desc_14.print(m_14_speed_limit, os, indent);             break;
        case 0x16: desc_16.print(m_16_minimap_colour, os, indent);          break;
        case 0x17: desc_17.print(m_17_introduction_date, os, indent);       break;
        case 0x18: desc_18.print(m_18_required_tramtypes, os, indent);      break;
        case 0x19: desc_19.print(m_19_introduced_tramtypes, os, indent);    break;
        case 0x1A: desc_1A.print(m_1A_sort_order, os, indent);              break;
        case 0x1B: desc_1B.print(m_1B_tram_type_name_id, os, indent);       break;
        case 0x1C: desc_1C.print(m_1C_maintenance_cost_factor, os, indent); break;
        case 0x1D: desc_1D.print(m_1D_alternate_tramtypes, os, indent);     break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00TramTypes::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_tramtype_label, is);          break;
            case 0x09: desc_09.parse(m_09_toolbar_caption_id, is);      break;
            case 0x0A: desc_0A.parse(m_0A_dropdown_text_id, is);        break;
            case 0x0B: desc_0B.parse(m_0B_window_caption_id, is);       break;
            case 0x0C: desc_0C.parse(m_0C_autoreplace_text_id, is);     break;
            case 0x0D: desc_0D.parse(m_0D_new_engine_text_id, is);      break;
            case 0x0F: desc_0F.parse(m_0F_powered_tramtypes, is);       break;
            case 0x10: desc_10.parse(m_10_tramtype_flags, is);          break;
            case 0x13: desc_13.parse(m_13_construction_costs, is);      break;
            case 0x14: desc_14.parse(m_14_speed_limit, is);             break;
            case 0x16: desc_16.parse(m_16_minimap_colour, is);          break;
            case 0x17: desc_17.parse(m_17_introduction_date, is);       break;
            case 0x18: desc_18.parse(m_18_required_tramtypes, is);      break;
            case 0x19: desc_19.parse(m_19_introduced_tramtypes, is);    break;
            case 0x1A: desc_1A.parse(m_1A_sort_order, is);              break;
            case 0x1B: desc_1B.parse(m_1B_tram_type_name_id, is);       break;
            case 0x1C: desc_1C.parse(m_1C_maintenance_cost_factor, is); break;
            case 0x1D: desc_1D.parse(m_1D_alternate_tramtypes, is);     break;
            default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", feature(), property);
}
