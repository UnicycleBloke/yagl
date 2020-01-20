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

    void prefix(std::ostream& os, uint8_t indent) const;
};


// struct StringDescriptor : PropertyDescriptor
// {
//     void print(const std::string& value, std::ostream& os, uint8_t indent) const
//     {
//         prefix(os, indent);
//         // Do we always want readable strings here?
//         os << "\"" << grf_string_to_readable_utf8(value) << "\";\n"; 
//     }

//     void parse(std::string& value, TokenStream& is) const
//     {
//         // TODO convert string *from* readable format.
//         value = is.match(TokenType::String);

//     }
// };


struct GRFStringDescriptor : PropertyDescriptor
{
    void print(const GRFString& value, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        value.print(os);
    }

    void parse(GRFString& value, TokenStream& is) const
    {
        value.parse(is);
    }
};

