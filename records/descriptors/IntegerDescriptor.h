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
#include "DescriptorBase.h"
#include "StreamHelpers.h"


enum class UIntFormat { Dec, Hex, Bool };


template <typename T>
std::string to_string(T value, UIntFormat format)
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


// TODO Can we retire this.
// Used in Actions 0B, 02Random, 02SpriteLayout, 02Variable, 03. 
template <typename T>
struct IntegerDescriptorT : PropertyDescriptor
{
    UIntFormat format;

    void print(T value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        os << to_string(value, format) << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value = is.match_uint<T>();
    }
};


// TODO Can we retire this.
template <typename T>
struct IntegerListDescriptorT : PropertyDescriptor
{
    UIntFormat format;

    void print(const std::vector<T>& values, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        os << "[";
        for (const T value: values)
        {
            os << " " << to_string(value, format);
        }
        os << " ];\n";
    }

    void parse(std::vector<T>& values, TokenStream& is) const
    {
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            T value = is.match_uint<T>();
            values.push_back(value);
        }
        is.match(TokenType::CloseBracket);
    }
};


// Use this in place of naked uint8_t, uint16_t and uint32_t.
// Should help to avoid errors by internalising the size of reads and 
// writes. UIntFormat is a print parameter, which is not true of all 
// types with the read(), write(), parse(), print() interface.
// TODO what about signed types?
template <typename T, bool EXT = false>
class UInt
{
public:
    using Type = T;
    static constexpr bool Ext = EXT;    

public:
    void print(std::ostream& os, UIntFormat format) const
    {
        os << to_string(m_value, format);
    }

    void parse(TokenStream& is)
    {
        m_value = is.match_uint<T>();
    }

    void read(std::istream& is)
    {
        m_value = read_uint<T, EXT>(is);
    }

    void write(std::ostream& os) const
    {        
        write_uint<T, EXT>(os, m_value);
    }

    // Work around for BitFieldDescriptor. For now.
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


// Fixed size array of any type with the standard read(), write(), 
// parse(), print() interface, with UIntFormat as a print parameter.
template <typename T, uint16_t SIZE>
class UIntArray
{
public:    
    explicit UIntArray(std::array<T, SIZE> values = {})
    : m_values{values}
    {
    }

    void print(std::ostream& os, UIntFormat format) const
    {
        os << "[";
        for (const auto& value: m_values)
        {
            os << " ";
            value.print(os, format);
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

    // Only added for testing.
    uint16_t size() const { return SIZE; }
    bool operator==(const UIntArray& other) const
    {
        return m_values == other.m_values;
    }

private:
    std::array<T, SIZE> m_values;
};


// Variable size array of any type with the standard read(), write(), 
// parse(), print() interface, with UIntFormat as a print parameter.
template <typename T>
class UIntVector
{
public:    
    explicit UIntVector(std::vector<T> values = {})
    : m_values{values}
    {
    }

    void print(std::ostream& os, UIntFormat format) const
    {
        os << "[";
        for (const auto& value: m_values)
        {
            os << " ";
            value.print(os, format);
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

    // Only added for testing.
    uint16_t size() const { return static_cast<uint16_t>(m_values.size()); }
    bool operator==(const UIntVector& other) const
    {
        return m_values == other.m_values;
    }

private:
    std::vector<T> m_values;
};


// TODO adding a print_property() member to the value would eliminate the need for this 
template <typename T>
struct UIntDescriptor : PropertyDescriptor
{
    void print(const T& value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        value.print(os, format);
        os << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value.parse(is);
    }

    UIntFormat format;
};


using UInt8    = UInt<uint8_t>;
using UInt8Ext = UInt<uint16_t, true>;
using UInt16   = UInt<uint16_t>;
using UInt32   = UInt<uint32_t>;

using UInt8Descriptor    = UIntDescriptor<UInt8>;
using UInt8ExtDescriptor = UIntDescriptor<UInt8Ext>;
using UInt16Descriptor   = UIntDescriptor<UInt16>;
using UInt32Descriptor   = UIntDescriptor<UInt32>;

// A specific use case
using CargoList           = UIntVector<UInt8>;
using CargoListDescriptor = UIntDescriptor<CargoList>;
