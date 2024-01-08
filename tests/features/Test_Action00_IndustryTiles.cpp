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
    "properties<IndustryTiles, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        substitute_building_id: 0x12;\n"
    "        industry_tile_override: 0x13;\n"
    "        tile_acceptance1: 0x1014;\n"
    "        tile_acceptance2: 0x0115;\n"
    "        tile_acceptance3: 0x2016;\n"
    "        land_shape_flags: 0x17;\n"
    "        callback_flags: 0x18;\n"
    "        animation_info: 0x3019;\n"
    "        animation_speed: 0x20;\n"
    "        callback_25_triggers: 0x21;\n"
    "        special_flags: 0x22;\n"
    "        cargo_acceptance_list: [ {0x02: 0x12} {0x03: 0x23} ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "09 "             // IndustryTiles
    "0C "             // 12 properties...
    "01 "             // ... for 1 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 12 "          // substitute_building_id
    "09 13 "          // industry_tile_override
    "0A 14 10 "       // tile_acceptance1
    "0B 15 01 "       // tile_acceptance2
    "0C 16 20 "       // tile_acceptance3
    "0D 17 "          // land_shape_flags
    "0E 18 "          // callback_flags
    "0F 19 30 "       // animation_info
    "10 20 "          // animation_speed
    "11 21 "          // callback_25_triggers
    "12 22 "          // special_flags
    "13 02 "          // cargo_acceptance_list
       "02 12 "
       "03 23 ";

} // namespace {


TEST_CASE("Action00IndustryTiles", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
