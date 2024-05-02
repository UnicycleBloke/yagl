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
#include <cstdint>
#include <iostream>
#include <vector>


struct BitfieldItem
{
    uint8_t     bit;
    const char* name;
};


template <typename T>
class BitfieldValue
{

public:
    BitfieldValue(const std::vector<BitfieldItem>& items)
    : m_items{items}
    {

    }

    void read(std::istream& is)
    {
        m_value = read_uint<T>(is);
    }

    void write(std::ostream& os) const
    {
        write_uint<T>(os, m_value);
    }

    void print(std::ostream& os, uint16_t indent) const
    {
        if (m_value == 0)
        {
            os << "null";
        }
        else
        {
            bool pipe = false;
            for (const auto& item: m_items)
            {
                if (m_value & (1 << item.bit))
                {
                    if (pipe) os << " | ";
                    os << item.name;
                    pipe = true;
                }
            }
        }
    }

    void parse(TokenStream& is)
    {
        m_value = 0;
        while (true)
        {
            std::string name = is.match(TokenType::Ident);
            for (const auto& item: m_items)
            {
                if (name == item.name)
                {
                    m_value |= (1U << item.bit);
                    break;
                }
            }

            const TokenValue& token = is.peek();
            if (token.type != TokenType::Pipe)
            {
                break;
            }

            is.match(TokenType::Pipe);
        }
    }

private:
    T m_value{};
    const std::vector<BitfieldItem>& m_items;
};

