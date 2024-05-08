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
#include "properties/AirportLayout.h"


static constexpr const char* str_YAGL = 
R"(
[
    layout<North>
    {
        old_tile(2, 3, 0x0034);
        new_tile(-2, -3, 0x1235);
        clearance(4, -5);
        old_tile(6, 7, 0x0037);
    }
    layout<South>
    {
        old_tile(2, 3, 0x0034);
        new_tile(-2, -3, 0x1237);
        clearance(4, -5);
        old_tile(6, 7, 0x0037);
    }
    layout<East>
    {
        old_tile(2, 3, 0x0034);
        new_tile(-2, -3, 0x1239);
        clearance(4, -5);
        old_tile(6, 7, 0x0037);
    }
    layout<West>
    {
        old_tile(2, 3, 0x0034);
        new_tile(-2, -3, 0x1231);
        clearance(4, -5);
        old_tile(6, 7, 0x0017);
    }
])";


// NFO matching the YAGL.
static constexpr const char* str_NFO = 
    "04 44 00 00 00 00 02 03 34 FE FD FE 35 12 04 FB FF 06 07 37 00 80 04 02 03 "
    "34 FE FD FE 37 12 04 FB FF 06 07 37 00 80 02 02 03 34 FE FD FE 39 12 04 FB FF "
    "06 07 37 00 80 06 02 03 34 FE FD FE 31 12 04 FB FF 06 07 17 00 80 ";


TEST_CASE("AirportLayout", "[properties]")
{
    test_yagl_fragment<AirportLayouts>(str_YAGL, str_NFO);
}


