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
#include "SnowLine.h"
#include "StreamHelpers.h"


void SnowLine::read(std::istream& is)
{
    for (auto& value: m_snow_heights)
    {
        value = read_uint8(is);
    }
}


void SnowLine::write(std::ostream& os) const
{
    for (auto value : m_snow_heights)
    {
        write_uint8(os, value);
    }
}


void SnowLine::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "[\n";

    uint16_t index = 0;
    for (uint8_t month = 0; month < 12; ++month)
    {
        os << pad(indent + 4);
        for (uint8_t day = 0; day < 32; ++day)
        {
            os << to_hex(m_snow_heights[index++]) << " ";
        }
        os << "\n";
    }

    os << pad(indent) << "]";
}


void SnowLine::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);

    // This is a bit crude - assumes that we have the correct
    // number of values. Throws exception if we don't.
    for (auto& value : m_snow_heights)
    {
        value = is.match_uint8();
    }

    is.match(TokenType::CloseBracket);
}
