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
#include "Action0ERecord.h"


namespace {

static constexpr const char* str_YAGL =
    "disable_grfs // Action0E\n"
    "{\n"
    "    grf_ids: [ \"ABCD\" \"ABC\\x01\" \"xV4\\x12\" ];\n"
    "}\n";
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0E "           // Action0E
    "03 "           // 3 items
    "41 42 43 44 "  // ABCD
    "41 42 43 01 "  // ABC\x01
    "78 56 34 12 "; // 0x12345678 -> xV4\x12

} // namespace {}


TEST_CASE("Action0ERecord", "[actions]")
{
    test_yagl<Action0ERecord, 0x0E>(str_YAGL, str_NFO);
}

