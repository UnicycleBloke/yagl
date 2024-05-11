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
#include "properties/GRFLabel.h"


static constexpr const char* str_YAGL = R"("ABCD")";
static constexpr const char* str_NFO  = "41 42 43 44 "; 

static constexpr const char* str_YAGL2 = R"("\x01\x02\x03\x04")";
static constexpr const char* str_NFO2  = "01 02 03 04 "; 

static constexpr const char* str_YAGL3 = R"--("\x0A\x0D:)")--";
static constexpr const char* str_NFO3  = "0A 0D 3A 29 "; 


TEST_CASE("GRFLabel", "[properties]")
{
    test_yagl_fragment<GRFLabel>(str_YAGL, str_NFO);
    test_yagl_fragment<GRFLabel>(str_YAGL2, str_NFO2);
    test_yagl_fragment<GRFLabel>(str_YAGL3, str_NFO3);
}
