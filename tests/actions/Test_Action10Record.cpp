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
#include "Action10Record.h"


namespace {

static constexpr const char* str_YAGL =
    "label<0x23> // Action10 - target for Action07 or Action09\n"
    "{\n"
    "    \"This is an optional comment.\";\n"
    "}\n";
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "10 " // Action10
    "23 " // Label number
    // Optional comment.
    "54 68 69 73 20 69 73 20 61 6E 20 6F 70 74 69 6F "
       "6E 61 6C 20 63 6F 6D 6D 65 6E 74 2E ";

static constexpr const char* str_YAGL2_IN =
    "label<0x25>{}";
static constexpr const char* str_YAGL2_OUT =
    "label<0x25> // Action10 - target for Action07 or Action09\n"
    "{\n"
    "}\n";
static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "10 "  // Action10
    "25 "; // Label number

} // namespace {}


TEST_CASE("Action10Record", "[actions]")
{
    test_yagl<Action10Record, 0x10>(str_YAGL, str_NFO);
    test_yagl<Action10Record, 0x10>(str_YAGL2_IN, str_NFO2, str_YAGL2_OUT);
}

