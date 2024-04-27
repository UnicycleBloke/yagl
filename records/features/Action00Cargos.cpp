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
#include "StreamHelpers.h"


namespace {


constexpr const char* str_bit_number                 = "bit_number";
constexpr const char* str_cargo_type_name_id         = "cargo_type_name_id";
constexpr const char* str_single_unit_name_id        = "single_unit_name_id";
constexpr const char* str_single_unit_id             = "single_unit_id";
constexpr const char* str_multiple_units_id          = "multiple_units_id";
constexpr const char* str_cargo_type_abbrev_id       = "cargo_type_abbrev_id";
constexpr const char* str_cargo_sprite_id            = "cargo_sprite_id";
constexpr const char* str_single_unit_weight         = "single_unit_weight";
constexpr const char* str_penalty_time_1             = "penalty_time_1";
constexpr const char* str_penalty_time_2             = "penalty_time_2";
constexpr const char* str_base_price                 = "base_price";
constexpr const char* str_station_list_colour        = "station_list_colour";
constexpr const char* str_payment_list_colour        = "payment_list_colour";
constexpr const char* str_is_freight                 = "is_freight";
constexpr const char* str_cargo_classes              = "cargo_classes";
constexpr const char* str_cargo_label                = "cargo_label";
constexpr const char* str_town_growth_effect         = "town_growth_effect";
constexpr const char* str_town_growth_multiplier     = "town_growth_multiplier";
constexpr const char* str_callback_flags             = "callback_flags";
constexpr const char* str_cargo_units_id             = "cargo_units_id";
constexpr const char* str_cargo_amount_id            = "cargo_amount_id";
constexpr const char* str_capacity_multiplier        = "capacity_multiplier";
constexpr const char* str_town_production_effect     = "town_production_effect";
constexpr const char* str_town_production_multiplier = "town_production_multiplier";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_bit_number,                 0x08 },
    { str_cargo_type_name_id,         0x09 },
    { str_single_unit_name_id,        0x0A },
    { str_single_unit_id,             0x0B },
    { str_multiple_units_id,          0x0C },
    { str_cargo_type_abbrev_id,       0x0D },
    { str_cargo_sprite_id,            0x0E },
    { str_single_unit_weight,         0x0F },
    { str_penalty_time_1,             0x10 },
    { str_penalty_time_2,             0x11 },
    { str_base_price,                 0x12 },
    { str_station_list_colour,        0x13 },
    { str_payment_list_colour,        0x14 },
    { str_is_freight,                 0x15 },
    { str_cargo_classes,              0x16 },
    { str_cargo_label,                0x17 },
    { str_town_growth_effect,         0x18 },
    { str_town_growth_multiplier,     0x19 },
    { str_callback_flags,             0x1A },
    { str_cargo_units_id,             0x1B },
    { str_cargo_amount_id,            0x1C },
    { str_capacity_multiplier,        0x1D },
    { str_town_production_effect,     0x1E },
    { str_town_production_multiplier, 0x1F },
};


constexpr UInt8Descriptor     desc_08 = { 0x08, str_bit_number,                 UIntFormat::Hex };
constexpr UInt16Descriptor    desc_09 = { 0x09, str_cargo_type_name_id,         UIntFormat::Hex };
constexpr UInt16Descriptor    desc_0A = { 0x0A, str_single_unit_name_id,        UIntFormat::Hex };
constexpr UInt16Descriptor    desc_0B = { 0x0B, str_single_unit_id,             UIntFormat::Hex };
constexpr UInt16Descriptor    desc_0C = { 0x0C, str_multiple_units_id,          UIntFormat::Hex };
constexpr UInt16Descriptor    desc_0D = { 0x0D, str_cargo_type_abbrev_id,       UIntFormat::Hex };
constexpr UInt16Descriptor    desc_0E = { 0x0E, str_cargo_sprite_id,            UIntFormat::Hex };
constexpr UInt8Descriptor     desc_0F = { 0x0F, str_single_unit_weight,         UIntFormat::Hex };
constexpr UInt8Descriptor     desc_10 = { 0x10, str_penalty_time_1,             UIntFormat::Hex };
constexpr UInt8Descriptor     desc_11 = { 0x11, str_penalty_time_2,             UIntFormat::Hex };
constexpr UInt32Descriptor    desc_12 = { 0x12, str_base_price,                 UIntFormat::Hex };
constexpr UInt8Descriptor     desc_13 = { 0x13, str_station_list_colour,        UIntFormat::Hex };
constexpr UInt8Descriptor     desc_14 = { 0x14, str_payment_list_colour,        UIntFormat::Hex };
constexpr BoolDescriptor      desc_15 = { 0x15, str_is_freight };
constexpr UInt16Descriptor    desc_16 = { 0x16, str_cargo_classes,              UIntFormat::Hex };
constexpr GRFLabelDescriptor  desc_17 = { 0x17, str_cargo_label };
constexpr UInt8Descriptor     desc_18 = { 0x18, str_town_growth_effect,         UIntFormat::Hex };
constexpr UInt16Descriptor    desc_19 = { 0x19, str_town_growth_multiplier,     UIntFormat::Hex };
constexpr UInt8Descriptor     desc_1A = { 0x1A, str_callback_flags,             UIntFormat::Hex };
constexpr UInt16Descriptor    desc_1B = { 0x1B, str_cargo_units_id,             UIntFormat::Hex };
constexpr UInt16Descriptor    desc_1C = { 0x1C, str_cargo_amount_id,            UIntFormat::Hex };
constexpr UInt16Descriptor    desc_1D = { 0x1D, str_capacity_multiplier,        UIntFormat::Hex };
constexpr UInt8Descriptor     desc_1E = { 0x1E, str_town_production_effect,     UIntFormat::Hex };
constexpr UInt16Descriptor    desc_1F = { 0x1F, str_town_production_multiplier, UIntFormat::Hex };


} // namespace {


bool Action00Cargos::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_bit_number.read(is);                 break;
        case 0x09: m_09_cargo_type_name_id.read(is);         break;
        case 0x0A: m_0A_single_unit_name_id.read(is);        break;
        case 0x0B: m_0B_single_unit_id.read(is);             break;
        case 0x0C: m_0C_multiple_units_id.read(is);          break;
        case 0x0D: m_0D_cargo_type_abbrev_id.read(is);       break;
        case 0x0E: m_0E_cargo_sprite_id.read(is);            break;
        case 0x0F: m_0F_single_unit_weight.read(is);         break;
        case 0x10: m_10_penalty_time_1.read(is);             break;
        case 0x11: m_11_penalty_time_2.read(is);             break;
        case 0x12: m_12_base_price.read(is);                 break;
        case 0x13: m_13_station_list_colour.read(is);        break;
        case 0x14: m_14_payment_list_colour.read(is);        break;
        case 0x15: m_15_is_freight.read(is);                 break;
        case 0x16: m_16_cargo_classes.read(is);              break;
        case 0x17: m_17_cargo_label.read(is);                break;
        case 0x18: m_18_town_growth_effect.read(is);         break;
        case 0x19: m_19_town_growth_multiplier.read(is);     break;
        case 0x1A: m_1A_callback_flags.read(is);             break;
        case 0x1B: m_1B_cargo_units_id.read(is);             break;
        case 0x1C: m_1C_cargo_amount_id.read(is);            break;
        case 0x1D: m_1D_capacity_multiplier.read(is);        break;
        case 0x1E: m_1E_town_production_effect.read(is);     break;
        case 0x1F: m_1F_town_production_multiplier.read(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Cargos::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_bit_number.write(os);                 break;
        case 0x09: m_09_cargo_type_name_id.write(os);         break;
        case 0x0A: m_0A_single_unit_name_id.write(os);        break;
        case 0x0B: m_0B_single_unit_id.write(os);             break;
        case 0x0C: m_0C_multiple_units_id.write(os);          break;
        case 0x0D: m_0D_cargo_type_abbrev_id.write(os);       break;
        case 0x0E: m_0E_cargo_sprite_id.write(os);            break;
        case 0x0F: m_0F_single_unit_weight.write(os);         break;
        case 0x10: m_10_penalty_time_1.write(os);             break;
        case 0x11: m_11_penalty_time_2.write(os);             break;
        case 0x12: m_12_base_price.write(os);                 break;
        case 0x13: m_13_station_list_colour.write(os);        break;
        case 0x14: m_14_payment_list_colour.write(os);        break;
        case 0x15: m_15_is_freight.write(os);                 break;
        case 0x16: m_16_cargo_classes.write(os);              break;
        case 0x17: m_17_cargo_label.write(os);                break;
        case 0x18: m_18_town_growth_effect.write(os);         break;
        case 0x19: m_19_town_growth_multiplier.write(os);     break;
        case 0x1A: m_1A_callback_flags.write(os);             break;
        case 0x1B: m_1B_cargo_units_id.write(os);             break;
        case 0x1C: m_1C_cargo_amount_id.write(os);            break;
        case 0x1D: m_1D_capacity_multiplier.write(os);        break;
        case 0x1E: m_1E_town_production_effect.write(os);     break;
        case 0x1F: m_1F_town_production_multiplier.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Cargos::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_bit_number, os, indent);                 break;
        case 0x09: desc_09.print(m_09_cargo_type_name_id, os, indent);         break;
        case 0x0A: desc_0A.print(m_0A_single_unit_name_id, os, indent);        break;
        case 0x0B: desc_0B.print(m_0B_single_unit_id, os, indent);             break;
        case 0x0C: desc_0C.print(m_0C_multiple_units_id, os, indent);          break;
        case 0x0D: desc_0D.print(m_0D_cargo_type_abbrev_id, os, indent);       break;
        case 0x0E: desc_0E.print(m_0E_cargo_sprite_id, os, indent);            break;
        case 0x0F: desc_0F.print(m_0F_single_unit_weight, os, indent);         break;
        case 0x10: desc_10.print(m_10_penalty_time_1, os, indent);             break;
        case 0x11: desc_11.print(m_11_penalty_time_2, os, indent);             break;
        case 0x12: desc_12.print(m_12_base_price, os, indent);                 break;
        case 0x13: desc_13.print(m_13_station_list_colour, os, indent);        break;
        case 0x14: desc_14.print(m_14_payment_list_colour, os, indent);        break;
        case 0x15: desc_15.print(m_15_is_freight, os, indent);                 break;
        case 0x16: desc_16.print(m_16_cargo_classes, os, indent);              break;
        case 0x17: desc_17.print(m_17_cargo_label, os, indent);                break;
        case 0x18: desc_18.print(m_18_town_growth_effect, os, indent);         break;
        case 0x19: desc_19.print(m_19_town_growth_multiplier, os, indent);     break;
        case 0x1A: desc_1A.print(m_1A_callback_flags, os, indent);             break;
        case 0x1B: desc_1B.print(m_1B_cargo_units_id, os, indent);             break;
        case 0x1C: desc_1C.print(m_1C_cargo_amount_id, os, indent);            break;
        case 0x1D: desc_1D.print(m_1D_capacity_multiplier, os, indent);        break;
        case 0x1E: desc_1E.print(m_1E_town_production_effect, os, indent);     break;
        case 0x1F: desc_1F.print(m_1F_town_production_multiplier, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Cargos::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_bit_number, is);                 break;
            case 0x09: desc_09.parse(m_09_cargo_type_name_id, is);         break;
            case 0x0A: desc_0A.parse(m_0A_single_unit_name_id, is);        break;
            case 0x0B: desc_0B.parse(m_0B_single_unit_id, is);             break;
            case 0x0C: desc_0C.parse(m_0C_multiple_units_id, is);          break;
            case 0x0D: desc_0D.parse(m_0D_cargo_type_abbrev_id, is);       break;
            case 0x0E: desc_0E.parse(m_0E_cargo_sprite_id, is);            break;
            case 0x0F: desc_0F.parse(m_0F_single_unit_weight, is);         break;
            case 0x10: desc_10.parse(m_10_penalty_time_1, is);             break;
            case 0x11: desc_11.parse(m_11_penalty_time_2, is);             break;
            case 0x12: desc_12.parse(m_12_base_price, is);                 break;
            case 0x13: desc_13.parse(m_13_station_list_colour, is);        break;
            case 0x14: desc_14.parse(m_14_payment_list_colour, is);        break;
            case 0x15: desc_15.parse(m_15_is_freight, is);                 break;
            case 0x16: desc_16.parse(m_16_cargo_classes, is);              break;
            case 0x17: desc_17.parse(m_17_cargo_label, is);                break;
            case 0x18: desc_18.parse(m_18_town_growth_effect, is);         break;
            case 0x19: desc_19.parse(m_19_town_growth_multiplier, is);     break;
            case 0x1A: desc_1A.parse(m_1A_callback_flags, is);             break;
            case 0x1B: desc_1B.parse(m_1B_cargo_units_id, is);             break;
            case 0x1C: desc_1C.parse(m_1C_cargo_amount_id, is);            break;
            case 0x1D: desc_1D.parse(m_1D_capacity_multiplier, is);        break;
            case 0x1E: desc_1E.parse(m_1E_town_production_effect, is);     break;
            case 0x1F: desc_1F.parse(m_1F_town_production_multiplier, is); break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
