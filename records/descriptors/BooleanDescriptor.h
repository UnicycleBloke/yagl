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


struct BooleanDescriptor : PropertyDescriptor
{
    void print(bool value, std::ostream& os, uint8_t indent) const;
    void parse(bool& value, TokenStream& is) const;
};


inline void BooleanDescriptor::print(bool value, std::ostream& os, uint8_t indent) const
{
    prefix(os, indent);
    os << std::boolalpha << value << ";"; 
}


inline void BooleanDescriptor::parse(bool& value, TokenStream& is) const
{
    value = is.match_bool();
}


