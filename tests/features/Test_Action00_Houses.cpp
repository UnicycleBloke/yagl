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
    "properties<Houses, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        substitute_building_id: 0x01;\n"
    "        building_flags: 0x02;\n"
    "        years_available: [ 1931 1967 ];\n"
    "        population: 0x20;\n"
    "        mail_multiplier: 0x03;\n"
    "        passenger_acceptance: 0x04;\n"
    "        mail_acceptance: 0x05;\n"
    "        goods_etc_acceptance: 0x06;\n"
    "        la_rating_decrease: 0x0107;\n"
    "        removal_cost_multiplier: 0x08;\n"
    "        building_name_id: 0xC109;\n"
    "        availability_mask: 0x1234;\n"
    "        callback_flags: 0x0A;\n"
    "        override_byte: 0x0B;\n"
    "        refresh_multiplier: 0x0C;\n"
    "        four_random_colours: [ 0x0D 0x0E 0x0F 0x10 ];\n"
    "        appearance_probability: 0x12;\n"
    "        extra_flags: 0x19;\n"
    "        animation_frames: 0x20;\n"
    "        animation_speed: 0x21;\n"
    "        building_class: 0x22;\n"
    "        callback_flags_2: 0x23;\n"
    "        accepted_cargo_types: [ 0x01 0x03 0x05 0x07 ];\n"
    "        minimum_life_years: 0x01C9;\n"
    "        accepted_cargo_list: [ 0x02 0x04 0x06 ];\n"
    "        long_minimum_year: 1931;\n"
    "        long_maximum_year: 1967;\n"
    "        tile_acceptance_list: [ {0x01: 0x12} {0x02: 0x13} ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "                 // Action00
    "07 "                 // Houses
    "1C "                 // 0x1C properties...
    "01 "                 // ... for 0x01 item
    "FF 23 00 "           // First ID 0x23 (extended byte)                substitute_building_id
    "08 01 "              // building_flags
    "09 02 "              // years_available
    "0A 0B 2F 0B 20 "     // population
    "0C 03 "              // mail_multiplier
    "0D 04 "              // passenger_acceptance
    "0E 05 "              // mail_acceptance
    "0F 06 "              // goods_etc_acceptance
    "10 07 01 "           // la_rating_decrease
    "11 08 "              // removal_cost_multiplier
    "12 09 C1 "           // building_name_id
    "13 34 12 "           // availability_mask
    "14 0A "              // callback_flags
    "15 0B "              // override_byte
    "16 0C "              // refresh_multiplier
    "17 0D 0E 0F 10 "     // four_random_colours
    "18 12 "              // appearance_probability
    "19 19 "              // extra_flags
    "1A 20 "              // animation_frames
    "1B 21 "              // animation_speed
    "1C 22 "              // building_class
    "1D 23 "              // callback_flags_2
    "1E 01 03 05 07 "     // accepted_cargo_types
    "1F C9 01 "           // minimum_life_years
    "20 03 02 04 06 "     // accepted_cargo_list
    "21 8B 07 "           // long_minimum_year
    "22 AF 07 "           // long_maximum_year
    "23 02 01 12 02 13 "; // tile_acceptance_list

} // namespace {


TEST_CASE("Action00Houses", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
