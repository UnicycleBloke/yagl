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
#include "Action05Record.h"

namespace {

// Add all properties, even with silly values, so that we can confirm the
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "replace_sprites<Aqueducts, 0x0000> // <new_feature_type, offset>  Action05\n"
    "{\n"
    "    // Replace Aqueducts sprite 0x0000\n"
    "    sprite_id<0x00000113>\n"
    "    {\n"
    "        [62, 39, -30, -14], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [10, 10];\n"
    "        [64, 45, -32, -13], normal, c32bpp | chunked, \"zbase_extra-32bpp-normal-0.png\", [10, 10];\n"
    "        [256, 179, -128, -51], zin4, c32bpp | chunked, \"zbase_extra-32bpp-zin4-0.png\", [10, 10];\n"
    "        [128, 90, -64, -26], zin2, c32bpp | chunked, \"zbase_extra-32bpp-zin2-0.png\", [10, 10];\n"
    "    }\n"
    "    // Replace Aqueducts sprite 0x0001\n"
    "    sprite_id<0x00000114>\n"
    "    {\n"
    "        [62, 39, -30, -14], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [82, 10];\n"
    "        [64, 45, -32, -13], normal, c32bpp | chunked, \"zbase_extra-32bpp-normal-0.png\", [84, 10];\n"
    "        [256, 179, -128, -51], zin4, c32bpp | chunked, \"zbase_extra-32bpp-zin4-0.png\", [276, 10];\n"
    "        [128, 90, -64, -26], zin2, c32bpp | chunked, \"zbase_extra-32bpp-zin2-0.png\", [148, 10];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "05 "         // Action05
    "12 "         // Aqueducts
    "FF 02 00 ";  // 2 sprites follow (extended byte)

} // namespace {


TEST_CASE("Action05Record", "[actions]")
{
    // TODO test_container<Action05Record, 0x05>(str_YAGL, str_NFO);
}
