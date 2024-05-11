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
#include "properties/SpriteLayout.h"


static constexpr const char* str_YAGL1 = 
R"(
{
    ground_sprite<0x000003F4>
    {
    }
    building_sprite<0x8000042D>
    {
        offset: 0x00, 0x05, 0x00;
        extent: 0x06, 0x06, 0x06;
        registers: 
        {
            hide_sprite: 0x84;
            sprite_offset: 0x85;
            sprite_var10: 0x01;
        }
    }
    building_sprite<0x8000042D>
    {
        offset: 0x0A, 0x05, 0x00;
        extent: 0x06, 0x06, 0x06;
        registers: 
        {
            hide_sprite: 0x86;
            sprite_offset: 0x87;
            sprite_var10: 0x01;
        }
    }
    building_sprite<0x8000042D>
    {
        offset: 0x00, 0x00, 0x00;
        extent: 0x10, 0x06, 0x06;
        registers: 
        {
            hide_sprite: 0x88;
            sprite_offset: 0x89;
            sprite_var10: 0x03;
        }
    }
    child_sprite<0x8000042D>
    {
        offset: 0x00, 0x00;
        registers: 
        {
            sprite_offset: 0x8A;
            sprite_var10: 0x04;
        }
    }
    building_sprite<0x8000042D>
    {
        offset: 0x00, 0x0A, 0x00;
        extent: 0x10, 0x06, 0x06;
        registers: 
        {
            hide_sprite: 0x8B;
            sprite_offset: 0x8C;
            sprite_var10: 0x03;
        }
    }
    child_sprite<0x8000042D>
    {
        offset: 0x00, 0x00;
        registers: 
        {
            sprite_offset: 0x8D;
            sprite_var10: 0x04;
        }
    }
})";

static constexpr const char* str_NFO1 = 
    "46 "                                            // Advance layout with 6 building sprites
    "F4 03 00 00 00 00 "                             // Ground sprite and flags
    "2D 04 00 80 43 00 00 05 00 06 06 06 84 85 01 "  // Building sprite, flags, offset, extent, registers
    "2D 04 00 80 43 00 0A 05 00 06 06 06 86 87 01 " 
    "2D 04 00 80 43 00 00 00 00 10 06 06 88 89 03 "
    "2D 04 00 80 42 00 00 00 80 8A 04 "
    "2D 04 00 80 43 00 00 0A 00 10 06 06 8B 8C 03 " 
    "2D 04 00 80 42 00 00 00 80 8D 04 ";


// Check all the registers for parent and child building sprites.
static constexpr const char* str_YAGL2 = 
R"(
{
    ground_sprite<0x000003F4>
    {
        registers: 
        {
            hide_sprite: 0x86;
            sprite_offset: 0x87;
            palette_offset: 0x21;
            palette_act01: true;
            offset_x: 0x23;
            offset_y: 0x24;
            offset_z: 0x25;
            sprite_var10: 0x11;
            palette_var10: 0x26;
        }
    }
    building_sprite<0x8000042D>
    {
        offset: 0x01, 0x03, 0xF0;
        extent: 0x06, 0x06, 0x06;
        registers: 
        {
            hide_sprite: 0x84;
            sprite_offset: 0x85;
            palette_offset: 0x11;
            palette_act01: true;
            offset_x: 0x13;
            offset_y: 0x14;
            offset_z: 0x15;
            sprite_var10: 0x01;
            palette_var10: 0x16;
        }
    }
    child_sprite<0x8000042D>
    {
        offset: 0x00, 0x00;
        registers: 
        {
            hide_sprite: 0x84;
            sprite_offset: 0x85;
            palette_offset: 0x11;
            palette_act01: true;
            offset_x: 0x13;
            offset_y: 0x14;
            sprite_var10: 0x01;
            palette_var10: 0x16;
        }
    }
})";

static constexpr const char* str_NFO2 = 
    "42 "
    "F4 03 00 00 FF 00 86 87 21 23 24 25 11 26 " 
    "2D 04 00 80 FF 00 01 03 F0 06 06 06 84 85 11 13 14 15 01 16 "
    "2D 04 00 80 FF 00 00 00 80 84 85 11 13 14 01 16 ";


TEST_CASE("SpriteLayout", "[properties]")
{
    test_yagl_fragment<SpriteLayout>(str_YAGL1, str_NFO1);
    test_yagl_fragment<SpriteLayout>(str_YAGL2, str_NFO2);
}
