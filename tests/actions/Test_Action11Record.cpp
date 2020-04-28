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
#include "Action11Record.h"


namespace {

static constexpr const char* str_YAGL =
    "sound_effects // Action11\n"
    "{\n"
    "    sprite_id<0x00004A72>\n"
    "    {\n"
    "        binary(\"sprites\\test.wav\");\n"
    "    }\n"
    "    sprite_id<0x00004A73>\n"
    "    {\n"
    "        binary(\"sprites\\test.wav\");\n"
    "    }\n"
    "    binary(\"sprites\\test.wav\");\n"
    "    binary(\"sprites\\test.wav\");\n"
    "    import(\"GRFx\", 0x0001);\n"
    "    import(\"GRFx\", 0x0002);\n"
    "}\n";

    static constexpr const char* str_NFO = 
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "11 "     // Action11
    "06 00 "; // 6 items

} // namespace {}


TEST_CASE("Action11Record", "[actions]")
{
    test_container<Action11Record, 0x11>(str_YAGL, str_NFO);
}
