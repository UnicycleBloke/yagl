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
    "        sprite_id<0x000001B2>\n"
    "        {\n"
    "            [8, 8, -3, -8], normal, c8bpp, \"dutchtrains-8bpp-normal-0.png\", [10, 10];\n"
    "        }\n"
    "        sprite_id<0x000001B3>\n"
    "        {\n"
    "            [9, 8, -1, -6], normal, c8bpp, \"dutchtrains-8bpp-normal-0.png\", [28, 10];\n"
    "        }\n"
    "    }\n"
    "    sprite_set // 0x0001\n"
    "    {\n"
    "        sprite_id<0x000001BA>\n"
    "        {\n"
    "            [8, 12, -3, -10], normal, c8bpp, \"dutchtrains-8bpp-normal-0.png\", [158, 10];\n"
    "        }\n"
    "        sprite_id<0x000001BB>\n"
    "        {\n"
    "            [12, 12, -4, -9], normal, c8bpp, \"dutchtrains-8bpp-normal-0.png\", [176, 10];\n"
    "        }\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "01 "
    "00 "
    "02 FF 02 00 ";
 
} // namespace {


TEST_CASE("Action01Record", "[actions]")
{
    test_yagl<Action01Record, 0x01>(str_YAGL, str_NFO);
}
