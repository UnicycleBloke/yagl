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
#include "properties/IndustryLayout.h"


static constexpr const char* str_YAGL = 
R"(
{
    layout_reference(0x25, 0x01);
    layout
    {
        old_tile(-1, -3, 0x0035);
        new_tile(5, -3, 0x2341);
        clearance(2, 3);
    }
    layout
    {
        old_tile(-1, -3, 0x0035);
        new_tile(5, -3, 0x2341);
        clearance(2, 3);
    }
})";

static constexpr const char* str_NFO = 
    "03 "                   // 3 layouts
       "1D 00 00 00 "       // Size of data
          "FE 25 01 "       // Layout 1 (reference)
          "FF FD 35 "       // Layout 2
          "05 FD FE 41 23 " 
          "02 03 FF "
          "00 80 "          // Terminator 
          "FF FD 35 "       // Layout 3
          "05 FD FE 41 23 " 
          "02 03 FF " 
          "00 80 ";         // Terminator 


TEST_CASE("IndustryLayout", "[properties]")
{
    test_yagl_fragment<IndustryLayouts>(str_YAGL, str_NFO);
}
