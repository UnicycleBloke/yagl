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
#include "Action00Common.h"
#include "Descriptors.h"


class Action00Trains : public Action00Common
{
public:
    Action00Trains() : Action00Common() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t   m_05_track_type;
    bool      m_08_ai_special_flag;
    uint16_t  m_09_speed_kmh;
    uint16_t  m_0B_power;
    uint8_t   m_0D_running_cost_factor;
    uint32_t  m_0E_running_cost_base;
    uint8_t   m_12_sprite_id;
    bool      m_13_is_dual_headed;
    uint8_t   m_14_cargo_capacity;
    uint8_t   m_15_cargo_type;
    uint8_t   m_16_weight_tons;
    uint8_t   m_17_cost_factor;
    uint8_t   m_18_ai_engine_rank;
    uint8_t   m_19_engine_traction_type;
    uint16_t  m_1A_sort_purchase_list;
    uint16_t  m_1B_power_from_each_wagon;
    uint8_t   m_1C_refit_cost;
    uint32_t  m_1D_refit_cargo_types;
    uint8_t   m_1E_callback_flags_mask;
    uint8_t   m_1F_coeff_of_tractive_effort;
    uint8_t   m_20_coeff_of_air_drag;
    uint8_t   m_21_shorten_vehicle;
    uint8_t   m_22_visual_effect_position;
    uint8_t   m_22_visual_effect_enum;
    bool      m_22_visual_effect_disable_wagons;
    uint8_t   m_23_weight_from_wagons;
    uint8_t   m_24_weight_high_byte;
    uint8_t   m_25_mask_for_var_42;
    uint8_t   m_26_retire_vehicle_early;
    uint8_t   m_27_miscellaneous_flags;
    uint16_t  m_28_refittable_cargo_classes;
    uint16_t  m_29_non_refittable_cargo_classes;
    uint32_t  m_2A_long_introduction_date;
    uint16_t  m_2B_custom_cargo_aging_period;   
    CargoList m_2C_always_refittable_cargos;
    CargoList m_2D_never_refittable_cargos;
};


