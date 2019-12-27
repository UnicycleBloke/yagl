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
#include <vector>


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
    struct IndustryTile
    {
        int8_t   x_off;  // 0x00 is terminator part 1
        int8_t   y_off;  // 0x80 is terminator part 2
        uint8_t  type;
        uint16_t tile;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

private:
    struct IndustryLayout
    {
        bool    is_reference;
        uint8_t industry;
        uint8_t layout_index;
        std::vector<IndustryTile> tiles;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

public:
    struct IndustryLayouts
    {
        std::vector<IndustryLayout> layouts;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

public:
    struct IndustrySFXList
    {
        std::vector<uint8_t> effects;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

private:
    uint8_t                m_08_substitute_industry_id;
    uint8_t                m_09_industry_type_override;
    IndustryLayouts        m_0A_industry_layout;
    uint8_t                m_0B_production_flags;
    uint16_t               m_0C_closure_msg_id;
    uint16_t               m_0D_production_up_id;
    uint16_t               m_0E_production_down_id;
    uint8_t                m_0F_fund_cost_multiplier;
    std::array<uint8_t, 2> m_10_production_cargo_types;
    std::array<uint8_t, 4> m_11_acceptance_cargo_types;
    uint8_t                m_12_production_multipliers_1;
    uint8_t                m_13_production_multipliers_2;
    uint8_t                m_14_minimum_distributed;
    IndustrySFXList        m_15_sound_effects;
    std::array<uint8_t, 3> m_16_conflicting_industries;
    std::array<uint8_t, 3> m_16_conflicting_old_new;
    uint8_t                m_17_random_probability;
    uint8_t                m_18_gameplay_probability;
    uint8_t                m_19_map_colour;
    uint32_t               m_1A_special_flags;
    uint16_t               m_1B_new_industry_text_id;
    uint16_t               m_1C_input_multipliers_A1;
    uint16_t               m_1C_input_multipliers_B1;
    uint16_t               m_1D_input_multipliers_A2;
    uint16_t               m_1D_input_multipliers_B2;
    uint16_t               m_1E_input_multipliers_A3;
    uint16_t               m_1E_input_multipliers_B3;
    uint16_t               m_1F_industry_name_id;
    uint32_t               m_20_prospecting_probability;
    uint8_t                m_21_callback_flags_1;
    uint8_t                m_22_callback_flags_2;
    uint32_t               m_23_destruction_cost_multiplier;
    uint16_t               m_24_nearby_station_text_id;
};


