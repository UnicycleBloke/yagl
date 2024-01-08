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
    "properties<Objects, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        class_label: \"ABCD\";\n"
    "        class_text_id: 0xC099;\n"
    "        object_text_id: 0xC09A;\n"
    "        climate_availability: Arctic | Toyland;\n"
    "        size_xy: 0x44;\n"
    "        cost_factor: 0x12;\n"
    "        introduction_date: date(1800/1/1);\n"
    "        end_of_life_date: date(2000/1/1);\n"
    "        object_flags: 0x1234;\n"
    "        animation_info: 0x2345;\n"
    "        animation_speed: 0x23;\n"
    "        animation_triggers: 0x1357;\n"
    "        removal_cost_factor: 0x45;\n"
    "        callback_flags: 0x1020;\n"
    "        building_height: 0x34;\n"
    "        number_of_views: 0x45;\n"
    "        number_on_creation: 0x56;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "0F "             // Objects
    "11 "             // 17 properties...
    "01 "             // ... for 1 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 41 42 43 44 " // class_label
    "09 99 C0 "       // class_text_id
    "0A 9A C0 "       // object_text_id
    "0B 0A "          // climate_availability
    "0C 44 "          // size_xy
    "0D 12 "          // cost_factor
    "0E 1D 08 0A 00 " // introduction_date
    "0F 75 25 0B 00 " // end_of_life_date
    "10 34 12 "       // object_flags
    "11 45 23 "       // animation_info
    "12 23 "          // animation_speed
    "13 57 13 "       // animation_triggers
    "14 45 "          // removal_cost_factor
    "15 20 10 "       // callback_flags
    "16 34 "          // building_height
    "17 45 "          // number_of_views
    "18 56 ";         // number_on_creation

} // namespace {


TEST_CASE("Action00Objects", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
