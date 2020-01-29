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


struct EnumDescriptor : PropertyDescriptor
{
    struct Item
    {
        uint32_t    value;
        const char* name;
    };

    void print_impl(uint32_t value, std::ostream& os, uint16_t indent) const;
    void print_value_impl(uint32_t value, std::ostream& os) const;
    void parse_impl(uint32_t& value, TokenStream& is) const;
    const char* value_impl(uint32_t value) const;

    std::vector<Item> items;
};


template <typename Enum>
struct EnumDescriptorT : EnumDescriptor
{
    void print(Enum value, std::ostream& os, uint16_t indent) const
    {
        uint32_t temp = static_cast<uint32_t>(value);
        print_impl(temp, os, indent);
    }

    void print_value(Enum value, std::ostream& os) const
    {
        uint32_t temp = static_cast<uint32_t>(value);
        print_value_impl(temp, os);
    }

    const char* value(Enum value) const
    {
        uint32_t temp = static_cast<uint32_t>(value);
        return value_impl(temp);
    }

    void parse(Enum& value, TokenStream& is) const
    {
        uint32_t temp;
        parse_impl(temp, is);
        value = static_cast<Enum>(temp);
    }
};

