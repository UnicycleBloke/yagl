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
#include "properties/BoolValue.h"


TEST_CASE("BoolValue", "[properties]")
{
    test_yagl_fragment<Bool>("true", "01 ");
    test_yagl_fragment<Bool>("false", "00 ");

    // According to the NewGRF specs: Is helicopter? 2=no, 0=yes 
    test_yagl_fragment<BoolHeli>("true", "00 ");
    test_yagl_fragment<BoolHeli>("false", "02 ");
}
