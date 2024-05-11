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
#include "TokenStream.h"
#include "properties/PropertyMap.h"
#include <cstdint>
#include <iostream>


template <uint8_t TRUE_VALUE, uint8_t FALSE_VALUE>
class BoolNewT
{
public:
    void read(std::istream& is)
    {
        uint8_t value = read_uint8(is);
        if ((value != TRUE_VALUE) && (value != FALSE_VALUE))
        {
            throw RUNTIME_ERROR("Unexpected value when reading boolean");
        }

        m_value = (value == TRUE_VALUE);
    }

    void write(std::ostream& os) const
    {
        uint8_t value = (m_value ? TRUE_VALUE : FALSE_VALUE);
        write_uint8(os, value);
    }

    void print(std::ostream& os, uint16_t indent = 0) const
    {
        os << std::boolalpha << m_value;
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_bool();
    }

private:
    bool m_value{};
};


// Regular boolean values where false is zero and true is 1.
using Bool = BoolNewT<1, 0>;
using BoolProperty = Property<Bool>;

// Special case for helicopters, for reasons.
using BoolHeli = BoolNewT<0, 2>;
using BoolHeliProperty = Property<BoolHeli>;
