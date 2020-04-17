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
#include "Action04Record.h"


namespace {

static constexpr const char* str_YAGL =
"strings<Trains, en_GB, 0xD098*> // <feature, language, first_id> Action04, English (GB)\n"
"{\n"
"    /* 0xD098 */ \"{black}StringA\";\n"
"    /* 0xD099 */ \"{black}StringB\";\n"
"    /* 0xD09A */ \"{black}StringC\";\n"
"}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "04 "    // Action04
    "00 "    // Trains
    "81 "    // English (GB) (01) and first ID is a word (80)
    "03 "    // 3 strings
    "98 D0 " // First ID
    "98 53 74 72 69 6E 67 41 00 "
    "98 53 74 72 69 6E 67 42 00 "
    "98 53 74 72 69 6E 67 43 00 ";

} // namespace {


TEST_CASE("Action04Record", "[actions]")
{
    test_yagl<Action04Record, 0x04>(str_YAGL, str_NFO);
}
