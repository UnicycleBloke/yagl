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
#include "properties/VisualEffect.h"


static constexpr const char* str_YAGL1 = "effect(SteamPuffs, 0x01, Enable)";
static constexpr const char* str_NFO1  = "11 "; 

static constexpr const char* str_YAGL2 = "effect(DieselFumes, 0x02, Disable)";
static constexpr const char* str_NFO2  = "A2 "; 

static constexpr const char* str_YAGL3 = "effect(ElectricSparks, 0x06, Enable)";
static constexpr const char* str_NFO3  = "36 "; 

static constexpr const char* str_YAGL4 = "effect(SteamCallback, 0x00, Disable)";
static constexpr const char* str_NFO4  = "C1 "; 

static constexpr const char* str_YAGL5 = "effect(SteamCallback, 0x00, Enable)";
static constexpr const char* str_NFO5  = "41 "; 

static constexpr const char* str_YAGL6 = "effect(ElectricCallback, 0x00, Enable)";
static constexpr const char* str_NFO6  = "43 "; 

TEST_CASE("VisualEffect", "[properties]")
{
    test_yagl_fragment<VisualEffect>(str_YAGL1, str_NFO1);
    test_yagl_fragment<VisualEffect>(str_YAGL2, str_NFO2);
    test_yagl_fragment<VisualEffect>(str_YAGL3, str_NFO3);
    test_yagl_fragment<VisualEffect>(str_YAGL4, str_NFO4);
    test_yagl_fragment<VisualEffect>(str_YAGL5, str_NFO5);
    test_yagl_fragment<VisualEffect>(str_YAGL6, str_NFO6);
}
