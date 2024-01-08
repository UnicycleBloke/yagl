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
#include "Action02IndustryRecord.h"


namespace {

static constexpr const char* str_YAGL0 =
    "industry<Industries, 0x45, Version0> // Action02 industry\n"
    "{\n"
    "    sub_in_amounts: [ 0x1001 0x2002 0x3003 ];\n"
    "    add_out_amounts: [ 0x4004 0x5005 ];\n"
    "    repeat_flag: false;\n"
    "}\n";

static constexpr const char* str_YAGL1 =
    "industry<Industries, 0x45, Version1> // Action02 industry\n"
    "{\n"
    "    sub_in_registers: [ 0x01 0x02 0x03 ];\n"
    "    add_out_registers: [ 0x04 0x05 ];\n"
    "    repeat_register: 0x06;\n"
    "}\n";

static constexpr const char* str_YAGL2 =
    "industry<Industries, 0x45, Version2> // Action02 industry\n"
    "{\n"
    "    sub_in_cargos: [ (0x01, 0x02) (0x04, 0x05) (0x61, 0x07) (0x08, 0x09) ];\n"
    "    add_out_cargos: [ (0x11, 0x12) (0x14, 0x15) (0x62, 0x17) ];\n"
    "    repeat_register: 0x06;\n"
    "}\n";


// NFO matching the YAGL.
static constexpr const char* str_NFO0 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "                // Action02
    "0A "                // Industries
    "45 "                // This set ID
    "00 "                // Version 0
    "01 10 02 20 03 30 " // Subtract in (3 words - amounts)
    "04 40 05 50 "       // Add out (2 words - amounts)
    "00 ";               // Repeat flag

static constexpr const char* str_NFO1 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "                // Action02
    "0A "                // Industries
    "45 "                // This set ID
    "01 "                // Version 1
    "01 02 03 "          // Subtract in (3 bytes - registers contain amounts)
    "04 05 "             // Add out (2 bytes - register contain amounts)
    "06 ";               // Repeat register - contains flag

static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "                // Action02
    "0A "                // Industries
    "45 "                // This set ID
    "02 "                // Version 2
    "04 "                // Subtract in - 4 items
       "01 02 "          // Cargo index and register for amount
       "04 05 "          // - ditto -
       "61 07 "          // - ditto -
       "08 09 "          // - ditto -
    "03 "                // Add out - 3 items
       "11 12 "          // Cargo index and register for amount
       "14 15 "          // - ditto -
       "62 17 "          // - ditto -
    "06 ";               // Repeat register - contains flag

} // namespace {


TEST_CASE("Action02IndustryRecord", "[actions]")
{
    test_yagl<Action02IndustryRecord, 0x02>(str_YAGL0, str_NFO0);
    test_yagl<Action02IndustryRecord, 0x02>(str_YAGL1, str_NFO1);
    test_yagl<Action02IndustryRecord, 0x02>(str_YAGL2, str_NFO2);
}

