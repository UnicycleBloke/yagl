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
#include "IntegerDescriptor.h"
#include "StreamHelpers.h"


// Years are stored as uint8_t, uint16_t or uint32_t. This structure helps to 
// regularise this. The uint8_t versions are years since 1920; the others since 
// year 0, to a maximum of 500,000 (I think). 
template <typename T>
class Year
{
    static constexpr uint32_t MIN_SHORT_YEAR = 1920; 
    static constexpr uint32_t MAX_SHORT_YEAR = MIN_SHORT_YEAR + 255; 

public:    
    void print(std::ostream& os) const
    {
        const IntegerDescriptorT<uint32_t> desc{0, "", PropFormat::Dec};
        os << desc.to_string(m_year);
    }

    void parse(TokenStream& is)
    {
        m_year = is.match_uint32();
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            if (m_year < MIN_SHORT_YEAR) throw PARSER_ERROR("Short years must be 1920 or later.", is.peek());
            if (m_year > MAX_SHORT_YEAR) throw PARSER_ERROR("Short years must be 2175 or sooner.", is.peek()); // 1920 + 255
        }
    }

    void read(std::istream& is)
    {
        // Read the binary in one of three formats.
        m_year = read_uint<T, false>(is);
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            m_year += 1920;
        }
    }

    void write(std::ostream& os) const
    {        
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            uint8_t temp = static_cast<uint8_t>(m_year - MIN_SHORT_YEAR);
            write_uint8(os, temp);
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            // 0xFFFF means forever.
            uint16_t temp = static_cast<uint16_t>(std::min<uint16_t>(m_year, 0xFFFF));
            write_uint16(os, temp);
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            write_uint32(os, m_year);
        }
    }

private:
    uint32_t m_year;
};


template <typename T>
class YearPair
{
public:
    void print(std::ostream& os) const
    {
        os << "[ ";
        m_first.print(os);
        os << ", ";
        m_last.print(os);
        os << " ]";
    }

    void parse(TokenStream& is)
    {
        is.match(TokenType::OpenBracket);
        m_first.parse(is);
        is.match(TokenType::Comma);
        m_last.parse(is);
        is.match(TokenType::CloseBracket);
    }

    void read(std::istream& is)
    {
        m_first.read(is);
        m_last.read(is);
    }

    void write(std::ostream& os) const
    {
        m_first.write(os);
        m_last.write(os);
    }

private:
    Year<T> m_first;
    Year<T> m_last;
};


template <typename T>
struct YearDescriptor : PropertyDescriptor
{
    void print(const Year<T>& year, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        year.print(os);
        os << ";\n";
    }

    void parse(Year<T>& year, TokenStream& is) const
    {
        year.parse(is);
    }
};


template <typename T>
struct YearPairDescriptor : PropertyDescriptor
{
    void print(const YearPair<T>& years, std::ostream& os, uint16_t indent) const
    {
        // This is the name of the property.
        prefix(os, indent);
        years.print(os);
        os << ";\n";
    }

    void parse(YearPair<T>& years, TokenStream& is) const
    {
        years.parse(is);
    }
};

