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
#include "Action01Record.h"


namespace {

// Add all properties, even with silly values, so that we can confirm the
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "sprite_sets<Trains, 0x0000> // <feature, first_set> Action01\n"
    "{\n"
    "    sprite_set // 0x0000\n"
    "    {\n"
    "        sprite_id<0x00001057>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "            [32, 34, -16, -23], normal, c32bpp | mask | chunked, \"zbase_extra-32bpp-normal-0.png\", [540, 6719], \"zbase_extra-mask-normal-0.png\", [10, 293];\n"
    "        }\n"
    "        sprite_id<0x00001058>\n"
    "        {\n"
    "            [20, 16, -14, -9], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [425, 7372];\n"
    "            [32, 24, -18, -13], normal, c32bpp | mask, \"zbase_extra-32bpp-normal-0.png\", [582, 6719], \"zbase_extra-mask-normal-0.png\", [52, 293];\n"
    "        }\n"
    "    }\n"
    "    sprite_set // 0x0001\n"
    "    {\n"
    "        sprite_id<0x00001059>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "            [32, 34, -16, -23], normal, c32bpp | mask | chunked, \"zbase_extra-32bpp-normal-0.png\", [540, 6719], \"zbase_extra-mask-normal-0.png\", [10, 293];\n"
    "        }\n"
    "        sprite_id<0x0000105A>\n"
    "        {\n"
    "            [20, 16, -6, -9], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [494, 7372];\n"
    "            [32, 24, -14, -13], normal, c32bpp | mask, \"zbase_extra-32bpp-normal-0.png\", [679, 6719], \"zbase_extra-mask-normal-0.png\", [149, 293];\n"
    "        }\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "01 "        // Action01
    "00 "        // Trains
    "02 "        // 2 sprite sets ...
    "FF 02 00 "; // ... each containing 2 sprites (extended byte)

} // namespace {


TEST_CASE("Action01Record", "[actions]")
{
    test_container<Action01Record, 0x01>(str_YAGL, str_NFO);
}
