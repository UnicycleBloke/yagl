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
#include "Action00Bridges.h"


namespace {


constexpr const char* str_table = "table";


} // namespace {


void BridgeTable::read(std::istream& is)
{
    for (uint32_t& sprite: m_sprites)
    {
        sprite = read_uint32(is);
    }
}


void BridgeTable::write(std::ostream& os) const
{
    for (const uint32_t& sprite: m_sprites)
    {
        write_uint32(os, sprite);
    }
}


void BridgeTable::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "{\n";

    uint16_t index = 0;
    for (const uint32_t& sprite: m_sprites)
    {
        if ((index % 8) == 0)
        {
            os << pad(indent + 4);
        }

        os << to_hex(sprite) << " ";

        ++index;
        if ((index % 8) == 0)
        {
            os << "\n";
        }
    }

    os << pad(indent) << "}\n";
}


void BridgeTable::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    for (uint32_t& sprite: m_sprites)
    {
        // We expect precisely 32 sprite indices. Anything else will lead to an exception.
        // More than 32 will throw when matching a brace chokes on an integer.
        // Fewer than 32 will throw when matching an integer chokes on a brace.
        sprite = is.match_uint32();
    }
    is.match(TokenType::CloseBrace);
}


void BridgeLayout::read(std::istream& is)
{
    m_first_table_id = read_uint8(is);

    uint8_t num_tables = read_uint8(is);
    m_tables.resize(num_tables);
    for (uint8_t i = 0; i < num_tables; ++i)
    {
        m_tables[i].read(is);
    }
}


void BridgeLayout::write(std::ostream& os) const
{
    write_uint8(os, m_first_table_id);

    write_uint8(os, uint8_t(m_tables.size()));
    for (const auto& table: m_tables)
    {
        table.write(os);
    }
}


void BridgeLayout::print(std::ostream& os, uint16_t indent) const
{
    uint8_t table_id = m_first_table_id;

    os << "\n";
    os << pad(indent) << "{\n";
    for (const auto& table: m_tables)
    {
        os << pad(indent + 4) << str_table << "<" << to_hex(table_id++) << ">\n";
        table.print(os, indent + 4);
    }
    os << pad(indent) << "}";
}


void BridgeLayout::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        is.match_ident(str_table);
        is.match(TokenType::OpenAngle);
        uint8_t table_id = is.match_uint8();
        is.match(TokenType::CloseAngle);

        BridgeTable table = {};
        table.parse(is);

        m_tables.push_back(table);
        if (m_tables.size() == 1)
        {
            m_first_table_id = table_id;
        }
    }
    is.match(TokenType::CloseBrace);
}
