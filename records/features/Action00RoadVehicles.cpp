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
#include "Action00RoadVehicles.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_roadtype_tramtype            = "roadtype_tramtype";
constexpr const char* str_speed_2_kmh                  = "speed_2_kmh";
constexpr const char* str_running_cost_factor          = "running_cost_factor";
constexpr const char* str_running_cost_base            = "running_cost_base";
constexpr const char* str_sprite_id                    = "sprite_id";
constexpr const char* str_cargo_capacity               = "cargo_capacity";
constexpr const char* str_cargo_type                   = "cargo_type";
constexpr const char* str_cost_factor                  = "cost_factor";
constexpr const char* str_sound_effect_type            = "sound_effect_type";
constexpr const char* str_power_10_hp                  = "power_10_hp";
constexpr const char* str_weight_quarter_tons          = "weight_quarter_tons";
constexpr const char* str_speed_half_kmh               = "speed_half_kmh";
constexpr const char* str_refit_cargo_types            = "refit_cargo_types";
constexpr const char* str_callback_flags_mask          = "callback_flags_mask";
constexpr const char* str_coeff_of_tractive_effort     = "coeff_of_tractive_effort";
constexpr const char* str_coeff_of_air_drag            = "coeff_of_air_drag";
constexpr const char* str_refit_cost                   = "refit_cost";
constexpr const char* str_retire_vehicle_early         = "retire_vehicle_early";
constexpr const char* str_miscellaneous_flags          = "miscellaneous_flags";
constexpr const char* str_refittable_cargo_classes     = "refittable_cargo_classes";
constexpr const char* str_non_refittable_cargo_classes = "non_refittable_cargo_classes";
constexpr const char* str_long_introduction_date       = "long_introduction_date";
constexpr const char* str_sort_purchase_list           = "sort_purchase_list";
constexpr const char* str_visual_effect                = "visual_effect";
constexpr const char* str_custom_cargo_aging_period    = "custom_cargo_aging_period";
constexpr const char* str_shorten_vehicle              = "shorten_vehicle";
constexpr const char* str_always_refittable_cargos     = "always_refittable_cargos";
constexpr const char* str_never_refittable_cargos      = "never_refittable_cargos";
constexpr const char* str_variant_group                = "variant_group";
constexpr const char* str_extra_flags                  = "extra_flags";
constexpr const char* str_extra_callback_flags_mask    = "extra_callback_flags_mask";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_roadtype_tramtype,            0x05 },
    { str_speed_2_kmh,                  0x08 },
    { str_running_cost_factor,          0x09 },
    { str_running_cost_base,            0x0A },
    { str_sprite_id,                    0x0E },
    { str_cargo_capacity,               0x0F },
    { str_cargo_type,                   0x10 },
    { str_cost_factor,                  0x11 },
    { str_sound_effect_type,            0x12 },
    { str_power_10_hp,                  0x13 },
    { str_weight_quarter_tons,          0x14 },
    { str_speed_half_kmh,               0x15 },
    { str_refit_cargo_types,            0x16 },
    { str_callback_flags_mask,          0x17 },
    { str_coeff_of_tractive_effort,     0x18 },
    { str_coeff_of_air_drag,            0x19 },
    { str_refit_cost,                   0x1A },
    { str_retire_vehicle_early,         0x1B },
    { str_miscellaneous_flags,          0x1C },
    { str_refittable_cargo_classes,     0x1D },
    { str_non_refittable_cargo_classes, 0x1E },
    { str_long_introduction_date,       0x1F },
    { str_sort_purchase_list,           0x20 },
    { str_visual_effect,                0x21 },
    { str_custom_cargo_aging_period,    0x22 },
    { str_shorten_vehicle,              0x23 },
    { str_always_refittable_cargos,     0x24 },
    { str_never_refittable_cargos,      0x25 },
    { str_variant_group,                0x26 },
    { str_extra_flags,                  0x27 },
    { str_extra_callback_flags_mask,    0x28 },
};


constexpr UInt8Descriptor        desc_05  = { 0x05, str_roadtype_tramtype,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_08  = { 0x08, str_speed_2_kmh,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_09  = { 0x09, str_running_cost_factor,          UIntFormat::Hex };
constexpr UInt32Descriptor       desc_0A  = { 0x0A, str_running_cost_base,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_0E  = { 0x0E, str_sprite_id,                    UIntFormat::Hex };
constexpr UInt8Descriptor        desc_0F  = { 0x0F, str_cargo_capacity,               UIntFormat::Hex };
constexpr UInt8Descriptor        desc_10  = { 0x10, str_cargo_type,                   UIntFormat::Hex };
constexpr UInt8Descriptor        desc_11  = { 0x11, str_cost_factor,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_12  = { 0x12, str_sound_effect_type,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_13  = { 0x13, str_power_10_hp,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_14  = { 0x14, str_weight_quarter_tons,          UIntFormat::Hex };
constexpr UInt8Descriptor        desc_15  = { 0x15, str_speed_half_kmh,               UIntFormat::Hex };
constexpr UInt32Descriptor       desc_16  = { 0x16, str_refit_cargo_types,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_17  = { 0x17, str_callback_flags_mask,          UIntFormat::Hex };
constexpr UInt8Descriptor        desc_18  = { 0x18, str_coeff_of_tractive_effort,     UIntFormat::Hex };
constexpr UInt8Descriptor        desc_19  = { 0x19, str_coeff_of_air_drag,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_1A  = { 0x1A, str_refit_cost,                   UIntFormat::Hex };
constexpr UInt8Descriptor        desc_1B  = { 0x1B, str_retire_vehicle_early,         UIntFormat::Hex };
constexpr UInt8Descriptor        desc_1C  = { 0x1C, str_miscellaneous_flags,          UIntFormat::Hex };
constexpr UInt16Descriptor       desc_1D  = { 0x1D, str_refittable_cargo_classes,     UIntFormat::Hex };
constexpr UInt16Descriptor       desc_1E  = { 0x1E, str_non_refittable_cargo_classes, UIntFormat::Hex };
constexpr LongDateDescriptor     desc_1F  = { 0x1F, str_long_introduction_date };
constexpr UInt8ExtDescriptor     desc_20  = { 0x20, str_sort_purchase_list,           UIntFormat::Hex };
constexpr VisualEffectDescriptor desc_21  = { 0x21, str_visual_effect };
constexpr UInt16Descriptor       desc_22  = { 0x22, str_custom_cargo_aging_period,    UIntFormat::Hex };
constexpr UInt8Descriptor        desc_23  = { 0x23, str_shorten_vehicle,              UIntFormat::Hex };
constexpr CargoListDescriptor    desc_24  = { 0x24, str_always_refittable_cargos };
constexpr CargoListDescriptor    desc_25  = { 0x25, str_never_refittable_cargos };
constexpr UInt16Descriptor       desc_26  = { 0x26, str_variant_group,                UIntFormat::Hex };
constexpr UInt32Descriptor       desc_27  = { 0x27, str_extra_flags,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_28  = { 0x28, str_extra_callback_flags_mask,    UIntFormat::Hex };


} // namespace {


bool Action00RoadVehicles::read_property(std::istream& is, uint8_t property)
{
    if (Action00Vehicles::read_property(is, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: m_05_roadtype_tramtype.read(is);            break;
        case 0x08: m_08_speed_2_kmh.read(is);                  break;
        case 0x09: m_09_running_cost_factor.read(is);          break;
        case 0x0A: m_0A_running_cost_base.read(is);            break;
        case 0x0E: m_0E_sprite_id.read(is);                    break;
        case 0x0F: m_0F_cargo_capacity.read(is);               break;
        case 0x10: m_10_cargo_type.read(is);                   break;
        case 0x11: m_11_cost_factor.read(is);                  break;
        case 0x12: m_12_sound_effect_type.read(is);            break;
        case 0x13: m_13_power_10_hp.read(is);                  break;
        case 0x14: m_14_weight_quarter_tons.read(is);          break;
        case 0x15: m_15_speed_half_kmh.read(is);               break;
        case 0x16: m_16_refit_cargo_types.read(is);            break;
        case 0x17: m_17_callback_flags_mask.read(is);          break;
        case 0x18: m_18_coeff_of_tractive_effort.read(is);     break;
        case 0x19: m_19_coeff_of_air_drag.read(is);            break;
        case 0x1A: m_1A_refit_cost.read(is);                   break;
        case 0x1B: m_1B_retire_vehicle_early.read(is);         break;
        case 0x1C: m_1C_miscellaneous_flags.read(is);          break;
        case 0x1D: m_1D_refittable_cargo_classes.read(is);     break;
        case 0x1E: m_1E_non_refittable_cargo_classes.read(is); break;
        case 0x1F: m_1F_long_introduction_date.read(is);       break;
        case 0x20: m_20_sort_purchase_list.read(is);           break;
        case 0x21: m_21_visual_effect.read(is);                break;
        case 0x22: m_22_custom_cargo_aging_period.read(is);    break;
        case 0x23: m_23_shorten_vehicle.read(is);              break;
        case 0x24: m_24_always_refittable_cargos.read(is);     break;
        case 0x25: m_25_never_refittable_cargos.read(is);      break;
        case 0x26: m_26_variant_group.read(is);                break;
        case 0x27: m_27_extra_flags.read(is);                  break;
        case 0x28: m_28_extra_callback_flags_mask.read(is);    break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadVehicles::write_property(std::ostream& os, uint8_t property) const
{
    if (Action00Vehicles::write_property(os, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: m_05_roadtype_tramtype.write(os);            break;
        case 0x08: m_08_speed_2_kmh.write(os);                  break;
        case 0x09: m_09_running_cost_factor.write(os);          break;
        case 0x0A: m_0A_running_cost_base.write(os);            break;
        case 0x0E: m_0E_sprite_id.write(os);                    break;
        case 0x0F: m_0F_cargo_capacity.write(os);               break;
        case 0x10: m_10_cargo_type.write(os);                   break;
        case 0x11: m_11_cost_factor.write(os);                  break;
        case 0x12: m_12_sound_effect_type.write(os);            break;
        case 0x13: m_13_power_10_hp.write(os);                  break;
        case 0x14: m_14_weight_quarter_tons.write(os);          break;
        case 0x15: m_15_speed_half_kmh.write(os);               break;
        case 0x16: m_16_refit_cargo_types.write(os);            break;
        case 0x17: m_17_callback_flags_mask.write(os);          break;
        case 0x18: m_18_coeff_of_tractive_effort.write(os);     break;
        case 0x19: m_19_coeff_of_air_drag.write(os);            break;
        case 0x1A: m_1A_refit_cost.write(os);                   break;
        case 0x1B: m_1B_retire_vehicle_early.write(os);         break;
        case 0x1C: m_1C_miscellaneous_flags.write(os);          break;
        case 0x1D: m_1D_refittable_cargo_classes.write(os);     break;
        case 0x1E: m_1E_non_refittable_cargo_classes.write(os); break;
        case 0x1F: m_1F_long_introduction_date.write(os);       break;
        case 0x20: m_20_sort_purchase_list.write(os);           break;
        case 0x21: m_21_visual_effect.write(os);                break;
        case 0x22: m_22_custom_cargo_aging_period.write(os);    break;
        case 0x23: m_23_shorten_vehicle.write(os);              break;
        case 0x24: m_24_always_refittable_cargos.write(os);     break;
        case 0x25: m_25_never_refittable_cargos.write(os);      break;
        case 0x26: m_26_variant_group.write(os);                break;
        case 0x27: m_27_extra_flags.write(os);                  break;
        case 0x28: m_28_extra_callback_flags_mask.write(os);    break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadVehicles::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    if (Action00Vehicles::print_property(os, property, indent))
    {
        return true;
    }

    switch (property)
    {
        case 0x05: desc_05.print(m_05_roadtype_tramtype, os, indent);            break;
        case 0x08: desc_08.print(m_08_speed_2_kmh, os, indent);                  break;
        case 0x09: desc_09.print(m_09_running_cost_factor, os, indent);          break;
        case 0x0A: desc_0A.print(m_0A_running_cost_base, os, indent);            break;
        case 0x0E: desc_0E.print(m_0E_sprite_id, os, indent);                    break;
        case 0x0F: desc_0F.print(m_0F_cargo_capacity, os, indent);               break;
        case 0x10: desc_10.print(m_10_cargo_type, os, indent);                   break;
        case 0x11: desc_11.print(m_11_cost_factor, os, indent);                  break;
        case 0x12: desc_12.print(m_12_sound_effect_type, os, indent);            break;
        case 0x13: desc_13.print(m_13_power_10_hp, os, indent);                  break;
        case 0x14: desc_14.print(m_14_weight_quarter_tons, os, indent);          break;
        case 0x15: desc_15.print(m_15_speed_half_kmh, os, indent);               break;
        case 0x16: desc_16.print(m_16_refit_cargo_types, os, indent);            break;
        case 0x17: desc_17.print(m_17_callback_flags_mask, os, indent);          break;
        case 0x18: desc_18.print(m_18_coeff_of_tractive_effort, os, indent);     break;
        case 0x19: desc_19.print(m_19_coeff_of_air_drag, os, indent);            break;
        case 0x1A: desc_1A.print(m_1A_refit_cost, os, indent);                   break;
        case 0x1B: desc_1B.print(m_1B_retire_vehicle_early, os, indent);         break;
        case 0x1C: desc_1C.print(m_1C_miscellaneous_flags, os, indent);          break;
        case 0x1D: desc_1D.print(m_1D_refittable_cargo_classes, os, indent);     break;
        case 0x1E: desc_1E.print(m_1E_non_refittable_cargo_classes, os, indent); break;
        case 0x1F: desc_1F.print(m_1F_long_introduction_date, os, indent);       break;
        case 0x20: desc_20.print(m_20_sort_purchase_list, os, indent);           break;
        case 0x21: desc_21.print(m_21_visual_effect, os, indent);                break;
        case 0x22: desc_22.print(m_22_custom_cargo_aging_period, os, indent);    break;
        case 0x23: desc_23.print(m_23_shorten_vehicle, os, indent);              break;
        case 0x24: desc_24.print(m_24_always_refittable_cargos, os, indent);     break;
        case 0x25: desc_25.print(m_25_never_refittable_cargos, os, indent);      break;
        case 0x26: desc_26.print(m_26_variant_group, os, indent);                break;
        case 0x27: desc_27.print(m_27_extra_flags, os, indent);                  break;
        case 0x28: desc_28.print(m_28_extra_callback_flags_mask, os, indent);    break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00RoadVehicles::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    if (Action00Vehicles::parse_property(is, name, property))
    {
        return true;
    }

    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x05: desc_05.parse(m_05_roadtype_tramtype, is);            break;
            case 0x08: desc_08.parse(m_08_speed_2_kmh, is);                  break;
            case 0x09: desc_09.parse(m_09_running_cost_factor, is);          break;
            case 0x0A: desc_0A.parse(m_0A_running_cost_base, is);            break;
            case 0x0E: desc_0E.parse(m_0E_sprite_id, is);                    break;
            case 0x0F: desc_0F.parse(m_0F_cargo_capacity, is);               break;
            case 0x10: desc_10.parse(m_10_cargo_type, is);                   break;
            case 0x11: desc_11.parse(m_11_cost_factor, is);                  break;
            case 0x12: desc_12.parse(m_12_sound_effect_type, is);            break;
            case 0x13: desc_13.parse(m_13_power_10_hp, is);                  break;
            case 0x14: desc_14.parse(m_14_weight_quarter_tons, is);          break;
            case 0x15: desc_15.parse(m_15_speed_half_kmh, is);               break;
            case 0x16: desc_16.parse(m_16_refit_cargo_types, is);            break;
            case 0x17: desc_17.parse(m_17_callback_flags_mask, is);          break;
            case 0x18: desc_18.parse(m_18_coeff_of_tractive_effort, is);     break;
            case 0x19: desc_19.parse(m_19_coeff_of_air_drag, is);            break;
            case 0x1A: desc_1A.parse(m_1A_refit_cost, is);                   break;
            case 0x1B: desc_1B.parse(m_1B_retire_vehicle_early, is);         break;
            case 0x1C: desc_1C.parse(m_1C_miscellaneous_flags, is);          break;
            case 0x1D: desc_1D.parse(m_1D_refittable_cargo_classes, is);     break;
            case 0x1E: desc_1E.parse(m_1E_non_refittable_cargo_classes, is); break;
            case 0x1F: desc_1F.parse(m_1F_long_introduction_date, is);       break;
            case 0x20: desc_20.parse(m_20_sort_purchase_list, is);           break;
            case 0x21: desc_21.parse(m_21_visual_effect, is);                break;
            case 0x22: desc_22.parse(m_22_custom_cargo_aging_period, is);    break;
            case 0x23: desc_23.parse(m_23_shorten_vehicle, is);              break;
            case 0x24: desc_24.parse(m_24_always_refittable_cargos, is);     break;
            case 0x25: desc_25.parse(m_25_never_refittable_cargos, is);      break;
            case 0x26: desc_26.parse(m_26_variant_group, is);                break;
            case 0x27: desc_27.parse(m_27_extra_flags, is);                  break;
            case 0x28: desc_28.parse(m_28_extra_callback_flags_mask, is);    break;
            default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", feature(), property);
}
