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
#include "StreamHelpers.h"


template <typename ActionRecord, uint8_t ACTION>
void test_yagl(const char* YAGL, const char* NFO)
{
    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts);

    std::ostringstream os;
    SpriteZoomMap sprites; // Empty set is fine for this record.
    //action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL);
    if (os.str() != YAGL)
    {
        std::cout << hex_dump(os.str(), true) << "\n";
        std::cout << hex_dump(YAGL, true) << "\n";
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
    CHECK(os.str() == YAGL);
}
