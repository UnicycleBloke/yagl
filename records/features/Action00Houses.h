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
#include "Action00Feature.h"
#include "Descriptors.h"


class Action00Houses : public Action00Feature
{
public:
    Action00Houses() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t                m_08_substitute_building_id;
    uint8_t                m_09_building_flags;
    uint8_t                m_0A_first_year_available;
    uint8_t                m_0A_last_year_available;
    uint8_t                m_0B_population;
    uint8_t                m_0C_mail_multiplier;
    uint8_t                m_0D_passenger_acceptance;
    uint8_t                m_0E_mail_acceptance;
    uint8_t                m_0F_goods_etc_acceptance;
    bool                   m_0F_accepts_goods;
    uint16_t               m_10_la_rating_decrease;
    uint8_t                m_11_removal_cost_multiplier;
    uint16_t               m_12_building_name_id;
    uint16_t               m_13_availability_mask;
    uint8_t                m_14_callback_flags;
    uint8_t                m_15_override_byte;
    uint8_t                m_16_refresh_multiplier;
    std::array<uint8_t, 4> m_17_four_random_colours;
    uint8_t                m_18_appearance_probability;
    uint8_t                m_19_extra_flags;
    uint8_t                m_1A_animation_frames;
    bool                   m_1A_animation_loops;
    uint8_t                m_1B_animation_speed;
    uint8_t                m_1C_building_class;
    uint8_t                m_1D_callback_flags_2;
    std::array<uint8_t, 4> m_1E_accepted_cargo_types;
    uint16_t               m_1F_minimum_life_years;
    CargoList              m_20_accepted_cargo_list;
    uint16_t               m_21_long_minimum_year;
    uint16_t               m_22_long_maximum_year;
};


