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
#include <sstream>
#include <iostream>
#include <array>
#include "TokenStream.h"
#include "Action08Record.h"
#include "StreamHelpers.h"


namespace {

using ActionXXRecord = Action08Record;

static constexpr uint8_t     ACTION = 0x08;
static constexpr const char* NAME   = "Action08";

static constexpr const char* YAGL =
"grf // Action08\n"
"{\n"
//"    grf_id: \"\xFB\xFB\x06\x01\";\n"
"    grf_id: \"ABCD\";\n"
"    version: GRF8;\n"
"    name: \"Dutch Trainset v7393 (474ff0e257af)\";\n"
"    description: \"{lt-gray}Dutch Trains for OpenTTD {new-line}{black}First vehicle: 1839\";\n"
"}\n";

// NFO matching the YAGL.
static const std::string NFO =
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


TEST_CASE(NAME, "[actions]")
{
    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL);
    TokenStream ts{is};
    ActionXXRecord action;
    action.parse(ts);

    std::ostringstream os;
    SpriteZoomMap sprites; // Empty set is fine for this record.
    action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL);

    // Confirm that the written binary matches the sample.
    os.str("");
    GRFInfo info; // Defaults to Container2 and GRF8.
    action.write(os, info);
    auto str = os.str();
    CHECK(str.size() == (NFO.size() / 3));
    CHECK(hex_dump(str) == NFO);

    // Confirm that reading the binary and printing the 
    // result gets us back to the example.
    std::istringstream is2(str);
    CHECK(uint8_t(is2.get()) == ACTION);
    ActionXXRecord action2;
    action2.read(is2, info);
    os.str("");
    action2.print(os, sprites, 0);
    CHECK(os.str() == YAGL);
}

