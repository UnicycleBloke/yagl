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
#include "Action02BasicRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "sprite_groups<Trains, 0xFE> // Action02 basic\n"
    "{\n"
    "    primary_spritesets: [ 0x0000 ];\n"
    "    secondary_spritesets: [ 0x0000 ];\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "     // Action02
    "00 "     // Feature - Trains
    "FE "     // Set ID
    "01 "     // Number of entities 1
    "01 "     // Number of entities 2
    "00 00 "  // Entities 1 - 1 word
    "00 00 "; // Entities 2 - 1 word

} // namespace {


TEST_CASE("Action02Basic", "[actions]")
{
    test_yagl<Action02BasicRecord, 0x02>(str_YAGL, str_NFO);
}

