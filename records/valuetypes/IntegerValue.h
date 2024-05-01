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
#include <type_traits>
#include <iostream>
#include <cstdint>


enum class UIntFormat2 { Hex, Decimal, Bool };


template <typename T, bool EXT = false>
class UInt2
{
static_assert(std::is_integral_v<T> & std::is_unsigned_v<T>);    
public:
    void read(std::istream& is)
    {
        m_value = read_uint<T, EXT>(is);
    }

    void write(std::ostream& os) const
    {
        write_uint<T, EXT>(os, m_value);
    }

    void print(std::ostream& os, uint16_t indent) const
    {
        os << to_string(m_value, UIntFormat::Hex);
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_uint<T>();
    }

    // Work around for BitFieldDescriptor. For now.
    UInt2(T value = 0) : m_value{value} {}
    operator T() const { return m_value; }

    T get() const     { return m_value; }
    void set(T value) { m_value = value; }

    // Only added for testing.
    bool operator==(const UInt2& other) const
    {
        return m_value == other.m_value;
    }

private:
    T m_value{};
};


// template <typename T, bool EXT = false>
// class IntValue
// {
// static_assert(std::is_integral_v<T> & std::is_signed_v<T>);    
// public:
//     void read(std::istream& is)
//     {
//         m_value = read_int<T, EXT>(is);
//     }

//     void write(std::ostream& os) const
//     {
//         write_int<T, EXT>(os, m_value);
//     }

//     void print(std::ostream& os, UIntFormat format) const
//     {
//         os << to_string(m_value, format);
//     }

//     void parse(TokenStream& is)
//     {
//         m_value = is.match_int<T>();
//     }

//     // Work around for BitFieldDescriptor. For now.
//     UInt(T value = 0) : m_value{value} {}
//     operator T() const { return m_value; }

//     T get() const     { return m_value; }
//     void set(T value) { m_value = value; }

//     // Only added for testing.
//     bool operator==(const UInt& other) const
//     {
//         return m_value == other.m_value;
//     }

// private:
//     T m_value{};
// };
