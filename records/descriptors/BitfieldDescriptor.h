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


struct BitfieldDescriptor : PropertyDescriptor
{
    struct Item
    {
        uint8_t     bit;
        const char* name;
    };

    void print_impl(uint32_t bits, std::ostream& os, uint16_t indent) const;
    void parse_impl(uint32_t& bits, TokenStream& is) const;

    std::vector<Item> items;
};


template <typename T>
struct BitfieldDescriptorT : BitfieldDescriptor
{
    void print(T bits, std::ostream& os, uint16_t indent) const
    {
        uint32_t temp = bits;
        print_impl(temp, os, indent);
    }

    void parse(T& bits, TokenStream& is) const
    {
        uint32_t temp;
        parse_impl(temp, is);
        bits = temp;
    }
};
