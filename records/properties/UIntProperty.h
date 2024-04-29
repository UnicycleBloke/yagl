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
#include "Action00Property.h"
#include "IntegerDescriptor.h"


template <typename T, bool EXT = false>
class UIntProperty : public Action00Property
{
public:
    UIntProperty(Action00Feature& container, uint8_t index, const std::string& label)
    : Action00Property(container, index, label) 
    {
    }

    void read(std::istream& is) override
    {        
        m_value = read_uint<T, EXT>(is);
    }

    void write(std::ostream& os) const override
    {        
        write_uint<T, EXT>(os, m_value);
    }

    void print(std::ostream& os, uint16_t indent) const override
    { 
        os << pad(indent) << label() << ": ";  
        os << to_string(m_value);
    }

    void parse(TokenStream& is) override
    {   
        m_value = is.match_uint<T>();     
    }

private:
    T m_value{};
};


using UInt8Property    = UIntProperty<uint8_t>;
using UInt16Property   = UIntProperty<uint16_t>;
using UInt32Property   = UIntProperty<uint32_t>;
using GRFLabelProperty = UIntProperty<uint32_t>;
