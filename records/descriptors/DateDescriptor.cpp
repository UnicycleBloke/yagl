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
static constexpr bool is_leap_year(uint32_t year)
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
static constexpr uint8_t days_in_month(uint32_t year, uint8_t month)
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
static constexpr uint32_t days_from_years(uint32_t years)
{
    constexpr uint32_t DAYS_400_YEARS = 365 * 400 + 97; // E.g. up to and including 31/12/399
    constexpr uint32_t DAYS_100_YEARS = 365 * 100 + 24; // E.g. up to and including 31/12/1899
    constexpr uint32_t DAYS_4_YEARS   = 365 * 4 + 1;    // E.g. up to and including 31/12/1919

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
        case 0: ;
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


void Date::from_days(uint32_t days)
{
    // I seem to have an off-by-one error somewhere.
    ++days;

    // Use a simple iterative approach to help guarantee that 
    // the date to days conversion matches the days to date conversion.
    year = days / 365;
    while (days_from_years(year) < days)
    {
        ++year;
    }
    while (days_from_years(year) >= days)
    {
        --year;
    }
    days -= days_from_years(year);

    month = 1;
    while (days > days_in_month(year, month))
    {
        days -= days_in_month(year, month);
        ++month;
    }

    day = static_cast<uint8_t>(days);
}


uint32_t Date::to_days() const
{
    uint32_t days = days_from_years(year);

    // Add days for the whole months already elapsed.
    switch (month)
    {
    case 12: days += 30; // Nov
    case 11: days += 31; // Oct
    case 10: days += 30; // Sep
    case  9: days += 31; // Aug
    case  8: days += 31; // Jul
    case  7: days += 30; // Jun
    case  6: days += 31; // May
    case  5: days += 30; // Apr
    case  4: days += 31; // Mar
    case  3: days += (is_leap_year(year) ? 29 : 28); // Feb
    case  2: days += 31; // Jan
    //case  1: days +=  0; 
    //default: 
    }

    days += day;

    // I seem to have an off-by-one error somewhere.
    --days;

    return days;
}


// Calculates the number of days from year zero 0000/1/1.
// This is expensive, but simple enough to be correct (hopefully). Used
// to check the calculation in days_from_years().
// static constexpr uint32_t days_from_years_test(uint32_t years)
// {
//     uint32_t days = 0;
//     for (uint32_t y = 0; y < years; ++y)
//     {
//         days += is_leap_year(y) ? 366 : 365;
//     }
//     return days;
// }


// void increment(Date& date)
// {
//     if (date.day < days_in_month(date.year, date.month))
//     {
//         ++date.day;
//     }
//     else
//     {
//         date.day = 1;
//         if (date.month < 12)
//         {
//             ++date.month;
//         }
//         else
//         {
//             date.month = 1;
//             ++date.year;
//         }
//     }
// }


// bool operator==(const Date& lhs, const Date& rhs)
// {
//     return (lhs.year == rhs.year) && (lhs.month == rhs.month) && (lhs.day == rhs.day);
// }


// bool operator!=(const Date& lhs, const Date& rhs)
// {
//     return !(lhs == rhs);
// }


// Date date{0, 1, 1};
// uint32_t days = days_from_date(date);

// for (uint32_t d = days; d < days + (365 * 10'000); ++d)
// {
//     if (d != days_from_date(date))
//     {
//         std::cout << "days " << d;
//     }

//     if (date != date_from_days(d))
//     {
//         std::cout << "date" << d;
//     }

//     increment(date);
// }
