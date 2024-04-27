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
#include "IntegerDescriptor.h"
#include "DateDescriptor.h"
#include "VisualEffect.h"


class Action00RoadVehicles : public Action00Vehicles
{
public:
    Action00RoadVehicles() : Action00Vehicles(FeatureType::RoadVehicles) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8         m_05_roadtype_tramtype;
    UInt8         m_08_speed_2_kmh{};
    UInt8         m_09_running_cost_factor{};
    UInt32        m_0A_running_cost_base{};
    UInt8         m_0E_sprite_id{};
    UInt8         m_0F_cargo_capacity{};
    UInt8         m_10_cargo_type{};
    UInt8         m_11_cost_factor{};
    UInt8         m_12_sound_effect_type{};
    UInt8         m_13_power_10_hp{};
    UInt8         m_14_weight_quarter_tons{};
    UInt8         m_15_speed_half_kmh{};
    UInt32        m_16_refit_cargo_types{};
    UInt8         m_17_callback_flags_mask{};
    UInt8         m_18_coeff_of_tractive_effort{};
    UInt8         m_19_coeff_of_air_drag{};
    UInt8         m_1A_refit_cost{};
    UInt8         m_1B_retire_vehicle_early{};
    UInt8         m_1C_miscellaneous_flags{};
    UInt16        m_1D_refittable_cargo_classes{};
    UInt16        m_1E_non_refittable_cargo_classes{};
    LongDate      m_1F_long_introduction_date{};
    UInt8Ext      m_20_sort_purchase_list{};
    VisualEffect  m_21_visual_effect{};
    UInt16        m_22_custom_cargo_aging_period{};
    UInt8         m_23_shorten_vehicle{};
    CargoList     m_24_always_refittable_cargos{};
    CargoList     m_25_never_refittable_cargos{};
    UInt16        m_26_variant_group{};
    UInt32        m_27_extra_flags{};
    UInt8         m_28_extra_callback_flags_mask{};
};



