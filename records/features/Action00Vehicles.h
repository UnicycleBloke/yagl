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


class Action00Vehicles : public Action00Common
{
public:
    Action00Vehicles() : Action00Common() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t   m_08_speed_2_kmh{};
    uint8_t   m_09_running_cost_factor{};
    uint32_t  m_0A_running_cost_base{};
    uint8_t   m_0E_sprite_id{};
    uint8_t   m_0F_cargo_capacity{};
    uint8_t   m_10_cargo_type{};
    uint8_t   m_11_cost_factor{};
    uint8_t   m_12_sound_effect_type{};
    uint8_t   m_13_power_10_hp{};
    uint8_t   m_14_weight_quarter_tons{};
    uint8_t   m_15_speed_half_kmh{};
    uint32_t  m_16_refit_cargo_types{};
    uint8_t   m_17_callback_flags_mask{};
    uint8_t   m_18_coeff_of_tractive_effort{};
    uint8_t   m_19_coeff_of_air_drag{};
    uint8_t   m_1A_refit_cost{};
    uint8_t   m_1B_retire_vehicle_early{};
    uint8_t   m_1C_miscellaneous_flags{};
    uint16_t  m_1D_refittable_cargo_classes{};
    uint16_t  m_1E_non_refittable_cargo_classes{};
    LongDate  m_1F_long_introduction_date{};
    uint16_t  m_20_sort_purchase_list{};
    uint8_t   m_21_visual_effect{};
    uint16_t  m_22_custom_cargo_aging_period{};
    uint8_t   m_23_shorten_vehicle{};
    CargoList m_24_always_refittable_cargos{};
    CargoList m_25_never_refittable_cargos{};
};



