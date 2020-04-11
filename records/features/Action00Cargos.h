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
#include "IntegerDescriptor.h"
#include "BooleanDescriptor.h"
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
    UInt8    m_08_bit_number{};
    UInt16   m_09_cargo_type_name_id{};
    UInt16   m_0A_single_unit_name_id{};
    UInt16   m_0B_single_unit_id{};
    UInt16   m_0C_multiple_units_id{};
    UInt16   m_0D_cargo_type_abbrev_id{};
    UInt16   m_0E_cargo_sprite_id{};
    UInt8    m_0F_single_unit_weight{};
    UInt8    m_10_penalty_time_1{};
    UInt8    m_11_penalty_time_2{};
    UInt32   m_12_base_price{};
    UInt8    m_13_station_list_colour{};
    UInt8    m_14_payment_list_colour{};
    Bool     m_15_is_freight{};
    UInt16   m_16_cargo_classes{};
    GRFLabel m_17_cargo_label{};
    UInt8    m_18_town_growth_effect{};
    UInt16   m_19_town_growth_multiplier{};
    UInt8    m_1A_callback_flags{};
    UInt16   m_1B_cargo_units_id{};
    UInt16   m_1C_cargo_amount_id{};
    UInt16   m_1D_capacity_multiplier{};
};


