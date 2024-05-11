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
#include "properties/YearValue.h"


static constexpr const char* str_YAGL1a = "1930";
static constexpr const char* str_NFO1a  = "0A "; // 1920 + 10 = 1930
static constexpr const char* str_YAGL1b = "2000";
static constexpr const char* str_NFO1b  = "50 "; // 1920 + 80 = 2000
static constexpr const char* str_YAGL1c = "2033";
static constexpr const char* str_NFO1c  = "71 "; // 1920 + 113 = 2033

static constexpr const char* str_YAGL2a = "1930";
static constexpr const char* str_NFO2a  = "8A 07 "; // 1930 in hex
static constexpr const char* str_YAGL2b = "2000";
static constexpr const char* str_NFO2b  = "D0 07 ";
static constexpr const char* str_YAGL2c = "2033";
static constexpr const char* str_NFO2c  = "F1 07 ";

static constexpr const char* str_YAGL3a = "1930";
static constexpr const char* str_NFO3a  = "8A 07 00 00 ";
static constexpr const char* str_YAGL3b = "2000";
static constexpr const char* str_NFO3b  = "D0 07 00 00 ";
static constexpr const char* str_YAGL3c = "2033";
static constexpr const char* str_NFO3c  = "F1 07 00 00 ";


TEST_CASE("YearValue", "[properties]")
{
    test_yagl_fragment<Year8>(str_YAGL1a, str_NFO1a);
    test_yagl_fragment<Year8>(str_YAGL1b, str_NFO1b);
    test_yagl_fragment<Year8>(str_YAGL1c, str_NFO1c);

    test_yagl_fragment<Year16>(str_YAGL2a, str_NFO2a);
    test_yagl_fragment<Year16>(str_YAGL2b, str_NFO2b);
    test_yagl_fragment<Year16>(str_YAGL2c, str_NFO2c);

    test_yagl_fragment<Year32>(str_YAGL3a, str_NFO3a);
    test_yagl_fragment<Year32>(str_YAGL3b, str_NFO3b);
    test_yagl_fragment<Year32>(str_YAGL3c, str_NFO3c);
}
