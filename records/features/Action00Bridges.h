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
#include "YearDescriptor.h"
#include "IntegerDescriptor.h"
#include "StreamHelpers.h"
#include <vector>
#include <array>


class BridgeTable
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    std::array<uint32_t, 32> m_sprites;
};


class BridgeLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    uint8_t                  m_first_table_id{};
    std::vector<BridgeTable> m_tables;
};


class Action00Bridges : public Action00Feature
{
public:
    Action00Bridges() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8          m_00_fallback_type_id{};
    Year8          m_08_year_available{};
    UInt8          m_09_minimum_length{};
    UInt8          m_0A_maximum_length{};
    UInt8          m_0B_cost_factor{};
    UInt16         m_0C_maximum_speed{};
    BridgeLayout   m_0D_bridge_layout{};
    UInt8          m_0E_various_flags{};
    Year32         m_0F_long_year_available{};
    UInt16         m_10_purchase_text{};
    UInt16         m_11_description_rail{};
    UInt16         m_12_description_road{};
    UInt16         m_13_cost_factor_word{};
};


