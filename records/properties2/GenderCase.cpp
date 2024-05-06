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
#include "GenderCase.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"


void GenderCase::read(std::istream& is)
{
    while (is.peek() != 0x00)
    {
        Item item;
        item.id   = read_uint8(is);
        item.name = read_string(is);
        m_items.push_back(item);
    }
    read_uint8(is);
}


void GenderCase::write(std::ostream& os) const
{
    for (const auto& item: m_items)
    {
        write_uint8(os, item.id);
        write_string(os, item.name);
    }
    write_uint8(os, 0x00);
}


void GenderCase::print(std::ostream& os, uint16_t indent) const
{
    os << "[ ";

    for (const auto& item: m_items)
    {
        os << to_hex(item.id) << ":\"" << item.name << "\" ";
    }

    os << "]";
}


void GenderCase::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        GenderCase::Item item;

        item.id = is.match_uint8();
        is.match(TokenType::Colon);
        item.name = is.match(TokenType::String);

        m_items.push_back(item);
    }

    is.match(TokenType::CloseBracket);
}
