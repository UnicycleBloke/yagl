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
#include "properties/CargoAcceptance.h"


static constexpr const char* str_YAGL  = "{0x02: 0x12}";
static constexpr const char* str_YAGL2 = "[ {0x02: 0x12} {0x03: 0x23} {0x05: 0x55} {0x06: 0x67} ]";


// NFO matching the YAGL. Same as Array except preceded by the length.
static constexpr const char* str_NFO  = "02 12 ";
static constexpr const char* str_NFO2 = "04 02 12 03 23 05 55 06 67 ";


TEST_CASE("CargoAcceptance", "[properties]")
{
    test_yagl_fragment<CargoAcceptance>(str_YAGL, str_NFO);
    test_yagl_fragment<CargoAcceptanceList>(str_YAGL2, str_NFO2);
}
