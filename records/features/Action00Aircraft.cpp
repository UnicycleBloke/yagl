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
#include "Action00Aircraft.h"
#include "Descriptors.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_sprite_id                    = "sprite_id";
constexpr const char* str_is_helicopter                = "is_helicopter";
constexpr const char* str_is_large                     = "is_large";
constexpr const char* str_cost_factor                  = "cost_factor";
constexpr const char* str_speed_8_mph                  = "speed_8_mph";
constexpr const char* str_acceleration                 = "acceleration";
constexpr const char* str_running_cost_factor          = "running_cost_factor";
constexpr const char* str_passenger_capacity           = "passenger_capacity";
constexpr const char* str_mail_capacity                = "mail_capacity";
constexpr const char* str_sound_effect_type            = "sound_effect_type";
constexpr const char* str_refit_cargo_types            = "refit_cargo_types";
constexpr const char* str_callback_flags_mask          = "callback_flags_mask";
constexpr const char* str_refit_cost                   = "refit_cost";
constexpr const char* str_retire_vehicle_early         = "retire_vehicle_early";
constexpr const char* str_miscellaneous_flags          = "miscellaneous_flags";
constexpr const char* str_refittable_cargo_classes     = "refittable_cargo_classes";
constexpr const char* str_non_refittable_cargo_classes = "non_refittable_cargo_classes";
constexpr const char* str_long_introduction_date       = "long_introduction_date";
constexpr const char* str_sort_purchase_list           = "sort_purchase_list";
constexpr const char* str_custom_cargo_aging_period    = "custom_cargo_aging_period";
constexpr const char* str_always_refittable_cargos     = "always_refittable_cargos";
constexpr const char* str_never_refittable_cargos      = "never_refittable_cargos";
constexpr const char* str_aircraft_range               = "aircraft_range";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_sprite_id,                    0x08'00 },
    { str_is_helicopter,                0x09'00 },
    { str_is_large,                     0x0A'00 },
    { str_cost_factor,                  0x0B'00 },
    { str_speed_8_mph,                  0x0C'00 },
    { str_acceleration,                 0x0D'00 },
    { str_running_cost_factor,          0x0E'00 },
    { str_passenger_capacity,           0x0F'00 },
    { str_mail_capacity,                0x11'00 },
    { str_sound_effect_type,            0x12'00 },
    { str_refit_cargo_types,            0x13'00 },
    { str_callback_flags_mask,          0x14'00 },
    { str_refit_cost,                   0x15'00 },
    { str_retire_vehicle_early,         0x16'00 },
    { str_miscellaneous_flags,          0x17'00 },
    { str_refittable_cargo_classes,     0x18'00 },
    { str_non_refittable_cargo_classes, 0x19'00 },
    { str_long_introduction_date,       0x1A'00 },
    { str_sort_purchase_list,           0x1B'00 },
    { str_custom_cargo_aging_period,    0x1C'00 },
    { str_always_refittable_cargos,     0x1D'00 },
    { str_never_refittable_cargos,      0x1E'00 },
    { str_aircraft_range,               0x1F'00 },
};


constexpr UInt8Descriptor      desc_08 = { 0x08, str_sprite_id,                     PropFormat::Hex };
constexpr BoolHeliDescriptor   desc_09 = { 0x09, str_is_helicopter };
constexpr BoolDescriptor       desc_0A = { 0x0A, str_is_large };
constexpr UInt8Descriptor      desc_0B = { 0x0B, str_cost_factor,                   PropFormat::Hex };
constexpr UInt8Descriptor      desc_0C = { 0x0C, str_speed_8_mph,                   PropFormat::Hex };
constexpr UInt8Descriptor      desc_0D = { 0x0D, str_acceleration,                  PropFormat::Hex };
constexpr UInt8Descriptor      desc_0E = { 0x0E, str_running_cost_factor,           PropFormat::Hex };
constexpr UInt16Descriptor     desc_0F = { 0x0F, str_passenger_capacity,            PropFormat::Hex };
constexpr UInt8Descriptor      desc_11 = { 0x11, str_mail_capacity,                 PropFormat::Hex };
constexpr UInt8Descriptor      desc_12 = { 0x12, str_sound_effect_type,             PropFormat::Hex };
constexpr UInt32Descriptor     desc_13 = { 0x13, str_refit_cargo_types,             PropFormat::Hex };
constexpr UInt8Descriptor      desc_14 = { 0x14, str_callback_flags_mask,           PropFormat::Hex };
constexpr UInt8Descriptor      desc_15 = { 0x15, str_refit_cost,                    PropFormat::Hex };
constexpr UInt8Descriptor      desc_16 = { 0x16, str_retire_vehicle_early,          PropFormat::Hex };
constexpr UInt8Descriptor      desc_17 = { 0x17, str_miscellaneous_flags,           PropFormat::Hex };
constexpr UInt16Descriptor     desc_18 = { 0x18, str_refittable_cargo_classes,      PropFormat::Hex };
constexpr UInt16Descriptor     desc_19 = { 0x19, str_non_refittable_cargo_classes,  PropFormat::Hex };
constexpr LongDateDescriptor   desc_1A = { 0x1A, str_long_introduction_date };
constexpr UInt8ExtDescriptor   desc_1B = { 0x1B, str_sort_purchase_list,            PropFormat::Hex };
constexpr UInt16Descriptor     desc_1C = { 0x1C, str_custom_cargo_aging_period,     PropFormat::Hex };
constexpr CargoListDescriptor  desc_1D = { 0x1D, str_always_refittable_cargos };
constexpr CargoListDescriptor  desc_1E = { 0x1E, str_never_refittable_cargos };
constexpr UInt16Descriptor     desc_1F = { 0x1F, str_aircraft_range,                PropFormat::Hex };


} // namespace


bool Action00Aircraft::read_property(std::istream& is, uint8_t property)
{
    if (Action00Common::read_property(is, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: m_08_sprite_id.read(is);                    break;
        case 0x09: m_09_is_helicopter.read(is);                break;
        case 0x0A: m_0A_is_large.read(is);                     break; 
        case 0x0B: m_0B_cost_factor.read(is);                  break;
        case 0x0C: m_0C_speed_8_mph.read(is);                  break;
        case 0x0D: m_0D_acceleration.read(is);                 break;
        case 0x0E: m_0E_running_cost_factor.read(is);          break;
        case 0x0F: m_0F_passenger_capacity.read(is);           break;
        case 0x11: m_11_mail_capacity.read(is);                break;
        case 0x12: m_12_sound_effect_type.read(is);            break;
        case 0x13: m_13_refit_cargo_types.read(is);            break;
        case 0x14: m_14_callback_flags_mask.read(is);          break;
        case 0x15: m_15_refit_cost.read(is);                   break;
        case 0x16: m_16_retire_vehicle_early.read(is);         break;
        case 0x17: m_17_miscellaneous_flags.read(is);          break;
        case 0x18: m_18_refittable_cargo_classes.read(is);     break;
        case 0x19: m_19_non_refittable_cargo_classes.read(is); break;
        case 0x1A: m_1A_long_introduction_date.read(is);       break;
        case 0x1B: m_1B_sort_purchase_list.read(is);           break;
        case 0x1C: m_1C_custom_cargo_aging_period.read(is);    break;
        case 0x1D: m_1D_always_refittable_cargos.read(is);     break;
        case 0x1E: m_1E_never_refittable_cargos.read(is);      break;
        case 0x1F: m_1F_aircraft_range.read(is);               break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Aircraft::write_property(std::ostream& os, uint8_t property) const
{
    if (Action00Common::write_property(os, property))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: m_08_sprite_id.write(os);                    break;
        case 0x09: m_09_is_helicopter.write(os);                break;
        case 0x0A: m_0A_is_large.write(os);                     break;
        case 0x0B: m_0B_cost_factor.write(os);                  break;
        case 0x0C: m_0C_speed_8_mph.write(os);                  break;
        case 0x0D: m_0D_acceleration.write(os);                 break;
        case 0x0E: m_0E_running_cost_factor.write(os);          break;
        case 0x0F: m_0F_passenger_capacity.write(os);           break;
        case 0x11: m_11_mail_capacity.write(os);                break;
        case 0x12: m_12_sound_effect_type.write(os);            break;
        case 0x13: m_13_refit_cargo_types.write(os);            break;
        case 0x14: m_14_callback_flags_mask.write(os);          break;
        case 0x15: m_15_refit_cost.write(os);                   break;
        case 0x16: m_16_retire_vehicle_early.write(os);         break;
        case 0x17: m_17_miscellaneous_flags.write(os);          break;
        case 0x18: m_18_refittable_cargo_classes.write(os);     break;
        case 0x19: m_19_non_refittable_cargo_classes.write(os); break;
        case 0x1A: m_1A_long_introduction_date.write(os);       break;
        case 0x1B: m_1B_sort_purchase_list.write(os);           break;
        case 0x1C: m_1C_custom_cargo_aging_period.write(os);    break;
        case 0x1D: m_1D_always_refittable_cargos.write(os);     break;
        case 0x1E: m_1E_never_refittable_cargos.write(os);      break;
        case 0x1F: m_1F_aircraft_range.write(os);               break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Aircraft::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    if (Action00Common::print_property(os, property, indent))
    {
        return true;
    }

    switch (property)
    {
        case 0x08: desc_08.print(m_08_sprite_id, os, indent);                    break;
        case 0x09: desc_09.print(m_09_is_helicopter, os, indent);                break;
        case 0x0A: desc_0A.print(m_0A_is_large, os, indent);                     break;
        case 0x0B: desc_0B.print(m_0B_cost_factor, os, indent);                  break;
        case 0x0C: desc_0C.print(m_0C_speed_8_mph, os, indent);                  break;
        case 0x0D: desc_0D.print(m_0D_acceleration, os, indent);                 break;
        case 0x0E: desc_0E.print(m_0E_running_cost_factor, os, indent);          break;
        case 0x0F: desc_0F.print(m_0F_passenger_capacity, os, indent);           break;
        case 0x11: desc_11.print(m_11_mail_capacity, os, indent);                break;
        case 0x12: desc_12.print(m_12_sound_effect_type, os, indent);            break;
        case 0x13: desc_13.print(m_13_refit_cargo_types, os, indent);            break;
        case 0x14: desc_14.print(m_14_callback_flags_mask, os, indent);          break;
        case 0x15: desc_15.print(m_15_refit_cost, os, indent);                   break;
        case 0x16: desc_16.print(m_16_retire_vehicle_early, os, indent);         break;
        case 0x17: desc_17.print(m_17_miscellaneous_flags, os, indent);          break;
        case 0x18: desc_18.print(m_18_refittable_cargo_classes, os, indent);     break;
        case 0x19: desc_19.print(m_19_non_refittable_cargo_classes, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_long_introduction_date, os, indent);       break;
        case 0x1B: desc_1B.print(m_1B_sort_purchase_list, os, indent);           break;
        case 0x1C: desc_1C.print(m_1C_custom_cargo_aging_period, os, indent);    break;
        case 0x1D: desc_1D.print(m_1D_always_refittable_cargos, os, indent);     break;
        case 0x1E: desc_1E.print(m_1E_never_refittable_cargos, os, indent);      break;
        case 0x1F: desc_1F.print(m_1F_aircraft_range, os, indent);               break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Aircraft::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
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
            case 0x08: desc_08.parse(m_08_sprite_id, is);                    break;
            case 0x09: desc_09.parse(m_09_is_helicopter, is);                break;
            case 0x0A: desc_0A.parse(m_0A_is_large, is);                     break;
            case 0x0B: desc_0B.parse(m_0B_cost_factor, is);                  break;
            case 0x0C: desc_0C.parse(m_0C_speed_8_mph, is);                  break;
            case 0x0D: desc_0D.parse(m_0D_acceleration, is);                 break;
            case 0x0E: desc_0E.parse(m_0E_running_cost_factor, is);          break;
            case 0x0F: desc_0F.parse(m_0F_passenger_capacity, is);           break;
            case 0x11: desc_11.parse(m_11_mail_capacity, is);                break;
            case 0x12: desc_12.parse(m_12_sound_effect_type, is);            break;
            case 0x13: desc_13.parse(m_13_refit_cargo_types, is);            break;
            case 0x14: desc_14.parse(m_14_callback_flags_mask, is);          break;
            case 0x15: desc_15.parse(m_15_refit_cost, is);                   break;
            case 0x16: desc_16.parse(m_16_retire_vehicle_early, is);         break;
            case 0x17: desc_17.parse(m_17_miscellaneous_flags, is);          break;
            case 0x18: desc_18.parse(m_18_refittable_cargo_classes, is);     break;
            case 0x19: desc_19.parse(m_19_non_refittable_cargo_classes, is); break;
            case 0x1A: desc_1A.parse(m_1A_long_introduction_date, is);       break;
            case 0x1B: desc_1B.parse(m_1B_sort_purchase_list, is);           break;
            case 0x1C: desc_1C.parse(m_1C_custom_cargo_aging_period, is);    break;
            case 0x1D: desc_1D.parse(m_1D_always_refittable_cargos, is);     break;
            case 0x1E: desc_1E.parse(m_1E_never_refittable_cargos, is);      break;
            case 0x1F: desc_1F.parse(m_1F_aircraft_range, is);               break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}


