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


class Action00Ships : public Action00Common
{
public:
    Action00Ships() : Action00Common() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t   m_08_sprite_id;
    bool      m_09_is_refittable;
    uint8_t   m_0A_cost_factor;
    uint8_t   m_0B_speed_2_kmh;
    uint8_t   m_0C_cargo_type;
    uint16_t  m_0D_cargo_capacity;
    uint8_t   m_0F_running_cost_factor;
    uint8_t   m_10_sound_effect_type;
    uint32_t  m_11_refit_cargo_types;
    uint8_t   m_12_callback_flags_mask;
    uint8_t   m_13_refit_cost;
    uint8_t   m_14_ocean_speed_fraction;
    uint8_t   m_15_canal_speed_fraction;
    uint8_t   m_16_retire_vehicle_early;
    uint8_t   m_17_miscellaneous_flags;
    uint16_t  m_18_refittable_cargo_classes;
    uint16_t  m_19_non_refittable_cargo_classes;
    uint32_t  m_1A_long_introduction_date;
    uint16_t  m_1B_sort_purchase_list;
    uint8_t   m_1C_visual_effect_position;
    uint8_t   m_1C_visual_effect_enum;
    uint16_t  m_1D_custom_cargo_aging_period;
    CargoList m_1E_always_refittable_cargos;
    CargoList m_1F_never_refittable_cargos;
};


