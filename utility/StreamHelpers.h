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
#include "Exceptions.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>


enum class ExtByteFormat { Short, Long };


uint8_t     read_uint8(std::istream& is);
uint16_t    read_uint8_ext(std::istream& is);
uint16_t    read_uint16(std::istream& is);
uint32_t    read_uint32(std::istream& is);


void write_uint8(std::ostream& os, uint8_t value);
void write_uint8_ext(std::ostream& os, uint16_t value, ExtByteFormat format = ExtByteFormat::Long);
void write_uint16(std::ostream& os, uint16_t value);
void write_uint32(std::ostream& os, uint32_t value);


template <typename T>
constexpr bool is_uint_v = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>;


// Useful alias for inside templates.
template <typename T, bool EXT = false>
T read_uint(std::istream& is)
{
    static_assert(is_uint_v<T>);
    static_assert(EXT == false || std::is_same_v<T, uint16_t>);

    if constexpr (std::is_same_v<T, uint8_t>)
    {
        return read_uint8(is);
    }
    else if constexpr (std::is_same_v<T, uint16_t>)
    {
        if constexpr (EXT)
            return read_uint8_ext(is);
        else
            return read_uint16(is);
    }
    else // if constexpr (std::is_same_v<T, uint32_t>)
    {
        return read_uint32(is);
    }
}


// Useful alias for inside templates.
template <typename T, bool EXT = false>
void write_uint(std::ostream& os, T value)
{
    static_assert(is_uint_v<T>);
    static_assert(EXT == false || std::is_same_v<T, uint16_t>);

    if constexpr (std::is_same_v<T, uint8_t>)
    {
        write_uint8(os, value);
    }
    else if constexpr (std::is_same_v<T, uint16_t>)
    {
        if constexpr (EXT)
            write_uint8_ext(os, value);
        else
            write_uint16(os, value);
    }
    else // if constexpr (std::is_same_v<T, uint32_t>)
    {
        return write_uint32(os, value);
    }
}


template <typename T>
std::string to_hex(T value, bool prefix = true)
{
    // Signed values are used here and there in the GRF specs. These should be converted to unsigned types of the 
    // same size so that the sign extension to 32 or 64 bits doesn't appear in the YAGL.
    using U = std::make_unsigned_t<T>;
    U unsigned_value = static_cast<U>(value);

    std::ostringstream os;
    if (prefix)
    {
        os << "0x";
    }
    os << std::uppercase << std::hex << std::setfill('0') << std::setw(2*sizeof(T)) << static_cast<uint64_t>(unsigned_value);
    return os.str();
}


// Simple manipulator to pad the output with leading spaces.
// Used for pretty printing the GRF data.
class Padding
{
public:
    explicit Padding(uint16_t len) : m_len{len} {}  
    void execute(std::ostream& os) const
    {
        std::string spaces(m_len, ' ');
        os << spaces;
    }
private:
    uint16_t m_len;
};


// Manipulator for use with output streams.
inline std::ostream& operator<<(std::ostream& os, const Padding& padding)
{
    padding.execute(os);
    return os;
}


// The actual manipulator: os << pad(4) << ... inserts 4 spaces into the stream.
// Using a function to return an object is how we can given the manipulator arguments.
inline Padding pad(uint16_t len)
{
    return Padding(len);
}


// Used for the unit tests.
std::string hex_dump(const std::string& data, bool split_lines = false);

