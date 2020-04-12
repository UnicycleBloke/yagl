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


enum class PropFormat { Dec, Hex, Bool, String };


template <typename T>
struct IntegerDescriptorT : PropertyDescriptor
{
    PropFormat format;

    void print(T value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        os << to_string(value) << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value = is.match_uint<T>();
    }

    std::string to_string(T value) const;
};


template <typename T>
struct IntegerListDescriptorT : PropertyDescriptor
{
    PropFormat format;

    void print(const std::vector<T>& values, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        os << to_string(values) << ";\n";
    }

    void parse(std::vector<T>& values, TokenStream& is) const;
    std::string to_string(const std::vector<T>& values) const;
};


template <typename T>
std::string IntegerDescriptorT<T>::to_string(T value) const
{
    char buffer[16];
    switch (format)
    {
        case PropFormat::Hex:
            switch (sizeof(T))
            {
                case 1: std::snprintf(buffer, 16, "0x%02X", value & 0xFF); break;
                case 2: std::snprintf(buffer, 16, "0x%04X", value & 0xFFFF); break;
                case 4: std::snprintf(buffer, 16, "0x%08X", value); break;
            }
            break;

        case PropFormat::Dec:
            std::snprintf(buffer, 16, "%u", value);
            break;

        case PropFormat::Bool:
            std::snprintf(buffer, 16, "%s", value != 0x00 ? "true" : "false");
            break;
  
        default:
            std::snprintf(buffer, 16, "<error>");
    }

    return std::string{buffer};
}


template <typename T>
std::string IntegerListDescriptorT<T>::to_string(const std::vector<T>& values) const
{
    const IntegerDescriptorT<T> desc{index, name, format};
    std::ostringstream os;
    os << "[";
    for (const T value: values)
    {
        os << " " << desc.to_string(value);
    }
    os << " ]";
    return os.str();
}


template <typename T>
void IntegerListDescriptorT<T>::parse(std::vector<T>& values, TokenStream& is) const
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        T value = is.match_uint<T>();
        values.push_back(value);
    }
    is.match(TokenType::CloseBracket);
}













template <typename T>
std::string to_string(T value, PropFormat format)
{
    char buffer[16];
    switch (format)
    {
        case PropFormat::Hex:
            switch (sizeof(T))
            {
                case 1: std::snprintf(buffer, 16, "0x%02X", value & 0xFF); break;
                case 2: std::snprintf(buffer, 16, "0x%04X", value & 0xFFFF); break;
                case 4: std::snprintf(buffer, 16, "0x%08X", value); break;
            }
            break;

        case PropFormat::Dec:
            std::snprintf(buffer, 16, "%u", value);
            break;

        case PropFormat::Bool:
            std::snprintf(buffer, 16, "%s", value != 0x00 ? "true" : "false");
            break;
  
        default:
            std::snprintf(buffer, 16, "<error>");
    }

    return std::string{buffer};
}


// Use this in place of naked uint8_t, uint16_t and uint32_t.
// Should help to avoid errors by internalising the size of reads and 
// writes.
// TODO what about signed types?
template <typename T, bool EXT = false>
class UInt
{
public:
    using Type = T;
    static constexpr bool Ext = EXT;    

public:
    void print(std::ostream& os, PropFormat format) const
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
  
private:
    T m_value{};
};


template <typename T, uint16_t SIZE>
class UIntArray
{
public:    
    void print(std::ostream& os, PropFormat format) const
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

private:
    std::array<T, SIZE> m_values;
};


template <typename T>
class UIntVector
{
public:    
    void print(std::ostream& os, PropFormat format) const
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

private:
    std::vector<T> m_values;
};


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

    PropFormat format;
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
