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


template <uint8_t TRUE, uint8_t FALSE>
class BoolT
{
public:
    void print(std::ostream& os) const
    {
        os << std::boolalpha << (m_value == TRUE); 
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_bool();
    }

    void read(std::istream& is)
    {
        uint8_t value = read_uint8(is);
        if ((value != TRUE) && (value != FALSE))
        {

        }
        m_value = (value == TRUE);
    }

    void write(std::ostream& os) const
    {        
        write_uint8(os, m_value ? TRUE : FALSE);
    }
  
private:
    bool m_value{};
};


// Regular boolean values where false is zero and true is 1.
using Bool     = BoolT<1, 0>;
// Special case for helicopters, for reasons.
using BoolHeli = BoolT<0, 2>;

using BoolDescriptor     = GenericDescriptor<Bool>;
using BoolHeliDescriptor = GenericDescriptor<BoolHeli>;




struct BooleanDescriptor : PropertyDescriptor
{
    void print(bool value, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        os << std::boolalpha << value << ";\n"; 
    }

    void parse(bool& value, TokenStream& is) const
    {
        value = is.match_bool();
    }
};


