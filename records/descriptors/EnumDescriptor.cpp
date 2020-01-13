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
#include <sstream>


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
    
    std::ostringstream ss;
    ss << "EnumDescriptor::print " << value;
    throw RUNTIME_ERROR(ss.str());
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
    
    std::ostringstream ss;
    ss << "EnumDescriptor::print_value " << value;
    throw RUNTIME_ERROR(ss.str());
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

    std::ostringstream ss;
    ss << "EnumDescriptor::value " << value;
    throw RUNTIME_ERROR(ss.str());
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

    std::ostringstream ss;
    ss << "EnumDescriptor::parse " << value;
    throw RUNTIME_ERROR(ss.str());
}




