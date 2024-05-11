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
#include "properties/DateValue.h"


static constexpr const char* str_YAGL = "date(2024/12/29)";


// NFO matching the YAGL. Same as Array except preceded by the length.
static constexpr const char* str_NFO  = "1E 49 0B 00 "; // 739,614 days. 701,265 is 1920/1/1.
static constexpr const char* str_NFO2 = "CD 95 ";       //  38,349 days. 739,614 - 701,265.



TEST_CASE("DateValue", "[properties]")
{
    test_yagl_fragment<LongDate>(str_YAGL, str_NFO);
    test_yagl_fragment<ShortDate>(str_YAGL, str_NFO2);
}
