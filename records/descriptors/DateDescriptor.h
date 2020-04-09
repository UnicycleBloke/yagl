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


struct Date
{
    uint32_t year  = 1920;
    uint8_t  month = 1;
    uint8_t  day   = 1;

    void from_days(uint32_t days);
    uint32_t to_days() const;
};


template <typename T>
struct DateDescriptor : PropertyDescriptor
{
    static constexpr Date BASE_DATE{1920, 1, 1};

    void print(const Date& date, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);

        const IntegerDescriptorT<uint32_t> desc{0, "", PropFormat::Dec};
        os << "date(";
        os << desc.to_string(date.year)  << "/";
        os << desc.to_string(date.month) << "/";
        os << desc.to_string(date.day)   << ");\n";
    }

    void parse(Date& date, TokenStream& is) const
    {
        is.match_ident("date");
        is.match(TokenType::OpenParen);
        date.year  = is.match_uint32();
        is.match(TokenType::OpDivide);
        date.month = is.match_uint8();
        is.match(TokenType::OpDivide);
        date.day   = is.match_uint8();
        is.match(TokenType::CloseParen);
    }

    void read(Date& date, std::istream& is) const
    {
        uint32_t days;
        if constexpr (std::is_same_v<uint16_t, T>)
        {
            days  = read_uint16(is);
            days += BASE_DATE.to_days();
        }
        else
        {
            days = read_uint32(is);
        }        
        date.from_days(days);
    }

    void write(const Date& date, std::ostream& os) const
    {
        uint32_t days = date.to_days();
        if constexpr (std::is_same_v<uint16_t, T>)
        {
            days -= BASE_DATE.to_days();
            write_uint16(os, days);
        }
        else
        {
            write_uint32(os, days);
        }
    }
};


using LongDateDescriptor  = DateDescriptor<uint32_t>;
using ShortDateDescriptor = DateDescriptor<uint16_t>;