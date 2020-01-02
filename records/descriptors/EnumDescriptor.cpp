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
#include "EnumDescriptor.h"


void EnumDescriptor::print_impl(uint32_t value, std::ostream& os, uint8_t indent) const
{
    prefix(os, indent);

    for (const auto& item: items)
    { 
        if (value == item.value)
        {
            os << item.name << ";\n";
            return;
        }
    }
    
    throw RUNTIME_ERROR("EnumDescriptor::print");
}


void EnumDescriptor::print_value_impl(uint32_t value, std::ostream& os) const
{
    for (const auto& item: items)
    { 
        if (value == item.value)
        {
            os << item.name;
            return;
        }
    }
    
    throw RUNTIME_ERROR("EnumDescriptor::print_value");
}


const char* EnumDescriptor::value_impl(uint32_t value) const
{
    for (const auto& item: items)
    { 
        if (value == item.value)
        {
            return item.name;
        }
    }

    throw RUNTIME_ERROR("EnumDescriptor::value");
}


void EnumDescriptor::parse_impl(uint32_t& value, TokenStream& is) const
{
    std::string name = is.match(TokenType::Ident);

    for (const auto& item: items)
    { 
        if (name == item.name)
        {
            value = item.value;
            return;
        }
    }

    throw RUNTIME_ERROR("EnumDescriptor::parse");
}




