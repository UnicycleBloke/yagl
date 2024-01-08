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
    explicit Year(uint32_t year = 1920)
    : m_year{year}
    {
    }

    void print(std::ostream& os) const
    {
        os << to_string(m_year, UIntFormat::Dec);
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

    uint32_t get() const { return m_year; }

private:
    uint32_t m_year;
};


using Year8  = Year<uint8_t>;
using Year16 = Year<uint16_t>;
using Year32 = Year<uint32_t>;

using Year8Descriptor  = GenericDescriptor<Year8>;
using Year16Descriptor = GenericDescriptor<Year16>;
using Year32Descriptor = GenericDescriptor<Year32>;

using Year8Pair  = Array<Year8,  2>;
using Year16Pair = Array<Year16, 2>;
using Year32Pair = Array<Year32, 2>;

using Year8PairDescriptor  = GenericDescriptor<Year8Pair>;
using Year16PairDescriptor = GenericDescriptor<Year16Pair>;
using Year32PairDescriptor = GenericDescriptor<Year32Pair>;
