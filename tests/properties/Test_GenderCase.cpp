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
#include "properties/GenderCase.h"


static constexpr const char* str_YAGL = "[ 0x01:\"one\" 0x02:\"two\" 0x04:\"four\" 0x10:\"sixteen\" 0x07:\"seven\" ]";


// NFO matching the YAGL. Same as Array except preceded by the length.
static constexpr const char* str_NFO = 
    "01 6F 6E 65 00 02 74 77 6F 00 04 66 6F 75 72 00 10 "
    "73 69 78 74 65 65 6E 00 07 73 65 76 65 6E 00 00 ";



TEST_CASE("GenderCase", "[properties]")
{
    test_yagl_fragment<GenderCase>(str_YAGL, str_NFO);
}
