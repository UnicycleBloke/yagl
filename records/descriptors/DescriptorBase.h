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
#include "Lexer.h"
#include "GRFStrings.h"
#include <cstdint>
#include <iostream>


enum class PropType   { Integer, Enumeration, BitField, Date };
enum class PropSize   { Byte, Word, DWord };
enum class PropFormat { Dec, Hex, Bool, String };


struct PropertyDescriptor
{
    uint8_t     index;
    const char* name;

    void prefix(std::ostream& os, uint16_t indent) const;
};


template <typename T>
struct GenericDescriptor : PropertyDescriptor
{
    void print(const T& value, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        value.print(os);
        os << ";\n";
    }

    void parse(T& value, TokenStream& is) const
    {
        value.parse(is);
    }
};


using GRFStringDescriptor = GenericDescriptor<GRFString>;


