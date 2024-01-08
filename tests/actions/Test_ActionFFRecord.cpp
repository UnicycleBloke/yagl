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
#include "ActionFFRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "binary(\"sprites/test.wav\");\n";
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "FF "  // ActionFF
    "08 "  // Name length is 8
    // Name of binary: test.wav
    "74 65 73 74 2E 77 61 76 00 "
    // Content of binary - this is actually just a short text file called test.wav
    "54 68 69 73 20 69 73 20 61 20 70 72 65 74 65 6E "
    "64 20 57 41 56 20 66 69 6C 65 2E ";

} // namespace {}


TEST_CASE("ActionFFRecord", "[actions]")
{
    test_yagl<ActionFFRecord, 0xFF>(str_YAGL, str_NFO);
}
