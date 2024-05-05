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
    "properties<Airports, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        airport_override_id: 0x12;\n"
    "        airport_layouts: \n"
    "        [\n"
    "            layout<North>\n"
    "            {\n"
    "                old_tile(-1, -3, 0x0035);\n" // Only 8 bits
    "                new_tile(5, -3, 0x2341);\n"
    "                clearance(2, 3);\n"
    "            }\n"
    "            layout<South>\n"
    "            {\n"
    "                old_tile(-1, -3, 0x0035);\n" // Only 8 bits
    "                new_tile(5, -3, 0x2341);\n"
    "                clearance(2, 3);\n"
    "            }\n"
    "        ];\n"
    "        years_available: [ 1900 1950 ];\n"
    "        compatible_ttd_airport: 0x12;\n"
    "        catchment_area: 0x09;\n"
    "        noise_level: 0x11;\n"
    "        airport_name_id: 0x1234;\n"
    "        maintenance_cost_factor: 0x5678;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "0D "             // Airports
    "08 "             // 0x08 properties...
    "01 "             // ... for 0x01 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 12 "
    "0A 02 "
       "1C 00 00 00 "
       "00 "
          "FF FD 35 "
          "05 FD FE 41 23 "
          "02 03 FF 00 80 "
       "04 "
          "FF FD 35 "
          "05 FD FE 41 23 "
          "02 03 FF 00 80 "
    "0C 6C 07 9E 07 "
    "0D 12 "
    "0E 09 "
    "0F 11 "
    "10 34 12 "
    "11 78 56 ";

} // namespace {


TEST_CASE("Action00Airports", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}


// TODO more detailed testing on AirportLayoutDescriptor



