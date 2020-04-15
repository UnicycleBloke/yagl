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
#include "Action06Record.h"
#include "StreamHelpers.h"


namespace {

using ActionXXRecord = Action06Record;

static constexpr uint8_t     ACTION = 0x06; 
static constexpr const char* NAME   = "Action06";

static constexpr const char* YAGL =
"modify_next // Action06\n"
"{\n"
"    // modification(value, num_bytes, offset, add?);\n"
"    modification(parameter[0x6C], 1, 8, false);\n"
"    modification(parameter[0x6B], 1, 9, false);\n"
"}\n";

// NFO matching the YAGL.
static const std::string NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "06 "              // Action06
    "6C 01 FF 08 00 "  // Parameter 6C, length 1, offset 8 (extended byte in long form)
    "6B 01 FF 09 00 "  // Parameter 6B, length 1, offset 9 (extended byte in long form)
    "FF ";             // Terminator

} // namespace {


TEST_CASE(NAME, "[actions]")
{
    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL);
    TokenStream ts{ is };
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
