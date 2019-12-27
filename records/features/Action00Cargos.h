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
#include "GRFStrings.h"


class Action00Cargos : public Action00Feature
{
public:
    Action00Cargos() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t  m_08_bit_number;
    uint16_t m_09_cargo_type_name_id;
    uint16_t m_0A_single_unit_name_id;
    uint16_t m_0B_single_unit_id;
    uint16_t m_0C_multiple_units_id;
    uint16_t m_0D_cargo_type_abbrev_id;
    uint16_t m_0E_cargo_sprite_id;
    uint8_t  m_0F_single_unit_weight;
    uint8_t  m_10_penalty_time_1;
    uint8_t  m_11_penalty_time_2;
    uint32_t m_12_base_price;
    uint8_t  m_13_station_list_colour;
    uint8_t  m_14_payment_list_colour;
    bool     m_15_is_freight;
    uint16_t m_16_cargo_classes;
    GRFLabel m_17_cargo_label;
    uint8_t  m_18_town_growth_effect;
    uint16_t m_19_town_growth_multiplier;
    uint8_t  m_1A_callback_flags;
    uint16_t m_1B_cargo_units_id;
    uint16_t m_1C_cargo_amount_id;
    uint16_t m_1D_capacity_multiplier;
};


