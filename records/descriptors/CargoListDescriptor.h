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
#include <vector>


class CargoList
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os) const;
    void parse(TokenStream& is);

private:
    // This is a list of indices into the (presumably global) cargo table.
    std::vector<uint8_t> m_cargo_ids;
};


inline void CargoList::read(std::istream& is)
{
    uint8_t num_cargos = read_uint8(is);
    for (uint8_t i = 0; i < num_cargos; ++i)
    {
        m_cargo_ids.push_back(read_uint8(is));
    }
}


inline void CargoList::write(std::ostream& os) const
{
    write_uint8(os, m_cargo_ids.size());
    for (auto cargo_id: m_cargo_ids)
    {
        write_uint8(os, cargo_id);
    }
}


inline void CargoList::print(std::ostream& os) const
{
    os << "[";
    for (auto cargo_id: m_cargo_ids)
    {
        os << ' ' << to_hex(cargo_id); 
    }
    os << " ];";
}


inline void CargoList::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        uint8_t value = is.match_integer();
        m_cargo_ids.push_back(value);
    }
    is.match(TokenType::CloseBracket);
}


struct CargoListDescriptor : PropertyDescriptor
{
    void print(const CargoList& cargos, std::ostream& os, uint8_t indent) const;
    void parse(CargoList& cargos, TokenStream& is) const;
};


inline void CargoListDescriptor::print(const CargoList& cargos, std::ostream& os, uint8_t indent) const
{
    prefix(os, indent);
    cargos.print(os);
}


inline void CargoListDescriptor::parse(CargoList& cargos, TokenStream& is) const
{
    cargos.parse(is);
}

