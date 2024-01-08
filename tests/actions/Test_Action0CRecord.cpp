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
#include "Action0CRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "ignore // Action0C\n"
    "{\n"
    "    \"This text is basically a comment preserved in the GRF file.\";\n"
    "}\n";
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0C "  // Action0C
    "54 68 69 73 20 74 65 78 74 20 69 73 20 62 61 73 "
    "69 63 61 6C 6C 79 20 61 20 63 6F 6D 6D 65 6E 74 "
    "20 70 72 65 73 65 72 76 65 64 20 69 6E 20 74 68 "
    "65 20 47 52 46 20 66 69 6C 65 2E ";

// This demonstrates string concantenation but the test would fail because
// printing the output does not split the string out again. Use an alternate
// string for the output which reflects this.
static constexpr const char* str_YAGL2_IN =
    "ignore // Action0C\n"
    "{\n"
    "    \"This text is basically a comment preserved in the GRF file. \"\n"
    "    \"This text is basically a comment preserved in the GRF file. \"\n"
    "    \"This text is basically a comment preserved in the GRF file. \"\n"
    "    \"This text is basically a comment preserved in the GRF file. \";\n"
    "}\n";
static constexpr const char* str_YAGL2_OUT =
    "ignore // Action0C\n"
    "{\n"
    "    \"This text is basically a comment preserved in the GRF file. "
    "This text is basically a comment preserved in the GRF file. "
    "This text is basically a comment preserved in the GRF file. "
    "This text is basically a comment preserved in the GRF file. \";\n"
    "}\n";
static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0C "  // Action0C
    "54 68 69 73 20 74 65 78 74 20 69 73 20 62 61 73 "
    "69 63 61 6C 6C 79 20 61 20 63 6F 6D 6D 65 6E 74 "
    "20 70 72 65 73 65 72 76 65 64 20 69 6E 20 74 68 "
    "65 20 47 52 46 20 66 69 6C 65 2E 20 "
    "54 68 69 73 20 74 65 78 74 20 69 73 20 62 61 73 "
    "69 63 61 6C 6C 79 20 61 20 63 6F 6D 6D 65 6E 74 "
    "20 70 72 65 73 65 72 76 65 64 20 69 6E 20 74 68 "
    "65 20 47 52 46 20 66 69 6C 65 2E 20 "
    "54 68 69 73 20 74 65 78 74 20 69 73 20 62 61 73 "
    "69 63 61 6C 6C 79 20 61 20 63 6F 6D 6D 65 6E 74 "
    "20 70 72 65 73 65 72 76 65 64 20 69 6E 20 74 68 "
    "65 20 47 52 46 20 66 69 6C 65 2E 20 "
    "54 68 69 73 20 74 65 78 74 20 69 73 20 62 61 73 "
    "69 63 61 6C 6C 79 20 61 20 63 6F 6D 6D 65 6E 74 "
    "20 70 72 65 73 65 72 76 65 64 20 69 6E 20 74 68 "
    "65 20 47 52 46 20 66 69 6C 65 2E 20 ";

} // namespace {


TEST_CASE("Action0CRecord", "[actions]")
{
    test_yagl<Action0CRecord, 0x0C>(str_YAGL, str_NFO);
    // This feature breaks others - namely we need lists of strings to be comma separated.
    //test_yagl<Action0CRecord, 0x0C>(str_YAGL2_IN, str_NFO2, str_YAGL2_OUT);
}
