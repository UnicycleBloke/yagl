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
#include <array>


// Represents a fixed-length collection of values of type T. 
// The size of the collection precedes the items in the the binary formet.
template <typename T, uint16_t Size>
//requires ValueType<T>
struct Array
{
    void read(std::istream& is) 
    {   
        for (auto& item: m_items)
        {
            item.read(is);
        }
    }

    void write(std::ostream& os) const 
    {
        for (const auto& item: m_items)
        {
            item.write(os);
        }
    }

    void print(std::ostream& os, uint16_t indent = 0) const 
    { 
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
        for (auto& item: m_items)
        {
            item.parse(is);
        }
        is.match(TokenType::CloseBracket);
    }

private:
    std::array<T, Size> m_items;
};