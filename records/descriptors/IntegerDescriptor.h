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
#include "properties/IntegerValue.h"


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


// TODO adding a print_property() member to the value would eliminate the need for this
template <typename T>
struct UIntDescriptor : PropertyDescriptor
{
    void print(const T& value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        value.print(os, indent);
        os << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value.parse(is);
    }

    //UIntFormat format = UIntFormat::Hex;
};


using UInt8Descriptor  = UIntDescriptor<UInt8>;
using UInt8XDescriptor = UIntDescriptor<UInt8X>;
using UInt16Descriptor = UIntDescriptor<UInt16>;
using UInt32Descriptor = UIntDescriptor<UInt32>;

