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


enum class StringTerm { None, Null };


uint8_t     read_uint8(std::istream& is);
uint16_t    read_uint8_ext(std::istream& is);
uint16_t    read_uint16(std::istream& is);
uint32_t    read_uint32(std::istream& is);
std::string read_string(std::istream& is);


void write_uint8(std::ostream& os, uint8_t value);
void write_uint8_ext(std::ostream& os, uint16_t value);
void write_uint16(std::ostream& os, uint16_t value);
void write_uint32(std::ostream& os, uint32_t value);
void write_string(std::ostream& os, const std::string& value);
void write_string(std::ostream& os, const std::string& value, StringTerm term);


template <typename T> struct HexFormat {};
template <> struct HexFormat<uint8_t>  { static constexpr uint8_t index = 0; };
template <> struct HexFormat<uint16_t> { static constexpr uint8_t index = 1; };
template <> struct HexFormat<char16_t> { static constexpr uint8_t index = 1; };
template <> struct HexFormat<uint32_t> { static constexpr uint8_t index = 2; };

template <typename T>
std::string to_hex(T value, bool prefix = true)
{
    std::ostringstream os;
    if (prefix)
    {
        os << "0x";
    }
    os << std::uppercase << std::hex << std::setfill('0') << std::setw(2*sizeof(T)) << (uint32_t)value;
    return os.str();
}


inline std::string to_bool(bool value)
{
    return value ? "true" : "false";
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


template <typename Enum>
struct EnumNames
{
    std::string name_of(Enum e) const
    {
        const auto& it = names.find(e);
        if (it != names.end())
        {
            return it->second;
        }

        throw RUNTIME_ERROR("EnumNames - name not found");
    }

    Enum value_of(const std::string& name) const
    {
        for (const auto& it: names)
        {
            if (it.second == name)
            {
                return it.first;
            }
        }

        throw RUNTIME_ERROR("RecordFromName");
    }

    const std::map<Enum, std::string> names;
};



