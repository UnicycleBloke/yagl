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
#include "ValueType.h"
#include "TokenStream.h"
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <vector>


// Represents a variable-length collection of values of type T. 
// The size of the collection precedes the items in the the binary formet.
// The size encoding is assumed to be extended byte, but could be make into
// a template argument for more flexibility.
template <typename T>
//requires ValueType<T>
struct Vector
{
    void read(std::istream& is) 
    {   
        uint16_t num_items = read_uint8_ext(is);
        for (uint8_t i = 0; i < num_items; ++i)
        {
            T item{};
            item.read(is);
            m_items.push_back(item);
        }
    }

    void write(std::ostream& os) const 
    {
        write_uint8_ext(os, uint16_t(m_items.size()));
        for (const auto& item: m_items)
        {
            item.write(os);
        }
    }

    void print(std::ostream& os, uint16_t indent) const 
    { 
        // TODO options here to control the layout with or without line breaks,
        // with or without commas, etc.
        os << "[";
        for (const auto& item: m_items)
        {
            os << " ";
            item.print(os, indent);
        }
        os << " ]";
    }

    void parse(TokenStream& is) 
    {   
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            T item;
            item.parse(is);
            m_items.push_back(item);
        }

        is.match(TokenType::CloseBracket);
    }

private:
    
    std::vector<T> m_items{};
};