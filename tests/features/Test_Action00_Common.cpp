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
    "properties<Ships, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    // These properties are common to all four vehicle types.
    "        introduction_date: date(1925/3/5);\n"
    "        reliability_decay_speed: 18;\n"
    "        vehicle_life_years: 36;\n"
    "        model_life_years: 53;\n"
    "        climate_availability: Temperate | Arctic | Tropical | Toyland;\n"
    "        loading_speed: 0x27;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "02 "             // Ships
    "06 "             // 1x06 properties...
    "01 "             // ... for 0x01 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "00 62 07 "       // introduction_date
    "02 12 "          // reliability_decay_speed
    "03 24 "          // vehicle_life_years
    "04 35 "          // model_life_years
    "06 0F "          // climate_availability
    "07 27 ";         // loading_speed

} // namespace {


TEST_CASE("Action00Vehicles", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
