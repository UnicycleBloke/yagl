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
#include "catch.hpp"
#include "Test_Shared.h"
#include "Action00Record.h"


namespace {

// Add all properties, even with silly values, so that we can confirm the 
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "properties<Cargos, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        bit_number: 0x01;\n"
    "        cargo_type_name_id: 0x0102;\n"
    "        single_unit_name_id: 0x2003;\n"
    "        single_unit_id: 0x1004;\n"
    "        multiple_units_id: 0x0305;\n"
    "        cargo_type_abbrev_id: 0x2006;\n"
    "        cargo_sprite_id: 0x0707;\n"
    "        single_unit_weight: 0x08;\n"
    "        penalty_time_1: 0x09;\n"
    "        penalty_time_2: 0x0A;\n"
    "        base_price: 0x11223344;\n"
    "        station_list_colour: 0xD3;\n"
    "        payment_list_colour: 0xD4;\n"
    "        is_freight: true;\n"
    "        cargo_classes: 0x1111;\n"
    "        cargo_label: \"ABCD\";\n"
    "        town_growth_effect: 0x23;\n"
    "        town_growth_multiplier: 0x1234;\n"
    "        callback_flags: 0x45;\n"
    "        cargo_units_id: 0xD001;\n"
    "        cargo_amount_id: 0xD002;\n"
    "        capacity_multiplier: 0x0102;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "0B "             // Cargos
    "16 "             // 1x06 properties... 
    "01 "             // ... for 0x01 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 01 "
    "09 02 01 "
    "0A 03 20 "
    "0B 04 10 "
    "0C 05 03 "
    "0D 06 20 "
    "0E 07 07 "
    "0F 08 "
    "10 09 "
    "11 0A "
    "12 44 33 22 11 "
    "13 D3 "
    "14 D4 "
    "15 01 "
    "16 11 11 "
    "17 41 42 43 44 "
    "18 23 "
    "19 34 12 "
    "1A 45 "
    "1B 01 D0 "
    "1C 02 D0 "
    "1D 02 01 ";

} // namespace {


TEST_CASE("Action00Cargos", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
