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
    "properties<Industries, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        substitute_industry_id: 0x01;\n"
    "        industry_type_override: 0x02;\n"
    "        industry_layout: \n"
    "        {\n"
    "            layout_reference(0x25, 0x01);\n"
    "            layout\n"
    "            {\n"
    "                old_tile(-1, -3, 0x0035);\n" // Only 8 bits
    "                new_tile(5, -3, 0x2341);\n"
    "                clearance(2, 3);\n"
    "            }\n"
    "            layout\n"
    "            {\n"
    "                old_tile(-1, -3, 0x0035);\n" // Only 8 bits
    "                new_tile(5, -3, 0x2341);\n"
    "                clearance(2, 3);\n"
    "            }\n"
    "        };\n"
    "        production_flags: 0x03;\n"
    "        closure_msg_id: 0x1002;\n"
    "        production_up_id: 0x1003;\n"
    "        production_down_id: 0x1004;\n"
    "        fund_cost_multiplier: 0x34;\n"
    "        production_cargo_types: [ 0x01 0x03 ];\n"
    "        acceptance_cargo_types: [ 0x11 0x12 0x13 0x14 ];\n"
    "        production_multipliers_1: 0x08;\n"
    "        production_multipliers_2: 0x09;\n"
    "        minimum_distributed: 0x12;\n"
    "        sound_effects: [ 0x01 0x02 0x03 0x04 0x05 ];\n"
    "        conflicting_industries: [ 0x32 0x33 0x34 ];\n"
    "        random_probability: 0x25;\n"
    "        gameplay_probability: 0x26;\n"
    "        map_colour: 0x27;\n"
    "        special_flags: 0x12345678;\n"
    "        new_industry_text_id: 0x1234;\n"
    "        input_multipliers1: 0x01020304;\n"
    "        input_multipliers2: 0x02030405;\n"
    "        input_multipliers3: 0x03040506;\n"
    "        industry_name_id: 0xD011;\n"
    "        prospecting_probability: 0x12348765;\n"
    "        callback_flags_1: 0x11;\n"
    "        callback_flags_2: 0x23;\n"
    "        destruction_cost_multiplier: 0x04050607;\n"
    "        nearby_station_text_id: 0xD045;\n"
    "        production_cargo_list: [ 0x01 ];\n"
    "        acceptance_cargo_list: [ 0x02 0x03 ];\n"
    "        production_multipliers: [ 0x04 0x05 0x06 ];\n"
    "        input_cargo_multipliers: [ [ 0x1013 0x1014 ] [ 0x1023 0x1024 ] [ 0x1033 0x1034 ] ];\n"
    "    }\n"
    "}\n";

  // NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "0A "             // Industries
    "21 "             // 33 properties... 
    "01 "             // ... for 1 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 01 " 
    "09 02 "
    "0A 03 "          // 3 layouts
       "1D 00 00 00 " // Size of data
          "FE 25 01 " 
          "FF FD 35 05 FD FE 41 23 02 03 FF 00 80 " 
          "FF FD 35 05 FD FE 41 23 02 03 FF 00 80 "
    "0B 03 "
    "0C 02 10 "
    "0D 03 10 "
    "0E 04 10 "
    "0F 34 "
    "10 01 03 "
    "11 11 12 13 14 "
    "12 08 "
    "13 09 "
    "14 12 "
    "15 05 01 02 03 04 05 "
    "16 32 33 34 "
    "17 25 "
    "18 26 "
    "19 27 "
    "1A 78 56 34 12 "
    "1B 34 12 "
    "1C 04 03 02 01 "
    "1D 05 04 03 02 "
    "1E 06 05 04 03 "
    "1F 11 D0 "
    "20 65 87 34 12 "
    "21 11 "
    "22 23 "
    "23 07 06 05 04 "
    "24 45 D0 "
    "25 01 01 "
    "26 02 02 03 "
    "27 03 04 05 06 "
    "28 03 02 "
       "13 10 14 10 "
       "23 10 24 10 "
       "33 10 34 10 ";

} // namespace {


TEST_CASE("Action00Industries", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
