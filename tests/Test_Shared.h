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
#pragma once
#include "catch.hpp"
#include "StreamHelpers.h"


template <typename ActionRecord, uint8_t ACTION>
void test_yagl_matches(const char* YAGL_IN, const char* YAGL_OUT)
{
    SpriteZoomMap sprites; 
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts, sprites);
    std::ostringstream os;
    action.print(os, sprites, 0);

    SpriteZoomMap sprites2; 
    std::istringstream is2(YAGL_IN);
    TokenStream ts2{is2};
    ActionRecord action2;
    action2.parse(ts2, sprites2);
    std::ostringstream os2;
    action2.print(os2, sprites2, 0);

    CHECK(os.str() == os2.str());
}


// This parses some YAGL and ensures that printing it back out again matches. The 
// second YAGL parameter can be used when we expect the printed output to be different
// to the input (due to ordering differences). The parsed information is also written 
// out as binary and read back in, and then printed again, to confirm that the binary 
// works as expected.
template <typename ActionRecord, uint8_t ACTION>
void test_yagl(const char* YAGL_IN, const char* NFO, const char* YAGL_OUT = nullptr)
{
    if (YAGL_OUT == nullptr)
    {
        YAGL_OUT = YAGL_IN;
    }
    if (YAGL_OUT != YAGL_IN)
    {
        test_yagl_matches<ActionRecord, ACTION>(YAGL_OUT, YAGL_IN);
    }

    SpriteZoomMap sprites; 

    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts, sprites);

    std::ostringstream os;
    //action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL_OUT);
    if (os.str() != YAGL_OUT)
    {
        std::cout << hex_dump(os.str(), true) << "\n";
        std::cout << hex_dump(YAGL_OUT, true) << "\n";
    }

    // Confirm that the written binary matches the sample.
    os.str("");
    GRFInfo info; // Defaults to Container2 and GRF8.
    action.write(os, info);
    auto str = os.str();
    CHECK(str.size() == (std::strlen(NFO) / 3));
    CHECK(hex_dump(str) == NFO);

    // Confirm that reading the binary and printing the 
    // result gets us back to the example.
    std::istringstream is2(str);
    CHECK(uint8_t(is2.get()) == ACTION);
    ActionRecord action2;
    action2.read(is2, info);
    os.str("");
    action2.print(os, sprites, 0);
    CHECK(os.str() == YAGL_OUT);
}
