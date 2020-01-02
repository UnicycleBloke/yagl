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
#include "Action00Trains.h"


namespace {


constexpr const char* str_track_type                   = "track_type";
constexpr const char* str_ai_special_flag              = "ai_special_flag";
constexpr const char* str_speed_kmh                    = "speed_kmh";
constexpr const char* str_power                        = "power";
constexpr const char* str_running_cost_factor          = "running_cost_factor";
constexpr const char* str_running_cost_base            = "running_cost_base";
constexpr const char* str_sprite_id                    = "sprite_id";
constexpr const char* str_is_dual_headed               = "is_dual_headed";
constexpr const char* str_cargo_capacity               = "cargo_capacity";
constexpr const char* str_cargo_type                   = "cargo_type";
constexpr const char* str_weight_tons                  = "weight_tons";
constexpr const char* str_cost_factor                  = "cost_factor";
constexpr const char* str_ai_engine_rank               = "ai_engine_rank";
constexpr const char* str_engine_traction_type         = "engine_traction_type";
constexpr const char* str_sort_purchase_list           = "sort_purchase_list";
constexpr const char* str_power_from_each_wagon        = "power_from_each_wagon";
constexpr const char* str_refit_cost                   = "refit_cost";
constexpr const char* str_refit_cargo_types            = "refit_cargo_types";
constexpr const char* str_callback_flags_mask          = "callback_flags_mask";
constexpr const char* str_coeff_of_tractive_effort     = "coeff_of_tractive_effort";
constexpr const char* str_coeff_of_air_drag            = "coeff_of_air_drag";
constexpr const char* str_shorten_vehicle              = "shorten_vehicle";
constexpr const char* str_visual_effect_position       = "visual_effect_position";
constexpr const char* str_visual_effect_enum           = "visual_effect_enum";
constexpr const char* str_visual_effect_disable_wagons = "visual_effect_disable_wagons";
constexpr const char* str_weight_from_wagons           = "weight_from_wagons";
constexpr const char* str_weight_high_byte             = "weight_high_byte";
constexpr const char* str_mask_for_var_42              = "mask_for_var_42";
constexpr const char* str_retire_vehicle_early         = "retire_vehicle_early";
constexpr const char* str_miscellaneous_flags          = "miscellaneous_flags";
constexpr const char* str_refittable_cargo_classes     = "refittable_cargo_classes";
constexpr const char* str_non_refittable_cargo_classes = "non_refittable_cargo_classes";
constexpr const char* str_long_introduction_date       = "long_introduction_date";
constexpr const char* str_custom_cargo_aging_period    = "custom_cargo_aging_period";
constexpr const char* str_always_refittable_cargos     = "always_refittable_cargos";
constexpr const char* str_never_refittable_cargos      = "never_refittable_cargos";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_track_type,                   0x05'00 },
    { str_ai_special_flag,              0x08'00 },
    { str_speed_kmh,                    0x09'00 },
    { str_power,                        0x0B'00 },
    { str_running_cost_factor,          0x0D'00 },
    { str_running_cost_base,            0x0E'00 },
    { str_sprite_id,                    0x12'00 },
    { str_is_dual_headed,               0x13'00 },
    { str_cargo_capacity,               0x14'00 },
    { str_cargo_type,                   0x15'00 },
    { str_weight_tons,                  0x16'00 },
    { str_cost_factor,                  0x17'00 },
    { str_ai_engine_rank,               0x18'00 },
    { str_engine_traction_type,         0x19'00 },
    { str_sort_purchase_list,           0x1A'00 },
    { str_power_from_each_wagon,        0x1B'00 },
    { str_refit_cost,                   0x1C'00 },
    { str_refit_cargo_types,            0x1D'00 },
    { str_callback_flags_mask,          0x1E'00 },
    { str_coeff_of_tractive_effort,     0x1F'00 },
    { str_coeff_of_air_drag,            0x20'00 },
    { str_shorten_vehicle,              0x21'00 },
    { str_visual_effect_position,       0x22'00 },
    { str_visual_effect_enum,           0x22'01 },
    { str_visual_effect_disable_wagons, 0x22'02 },
    { str_weight_from_wagons,           0x23'00 },
    { str_weight_high_byte,             0x24'00 },
    { str_mask_for_var_42,              0x25'00 },
    { str_retire_vehicle_early,         0x26'00 },
    { str_miscellaneous_flags,          0x27'00 },
    { str_refittable_cargo_classes,     0x28'00 },
    { str_non_refittable_cargo_classes, 0x29'00 },
    { str_long_introduction_date,       0x2A'00 },
    { str_custom_cargo_aging_period,    0x2B'00 },    
    { str_always_refittable_cargos,     0x2C'00 },
    { str_never_refittable_cargos,      0x2D'00 },
};


// This property also depends on whether a rail type table is installed, in which case it is the index into 
// that table.    
//const     EnumDescriptor    desc_05 = { 0x05, "track_type",                   
//    {{0, "Rail"}, {1, "Monorail"}, {2, "Maglev"}} };
constexpr IntegerDescriptorT<uint8_t>  desc_05  = { 0x08, str_track_type,                   PropFormat::Dec };
constexpr BooleanDescriptor            desc_08  = { 0x08, str_ai_special_flag };
constexpr IntegerDescriptorT<uint16_t> desc_09  = { 0x09, str_speed_kmh,                    PropFormat::Dec };
constexpr IntegerDescriptorT<uint16_t> desc_0B  = { 0x0B, str_power,                        PropFormat::Dec };
constexpr IntegerDescriptorT<uint8_t>  desc_0D  = { 0x0D, str_running_cost_factor,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_0E  = { 0x0E, str_running_cost_base,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_12  = { 0x12, str_sprite_id,                    PropFormat::Hex };
constexpr BooleanDescriptor            desc_13  = { 0x13, str_is_dual_headed };
constexpr IntegerDescriptorT<uint8_t>  desc_14  = { 0x14, str_cargo_capacity,               PropFormat::Dec };
constexpr IntegerDescriptorT<uint8_t>  desc_15  = { 0x15, str_cargo_type,                   PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_16  = { 0x16, str_weight_tons,                  PropFormat::Dec };
constexpr IntegerDescriptorT<uint8_t>  desc_17  = { 0x17, str_cost_factor,                  PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_18  = { 0x18, str_ai_engine_rank,               PropFormat::Hex };
//const     RangeDescriptor              desc_19  = { 0x19, str_engine_traction_type,         
//    {{0, 7, "Steam"}, {8, 27, "Diesel"}, {28, 31, "Electric"}, {32, 37, "Monorail"}, {38, 41, "Maglev"}} };
constexpr IntegerDescriptorT<uint8_t>  desc_19  = { 0x19, str_engine_traction_type,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1A  = { 0x1A, str_sort_purchase_list,           PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1B  = { 0x1B, str_power_from_each_wagon,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_1C  = { 0x1C, str_refit_cost,                   PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_1D  = { 0x1D, str_refit_cargo_types,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_1E  = { 0x1E, str_callback_flags_mask,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_1F  = { 0x1F, str_coeff_of_tractive_effort,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_20  = { 0x20, str_coeff_of_air_drag,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_21  = { 0x21, str_shorten_vehicle,              PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_220 = { 0x22, str_visual_effect_position,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_221 = { 0x22, str_visual_effect_enum,           PropFormat::Hex };
constexpr BooleanDescriptor            desc_222 = { 0x22, str_visual_effect_disable_wagons };
constexpr IntegerDescriptorT<uint8_t>  desc_23  = { 0x23, str_weight_from_wagons,           PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_24  = { 0x24, str_weight_high_byte,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_25  = { 0x25, str_mask_for_var_42,              PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_26  = { 0x26, str_retire_vehicle_early,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_27  = { 0x27, str_miscellaneous_flags,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_28  = { 0x28, str_refittable_cargo_classes,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_29  = { 0x29, str_non_refittable_cargo_classes, PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_2A  = { 0x2A, str_long_introduction_date,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_2B  = { 0x2B, str_custom_cargo_aging_period,    PropFormat::Hex };
constexpr CargoListDescriptor          desc_2C  = { 0x2C, str_always_refittable_cargos };
constexpr CargoListDescriptor          desc_2D  = { 0x2D, str_never_refittable_cargos };


} // namespace {


bool Action00Trains::read_property(std::istream& is, uint8_t property)
{
    if (Action00Common::read_property(is, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: m_05_track_type                   = read_uint8(is); break;
        case 0x08: m_08_ai_special_flag              = (read_uint8(is) == 0x01); break;
        case 0x09: m_09_speed_kmh                    = read_uint16(is); break;
        case 0x0B: m_0B_power                        = read_uint16(is); break;
        case 0x0D: m_0D_running_cost_factor          = read_uint8(is); break;
        case 0x0E: m_0E_running_cost_base            = read_uint32(is); break;
        case 0x12: m_12_sprite_id                    = read_uint8(is); break;
        case 0x13: m_13_is_dual_headed               = (read_uint8(is) == 0x01); break;
        case 0x14: m_14_cargo_capacity               = read_uint8(is); break;
        case 0x15: m_15_cargo_type                   = read_uint8(is); break;
        case 0x16: m_16_weight_tons                  = read_uint8(is); break;
        case 0x17: m_17_cost_factor                  = read_uint8(is); break;
        case 0x18: m_18_ai_engine_rank               = read_uint8(is); break;
        case 0x19: m_19_engine_traction_type         = read_uint8(is); break;
        case 0x1A: m_1A_sort_purchase_list           = read_uint8_ext(is); break;
        case 0x1B: m_1B_power_from_each_wagon        = read_uint16(is); break;
        case 0x1C: m_1C_refit_cost                   = read_uint8(is); break;
        case 0x1D: m_1D_refit_cargo_types            = read_uint32(is); break;
        case 0x1E: m_1E_callback_flags_mask          = read_uint8(is); break;
        case 0x1F: m_1F_coeff_of_tractive_effort     = read_uint8(is); break;
        case 0x20: m_20_coeff_of_air_drag            = read_uint8(is); break;
        case 0x21: m_21_shorten_vehicle              = read_uint8(is); break;
        case 0x22: m_22_visual_effect_position       = read_uint8(is);
                   m_22_visual_effect_enum           = (m_22_visual_effect_position & 0x70);
                   m_22_visual_effect_disable_wagons = ((m_22_visual_effect_position & 0x80) != 0x00);
                   m_22_visual_effect_position      &= 0x0F; break;
        case 0x23: m_23_weight_from_wagons           = read_uint8(is); break;
        case 0x24: m_24_weight_high_byte             = read_uint8(is); break;
        case 0x25: m_25_mask_for_var_42              = read_uint8(is); break;
        case 0x26: m_26_retire_vehicle_early         = read_uint8(is); break;
        case 0x27: m_27_miscellaneous_flags          = read_uint8(is); break;
        case 0x28: m_28_refittable_cargo_classes     = read_uint16(is); break;
        case 0x29: m_29_non_refittable_cargo_classes = read_uint16(is); break;
        case 0x2A: m_2A_long_introduction_date       = read_uint32(is); break;
        case 0x2B: m_2B_custom_cargo_aging_period    = read_uint16(is); break;
        case 0x2C: m_2C_always_refittable_cargos.read(is); break;
        case 0x2D: m_2D_never_refittable_cargos.read(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Trains::write_property(std::ostream& os, uint8_t property) const
{
    if (Action00Common::write_property(os, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: write_uint8(os, m_05_track_type); break;
        case 0x08: write_uint8(os, m_08_ai_special_flag ? 0x01 : 0x00); break;
        case 0x09: write_uint16(os, m_09_speed_kmh); break;
        case 0x0B: write_uint16(os, m_0B_power); break;
        case 0x0D: write_uint8(os, m_0D_running_cost_factor); break;
        case 0x0E: write_uint32(os, m_0E_running_cost_base); break;
        case 0x12: write_uint8(os, m_12_sprite_id); break;
        case 0x13: write_uint8(os, m_13_is_dual_headed ? 0x01 : 0x00); break;
        case 0x14: write_uint8(os, m_14_cargo_capacity); break;
        case 0x15: write_uint8(os, m_15_cargo_type); break;
        case 0x16: write_uint8(os, m_16_weight_tons); break;
        case 0x17: write_uint8(os, m_17_cost_factor); break;
        case 0x18: write_uint8(os, m_18_ai_engine_rank); break;
        case 0x19: write_uint8(os, m_19_engine_traction_type); break;
        case 0x1A: write_uint8_ext(os, m_1A_sort_purchase_list); break;
        case 0x1B: write_uint16(os, m_1B_power_from_each_wagon); break;
        case 0x1C: write_uint8(os, m_1C_refit_cost); break;
        case 0x1D: write_uint32(os, m_1D_refit_cargo_types); break;
        case 0x1E: write_uint8(os, m_1E_callback_flags_mask); break;
        case 0x1F: write_uint8(os, m_1F_coeff_of_tractive_effort); break;
        case 0x20: write_uint8(os, m_20_coeff_of_air_drag); break;
        case 0x21: write_uint8(os, m_21_shorten_vehicle); break;
        case 0x22: write_uint8(os, m_22_visual_effect_position | 
                                   m_22_visual_effect_enum | 
                                   (m_22_visual_effect_disable_wagons ? 0x80 : 0x00)); break;
        case 0x23: write_uint8(os, m_23_weight_from_wagons); break;
        case 0x24: write_uint8(os, m_24_weight_high_byte); break;
        case 0x25: write_uint8(os, m_25_mask_for_var_42); break;
        case 0x26: write_uint8(os, m_26_retire_vehicle_early); break;
        case 0x27: write_uint8(os, m_27_miscellaneous_flags); break;
        case 0x28: write_uint16(os, m_28_refittable_cargo_classes); break;
        case 0x29: write_uint16(os, m_29_non_refittable_cargo_classes); break;
        case 0x2A: write_uint32(os, m_2A_long_introduction_date); break;
        case 0x2B: write_uint16(os, m_2B_custom_cargo_aging_period); break;        
        case 0x2C: m_2C_always_refittable_cargos.write(os); break;
        case 0x2D: m_2D_never_refittable_cargos.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Trains::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    if (Action00Common::print_property(os, property, indent))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: desc_05.print(m_05_track_type, os, indent); break;
        case 0x08: desc_08.print(m_08_ai_special_flag, os, indent); break;
        case 0x09: desc_09.print(m_09_speed_kmh, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_power, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_running_cost_factor, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_running_cost_base, os, indent); break;
        case 0x12: desc_12.print(m_12_sprite_id, os, indent); break;
        case 0x13: desc_13.print(m_13_is_dual_headed, os, indent); break;
        case 0x14: desc_14.print(m_14_cargo_capacity, os, indent); break;
        case 0x15: desc_15.print(m_15_cargo_type, os, indent); break;
        case 0x16: desc_16.print(m_16_weight_tons, os, indent); break;
        case 0x17: desc_17.print(m_17_cost_factor, os, indent); break;
        case 0x18: desc_18.print(m_18_ai_engine_rank, os, indent); break;
        case 0x19: desc_19.print(m_19_engine_traction_type, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_sort_purchase_list, os, indent); break;
        case 0x1B: desc_1B.print(m_1B_power_from_each_wagon, os, indent); break;
        case 0x1C: desc_1C.print(m_1C_refit_cost, os, indent); break;
        case 0x1D: desc_1D.print(m_1D_refit_cargo_types, os, indent); break;
        case 0x1E: desc_1E.print(m_1E_callback_flags_mask, os, indent); break;
        case 0x1F: desc_1F.print(m_1F_coeff_of_tractive_effort, os, indent); break;
        case 0x20: desc_20.print(m_20_coeff_of_air_drag, os, indent); break;
        case 0x21: desc_21.print(m_21_shorten_vehicle, os, indent); break;
        case 0x22: desc_220.print(m_22_visual_effect_position, os, indent);
                   desc_221.print(m_22_visual_effect_enum, os, indent); 
                   desc_222.print(m_22_visual_effect_disable_wagons, os, indent); break;
        case 0x23: desc_23.print(m_23_weight_from_wagons, os, indent); break;
        case 0x24: desc_24.print(m_24_weight_high_byte, os, indent); break;
        case 0x25: desc_25.print(m_25_mask_for_var_42, os, indent); break;
        case 0x26: desc_26.print(m_26_retire_vehicle_early, os, indent); break;
        case 0x27: desc_27.print(m_27_miscellaneous_flags, os, indent); break;
        case 0x28: desc_28.print(m_28_refittable_cargo_classes, os, indent); break;
        case 0x29: desc_29.print(m_29_non_refittable_cargo_classes, os, indent); break;
        case 0x2A: desc_2A.print(m_2A_long_introduction_date, os, indent); break;
        case 0x2B: desc_2B.print(m_2B_custom_cargo_aging_period, os, indent); break;
        case 0x2C: desc_2C.print(m_2C_always_refittable_cargos, os, indent); break;
        case 0x2D: desc_2D.print(m_2D_never_refittable_cargos, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Trains::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    if (Action00Common::parse_property(is, name, property))
    {
        return true;
    }

    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x05'00: desc_05.parse(m_05_track_type, is); break;
            case 0x08'00: desc_08.parse(m_08_ai_special_flag, is); break;
            case 0x09'00: desc_09.parse(m_09_speed_kmh, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_power, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_running_cost_factor, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_running_cost_base, is); break;
            case 0x12'00: desc_12.parse(m_12_sprite_id, is); break;
            case 0x13'00: desc_13.parse(m_13_is_dual_headed, is); break;
            case 0x14'00: desc_14.parse(m_14_cargo_capacity, is); break;
            case 0x15'00: desc_15.parse(m_15_cargo_type, is); break;
            case 0x16'00: desc_16.parse(m_16_weight_tons, is); break;
            case 0x17'00: desc_17.parse(m_17_cost_factor, is); break;
            case 0x18'00: desc_18.parse(m_18_ai_engine_rank, is); break;
            case 0x19'00: desc_19.parse(m_19_engine_traction_type, is); break;
            case 0x1A'00: desc_1A.parse(m_1A_sort_purchase_list, is); break;
            case 0x1B'00: desc_1B.parse(m_1B_power_from_each_wagon, is); break;
            case 0x1C'00: desc_1C.parse(m_1C_refit_cost, is); break;
            case 0x1D'00: desc_1D.parse(m_1D_refit_cargo_types, is); break;
            case 0x1E'00: desc_1E.parse(m_1E_callback_flags_mask, is); break;
            case 0x1F'00: desc_1F.parse(m_1F_coeff_of_tractive_effort, is); break;
            case 0x20'00: desc_20.parse(m_20_coeff_of_air_drag, is); break;
            case 0x21'00: desc_21.parse(m_21_shorten_vehicle, is); break;
            case 0x22'00: desc_220.parse(m_22_visual_effect_position, is); 
            case 0x22'01: desc_221.parse(m_22_visual_effect_enum, is); 
            case 0x22'02: desc_222.parse(m_22_visual_effect_disable_wagons, is); break;
            case 0x23'00: desc_23.parse(m_23_weight_from_wagons, is); break;
            case 0x24'00: desc_24.parse(m_24_weight_high_byte, is); break;
            case 0x25'00: desc_25.parse(m_25_mask_for_var_42, is); break;
            case 0x26'00: desc_26.parse(m_26_retire_vehicle_early, is); break;
            case 0x27'00: desc_27.parse(m_27_miscellaneous_flags, is); break;
            case 0x28'00: desc_28.parse(m_28_refittable_cargo_classes, is); break;
            case 0x29'00: desc_29.parse(m_29_non_refittable_cargo_classes, is); break;
            case 0x2A'00: desc_2A.parse(m_2A_long_introduction_date, is); break;
            case 0x2B'00: desc_2B.parse(m_2B_custom_cargo_aging_period, is); break;
            case 0x2C'00: desc_2C.parse(m_2C_always_refittable_cargos, is); break;
            case 0x2D'00: desc_2D.parse(m_2D_never_refittable_cargos, is); break;
            default:      throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
