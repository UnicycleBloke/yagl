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
#include "Action12Record.h"


namespace {

// Add all properties, even with silly values, so that we can confirm the
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "unicode_glyphs // Action12\n"
    "{\n"
    "    range<SmallProp, 0x0AAD> // <font, base_char>\n"
    "    {\n"
    "        // Replace character 0x00000AAD\n"
    "        sprite_id<0x00001057>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "        }\n"
    "        // Replace character 0x00000AAE\n"
    "        sprite_id<0x00001058>\n"
    "        {\n"
    "            [20, 16, -14, -9], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [425, 7372];\n"
    "        }\n"
    "    }\n"
    "    range<LargeProp, 0x0AAE> // <font, base_char>\n"
    "    {\n"
    "        // Replace character 0x00000AAE\n"
    "        sprite_id<0x00001059>\n"
    "        {\n"
    "            [8, 21, -3, -11], normal, c8bpp, \"zbase_extra-8bpp-normal-0.png\", [407, 7372];\n"
    "        }\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "12 "     // Action12
    "02 "     // 2 sprite sets
    // First set:
    "01 "     // SmallProp
    "02 "     // 2 glyphs
    "AD 0A "  // First character
    // Second set:
    "02 "     // LargeProp
    "01 "     // 1 glyph
    "AE 0A "; // First character

} // namespace {


TEST_CASE("Action12Record", "[actions]")
{
    test_container<Action12Record, 0x12>(str_YAGL, str_NFO);
}
