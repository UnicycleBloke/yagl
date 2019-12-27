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
#include "Action00Cargos.h"
#include "Descriptors.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_bit_number             = "bit_number";
constexpr const char* str_cargo_type_name_id     = "cargo_type_name_id";
constexpr const char* str_single_unit_name_id    = "single_unit_name_id";
constexpr const char* str_single_unit_id         = "single_unit_id";
constexpr const char* str_multiple_units_id      = "multiple_units_id";
constexpr const char* str_cargo_type_abbrev_id   = "cargo_type_abbrev_id";
constexpr const char* str_cargo_sprite_id        = "cargo_sprite_id";
constexpr const char* str_single_unit_weight     = "single_unit_weight";
constexpr const char* str_penalty_time_1         = "penalty_time_1";
constexpr const char* str_penalty_time_2         = "penalty_time_2";
constexpr const char* str_base_price             = "base_price";
constexpr const char* str_station_list_colour    = "station_list_colour";
constexpr const char* str_payment_list_colour    = "payment_list_colour";
constexpr const char* str_is_freight             = "is_freight";
constexpr const char* str_cargo_classes          = "cargo_classes";
constexpr const char* str_cargo_label            = "cargo_label";
constexpr const char* str_town_growth_effect     = "town_growth_effect";
constexpr const char* str_town_growth_multiplier = "town_growth_multiplier";
constexpr const char* str_callback_flags         = "callback_flags";
constexpr const char* str_cargo_units_id         = "cargo_units_id";
constexpr const char* str_cargo_amount_id        = "cargo_amount_id";
constexpr const char* str_capacity_multiplier    = "capacity_multiplier";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_bit_number,            0x08'00 },
    { str_cargo_type_name_id,    0x09'00 },
    { str_single_unit_name_id,   0x0A'00 },
    { str_single_unit_id,        0x0B'00 },
    { str_multiple_units_id,     0x0C'00 },
    { str_cargo_type_abbrev_id,  0x0D'00 },
    { str_cargo_sprite_id,       0x0E'00 },
    { str_single_unit_weight,    0x0F'00 },
    { str_penalty_time_1,        0x10'00 },
    { str_penalty_time_2,        0x11'00 },
    { str_base_price,            0x12'00 },
    { str_station_list_colour,   0x13'00 },
    { str_payment_list_colour,   0x14'00 },
    { str_is_freight,            0x15'00 },
    { str_cargo_classes,         0x16'00 },
    { str_cargo_label,           0x17'00 },
    { str_town_growth_effect,    0x18'00 },
    { str_town_growth_multiplier,0x19'00 },
    { str_callback_flags,        0x1A'00 },
    { str_cargo_units_id,        0x1B'00 },
    { str_cargo_amount_id,       0x1C'00 },
    { str_capacity_multiplier,   0x1D'00 },
};


constexpr IntegerDescriptorT<uint8_t>  desc_08 = { 0x08, str_bit_number,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_09 = { 0x09, str_cargo_type_name_id,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0A = { 0x0A, str_single_unit_name_id,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0B = { 0x0B, str_single_unit_id,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0C = { 0x0C, str_multiple_units_id,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0D = { 0x0D, str_cargo_type_abbrev_id,   PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0E = { 0x0E, str_cargo_sprite_id,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0F = { 0x0F, str_single_unit_weight,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_10 = { 0x10, str_penalty_time_1,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_11 = { 0x11, str_penalty_time_2,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_12 = { 0x12, str_base_price,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_13 = { 0x13, str_station_list_colour,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_14 = { 0x14, str_payment_list_colour,    PropFormat::Hex };
constexpr BooleanDescriptor            desc_15 = { 0x15, str_is_freight };
constexpr IntegerDescriptorT<uint16_t> desc_16 = { 0x16, str_cargo_classes,          PropFormat::Hex };
constexpr GRFLabelDescriptor           desc_17 = { 0x17, str_cargo_label };
constexpr IntegerDescriptorT<uint8_t>  desc_18 = { 0x18, str_town_growth_effect,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_19 = { 0x19, str_town_growth_multiplier, PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_1A = { 0x1A, str_callback_flags,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1B = { 0x1B, str_cargo_units_id,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1C = { 0x1C, str_cargo_amount_id,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1D = { 0x1D, str_capacity_multiplier,    PropFormat::Hex };


} // namespace {


bool Action00Cargos::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_bit_number             = read_uint8(is); break;
        case 0x09: m_09_cargo_type_name_id     = read_uint16(is); break;
        case 0x0A: m_0A_single_unit_name_id    = read_uint16(is); break;
        case 0x0B: m_0B_single_unit_id         = read_uint16(is); break;
        case 0x0C: m_0C_multiple_units_id      = read_uint16(is); break;
        case 0x0D: m_0D_cargo_type_abbrev_id   = read_uint16(is); break;
        case 0x0E: m_0E_cargo_sprite_id        = read_uint16(is); break;
        case 0x0F: m_0F_single_unit_weight     = read_uint8(is); break;
        case 0x10: m_10_penalty_time_1         = read_uint8(is); break;
        case 0x11: m_11_penalty_time_2         = read_uint8(is); break;
        case 0x12: m_12_base_price             = read_uint32(is); break;
        case 0x13: m_13_station_list_colour    = read_uint8(is); break;
        case 0x14: m_14_payment_list_colour    = read_uint8(is); break;
        case 0x15: m_15_is_freight             = (read_uint8(is) == 0x01); break;
        case 0x16: m_16_cargo_classes          = read_uint16(is); break;
        case 0x17: m_17_cargo_label.read(is); break;
        case 0x18: m_18_town_growth_effect     = read_uint8(is); break;
        case 0x19: m_19_town_growth_multiplier = read_uint16(is); break;
        case 0x1A: m_1A_callback_flags         = read_uint8(is); break;
        case 0x1B: m_1B_cargo_units_id         = read_uint16(is); break;
        case 0x1C: m_1C_cargo_amount_id        = read_uint16(is); break;
        case 0x1D: m_1D_capacity_multiplier    = read_uint16(is); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}   


bool Action00Cargos::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint8(os, m_08_bit_number); break;
        case 0x09: write_uint16(os, m_09_cargo_type_name_id); break;
        case 0x0A: write_uint16(os, m_0A_single_unit_name_id); break;
        case 0x0B: write_uint16(os, m_0B_single_unit_id); break;
        case 0x0C: write_uint16(os, m_0C_multiple_units_id); break;
        case 0x0D: write_uint16(os, m_0D_cargo_type_abbrev_id); break;
        case 0x0E: write_uint16(os, m_0E_cargo_sprite_id); break;
        case 0x0F: write_uint8(os, m_0F_single_unit_weight); break;
        case 0x10: write_uint8(os, m_10_penalty_time_1); break;
        case 0x11: write_uint8(os, m_11_penalty_time_2); break;
        case 0x12: write_uint32(os, m_12_base_price); break;
        case 0x13: write_uint8(os, m_13_station_list_colour); break;
        case 0x14: write_uint8(os, m_14_payment_list_colour); break;
        case 0x15: write_uint8(os, (m_15_is_freight ? 0x01 : 0x00)); break;
        case 0x16: write_uint16(os, m_16_cargo_classes); break;
        case 0x17: m_17_cargo_label.write(os); break;
        case 0x18: write_uint8(os, m_18_town_growth_effect); break;
        case 0x19: write_uint16(os, m_19_town_growth_multiplier); break;
        case 0x1A: write_uint8(os, m_1A_callback_flags); break;
        case 0x1B: write_uint16(os, m_1B_cargo_units_id); break;
        case 0x1C: write_uint16(os, m_1C_cargo_amount_id); break;
        case 0x1D: write_uint16(os, m_1D_capacity_multiplier); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}


bool Action00Cargos::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_bit_number, os, indent); break;
        case 0x09: desc_09.print(m_09_cargo_type_name_id, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_single_unit_name_id, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_single_unit_id, os, indent); break;
        case 0x0C: desc_0C.print(m_0C_multiple_units_id, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_cargo_type_abbrev_id, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_cargo_sprite_id, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_single_unit_weight, os, indent); break;
        case 0x10: desc_10.print(m_10_penalty_time_1, os, indent); break;
        case 0x11: desc_11.print(m_11_penalty_time_2, os, indent); break;
        case 0x12: desc_12.print(m_12_base_price, os, indent); break;
        case 0x13: desc_13.print(m_13_station_list_colour, os, indent); break;
        case 0x14: desc_14.print(m_14_payment_list_colour, os, indent); break;
        case 0x15: desc_15.print(m_15_is_freight, os, indent); break;
        case 0x16: desc_16.print(m_16_cargo_classes, os, indent); break;
        case 0x17: desc_17.print(m_17_cargo_label, os, indent); break;
        case 0x18: desc_18.print(m_18_town_growth_effect, os, indent); break;
        case 0x19: desc_19.print(m_19_town_growth_multiplier, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_callback_flags, os, indent); break;
        case 0x1B: desc_1B.print(m_1B_cargo_units_id, os, indent); break;
        case 0x1C: desc_1C.print(m_1C_cargo_amount_id, os, indent); break;
        case 0x1D: desc_1D.print(m_1D_capacity_multiplier, os, indent); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    os << "\n";
    return true;
}


bool Action00Cargos::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_bit_number, is); break;
            case 0x09'00: desc_09.parse(m_09_cargo_type_name_id, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_single_unit_name_id, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_single_unit_id, is); break;
            case 0x0C'00: desc_0C.parse(m_0C_multiple_units_id, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_cargo_type_abbrev_id, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_cargo_sprite_id, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_single_unit_weight, is); break;
            case 0x10'00: desc_10.parse(m_10_penalty_time_1, is); break;
            case 0x11'00: desc_11.parse(m_11_penalty_time_2, is); break;
            case 0x12'00: desc_12.parse(m_12_base_price, is); break;
            case 0x13'00: desc_13.parse(m_13_station_list_colour, is); break;
            case 0x14'00: desc_14.parse(m_14_payment_list_colour, is); break;
            case 0x15'00: desc_15.parse(m_15_is_freight, is); break;
            case 0x16'00: desc_16.parse(m_16_cargo_classes, is); break;
            case 0x17'00: desc_17.parse(m_17_cargo_label, is); break;
            case 0x18'00: desc_18.parse(m_18_town_growth_effect, is); break;
            case 0x19'00: desc_19.parse(m_19_town_growth_multiplier, is); break;
            case 0x1A'00: desc_1A.parse(m_1A_callback_flags, is); break;
            case 0x1B'00: desc_1B.parse(m_1B_cargo_units_id, is); break;
            case 0x1C'00: desc_1C.parse(m_1C_cargo_amount_id, is); break;
            case 0x1D'00: desc_1D.parse(m_1D_capacity_multiplier, is); break;
            default:      throw RUNTIME_ERROR("Unknown property");
        }

        return true;
    }

    throw RUNTIME_ERROR("Unknown property");
}
