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
#include "utility/StreamHelpers.h"
#include <cstdint>
#include <iostream>
#include <array>
#include <vector>


struct PropertyDescriptor
{
    uint8_t     index;
    const char* name;

    void prefix(std::ostream& os, uint16_t indent) const;
};


// We optionally forward the indent to the value being printed. This is
// used for example by the SnowLine.
template <typename T, bool INDENT = false>
struct GenericDescriptor : PropertyDescriptor
{
    void print(const T& value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        if constexpr (INDENT)
            value.print(os, indent);
        else
            value.print(os);
        os << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value.parse(is);
    }
};


// Useful for any array of objects with the standard interface:
// print(), parse(), read(), write(). Integers have an additional
// format parameter passed to print(), so there is a different Array
// for these.
template <typename T, uint16_t SIZE>
class ArrayOld
{
public:
    void print(std::ostream& os) const
    {
        os << "[";
        for (const auto& value: m_values)
        {
            os << " ";
            value.print(os);
        }
        os << " ]";
    }

    void parse(TokenStream& is)
    {
        is.match(TokenType::OpenBracket);
        for (auto& value: m_values)
        {
            value.parse(is);
        }
        is.match(TokenType::CloseBracket);
    }

    void read(std::istream& is)
    {
        for (auto& value: m_values)
        {
            value.read(is);
        }
    }

    void write(std::ostream& os) const
    {
        for (const auto& value: m_values)
        {
            value.write(os);
        }
    }

private:
    std::array<T, SIZE> m_values;
};


// Useful for any vector of objects with the standard interface:
// print(), parse(), read(), write(). Integers have an additional
// format parameter passed to print(), so there is a different Array
// for these. We assume that the binary format includes a uint8_t
// length before the items.
template <typename T>
class VectorOld
{
public:
    void print(std::ostream& os) const
    {
        os << "[";
        for (const auto& value: m_values)
        {
            os << " ";
            value.print(os);
        }
        os << " ]";
    }

    void parse(TokenStream& is)
    {
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            T value;
            value.parse(is);
            m_values.push_back(value);
        }

        is.match(TokenType::CloseBracket);
    }

    void read(std::istream& is)
    {
        uint8_t num_items = read_uint8(is);
        for (uint8_t i = 0; i < num_items; ++i)
        {
            T value;
            value.read(is);
            m_values.push_back(value);
        }
    }

    void write(std::ostream& os) const
    {
        write_uint8(os, uint8_t(m_values.size()));
        for (const auto& value: m_values)
        {
            value.write(os);
        }
    }

private:
    std::vector<T> m_values;
};



// TODO Replacement for Vector with a more consistent interface for print()
// This only used for SpriteLayout
template <typename T>
class Vector2Old
{
public:
    void read(std::istream& is)
    {
        uint16_t num_items = read_uint8_ext(is);
        for (uint8_t i = 0; i < num_items; ++i)
        {
            T value;
            value.read(is);
            m_values.push_back(value);
        }
    }

    void write(std::ostream& os) const
    {
        write_uint8_ext(os, uint16_t(m_values.size()));
        for (const auto& value: m_values)
        {
            value.write(os);
        }
    }

    void print(std::ostream& os, uint16_t indent) const
    {
        os << pad(indent) << "[";
        for (const auto& value: m_values)
        {
            os << " ";
            value.print(os, indent + 4);
        }
        os  << pad(indent) << " ]";
    }

    void parse(TokenStream& is)
    {
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            T value;
            value.parse(is);
            m_values.push_back(value);
        }
        is.match(TokenType::CloseBracket);
    }

private:
    std::vector<T> m_values;
};







