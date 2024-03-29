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
    "properties<GlobalSettings, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        cost_base_multipliers: 0x35;\n"
    "        cargo_translation_table: \"COAL\";\n"
    "        currency_display_names: 0xD012;\n"
    "        currency_multipliers: 0x12345678;\n"
    "        currency_options: 0x1040;\n"
    "        currency_symbols_prefix: \"CUR1\";\n"
    "        currency_symbols_suffix: \"CUR2\";\n"
    "        euro_introduction_dates: 0x1234;\n"
    "        snow_line_table: \n"
    "        [\n"
    "            0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 \n"
    "            0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 \n"
    "            0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 \n"
    "            0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 \n"
    "            0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 \n"
    "            0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 \n"
    "            0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 \n"
    "            0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 \n"
    "            0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 \n"
    "            0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 \n"
    "            0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 \n"
    "            0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 \n"
    "        ];\n"
    "        grf_overrides: [ \"ABCD\" \"EFGH\" ];\n"
    "        railtype_translation_table: \"XYZ1\";\n"
    "        gender_translation_table: [ 0x01:\"male\" 0x02:\"female\" ];\n"
    "        case_translation_table: [ 0x03:\"males\" 0x04:\"females\" ];\n"
    "        plural_form: 0x78;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "                // Action00
    "08 "                // GlobalSettings
    "0E "                // 14 properties...
    "01 "                // ... for 1 item
    "FF 23 00 "          // First ID 0x23 (extended byte)
    "08 35 "             // cost_base_multipliers
    "09 43 4F 41 4C "    // cargo_translation_table
    "0A 12 D0 "          // currency_display_names
    "0B 78 56 34 12 "    // currency_multipliers
    "0C 40 10 "          // currency_options
    "0D 43 55 52 31 "    // currency_symbols_prefix
    "0E 43 55 52 32 "    // currency_symbols_suffix
    "0F 34 12 "          // euro_introduction_dates
    "10 "                // snow_line_table
       "01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 "
       "11 12 13 14 15 16 17 18 11 12 13 14 15 16 17 18 11 12 13 14 15 16 17 18 11 12 13 14 15 16 17 18 "
       "21 22 23 24 25 26 27 28 21 22 23 24 25 26 27 28 21 22 23 24 25 26 27 28 21 22 23 24 25 26 27 28 "
       "31 32 33 34 35 36 37 38 31 32 33 34 35 36 37 38 31 32 33 34 35 36 37 38 31 32 33 34 35 36 37 38 "
       "41 42 43 44 45 46 47 48 41 42 43 44 45 46 47 48 41 42 43 44 45 46 47 48 41 42 43 44 45 46 47 48 "
       "51 52 53 54 55 56 57 58 51 52 53 54 55 56 57 58 51 52 53 54 55 56 57 58 51 52 53 54 55 56 57 58 "
       "61 62 63 64 65 66 67 68 61 62 63 64 65 66 67 68 61 62 63 64 65 66 67 68 61 62 63 64 65 66 67 68 "
       "71 72 73 74 75 76 77 78 71 72 73 74 75 76 77 78 71 72 73 74 75 76 77 78 71 72 73 74 75 76 77 78 "
       "81 82 83 84 85 86 87 88 81 82 83 84 85 86 87 88 81 82 83 84 85 86 87 88 81 82 83 84 85 86 87 88 "
       "91 92 93 94 95 96 97 98 91 92 93 94 95 96 97 98 91 92 93 94 95 96 97 98 91 92 93 94 95 96 97 98 "
       "A1 A2 A3 A4 A5 A6 A7 A8 A1 A2 A3 A4 A5 A6 A7 A8 A1 A2 A3 A4 A5 A6 A7 A8 A1 A2 A3 A4 A5 A6 A7 A8 "
       "B1 B2 B3 B4 B5 B6 B7 B8 B1 B2 B3 B4 B5 B6 B7 B8 B1 B2 B3 B4 B5 B6 B7 B8 B1 B2 B3 B4 B5 B6 B7 B8 "
    "11 41 42 43 44 45 46 47 48 "      // grf_overrides
    "12 58 59 5A 31 "                  // railtype_translation_table
    "13 "                              // gender_translation_table
       "01 6D 61 6C 65 00 "
       "02 66 65 6D 61 6C 65 00 "
       "00 "
    "14 "                              // case_translation_table
       "03 6D 61 6C 65 73 00 "
       "04 66 65 6D 61 6C 65 73 00 "
       "00 "
    "15 78 ";                          // plural_form

} // namespace {


TEST_CASE("Action00GlobalSettings", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
