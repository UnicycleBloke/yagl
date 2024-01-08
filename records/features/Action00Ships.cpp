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
#include "Action00Ships.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_sprite_id                    = "sprite_id";
constexpr const char* str_is_refittable                = "is_refittable";
constexpr const char* str_cost_factor                  = "cost_factor";
constexpr const char* str_speed_2_kmh                  = "speed_2_kmh";
constexpr const char* str_cargo_type                   = "cargo_type";
constexpr const char* str_cargo_capacity               = "cargo_capacity";
constexpr const char* str_running_cost_factor          = "running_cost_factor";
constexpr const char* str_sound_effect_type            = "sound_effect_type";
constexpr const char* str_refit_cargo_types            = "refit_cargo_types";
constexpr const char* str_callback_flags_mask          = "callback_flags_mask";
constexpr const char* str_refit_cost                   = "refit_cost";
constexpr const char* str_ocean_speed_fraction         = "ocean_speed_fraction";
constexpr const char* str_canal_speed_fraction         = "canal_speed_fraction";
constexpr const char* str_retire_vehicle_early         = "retire_vehicle_early";
constexpr const char* str_miscellaneous_flags          = "miscellaneous_flags";
constexpr const char* str_refittable_cargo_classes     = "refittable_cargo_classes";
constexpr const char* str_non_refittable_cargo_classes = "non_refittable_cargo_classes";
constexpr const char* str_long_introduction_date       = "long_introduction_date";
constexpr const char* str_sort_purchase_list           = "sort_purchase_list";
constexpr const char* str_visual_effect                = "visual_effect";
constexpr const char* str_custom_cargo_aging_period    = "custom_cargo_aging_period";
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
    { str_sprite_id,                    0x08 },
    { str_is_refittable,                0x09 },
    { str_cost_factor,                  0x0A },
    { str_speed_2_kmh,                  0x0B },
    { str_cargo_type,                   0x0C },
    { str_cargo_capacity,               0x0D },
    { str_running_cost_factor,          0x0F },
    { str_sound_effect_type,            0x10 },
    { str_refit_cargo_types,            0x11 },
    { str_callback_flags_mask,          0x12 },
    { str_refit_cost,                   0x13 },
    { str_ocean_speed_fraction,         0x14 },
    { str_canal_speed_fraction,         0x15 },
    { str_retire_vehicle_early,         0x16 },
    { str_miscellaneous_flags,          0x17 },
    { str_refittable_cargo_classes,     0x18 },
    { str_non_refittable_cargo_classes, 0x19 },
    { str_long_introduction_date,       0x1A },
    { str_sort_purchase_list,           0x1B },
    { str_visual_effect,                0x1C },
    { str_custom_cargo_aging_period,    0x1D },
    { str_always_refittable_cargos,     0x1E },
    { str_never_refittable_cargos,      0x1F },
    { str_variant_group,                0x20 },
    { str_extra_flags,                  0x21 },
    { str_extra_callback_flags_mask,    0x22 },
};


constexpr UInt8Descriptor        desc_08  = { 0x08, str_sprite_id,                    UIntFormat::Hex };
constexpr BoolDescriptor         desc_09  = { 0x09, str_is_refittable };
constexpr UInt8Descriptor        desc_0A  = { 0x0A, str_cost_factor,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_0B  = { 0x0B, str_speed_2_kmh,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_0C  = { 0x0C, str_cargo_type,                   UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0D  = { 0x0D, str_cargo_capacity,               UIntFormat::Hex };
constexpr UInt8Descriptor        desc_0F  = { 0x0F, str_running_cost_factor,          UIntFormat::Hex };
constexpr UInt8Descriptor        desc_10  = { 0x10, str_sound_effect_type,            UIntFormat::Hex };
constexpr UInt32Descriptor       desc_11  = { 0x11, str_refit_cargo_types,            UIntFormat::Hex };
constexpr UInt8Descriptor        desc_12  = { 0x12, str_callback_flags_mask,          UIntFormat::Hex };
constexpr UInt8Descriptor        desc_13  = { 0x13, str_refit_cost,                   UIntFormat::Hex };
constexpr UInt8Descriptor        desc_14  = { 0x14, str_ocean_speed_fraction,         UIntFormat::Hex };
constexpr UInt8Descriptor        desc_15  = { 0x15, str_canal_speed_fraction,         UIntFormat::Hex };
constexpr UInt8Descriptor        desc_16  = { 0x16, str_retire_vehicle_early,         UIntFormat::Hex };
constexpr UInt8Descriptor        desc_17  = { 0x17, str_miscellaneous_flags,          UIntFormat::Hex };
constexpr UInt16Descriptor       desc_18  = { 0x18, str_refittable_cargo_classes,     UIntFormat::Hex };
constexpr UInt16Descriptor       desc_19  = { 0x19, str_non_refittable_cargo_classes, UIntFormat::Hex };
constexpr LongDateDescriptor     desc_1A  = { 0x1A, str_long_introduction_date };
constexpr UInt8ExtDescriptor     desc_1B  = { 0x1B, str_sort_purchase_list,           UIntFormat::Hex };
constexpr VisualEffectDescriptor desc_1C  = { 0x1C, str_visual_effect };
constexpr UInt16Descriptor       desc_1D  = { 0x1D, str_custom_cargo_aging_period,    UIntFormat::Hex };
constexpr CargoListDescriptor    desc_1E  = { 0x1E, str_always_refittable_cargos };
constexpr CargoListDescriptor    desc_1F  = { 0x1F, str_never_refittable_cargos };
constexpr UInt16Descriptor       desc_20  = { 0x20, str_variant_group,                UIntFormat::Hex };
constexpr UInt32Descriptor       desc_21  = { 0x21, str_extra_flags,                  UIntFormat::Hex };
constexpr UInt8Descriptor        desc_22  = { 0x22, str_extra_callback_flags_mask,    UIntFormat::Hex };


} // namespace {


bool Action00Ships::read_property(std::istream& is, uint8_t property)
{
    if (Action00Common::read_property(is, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: m_08_sprite_id.read(is);                    break;
        case 0x09: m_09_is_refittable.read(is);                break;
        case 0x0A: m_0A_cost_factor.read(is);                  break;
        case 0x0B: m_0B_speed_2_kmh.read(is);                  break;
        case 0x0C: m_0C_cargo_type.read(is);                   break;
        case 0x0D: m_0D_cargo_capacity.read(is);               break;
        case 0x0F: m_0F_running_cost_factor.read(is);          break;
        case 0x10: m_10_sound_effect_type.read(is);            break;
        case 0x11: m_11_refit_cargo_types.read(is);            break;
        case 0x12: m_12_callback_flags_mask.read(is);          break;
        case 0x13: m_13_refit_cost.read(is);                   break;
        case 0x14: m_14_ocean_speed_fraction.read(is);         break;
        case 0x15: m_15_canal_speed_fraction.read(is);         break;
        case 0x16: m_16_retire_vehicle_early.read(is);         break;
        case 0x17: m_17_miscellaneous_flags.read(is);          break;
        case 0x18: m_18_refittable_cargo_classes.read(is);     break;
        case 0x19: m_19_non_refittable_cargo_classes.read(is); break;
        case 0x1A: m_1A_long_introduction_date.read(is);       break;
        case 0x1B: m_1B_sort_purchase_list.read(is);           break;
        case 0x1C: m_1C_visual_effect.read(is);                break;
        case 0x1D: m_1D_custom_cargo_aging_period.read(is);    break;
        case 0x1E: m_1E_always_refittable_cargos.read(is);     break;
        case 0x1F: m_1F_never_refittable_cargos.read(is);      break;
        case 0x20: m_20_variant_group.read(is);                break;
        case 0x21: m_21_extra_flags.read(is);                  break;
        case 0x22: m_22_extra_callback_flags_mask.read(is);    break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Ships::write_property(std::ostream& os, uint8_t property) const
{
    if (Action00Common::write_property(os, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: m_08_sprite_id.write(os);                    break;
        case 0x09: m_09_is_refittable.write(os);                break;
        case 0x0A: m_0A_cost_factor.write(os);                  break;
        case 0x0B: m_0B_speed_2_kmh.write(os);                  break;
        case 0x0C: m_0C_cargo_type.write(os);                   break;
        case 0x0D: m_0D_cargo_capacity.write(os);               break;
        case 0x0F: m_0F_running_cost_factor.write(os);          break;
        case 0x10: m_10_sound_effect_type.write(os);            break;
        case 0x11: m_11_refit_cargo_types.write(os);            break;
        case 0x12: m_12_callback_flags_mask.write(os);          break;
        case 0x13: m_13_refit_cost.write(os);                   break;
        case 0x14: m_14_ocean_speed_fraction.write(os);         break;
        case 0x15: m_15_canal_speed_fraction.write(os);         break;
        case 0x16: m_16_retire_vehicle_early.write(os);         break;
        case 0x17: m_17_miscellaneous_flags.write(os);          break;
        case 0x18: m_18_refittable_cargo_classes.write(os);     break;
        case 0x19: m_19_non_refittable_cargo_classes.write(os); break;
        case 0x1A: m_1A_long_introduction_date.write(os);       break;
        case 0x1B: m_1B_sort_purchase_list.write(os);           break;
        case 0x1C: m_1C_visual_effect.write(os);                break;
        case 0x1D: m_1D_custom_cargo_aging_period.write(os);    break;
        case 0x1E: m_1E_always_refittable_cargos.write(os);     break;
        case 0x1F: m_1F_never_refittable_cargos.write(os);      break;
        case 0x20: m_20_variant_group.write(os);                break;
        case 0x21: m_21_extra_flags.write(os);                  break;
        case 0x22: m_22_extra_callback_flags_mask.write(os);    break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Ships::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    if (Action00Common::print_property(os, property, indent))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: desc_08.print(m_08_sprite_id, os, indent);                    break;
        case 0x09: desc_09.print(m_09_is_refittable, os, indent);                break;
        case 0x0A: desc_0A.print(m_0A_cost_factor, os, indent);                  break;
        case 0x0B: desc_0B.print(m_0B_speed_2_kmh, os, indent);                  break;
        case 0x0C: desc_0C.print(m_0C_cargo_type, os, indent);                   break;
        case 0x0D: desc_0D.print(m_0D_cargo_capacity, os, indent);               break;
        case 0x0F: desc_0F.print(m_0F_running_cost_factor, os, indent);          break;
        case 0x10: desc_10.print(m_10_sound_effect_type, os, indent);            break;
        case 0x11: desc_11.print(m_11_refit_cargo_types, os, indent);            break;
        case 0x12: desc_12.print(m_12_callback_flags_mask, os, indent);          break;
        case 0x13: desc_13.print(m_13_refit_cost, os, indent);                   break;
        case 0x14: desc_14.print(m_14_ocean_speed_fraction, os, indent);         break;
        case 0x15: desc_15.print(m_15_canal_speed_fraction, os, indent);         break;
        case 0x16: desc_16.print(m_16_retire_vehicle_early, os, indent);         break;
        case 0x17: desc_17.print(m_17_miscellaneous_flags, os, indent);          break;
        case 0x18: desc_18.print(m_18_refittable_cargo_classes, os, indent);     break;
        case 0x19: desc_19.print(m_19_non_refittable_cargo_classes, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_long_introduction_date, os, indent);       break;
        case 0x1B: desc_1B.print(m_1B_sort_purchase_list, os, indent);           break;
        case 0x1C: desc_1C.print(m_1C_visual_effect, os, indent);                break;
        case 0x1D: desc_1D.print(m_1D_custom_cargo_aging_period, os, indent);    break;
        case 0x1E: desc_1E.print(m_1E_always_refittable_cargos, os, indent);     break;
        case 0x1F: desc_1F.print(m_1F_never_refittable_cargos, os, indent);      break;
        case 0x20: desc_20.print(m_20_variant_group, os, indent);                break;
        case 0x21: desc_21.print(m_21_extra_flags, os, indent);                  break;
        case 0x22: desc_22.print(m_22_extra_callback_flags_mask, os, indent);    break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Ships::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
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
            case 0x08: desc_08.parse(m_08_sprite_id, is);                    break;
            case 0x09: desc_09.parse(m_09_is_refittable, is);                break;
            case 0x0A: desc_0A.parse(m_0A_cost_factor, is);                  break;
            case 0x0B: desc_0B.parse(m_0B_speed_2_kmh, is);                  break;
            case 0x0C: desc_0C.parse(m_0C_cargo_type, is);                   break;
            case 0x0D: desc_0D.parse(m_0D_cargo_capacity, is);               break;
            case 0x0F: desc_0F.parse(m_0F_running_cost_factor, is);          break;
            case 0x10: desc_10.parse(m_10_sound_effect_type, is);            break;
            case 0x11: desc_11.parse(m_11_refit_cargo_types, is);            break;
            case 0x12: desc_12.parse(m_12_callback_flags_mask, is);          break;
            case 0x13: desc_13.parse(m_13_refit_cost, is);                   break;
            case 0x14: desc_14.parse(m_14_ocean_speed_fraction, is);         break;
            case 0x15: desc_15.parse(m_15_canal_speed_fraction, is);         break;
            case 0x16: desc_16.parse(m_16_retire_vehicle_early, is);         break;
            case 0x17: desc_17.parse(m_17_miscellaneous_flags, is);          break;
            case 0x18: desc_18.parse(m_18_refittable_cargo_classes, is);     break;
            case 0x19: desc_19.parse(m_19_non_refittable_cargo_classes, is); break;
            case 0x1A: desc_1A.parse(m_1A_long_introduction_date, is);       break;
            case 0x1B: desc_1B.parse(m_1B_sort_purchase_list, is);           break;
            case 0x1C: desc_1C.parse(m_1C_visual_effect, is);                break;
            case 0x1D: desc_1D.parse(m_1D_custom_cargo_aging_period, is);    break;
            case 0x1E: desc_1E.parse(m_1E_always_refittable_cargos, is);     break;
            case 0x1F: desc_1F.parse(m_1F_never_refittable_cargos, is);      break;
            case 0x20: desc_20.parse(m_20_variant_group, is);                break;
            case 0x21: desc_21.parse(m_21_extra_flags, is);                  break;
            case 0x22: desc_22.parse(m_22_extra_callback_flags_mask, is);    break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
