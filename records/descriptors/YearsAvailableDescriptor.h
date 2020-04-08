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


template <typename T>
using YearsAvailable = std::array<T, 2>;
using YearsAvailable8  = YearsAvailable<uint8_t>;
using YearsAvailable16 = YearsAvailable<uint16_t>;


template <typename T>
struct YearsAvailableDescriptor : PropertyDescriptor
{
    PropFormat format;

    void print(const YearsAvailable<T>& values, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);

        const IntegerDescriptorT<uint16_t> desc{index, name, format};
        os << "[";
        os << " " << desc.to_string(values[0]);
        os << " " << desc.to_string(values[1]);
        os << " ];\n";
    }

    void parse(YearsAvailable<T>& values, TokenStream& is) const
    {
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            values[0] = is.match_integer_t<T>();
            values[1] = is.match_integer_t<T>();
        }
        is.match(TokenType::CloseBracket);
    }

    void read(YearsAvailable<T>& values, std::istream& is) const
    {
        values[0] = read_uint<T>(is);
        values[1] = read_uint<T>(is);
    }

    void write(const YearsAvailable<T>& values, std::ostream& os) const
    {
        write_uint<T>(os, values[0]);
        write_uint<T>(os, values[1]);
    }
};


using YearsAvailableDescriptor8  = YearsAvailableDescriptor<uint8_t>;
using YearsAvailableDescriptor16 = YearsAvailableDescriptor<uint16_t>;
