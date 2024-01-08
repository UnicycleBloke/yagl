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
#include "DescriptorBase.h"
#include "IntegerDescriptor.h"
#include "StreamHelpers.h"


bool     is_leap_year(uint32_t year);
uint8_t  days_in_month(uint32_t year, uint8_t month);
uint32_t days_from_years(uint32_t years);


template <typename T>
class Date
{
private:
    static constexpr uint32_t    BASE_DAYS = 701'265; // See specs: 1920/1/1
    static constexpr const char* str_date  = "date";

public:
    // Only really needed for testing.
    Date(uint32_t year, uint8_t month, uint8_t day)
    : m_year{year}
    , m_month{month}
    , m_day{day}
    {
    }

    // Required or default ctor would be deleted.
    Date()
    {
    }

    void print(std::ostream& os) const
    {
        os << str_date << "(";
        os << to_string(m_year, UIntFormat::Dec)  << "/";
        os << to_string(m_month, UIntFormat::Dec) << "/";
        os << to_string(m_day, UIntFormat::Dec)   << ")";
    }

    void parse(TokenStream& is)
    {
        is.match_ident(str_date);
        is.match(TokenType::OpenParen);
        m_year  = is.match_uint32();
        is.match(TokenType::OpDivide);
        m_month = is.match_uint8();
        is.match(TokenType::OpDivide);
        m_day   = is.match_uint8();
        is.match(TokenType::CloseParen);
    }

    void read(std::istream& is)
    {
        uint32_t days;
        if constexpr (std::is_same_v<uint16_t, T>)
        {
            days  = read_uint16(is);
            //days += BASE_DATE.to_days();
            days += BASE_DAYS;
        }
        else
        {
            days = read_uint32(is);
        }
        from_days(days);
    }

    void write(std::ostream& os) const
    {
        uint32_t days = to_days();
        if constexpr (std::is_same_v<uint16_t, T>)
        {
            //days -= BASE_DATE.to_days();
            days -= BASE_DAYS;
            write_uint16(os, days);
        }
        else
        {
            write_uint32(os, days);
        }
    }

    // Only really needed for testing.
    uint32_t year() const  { return m_year; }
    uint8_t  month() const { return m_month; }
    uint8_t  day() const   { return m_day; }
    uint32_t days() const
    {
        uint32_t days = to_days();
        if constexpr (std::is_same_v<uint16_t, T>)
        {
            days -= BASE_DAYS;
        }
        return days;
    }

private:
    void from_days(uint32_t days);
    uint32_t to_days() const;

private:
    uint32_t m_year  = 1920;
    uint8_t  m_month = 1;
    uint8_t  m_day   = 1;
};


template <typename T>
void Date<T>::from_days(uint32_t days)
{
    // I seem to have an off-by-one error somewhere.
    ++days;

    // Use a simple iterative approach to help guarantee that
    // the date to days conversion matches the days to date conversion.
    m_year = days / 365;
    while (days_from_years(m_year) < days)
    {
        ++m_year;
    }
    while (days_from_years(m_year) >= days)
    {
        --m_year;
    }
    days -= days_from_years(m_year);

    m_month = 1;
    while (days > days_in_month(m_year, m_month))
    {
        days -= days_in_month(m_year, m_month);
        ++m_month;
    }

    m_day = static_cast<uint8_t>(days);
}


template <typename T>
uint32_t Date<T>::to_days() const
{
    uint32_t days = days_from_years(m_year);

    // Add days for the whole months already elapsed.
    switch (m_month)
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
    case  3: days += (is_leap_year(m_year) ? 29 : 28); // Feb
    case  2: days += 31; // Jan
    //case  1: days +=  0;
    //default:
    }

    days += m_day;

    // I seem to have an off-by-one error somewhere.
    --days;

    return days;
}


using LongDate  = Date<uint32_t>;
using ShortDate = Date<uint16_t>;

using LongDateDescriptor  = GenericDescriptor<LongDate>;
using ShortDateDescriptor = GenericDescriptor<ShortDate>;