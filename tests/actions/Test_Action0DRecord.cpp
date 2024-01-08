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
#include "Action0DRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "set_parameter<Param> // Action0D\n"
    "{\n"
    "    expression: parameter[0x6B] = parameter[0x6A] & parameter[0x69];\n"
    "    not_if_defined: false;\n"
    "}\n";
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0D "  // Action0D
    "6B "  // Target parameter = 6B
    "07 "  // Operation = bitwise AND
    "6A "  // First argument parameter = 6A
    "69 "; // Second argument parameter = 69

static constexpr const char* str_YAGL2 =
    "set_parameter<Param> // Action0D\n"
    "{\n"
    "    expression: parameter[0x6B] = parameter[0x6A] | parameter[0x69];\n"
    "    not_if_defined: true;\n"
    "}\n";
static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0D "  // Action0D
    "6B "  // Target parameter = 6B
    "88 "  // Operation = bitwise OR, do nothing if parameter defined already (80)
    "6A "  // First argument parameter = 6A
    "69 "; // Second argument parameter = 69

static constexpr const char* str_YAGL3 =
    "set_parameter<Param> // Action0D\n"
    "{\n"
    "    expression: parameter[0x6B] = parameter[0x6A] % parameter[0x69], signed;\n"
    "    not_if_defined: false;\n"
    "}\n";
static constexpr const char* str_NFO3 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0D "  // Action0D
    "6B "  // Target parameter = 6B
    "0C "  // Operation = signed modulo
    "6A "  // First argument parameter = 6A
    "69 "; // Second argument parameter = 69

static constexpr const char* str_YAGL4 =
    "set_parameter<Param> // Action0D\n"
    "{\n"
    "    expression: parameter[0x6B] = parameter[0x6A] % parameter[0x69], unsigned;\n"
    "    not_if_defined: false;\n"
    "}\n";
static constexpr const char* str_NFO4 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0D "  // Action0D
    "6B "  // Target parameter = 6B
    "0B "  // Operation = unsigned modulo
    "6A "  // First argument parameter = 6A
    "69 "; // Second argument parameter = 69

} // namespace {}


TEST_CASE("Action0DRecord", "[actions]")
{
    test_yagl<Action0DRecord, 0x0D>(str_YAGL, str_NFO);
    test_yagl<Action0DRecord, 0x0D>(str_YAGL2, str_NFO2);
    test_yagl<Action0DRecord, 0x0D>(str_YAGL3, str_NFO3);
    test_yagl<Action0DRecord, 0x0D>(str_YAGL4, str_NFO4);
}

