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
#include "properties/Vector.h"
#include "properties/IntegerValue.h"


static constexpr const char* str_YAGL16 = "[ 0x1020 0x2030 0x3243 ]";
static constexpr const char* str_YAGL32 = "[ 0x00001020 0x00002030 0x00003243 0x12235435 ]";

// NFO matching the YAGL. Same as Array except preceded by the length.
static constexpr const char* str_NFO16 = "03 20 10 30 20 43 32 ";
static constexpr const char* str_NFO32 = "04 20 10 00 00 30 20 00 00 43 32 00 00 35 54 23 12 ";


TEST_CASE("Vector<UInt16>", "[properties]")
{
    test_yagl_fragment<Vector<UInt16>>(str_YAGL16, str_NFO16);
}


TEST_CASE("Vector<UInt32>", "[properties]")
{
    test_yagl_fragment<Vector<UInt32>>(str_YAGL32, str_NFO32);
}


