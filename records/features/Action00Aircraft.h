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
#pragma once
#include "Action00Vehicles.h"
#include "BooleanDescriptor.h"
#include "IntegerDescriptor.h"


class Action00Aircraft : public Action00Vehicles
{
public:
    Action00Aircraft() : Action00Vehicles(FeatureType::Aircraft) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8     m_08_sprite_id{};
    BoolHeli  m_09_is_helicopter{};
    Bool      m_0A_is_large{};
    UInt8     m_0B_cost_factor{};
    UInt8     m_0C_speed_8_mph{};
    UInt8     m_0D_acceleration{};
    UInt8     m_0E_running_cost_factor{};
    UInt16    m_0F_passenger_capacity{};
    UInt8     m_11_mail_capacity{};
    UInt8     m_12_sound_effect_type{};
    UInt32    m_13_refit_cargo_types{};   // GRFv >= 6
    UInt8     m_14_callback_flags_mask{}; // GRFv >= 6
    UInt8     m_15_refit_cost{};
    UInt8     m_16_retire_vehicle_early{};
    UInt8     m_17_miscellaneous_flags{};
    UInt16    m_18_refittable_cargo_classes{};
    UInt16    m_19_non_refittable_cargo_classes{};
    LongDate  m_1A_long_introduction_date{};
    UInt8Ext  m_1B_sort_purchase_list{};
    UInt16    m_1C_custom_cargo_aging_period{};
    CargoList m_1D_always_refittable_cargos{};
    CargoList m_1E_never_refittable_cargos{};
    UInt16    m_1F_aircraft_range{};
    UInt16    m_20_variant_group{};
    UInt32    m_21_extra_flags{};
    UInt8     m_22_extra_callback_flags_mask{};
};
