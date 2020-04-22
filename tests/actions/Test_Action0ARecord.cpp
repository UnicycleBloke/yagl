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
#include "Action0ARecord.h"

namespace {

// Add all properties, even with silly values, so that we can confirm the 
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "replacement_sprite_sets // Action0A\n"
    "{\n"
    "    replacement_sprite_set<0x0AAD> // <first_sprite>\n"
    "    {\n"
    "        // Replace sprite 0x0AAD:\n"
    "        sprite_id<0x00001057>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "            [32, 34, -16, -23], normal, c32bpp | mask | chunked, \"zbase_extra-32bpp-normal-0.png\", [540, 6719], \"zbase_extra-mask-normal-0.png\", [10, 293];\n"
    "            [128, 135, -64, -91], zin4, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin4-5.png\", [618, 8852], \"zbase_extra-mask-zin4-0.png\", [437, 3470];\n"
    "            [64, 68, -32, -46], zin2, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin2-1.png\", [516, 5703], \"zbase_extra-mask-zin2-0.png\", [289, 893];\n"
    "        }\n"
    "        // Replace sprite 0x0AAE:\n"
    "        sprite_id<0x00001058>\n"
    "        {\n"
    "            [20, 16, -14, -9], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [425, 7372];\n"
    "            [32, 24, -18, -13], normal, c32bpp | mask, \"zbase_extra-32bpp-normal-0.png\", [582, 6719], \"zbase_extra-mask-normal-0.png\", [52, 293];\n"
    "            [128, 95, -72, -51], zin4, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin4-5.png\", [10, 8997], \"zbase_extra-mask-zin4-0.png\", [575, 3470];\n"
    "            [64, 48, -36, -26], zin2, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin2-1.png\", [590, 5703], \"zbase_extra-mask-zin2-0.png\", [363, 893];\n"
    "        }\n"
    "    }\n"
    "    replacement_sprite_set<0x0ABD> // <first_sprite>\n"
    "    {\n"
    "        // Replace sprite 0x0ABD:\n"
    "        sprite_id<0x00001059>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "            [32, 34, -16, -23], normal, c32bpp | mask | chunked, \"zbase_extra-32bpp-normal-0.png\", [540, 6719], \"zbase_extra-mask-normal-0.png\", [10, 293];\n"
    "            [128, 135, -64, -91], zin4, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin4-5.png\", [618, 8852], \"zbase_extra-mask-zin4-0.png\", [437, 3470];\n"
    "            [64, 68, -32, -46], zin2, c32bpp | mask | chunked, \"zbase_extra-32bpp-zin2-1.png\", [516, 5703], \"zbase_extra-mask-zin2-0.png\", [289, 893];\n"
    "        }\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0A "     // Action0A
    "02 "     // 2 sprite sets
    "02 "     // 2 sprites in the first sprite set
    "AD 0A "  // First sprite ID 0x0AAD
    "01 "     // 1 sprite in the second sprite set 
    "BD 0A "; // First sprite ID 0x0ABD
 
} // namespace {


TEST_CASE("Action0ARecord", "[actions]")
{
    test_container<Action0ARecord, 0x0A>(str_YAGL, str_NFO);
}


