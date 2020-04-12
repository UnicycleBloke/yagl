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
    UInt8                 m_08_substitute_building_id{};
    UInt8                 m_09_building_flags{};
    Year8Pair             m_0A_years_available{};
    UInt8                 m_0B_population{};
    UInt8                 m_0C_mail_multiplier{};
    UInt8                 m_0D_passenger_acceptance{};
    UInt8                 m_0E_mail_acceptance{};
    UInt8                 m_0F_goods_etc_acceptance{};
    UInt16                m_10_la_rating_decrease{};
    UInt8                 m_11_removal_cost_multiplier{};
    UInt16                m_12_building_name_id{};
    UInt16                m_13_availability_mask{};
    UInt8                 m_14_callback_flags{};
    UInt8                 m_15_override_byte{};
    UInt8                 m_16_refresh_multiplier{};
    UIntArray<UInt8, 4>   m_17_four_random_colours{};
    UInt8                 m_18_appearance_probability{};
    UInt8                 m_19_extra_flags{};
    UInt8                 m_1A_animation_frames{};
    UInt8                 m_1B_animation_speed{};
    UInt8                 m_1C_building_class{};
    UInt8                 m_1D_callback_flags_2{};
    UIntArray<UInt8, 4>   m_1E_accepted_cargo_types{};
    UInt16                m_1F_minimum_life_years{};
    CargoList             m_20_accepted_cargo_list{};
    Year16                m_21_long_minimum_year{};
    Year16                m_22_long_maximum_year{};
    CargoAcceptanceList   m_23_tile_acceptance_list{};
};


