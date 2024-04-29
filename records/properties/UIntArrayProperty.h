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
#include <array>


template <typename T, uint16_t SIZE, bool EXT = false>
class UIntArrayProperty : public Action00Property
{
public:
    UIntArrayProperty(Action00Feature& container, uint8_t index, const std::string& label)
    : Action00Property(container, index, label) 
    {
    }

    void read(std::istream& is) override
    {   
        for (auto& value: m_values)
        {
            value = read_uint<T, EXT>(is);
        }
    }

    void write(std::ostream& os) const override
    {
        for (const auto& value: m_values)
        {
            write_uint<T, EXT>(os, m_value);
        }
    }

    void print(std::ostream& os, uint16_t indent) const override
    { 
        os << pad(indent) << label() << ": ";  
        os << "[";
        for (const auto& value: m_values)
        {
            os << " " << to_string(value);
        }
        os << " ]";
    }

    void parse(TokenStream& is) override
    {   
        is.match(TokenType::OpenBracket);
        for (auto& value: m_values)
        {
            value = is.match_uint<T>();     
        }
        is.match(TokenType::CloseBracket);
    }

private:
    std::array<T, SIZE> m_values{};
};


template <uint16_t SIZE> using UInt8ArrayProperty    = UIntArrayProperty<uint8_t, SIZE>;
template <uint16_t SIZE> using UInt16ArrayProperty   = UIntArrayProperty<uint16_t, SIZE>;
template <uint16_t SIZE> using UInt32ArrayProperty   = UIntArrayProperty<uint32_t, SIZE>;
template <uint16_t SIZE> using GRFLabelArrayProperty = UIntArrayProperty<uint32_t, SIZE>;

