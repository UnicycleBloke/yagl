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
        value = is.match_integer_t<T>();
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
        T value = is.match_integer_t<T>();
        values.push_back(value);
    }
    is.match(TokenType::CloseBracket);
}
