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
#include "DateDescriptor.h"


// Based on the rules for the Gregorian calendar.
bool is_leap_year(uint32_t year)
{
    // It is a leap if it divides by 400.
    if ((year % 400) == 0)
        return true;
    // Otherwise it is not a leap year if it divides by 100.
    if ((year % 100) == 0)
        return false;
    // Otherwise it is a leap year if it divides by 4.
    return (year % 4) == 0;
}


// A table would be nicer, but we have to account for leap years.
uint8_t days_in_month(uint32_t year, uint8_t month)
{
    switch (month)
    {
    case 1:  return 31;
    case 2:  return (is_leap_year(year) ? 29 : 28);
    case 3:  return 31;
    case 4:  return 30;
    case 5:  return 31;
    case 6:  return 30;
    case 7:  return 31;
    case 8:  return 31;
    case 9:  return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    }

    // Should never get here.
    return 0;
}


// Calculates the number of days from year zero 0000/1/1.
uint32_t days_from_years(uint32_t years)
{
    constexpr uint32_t DAYS_400_YEARS = 365 * 400 + 97; // E.g. up to and including 31/12/399
    constexpr uint32_t DAYS_100_YEARS = 365 * 100 + 24; // E.g. up to and including 31/12/1899
    constexpr uint32_t DAYS_4_YEARS = 365 * 4 + 1;    // E.g. up to and including 31/12/1919

    uint32_t year = years;
    uint32_t days = 0;
    // Blocks of 400 years.
    days += (year / 400) * DAYS_400_YEARS;
    year = year % 400;
    // Then remaining blocks of 100 years.
    days += (year / 100) * DAYS_100_YEARS;
    year = year % 100;
    // Then remaining blocks of 4 years.
    days += (year / 4) * DAYS_4_YEARS;
    year = year % 4;

    // The odd years add more days on. We have to account for leaps years.
    // The thinking is that is the year is 1922, then we have already got the days up to
    // the beginning of 1920, and we have to add the days for 1920 (366 as it is a leap year),
    // and the days for 1921 (365). Not very happy with this calculation.
    switch (year)
    {
    case 3: days += is_leap_year(years - 3) ? 366 : 365;
    case 2: days += is_leap_year(years - 2) ? 366 : 365;
    case 1: days += is_leap_year(years - 1) ? 366 : 365;
    case 0:;
    }

    // Something I don't understand here. The century years which are not four century years
    // end up with a missing day somehow. Add it back here.
    year = years - (years % 4);
    if (((year % 400) != 0) && ((year % 100) == 0))
    {
        ++days;
    }

    return days;
}
