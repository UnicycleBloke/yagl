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
#include <array>


template <typename T, uint16_t N>
struct ArrayDescriptorT : PropertyDescriptor
{
    PropFormat format;

    void print(const std::array<T, N>& values, std::ostream& os, uint16_t indent) const;
    void parse(std::array<T, N>& values, TokenStream& is) const;
};


template <typename T, uint16_t N>
void ArrayDescriptorT<T, N>::print(const std::array<T, N>& values, std::ostream& os, uint16_t indent) const
{
    // This is the name of the property.
    prefix(os, indent);
    os << "[ ";

    for (const auto& value: values)
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
        os << buffer << " ";
    }
    os << "];\n";
}


template <typename T, uint16_t N>
void ArrayDescriptorT<T,N>::parse(std::array<T, N>& values, TokenStream& is) const
{
    is.match(TokenType::OpenBracket);

    for (auto& value: values)
    {
        value = is.match_uint<T>();
    }

    is.match(TokenType::CloseBracket);
}
