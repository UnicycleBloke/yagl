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
#include <vector>


class Multipliers
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os) const;
    void parse(TokenStream& is);

private:
    uint8_t               m_num_inputs{};
    uint8_t               m_num_outputs{};
    std::vector<uint16_t> m_items;
};


class Action00Industries : public Action00Feature
{
public:
    Action00Industries() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8                  m_08_substitute_industry_id{};
    UInt8                  m_09_industry_type_override{};
    IndustryLayouts        m_0A_industry_layout{};
    UInt8                  m_0B_production_flags{};
    UInt16                 m_0C_closure_msg_id{};
    UInt16                 m_0D_production_up_id{};
    UInt16                 m_0E_production_down_id{};
    UInt8                  m_0F_fund_cost_multiplier{};
    UIntArray<UInt8, 2>    m_10_production_cargo_types{};
    UIntArray<UInt8, 4>    m_11_acceptance_cargo_types{};
    UInt8                  m_12_production_multipliers_1{};
    UInt8                  m_13_production_multipliers_2{};
    UInt8                  m_14_minimum_distributed{};
    UIntVector<UInt8>      m_15_sound_effects{};
    UIntArray<UInt8, 3>    m_16_conflicting_industries{};
    UInt8                  m_17_random_probability{};
    UInt8                  m_18_gameplay_probability{};
    UInt8                  m_19_map_colour{};
    UInt32                 m_1A_special_flags{};
    UInt16                 m_1B_new_industry_text_id{};
    UInt32                 m_1C_input_multipliers1{};
    UInt32                 m_1D_input_multipliers2{};
    UInt32                 m_1E_input_multipliers3{};
    UInt16                 m_1F_industry_name_id{};
    UInt32                 m_20_prospecting_probability{};
    UInt8                  m_21_callback_flags_1{};
    UInt8                  m_22_callback_flags_2{};
    UInt32                 m_23_destruction_cost_multiplier{};
    UInt16                 m_24_nearby_station_text_id{};
    UIntVector<UInt8>      m_25_production_cargo_list{};
    UIntVector<UInt8>      m_26_acceptance_cargo_list{};
    UIntVector<UInt8>      m_27_production_multipliers{};
    Multipliers            m_28_input_cargo_multipliers{};
};


