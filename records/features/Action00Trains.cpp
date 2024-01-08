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
constexpr const char* str_visual_effect                = "visual_effect";
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
constexpr const char* str_maximum_curve_speed_modifier = "maximum_curve_speed_modifier";
constexpr const char* str_variant_group                = "variant_group";
constexpr const char* str_extra_flags                  = "extra_flags";
constexpr const char* str_extra_callback_flags_mask    = "extra_callback_flags_mask";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_track_type,                   0x05 },
    { str_ai_special_flag,              0x08 },
    { str_speed_kmh,                    0x09 },
    { str_power,                        0x0B },
    { str_running_cost_factor,          0x0D },
    { str_running_cost_base,            0x0E },
    { str_sprite_id,                    0x12 },
    { str_is_dual_headed,               0x13 },
    { str_cargo_capacity,               0x14 },
    { str_cargo_type,                   0x15 },
    { str_weight_tons,                  0x16 },
    { str_cost_factor,                  0x17 },
    { str_ai_engine_rank,               0x18 },
    { str_engine_traction_type,         0x19 },
    { str_sort_purchase_list,           0x1A },
    { str_power_from_each_wagon,        0x1B },
    { str_refit_cost,                   0x1C },
    { str_refit_cargo_types,            0x1D },
    { str_callback_flags_mask,          0x1E },
    { str_coeff_of_tractive_effort,     0x1F },
    { str_coeff_of_air_drag,            0x20 },
    { str_shorten_vehicle,              0x21 },
    { str_visual_effect,                0x22 },
    { str_weight_from_wagons,           0x23 },
    { str_weight_high_byte,             0x24 },
    { str_mask_for_var_42,              0x25 },
    { str_retire_vehicle_early,         0x26 },
    { str_miscellaneous_flags,          0x27 },
    { str_refittable_cargo_classes,     0x28 },
    { str_non_refittable_cargo_classes, 0x29 },
    { str_long_introduction_date,       0x2A },
    { str_custom_cargo_aging_period,    0x2B },
    { str_always_refittable_cargos,     0x2C },
    { str_never_refittable_cargos,      0x2D },
    { str_maximum_curve_speed_modifier, 0x2E },
    { str_variant_group,                0x2F },
    { str_extra_flags,                  0x30 },
    { str_extra_callback_flags_mask,    0x31 },
};


constexpr UInt8Descriptor         desc_05  = { 0x08, str_track_type,                   UIntFormat::Dec };
constexpr BoolDescriptor          desc_08  = { 0x08, str_ai_special_flag };
constexpr UInt16Descriptor        desc_09  = { 0x09, str_speed_kmh,                    UIntFormat::Dec };
constexpr UInt16Descriptor        desc_0B  = { 0x0B, str_power,                        UIntFormat::Dec };
constexpr UInt8Descriptor         desc_0D  = { 0x0D, str_running_cost_factor,          UIntFormat::Hex };
constexpr UInt32Descriptor        desc_0E  = { 0x0E, str_running_cost_base,            UIntFormat::Hex };
constexpr UInt8Descriptor         desc_12  = { 0x12, str_sprite_id,                    UIntFormat::Hex };
constexpr BoolDescriptor          desc_13  = { 0x13, str_is_dual_headed };
constexpr UInt8Descriptor         desc_14  = { 0x14, str_cargo_capacity,               UIntFormat::Dec };
constexpr UInt8Descriptor         desc_15  = { 0x15, str_cargo_type,                   UIntFormat::Hex };
constexpr UInt8Descriptor         desc_16  = { 0x16, str_weight_tons,                  UIntFormat::Dec };
constexpr UInt8Descriptor         desc_17  = { 0x17, str_cost_factor,                  UIntFormat::Hex };
constexpr UInt8Descriptor         desc_18  = { 0x18, str_ai_engine_rank,               UIntFormat::Hex };
constexpr UInt8Descriptor         desc_19  = { 0x19, str_engine_traction_type,         UIntFormat::Hex };
constexpr UInt8ExtDescriptor      desc_1A  = { 0x1A, str_sort_purchase_list,           UIntFormat::Hex };
constexpr UInt16Descriptor        desc_1B  = { 0x1B, str_power_from_each_wagon,        UIntFormat::Hex };
constexpr UInt8Descriptor         desc_1C  = { 0x1C, str_refit_cost,                   UIntFormat::Hex };
constexpr UInt32Descriptor        desc_1D  = { 0x1D, str_refit_cargo_types,            UIntFormat::Hex };
constexpr UInt8Descriptor         desc_1E  = { 0x1E, str_callback_flags_mask,          UIntFormat::Hex };
constexpr UInt8Descriptor         desc_1F  = { 0x1F, str_coeff_of_tractive_effort,     UIntFormat::Hex };
constexpr UInt8Descriptor         desc_20  = { 0x20, str_coeff_of_air_drag,            UIntFormat::Hex };
constexpr UInt8Descriptor         desc_21  = { 0x21, str_shorten_vehicle,              UIntFormat::Hex };
constexpr VisualEffectDescriptor  desc_22  = { 0x22, str_visual_effect };
constexpr UInt8Descriptor         desc_23  = { 0x23, str_weight_from_wagons,           UIntFormat::Hex };
constexpr UInt8Descriptor         desc_24  = { 0x24, str_weight_high_byte,             UIntFormat::Hex };
constexpr UInt8Descriptor         desc_25  = { 0x25, str_mask_for_var_42,              UIntFormat::Hex };
constexpr UInt8Descriptor         desc_26  = { 0x26, str_retire_vehicle_early,         UIntFormat::Hex };
constexpr UInt8Descriptor         desc_27  = { 0x27, str_miscellaneous_flags,          UIntFormat::Hex };
constexpr UInt16Descriptor        desc_28  = { 0x28, str_refittable_cargo_classes,     UIntFormat::Hex };
constexpr UInt16Descriptor        desc_29  = { 0x29, str_non_refittable_cargo_classes, UIntFormat::Hex };
constexpr LongDateDescriptor      desc_2A  = { 0x2A, str_long_introduction_date };
constexpr UInt16Descriptor        desc_2B  = { 0x2B, str_custom_cargo_aging_period,    UIntFormat::Hex };
constexpr CargoListDescriptor     desc_2C  = { 0x2C, str_always_refittable_cargos };
constexpr CargoListDescriptor     desc_2D  = { 0x2D, str_never_refittable_cargos };
constexpr UInt16Descriptor        desc_2E  = { 0x2E, str_maximum_curve_speed_modifier, UIntFormat::Hex };
constexpr UInt16Descriptor        desc_2F  = { 0x2F, str_variant_group,                UIntFormat::Hex };
constexpr UInt32Descriptor        desc_30  = { 0x30, str_extra_flags,                  UIntFormat::Hex };
constexpr UInt8Descriptor         desc_31  = { 0x31, str_extra_callback_flags_mask,    UIntFormat::Hex };


} // namespace {


bool Action00Trains::read_property(std::istream& is, uint8_t property)
{
    if (Action00Common::read_property(is, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: m_05_track_type.read(is);                   break;
        case 0x08: m_08_ai_special_flag.read(is);              break;
        case 0x09: m_09_speed_kmh.read(is);                    break;
        case 0x0B: m_0B_power.read(is);                        break;
        case 0x0D: m_0D_running_cost_factor.read(is);          break;
        case 0x0E: m_0E_running_cost_base.read(is);            break;
        case 0x12: m_12_sprite_id.read(is);                    break;
        case 0x13: m_13_is_dual_headed.read(is);               break;
        case 0x14: m_14_cargo_capacity.read(is);               break;
        case 0x15: m_15_cargo_type.read(is);                   break;
        case 0x16: m_16_weight_tons.read(is);                  break;
        case 0x17: m_17_cost_factor.read(is);                  break;
        case 0x18: m_18_ai_engine_rank.read(is);               break;
        case 0x19: m_19_engine_traction_type.read(is);         break;
        case 0x1A: m_1A_sort_purchase_list.read(is);           break;
        case 0x1B: m_1B_power_from_each_wagon.read(is);        break;
        case 0x1C: m_1C_refit_cost.read(is);                   break;
        case 0x1D: m_1D_refit_cargo_types.read(is);            break;
        case 0x1E: m_1E_callback_flags_mask.read(is);          break;
        case 0x1F: m_1F_coeff_of_tractive_effort.read(is);     break;
        case 0x20: m_20_coeff_of_air_drag.read(is);            break;
        case 0x21: m_21_shorten_vehicle.read(is);              break;
        case 0x22: m_22_visual_effect.read(is);                break;
        case 0x23: m_23_weight_from_wagons.read(is);           break;
        case 0x24: m_24_weight_high_byte.read(is);             break;
        case 0x25: m_25_mask_for_var_42.read(is);              break;
        case 0x26: m_26_retire_vehicle_early.read(is);         break;
        case 0x27: m_27_miscellaneous_flags.read(is);          break;
        case 0x28: m_28_refittable_cargo_classes.read(is);     break;
        case 0x29: m_29_non_refittable_cargo_classes.read(is); break;
        case 0x2A: m_2A_long_introduction_date.read(is);       break;
        case 0x2B: m_2B_custom_cargo_aging_period.read(is);    break;
        case 0x2C: m_2C_always_refittable_cargos.read(is);     break;
        case 0x2D: m_2D_never_refittable_cargos.read(is);      break;
        case 0x2E: m_2E_maximum_curve_speed_modifier.read(is); break;
        case 0x2F: m_2F_variant_group.read(is);                break;
        case 0x30: m_30_extra_flags.read(is);                  break;
        case 0x31: m_31_extra_callback_flags_mask.read(is);    break;
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
        case 0x05: m_05_track_type.write(os);                   break;
        case 0x08: m_08_ai_special_flag.write(os);              break;
        case 0x09: m_09_speed_kmh.write(os);                    break;
        case 0x0B: m_0B_power.write(os);                        break;
        case 0x0D: m_0D_running_cost_factor.write(os);          break;
        case 0x0E: m_0E_running_cost_base.write(os);            break;
        case 0x12: m_12_sprite_id.write(os);                    break;
        case 0x13: m_13_is_dual_headed.write(os);               break;
        case 0x14: m_14_cargo_capacity.write(os);               break;
        case 0x15: m_15_cargo_type.write(os);                   break;
        case 0x16: m_16_weight_tons.write(os);                  break;
        case 0x17: m_17_cost_factor.write(os);                  break;
        case 0x18: m_18_ai_engine_rank.write(os);               break;
        case 0x19: m_19_engine_traction_type.write(os);         break;
        case 0x1A: m_1A_sort_purchase_list.write(os);           break;
        case 0x1B: m_1B_power_from_each_wagon.write(os);        break;
        case 0x1C: m_1C_refit_cost.write(os);                   break;
        case 0x1D: m_1D_refit_cargo_types.write(os);            break;
        case 0x1E: m_1E_callback_flags_mask.write(os);          break;
        case 0x1F: m_1F_coeff_of_tractive_effort.write(os);     break;
        case 0x20: m_20_coeff_of_air_drag.write(os);            break;
        case 0x21: m_21_shorten_vehicle.write(os);              break;
        case 0x22: m_22_visual_effect.write(os);                break;
        case 0x23: m_23_weight_from_wagons.write(os);           break;
        case 0x24: m_24_weight_high_byte.write(os);             break;
        case 0x25: m_25_mask_for_var_42.write(os);              break;
        case 0x26: m_26_retire_vehicle_early.write(os);         break;
        case 0x27: m_27_miscellaneous_flags.write(os);          break;
        case 0x28: m_28_refittable_cargo_classes.write(os);     break;
        case 0x29: m_29_non_refittable_cargo_classes.write(os); break;
        case 0x2A: m_2A_long_introduction_date.write(os);       break;
        case 0x2B: m_2B_custom_cargo_aging_period.write(os);    break;
        case 0x2C: m_2C_always_refittable_cargos.write(os);     break;
        case 0x2D: m_2D_never_refittable_cargos.write(os);      break;
        case 0x2E: m_2E_maximum_curve_speed_modifier.write(os); break;
        case 0x2F: m_2F_variant_group.write(os);                break;
        case 0x30: m_30_extra_flags.write(os);                  break;
        case 0x31: m_31_extra_callback_flags_mask.write(os);    break;
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
        case 0x05: desc_05.print(m_05_track_type, os, indent);                   break;
        case 0x08: desc_08.print(m_08_ai_special_flag, os, indent);              break;
        case 0x09: desc_09.print(m_09_speed_kmh, os, indent);                    break;
        case 0x0B: desc_0B.print(m_0B_power, os, indent);                        break;
        case 0x0D: desc_0D.print(m_0D_running_cost_factor, os, indent);          break;
        case 0x0E: desc_0E.print(m_0E_running_cost_base, os, indent);            break;
        case 0x12: desc_12.print(m_12_sprite_id, os, indent);                    break;
        case 0x13: desc_13.print(m_13_is_dual_headed, os, indent);               break;
        case 0x14: desc_14.print(m_14_cargo_capacity, os, indent);               break;
        case 0x15: desc_15.print(m_15_cargo_type, os, indent);                   break;
        case 0x16: desc_16.print(m_16_weight_tons, os, indent);                  break;
        case 0x17: desc_17.print(m_17_cost_factor, os, indent);                  break;
        case 0x18: desc_18.print(m_18_ai_engine_rank, os, indent);               break;
        case 0x19: desc_19.print(m_19_engine_traction_type, os, indent);         break;
        case 0x1A: desc_1A.print(m_1A_sort_purchase_list, os, indent);           break;
        case 0x1B: desc_1B.print(m_1B_power_from_each_wagon, os, indent);        break;
        case 0x1C: desc_1C.print(m_1C_refit_cost, os, indent);                   break;
        case 0x1D: desc_1D.print(m_1D_refit_cargo_types, os, indent);            break;
        case 0x1E: desc_1E.print(m_1E_callback_flags_mask, os, indent);          break;
        case 0x1F: desc_1F.print(m_1F_coeff_of_tractive_effort, os, indent);     break;
        case 0x20: desc_20.print(m_20_coeff_of_air_drag, os, indent);            break;
        case 0x21: desc_21.print(m_21_shorten_vehicle, os, indent);              break;
        case 0x22: desc_22.print(m_22_visual_effect, os, indent);                break;
        case 0x23: desc_23.print(m_23_weight_from_wagons, os, indent);           break;
        case 0x24: desc_24.print(m_24_weight_high_byte, os, indent);             break;
        case 0x25: desc_25.print(m_25_mask_for_var_42, os, indent);              break;
        case 0x26: desc_26.print(m_26_retire_vehicle_early, os, indent);         break;
        case 0x27: desc_27.print(m_27_miscellaneous_flags, os, indent);          break;
        case 0x28: desc_28.print(m_28_refittable_cargo_classes, os, indent);     break;
        case 0x29: desc_29.print(m_29_non_refittable_cargo_classes, os, indent); break;
        case 0x2A: desc_2A.print(m_2A_long_introduction_date, os, indent);       break;
        case 0x2B: desc_2B.print(m_2B_custom_cargo_aging_period, os, indent);    break;
        case 0x2C: desc_2C.print(m_2C_always_refittable_cargos, os, indent);     break;
        case 0x2D: desc_2D.print(m_2D_never_refittable_cargos, os, indent);      break;
        case 0x2E: desc_2E.print(m_2E_maximum_curve_speed_modifier, os, indent); break;
        case 0x2F: desc_2F.print(m_2F_variant_group, os, indent);                break;
        case 0x30: desc_30.print(m_30_extra_flags, os, indent);                  break;
        case 0x31: desc_31.print(m_31_extra_callback_flags_mask, os, indent);    break;
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
        property = it->second;
        switch (property)
        {
            case 0x05: desc_05.parse(m_05_track_type, is);                   break;
            case 0x08: desc_08.parse(m_08_ai_special_flag, is);              break;
            case 0x09: desc_09.parse(m_09_speed_kmh, is);                    break;
            case 0x0B: desc_0B.parse(m_0B_power, is);                        break;
            case 0x0D: desc_0D.parse(m_0D_running_cost_factor, is);          break;
            case 0x0E: desc_0E.parse(m_0E_running_cost_base, is);            break;
            case 0x12: desc_12.parse(m_12_sprite_id, is);                    break;
            case 0x13: desc_13.parse(m_13_is_dual_headed, is);               break;
            case 0x14: desc_14.parse(m_14_cargo_capacity, is);               break;
            case 0x15: desc_15.parse(m_15_cargo_type, is);                   break;
            case 0x16: desc_16.parse(m_16_weight_tons, is);                  break;
            case 0x17: desc_17.parse(m_17_cost_factor, is);                  break;
            case 0x18: desc_18.parse(m_18_ai_engine_rank, is);               break;
            case 0x19: desc_19.parse(m_19_engine_traction_type, is);         break;
            case 0x1A: desc_1A.parse(m_1A_sort_purchase_list, is);           break;
            case 0x1B: desc_1B.parse(m_1B_power_from_each_wagon, is);        break;
            case 0x1C: desc_1C.parse(m_1C_refit_cost, is);                   break;
            case 0x1D: desc_1D.parse(m_1D_refit_cargo_types, is);            break;
            case 0x1E: desc_1E.parse(m_1E_callback_flags_mask, is);          break;
            case 0x1F: desc_1F.parse(m_1F_coeff_of_tractive_effort, is);     break;
            case 0x20: desc_20.parse(m_20_coeff_of_air_drag, is);            break;
            case 0x21: desc_21.parse(m_21_shorten_vehicle, is);              break;
            case 0x22: desc_22.parse(m_22_visual_effect, is);                break;
            case 0x23: desc_23.parse(m_23_weight_from_wagons, is);           break;
            case 0x24: desc_24.parse(m_24_weight_high_byte, is);             break;
            case 0x25: desc_25.parse(m_25_mask_for_var_42, is);              break;
            case 0x26: desc_26.parse(m_26_retire_vehicle_early, is);         break;
            case 0x27: desc_27.parse(m_27_miscellaneous_flags, is);          break;
            case 0x28: desc_28.parse(m_28_refittable_cargo_classes, is);     break;
            case 0x29: desc_29.parse(m_29_non_refittable_cargo_classes, is); break;
            case 0x2A: desc_2A.parse(m_2A_long_introduction_date, is);       break;
            case 0x2B: desc_2B.parse(m_2B_custom_cargo_aging_period, is);    break;
            case 0x2C: desc_2C.parse(m_2C_always_refittable_cargos, is);     break;
            case 0x2D: desc_2D.parse(m_2D_never_refittable_cargos, is);      break;
            case 0x2E: desc_2E.parse(m_2E_maximum_curve_speed_modifier, is); break;
            case 0x2F: desc_2F.parse(m_2F_variant_group, is);                break;
            case 0x30: desc_30.parse(m_30_extra_flags, is);                  break;
            case 0x31: desc_31.parse(m_31_extra_callback_flags_mask, is);    break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
