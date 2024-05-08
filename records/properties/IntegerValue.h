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
//#include "IntegerDescriptor.h"
#include "properties/Vector.h"
#include "properties/Array.h"
#include "properties/PropertyMap.h"
#include <type_traits>
#include <iostream>
#include <cstdint>


enum class UIntFormat { Dec, Hex, Bool };


template <typename T>
std::string to_string(T value, UIntFormat format = UIntFormat::Hex)
{
    char buffer[24];
    switch (format)
    {
        case UIntFormat::Hex:
            if constexpr (sizeof(T) == 1)
                std::snprintf(buffer, 16, "0x%02X", value & 0xFF);
            else if constexpr (sizeof(T) == 2)
                std::snprintf(buffer, 16, "0x%04X", value & 0xFFFF);
            else if constexpr (sizeof(T) == 4)
                std::snprintf(buffer, 16, "0x%08X", value & 0xFFFF'FFFF);
            else if constexpr (sizeof(T) == 8)
                std::snprintf(buffer, 16, "0x%016X", value);
            break;

        case UIntFormat::Dec:
            std::snprintf(buffer, 16, "%u", value);
            break;

        case UIntFormat::Bool:
            std::snprintf(buffer, 16, "%s", value != 0x00 ? "true" : "false");
            break;

        default:
            std::snprintf(buffer, 16, "<error>");
    }

    return std::string{buffer};
}


template <typename T, bool EXT = false, UIntFormat FORMAT = UIntFormat::Hex>
class UInt
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

    void print(std::ostream& os, uint16_t indent = 0) const
    {
        os << to_string(m_value, FORMAT);
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_uint<T>();
    }

    UInt(T value = 0) : m_value{value} {}
    operator T() const { return m_value; }

    T get() const     { return m_value; }
    void set(T value) { m_value = value; }

    // Only added for testing.
    bool operator==(const UInt& other) const
    {
        return m_value == other.m_value;
    }

private:
    T m_value{};
};


using UInt8  = UInt<uint8_t>;
using UInt8X = UInt<uint16_t, true>;
using UInt16 = UInt<uint16_t>;
using UInt32 = UInt<uint32_t>;

using UInt8Property  = Property<UInt8>;
using UInt8XProperty = Property<UInt8X>;
using UInt16Property = Property<UInt16>;
using UInt32Property = Property<UInt32>;

using UInt8ListProperty  = Property<Vector<UInt8>>;
using UInt8XListProperty = Property<Vector<UInt8X>>;
using UInt16ListProperty = Property<Vector<UInt16>>;
using UInt32ListProperty = Property<Vector<UInt32>>;

template <uint16_t Size> using UInt8ArrayProperty  = Property<Array<UInt8,  Size>>;
template <uint16_t Size> using UInt8XArrayProperty = Property<Array<UInt8X, Size>>;
template <uint16_t Size> using UInt16ArrayProperty = Property<Array<UInt16, Size>>;
template <uint16_t Size> using UInt32ArrayProperty = Property<Array<UInt32, Size>>;
