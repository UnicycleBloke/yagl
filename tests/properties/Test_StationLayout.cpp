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
#include "properties/StationLayout.h"


static constexpr const char* str_YAGL = 
R"(
[
    tile<0x13245768>
    {
        sprite(0x12345678, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06);
        sprite(0x1234567A, 0x11, 0x22);
    }
    tile<0x13245769>
    {
        sprite(0x12345678, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06);
        sprite(0x1234567C, 0x21, 0x32);
        sprite(0x1234567D, 0x23, 0x34);
    }
])";

static constexpr const char* str_NFO = 
  "FF 02 00 "                         // 2 tiles
  "68 57 24 13 "                      // Tile 1
     "01 02 03 04 05 06 78 56 34 12 " 
     "11 22 80 00 00 00 7A 56 34 12 "
     "80 "
  "69 57 24 13 "                      // Tile 2
     "01 02 03 04 05 06 78 56 34 12 " 
     "21 32 80 00 00 00 7C 56 34 12 "
     "23 34 80 00 00 00 7D 56 34 12 "
     "80 ";


TEST_CASE("StationLayout", "[properties]")
{
    test_yagl_fragment<StationLayout>(str_YAGL, str_NFO);
}
