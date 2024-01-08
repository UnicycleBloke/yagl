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
#include "Action02VariableRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "switch<Trains, 0xFD, PrimaryDWord> // Action02 variable\n"
    "{\n"
    "    expression:\n"
    "    {\n"
    "        value1 = variable[0x0C] & 0x0000FFFF;\n"
    "    };\n"
    "    ranges:\n"
    "    {\n"
    "        0x00000000: 0x00FA;\n"
    "        0x00000016: 0x00FE;\n"
    "        0x00000023: 0x8000;\n"
    "    };\n"
    "    default: 0x00FD;\n"
    "}\n";
// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "                // Action02
    "00 "                // Trains
    "FD "                // This set's ID
    "89 "                // VarAction type - primary object, double word size
    "0C "                // Variable
       "00 "             // Shift num
       "FF FF 00 00 "    // AND mask
    "03 "                // 3 ranges
       "FA 00 "          // Value
          "00 00 00 00 " // Range min
          "00 00 00 00 " // Range max
       "FE 00 "
          "16 00 00 00 "
          "16 00 00 00 "
       "00 80 "
          "23 00 00 00 "
          "23 00 00 00 "
    "FD 00 ";            // Default

} // namespace {


TEST_CASE("Action02VariableRecord", "[actions]")
{
    test_yagl<Action02VariableRecord, 0x02>(str_YAGL, str_NFO);
}

