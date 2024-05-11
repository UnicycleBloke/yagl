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
#include "IndustryMultipliers.h"
#include "StreamHelpers.h"


void IndustryMultipliers::read(std::istream& is)
{
    m_num_inputs  = read_uint8(is);
    m_num_outputs = read_uint8(is);
    uint16_t num_items = m_num_inputs * m_num_outputs;
    for (uint16_t i = 0; i < num_items; ++i)
    {
        m_items.push_back(read_uint16(is));
    }
}


void IndustryMultipliers::write(std::ostream& os) const
{
    write_uint8(os, m_num_inputs);
    write_uint8(os, m_num_outputs);
    for (uint16_t item: m_items)
    {
        write_uint16(os, item);
    }
}


// TODO This layout seems a bit unhelpful. Could add some line breaks. 
// This is basically a Vector of Vector.
void IndustryMultipliers::print(std::ostream& os, uint16_t indent) const
{
    uint16_t index = 0;
    os << "[";
    for (uint8_t i = 0; i < m_num_inputs; ++i)
    {
        os << " [";
        for (uint8_t o = 0; o < m_num_outputs; ++o)
        {
            os << " " << to_hex(m_items[index++]);
        }
        os << " ]";
    }
    os << " ]";
}


void IndustryMultipliers::parse(TokenStream& is)
{
    uint16_t inputs  = 0;
    uint16_t outputs = 0;

    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        ++inputs;
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            ++outputs;
            uint16_t value = is.match_uint16();
            m_items.push_back(value);
        }
        is.match(TokenType::CloseBracket);
    }
    is.match(TokenType::CloseBracket);

    m_num_inputs  = static_cast<uint8_t>(inputs);
    m_num_outputs = static_cast<uint8_t>(outputs / inputs);

    // TODO should refactor to check that all subgroups are the same size.
    // TODO dimensions of this matrix should match properties 0x25 and 0x26
    RUNTIME_TEST(inputs <= 0xFF, "Industry multipliers: too many inputs");
    RUNTIME_TEST((outputs % inputs) == 0, "Industry multipliers: invalid counts");
    RUNTIME_TEST((outputs / inputs) <= 0xFF, "Industry multipliers: too many outputs");
}
