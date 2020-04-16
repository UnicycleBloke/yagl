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
#include "Action06Record.h"


namespace {

static constexpr const char* str_YAGL =
"modify_next // Action06\n"
"{\n"
"    // modification(value, num_bytes, offset, add?);\n"
"    modification(parameter[0x6C], 1, 8, false);\n"
"    modification(parameter[0x6B], 1, 9, false);\n"
"}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "06 "              // Action06
    "6C 01 FF 08 00 "  // Parameter 6C, length 1, offset 8 (extended byte in long form)
    "6B 01 FF 09 00 "  // Parameter 6B, length 1, offset 9 (extended byte in long form)
    "FF ";             // Terminator

} // namespace {


TEST_CASE("Action06Record", "[actions]")
{
    test_yagl<Action06Record, 0x06>(str_YAGL, str_NFO);
}
