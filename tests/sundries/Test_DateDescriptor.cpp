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
#include "DateDescriptor.h"
#include "TokenStream.h"
#include <sstream>


template <typename T>
Date<T> increment(const Date<T>& date)
{
    uint32_t year  = date.year();
    uint8_t  month = date.month();
    uint8_t  day   = date.day();  

    ++day;
    if (day > days_in_month(year, month))
    {
        day = 1;
        ++month;
        if (month > 12)
        {
            month = 1;
            ++year;
        }
    }

    return Date<T>{year, month, day};
}


TEST_CASE("Long and short dates", "[integers]")
{
    SECTION("LongDate")
    {
        LongDate date{1066, 10, 14};
        CHECK(date.year()  == 1066);
        CHECK(date.month() == 10);
        CHECK(date.day()   == 14);

        std::ostringstream os;
        date.print(os);
        CHECK(os.str() == "date(1066/10/14)");

        std::istringstream is(os.str());
        TokenStream ts{is};
        LongDate date2;
        CHECK(date2.year()  == 1920);
        CHECK(date2.month() == 1);
        CHECK(date2.day()   == 1);
        CHECK(date2.days()  == 701'265);
        date2.parse(ts);
        CHECK(date2.year()  == 1066);
        CHECK(date2.month() == 10);
        CHECK(date2.day()   == 14);

        // Short years are offset from 1920 and must be positive values.
        std::ostringstream os2;
        date2.write(os2);
        CHECK(os2.str().size() == 4);

        const uint32_t days_max = days_from_years(3'000);
        LongDate date3{0, 1, 1};
        for (uint32_t days = 0; days < days_max; ++days)
        {
            CHECK(date3.days() == days);
            date3 = increment(date3);   
        }
    }

    SECTION("ShortDate")
    {
        ShortDate date{1966, 12, 29};
        CHECK(date.year()  == 1966);
        CHECK(date.month() == 12);
        CHECK(date.day()   == 29);

        std::ostringstream os;
        date.print(os);
        CHECK(os.str() == "date(1966/12/29)");

        std::istringstream is(os.str());
        TokenStream ts{is};
        ShortDate date2;
        CHECK(date2.year()  == 1920);
        CHECK(date2.month() == 1);
        CHECK(date2.day()   == 1);
        CHECK(date2.days()  == 0);
        date2.parse(ts);
        CHECK(date2.year()  == 1966);
        CHECK(date2.month() == 12);
        CHECK(date2.day()   == 29);

        // Short years are offset from 1920 and must be positive values.
        std::ostringstream os2;
        date2.write(os2);
        CHECK(os2.str().size() == 2);

        const uint32_t days_max = days_from_years(3'000) - 701'265;
        ShortDate date3;
        for (uint32_t days = 0; days < days_max; ++days)
        {
            CHECK(date3.days() == days);
            date3 = increment(date3);   
        }
    }
}