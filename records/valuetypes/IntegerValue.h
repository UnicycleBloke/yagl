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
#include "IntegerDescriptor.h"
#include "Vector.h"
#include "Array.h"
#include "PropertyMap.h"
#include <type_traits>
#include <iostream>
#include <cstdint>


template <typename T, bool EXT = false, UIntFormat FORMAT = UIntFormat::Hex>
class UIntNew
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
        os << to_string(m_value, FORMAT);
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_uint<T>();
    }

    // Work around for BitFieldDescriptor. For now.
    UIntNew(T value = 0) : m_value{value} {}
    operator T() const { return m_value; }

    T get() const     { return m_value; }
    void set(T value) { m_value = value; }

    // Only added for testing.
    bool operator==(const UIntNew& other) const
    {
        return m_value == other.m_value;
    }

private:
    T m_value{};
};


using UInt8New  = UIntNew<uint8_t>;
using UInt8XNew = UIntNew<uint16_t, true>;
using UInt16New = UIntNew<uint16_t>;
using UInt32New = UIntNew<uint32_t>;

using UInt8Property  = Property<UInt8New>;
using UInt8XProperty = Property<UInt8XNew>;
using UInt16Property = Property<UInt16New>;
using UInt32Property = Property<UInt32New>;

using UInt8ListProperty  = Property<Vector<UInt8New>>;
using UInt8XListProperty = Property<Vector<UInt8XNew>>;
using UInt16ListProperty = Property<Vector<UInt16New>>;
using UInt32ListProperty = Property<Vector<UInt32New>>;

template <uint16_t Size> using UInt8ArrayProperty  = Property<Array<UInt8New,  Size>>;
template <uint16_t Size> using UInt8XArrayProperty = Property<Array<UInt8XNew, Size>>;
template <uint16_t Size> using UInt16ArrayProperty = Property<Array<UInt16New, Size>>;
template <uint16_t Size> using UInt32ArrayProperty = Property<Array<UInt32New, Size>>;
