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
