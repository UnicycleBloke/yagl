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
#include "properties/CustomStation.h"


static constexpr const char* str_YAGL = 
R"(
[
    layout<NE_SW>
    {
        // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight
        B B B B B B ;
        B B B B B B ;
        B B B B B B ;
    }
    layout<NW_SE>
    {
        // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight
        B B P P P B B R B B ;
        L R B P P P P B B B ;
    }
])";

static constexpr const char* str_NFO = 
  "06 03 02 02 02 02 02 02 02 02 02 02 02 02 02 02 02 02 02 02 0A 02 03 03 01 "
  "01 01 03 03 07 03 03 05 07 03 01 01 01 01 03 03 03 00 00 ";


TEST_CASE("CustomStation", "[properties]")
{
    test_yagl_fragment<CustomStation>(str_YAGL, str_NFO);
}
