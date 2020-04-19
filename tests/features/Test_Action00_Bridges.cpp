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
    "properties<Bridges, 0x0009> // Action00\n"
    "{\n"
    "    // instance_id: 0x0009\n"
    "    {\n"
    "        fallback_type_id: 0x01;\n"
    "        year_available: 1927;\n"
    "        minimum_length: 0x03;\n"
    "        maximum_length: 0x32;\n"
    "        cost_factor: 0x56;\n"
    "        maximum_speed: 0x1245;\n"
    "        bridge_layout: \n"
    "        {\n"
    "            table<0x12>\n"
    "            {\n"
    "                0x12345600 0x12345601 0x12345602 0x12345603 0x12345604 0x12345605 0x12345606 0x12345607 \n"
    "                0x12345608 0x12345609 0x1234560A 0x1234560B 0x1234560C 0x1234560D 0x1234560E 0x1234560F \n"
    "                0x12345610 0x12345611 0x12345612 0x12345613 0x12345614 0x12345615 0x12345616 0x12345617 \n"
    "                0x12345618 0x12345619 0x1234561A 0x1234561B 0x1234561C 0x1234561D 0x1234561E 0x1234561F \n"
    "            }\n"
    "            table<0x13>\n"
    "            {\n"
    "                0x22345600 0x22345601 0x22345602 0x22345603 0x22345604 0x22345605 0x22345606 0x22345607 \n"
    "                0x22345608 0x22345609 0x2234560A 0x2234560B 0x2234560C 0x2234560D 0x2234560E 0x2234560F \n"
    "                0x22345610 0x22345611 0x22345612 0x22345613 0x22345614 0x22345615 0x22345616 0x22345617 \n"
    "                0x22345618 0x22345619 0x2234561A 0x2234561B 0x2234561C 0x2234561D 0x2234561E 0x2234561F \n"
    "            }\n"
    "        };\n"
    "        various_flags: 0x12;\n"
    "        long_year_available: 1927;\n"
    "        purchase_text: 0xD123;\n"
    "        description_rail: 0xD124;\n"
    "        description_road: 0xD125;\n"
    "        cost_factor_word: 0xD126;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "06 "             // AirportTiles
    "0D "             // 0x06 properties... 
    "01 "             // ... for 0x01 item
    "FF 09 00 "       // First ID 0x23 (extended byte)
    "00 01 "
    "08 07 "
    "09 03 "
    "0A 32 "
    "0B 56 "
    "0C 45 12 "
    "0D "
       "12 "          // First ID 
       "02 "          // 2 tables  
       // Table 12
       "00 56 34 12 01 56 34 12 02 56 34 12 03 56 34 12 04 56 34 12 05 56 34 12 06 56 34 12 07 56 34 12 "
       "08 56 34 12 09 56 34 12 0A 56 34 12 0B 56 34 12 0C 56 34 12 0D 56 34 12 0E 56 34 12 0F 56 34 12 "
       "10 56 34 12 11 56 34 12 12 56 34 12 13 56 34 12 14 56 34 12 15 56 34 12 16 56 34 12 17 56 34 12 "
       "18 56 34 12 19 56 34 12 1A 56 34 12 1B 56 34 12 1C 56 34 12 1D 56 34 12 1E 56 34 12 1F 56 34 12 "
       // Table 13
       "00 56 34 22 01 56 34 22 02 56 34 22 03 56 34 22 04 56 34 22 05 56 34 22 06 56 34 22 07 56 34 22 "
       "08 56 34 22 09 56 34 22 0A 56 34 22 0B 56 34 22 0C 56 34 22 0D 56 34 22 0E 56 34 22 0F 56 34 22 "
       "10 56 34 22 11 56 34 22 12 56 34 22 13 56 34 22 14 56 34 22 15 56 34 22 16 56 34 22 17 56 34 22 "
       "18 56 34 22 19 56 34 22 1A 56 34 22 1B 56 34 22 1C 56 34 22 1D 56 34 22 1E 56 34 22 1F 56 34 22 "
    "0E 12 "
    "0F 87 07 00 00 "
    "10 23 D1 "
    "11 24 D1 "
    "12 25 D1 "
    "13 26 D1 ";

} // namespace {


TEST_CASE("Action00Bridges", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}

