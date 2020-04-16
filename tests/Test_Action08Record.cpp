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
#include "Action08Record.h"


namespace {

static constexpr const char* str_YAGL =
"grf // Action08\n"
"{\n"
//"    grf_id: \"\xFB\xFB\x06\x01\";\n"
"    grf_id: \"ABCD\";\n"
"    version: GRF8;\n"
"    name: \"Dutch Trainset v7393 (474ff0e257af)\";\n"
"    description: \"{lt-gray}Dutch Trains for OpenTTD {new-line}{black}First vehicle: 1839\";\n"
"}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "08 "           // Action08
    "08 "           // GRF8
    "41 42 43 44 "  // ABCD
    // Name
    "44 75 74 63 68 20 54 72 61 69 6E 73 65 74 20 76 "
    "37 33 39 33 20 28 34 37 34 66 66 30 65 32 35 37 "
    "61 66 29 00 "
    // Description
    "89 44 75 74 63 68 20 54 72 61 69 6E 73 20 66 6F "
    "72 20 4F 70 65 6E 54 54 44 20 0D 98 46 69 72 73 "
    "74 20 76 65 68 69 63 6C 65 3A 20 31 38 33 39 00 ";
} // namespace {


TEST_CASE("Action08Record", "[actions]")
{
    test_yagl<Action08Record, 0x08>(str_YAGL, str_NFO);
}

