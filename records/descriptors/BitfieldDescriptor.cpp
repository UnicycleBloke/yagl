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
#include "BitfieldDescriptor.h"


void BitfieldDescriptor::print_impl(uint32_t bits, std::ostream& os, uint8_t indent) const
{
    prefix(os, indent);

    if (bits == 0)
    {
        os << "<no_bits_set>";
    }
    else
    {
        bool pipe = false;
        for (const auto& item: items)
        { 
            if (bits & (1 << item.bit))
            {
                if (pipe) os << " | ";
                os << item.name;
                pipe = true;
            }     
        }
    }
    os << ";\n";
}


void BitfieldDescriptor::parse_impl(uint32_t& bits, TokenStream& is) const
{
    bits = 0;
    do
    {
        std::string name = is.match(TokenType::Ident);
        for (const auto& item: items)
        { 
            if (name == item.name)
            {
                bits |= item.bit;
                break;
            }
        
            throw RUNTIME_ERROR("BitfieldDescriptor::parse");
        }
    }
    while (is.peek().type == TokenType::Pipe);
}


