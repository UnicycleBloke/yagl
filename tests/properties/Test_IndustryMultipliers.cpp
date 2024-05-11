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
#include "properties/IndustryMultipliers.h"


static constexpr const char* str_YAGL = 
    "[ "
    "[ 0x0111 0x0222 0x0333 0x0444 ] " 
    "[ 0xA111 0xA222 0xA333 0xA444 ] "
    "[ 0xB111 0xB222 0xB333 0xB444 ] "
    "]";

static constexpr const char* str_NFO = 
    "03 04 11 01 22 02 33 03 44 04 11 A1 22 A2 33 A3 44 A4 11 B1 22 B2 33 B3 44 B4 ";


TEST_CASE("IndustryMultipliers", "[properties]")
{
    test_yagl_fragment<IndustryMultipliers>(str_YAGL, str_NFO);
}
