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
#include <vector>


template <typename T, bool EXT = false>
class UIntListProperty : public Action00Property
{
public:
    UIntListProperty(Action00Feature& container, uint8_t index, const std::string& label)
    : Action00Property(container, index, label) 
    {
    }

    void read(std::istream& is) override
    {   
        uint8_t num_items = read_uint8(is);
        for (uint8_t i = 0; i < num_items; ++i)
        {
            auto value = read_uint<T, EXT>(is);
            m_values.push_back(value);
        }
    }

    void write(std::ostream& os) const override
    {
        write_uint8(os, uint8_t(m_values.size()));
        for (const auto& value: m_values)
        {
            write_uint<T, EXT>(os, value);
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
        while (is.peek().type != TokenType::CloseBracket)
        {
            auto value = is.match_uint<T>();     
            m_values.push_back(value);
        }

        is.match(TokenType::CloseBracket);
    }

private:
    std::vector<T> m_values{};
};


using UInt8ListProperty    = UIntListProperty<uint8_t>;
using UInt16ListProperty   = UIntListProperty<uint16_t>;
using UInt32ListProperty   = UIntListProperty<uint32_t>;
using GRFLabelListProperty = UIntListProperty<uint32_t>;
