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
#include "Action02SpriteLayoutRecord.h"


namespace {

static constexpr const char* str_YAGL =
    "sprite_layout<Houses, 0xF4> // Action02 sprite layout\n"
    "{\n"
    "    ground_sprite<0x10101010>\n"
    "    {\n"
    "        registers: \n" 
    "        {\n"
    "            hide_sprite: 0x12;\n"
    "            sprite_offset: 0x23;\n"
    "            palette_offset: 0x34;\n"
    "            palette_act01: true;\n"
    "            offset_x: 0x45;\n"
    "            offset_y: 0x56;\n"
    "            offset_z: 0x57;\n"
    "            sprite_var10: 0x67;\n"
    "            palette_var10: 0x78;\n"
    "        }\n"
    "    }\n"
    "    building_sprite<0x10101020>\n"
    "    {\n"
    "        offset: 0x01, 0x02, 0x03;\n"
    "        extent: 0x04, 0x05, 0x06;\n"
    "        registers: \n"
    "        {\n"
    "            hide_sprite: 0x12;\n"
    "            sprite_offset: 0x23;\n"
    "            palette_offset: 0x34;\n"
    "            palette_act01: true;\n"
    "            offset_x: 0x45;\n"
    "            offset_y: 0x56;\n"
    "            offset_z: 0x57;\n"
    "            sprite_var10: 0x67;\n"
    "            palette_var10: 0x78;\n"
    "        }\n"
    "    }\n"
    "    child_sprite<0x10101030>\n"
    "    {\n"
    "        offset: 0x07, 0x08;\n"
    "        registers: \n"
    "        {\n"
    "            hide_sprite: 0x11;\n"
    "            sprite_offset: 0x22;\n"
    "            palette_offset: 0x33;\n"
    "            palette_act01: true;\n"
    "            offset_x: 0x44;\n"
    "            offset_y: 0x55;\n"
    "            sprite_var10: 0x66;\n"
    "            palette_var10: 0x77;\n"
    "        }\n"
    "    }\n"
    "}\n";
// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 "
    "07 "
    "F4 "
    "42 "
       "10 10 10 10 "
          "FF 00 "
          "12 "
          "23 "
          "34 "
          "45 "
          "56 "
          "57 "
          "67 "
          "78 "
       "20 10 10 10 "
          "FF 00 "
          "01 02 03 "
          "04 05 06 " 
          "12 "
          "23 "
          "34 "
          "45 "
          "56 "
          "57 "
          "67 "
          "78 "
       "30 10 10 10 "
          "FF 00 "
          "07 08 80 "
          "11 "
          "22 "
          "33 "
          "44 "
          "55 "
          "66 "
          "77 ";

} // namespace {


TEST_CASE("Action02SpriteLayoutRecord", "[actions]")
{
    test_yagl<Action02SpriteLayoutRecord, 0x02>(str_YAGL, str_NFO);
}

