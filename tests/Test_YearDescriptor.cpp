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
#include "YearDescriptor.h"
#include "TokenStream.h"
#include <sstream>


TEST_CASE("YearDescriptor", "[integers]")
{
    SECTION("Year8")
    {
        for (uint16_t y = 1920; y < 2175; ++y)
        {
            Year8 year{y};
            CHECK(year.get() == y);

            std::ostringstream os;
            year.print(os);
            CHECK(os.str() == to_string(y, UIntFormat::Dec));

            std::istringstream is(os.str());
            TokenStream ts{is};
            Year8 year2;
            year2.parse(ts);
            CHECK(year2.get() == y);

            // Short years are offset from 1920 and must be positive values.
            std::ostringstream os2;
            year.write(os2);
            CHECK(os2.str().size() == 1);
            CHECK(uint8_t(os2.str()[0]) == (y - 1920));
        }
    }

    SECTION("Year16")
    {
        for (uint16_t y = 0; y < 50'000; y += 11)
        {
            Year16 year{ y };
            CHECK(year.get() == y);

            std::ostringstream os;
            year.print(os);
            CHECK(os.str() == to_string(y, UIntFormat::Dec));

            std::istringstream is(os.str());
            TokenStream ts{ is };
            Year16 year2;
            year2.parse(ts);
            CHECK(year2.get() == y);

            std::ostringstream os2;
            year.write(os2);
            CHECK(os2.str().size() == 2);
            CHECK(uint8_t(os2.str()[0]) == (y & 0xFF));
            CHECK(uint8_t(os2.str()[1]) == ((y >> 8) & 0xFF));
        }
    }

    SECTION("Year32")
    {
        for (uint32_t y = 0; y < 500'000; y += 111)
        {
            Year32 year{ y };
            CHECK(year.get() == y);

            std::ostringstream os;
            year.print(os);
            CHECK(os.str() == to_string(y, UIntFormat::Dec));

            std::istringstream is(os.str());
            TokenStream ts{ is };
            Year32 year2;
            year2.parse(ts);
            CHECK(year2.get() == y);

            std::ostringstream os2;
            year.write(os2);
            CHECK(os2.str().size() == 4);
            CHECK(uint8_t(os2.str()[0]) == (y & 0xFF));
            CHECK(uint8_t(os2.str()[1]) == ((y >> 8) & 0xFF));
            CHECK(uint8_t(os2.str()[2]) == ((y >> 16) & 0xFF));
            CHECK(uint8_t(os2.str()[3]) == ((y >> 24) & 0xFF));
        }
    }
}
