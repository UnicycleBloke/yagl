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
#include "CargoAcceptanceDescriptor.h"
#include "StreamHelpers.h"


void CargoAcceptanceList::read(std::istream& is)
{
    uint8_t num_items = read_uint8(is);
    for (uint8_t i = 0; i < num_items; ++i)
    {
        CargoAcceptance item;
        item.cargo_type = read_uint8(is);
        item.acceptance = read_uint8(is);
        m_items.push_back(item);
    }
}


void CargoAcceptanceList::write(std::ostream& os) const
{
    write_uint8(os, m_items.size());
    for (const auto& item: m_items)
    {
        write_uint8(os, item.cargo_type);
        write_uint8(os, item.acceptance);
    }
}


void CargoAcceptanceList::print(std::ostream& os, uint16_t indent) const
{
    os << "[";
    for (const auto& item: m_items)
    {
        os << " {" << to_hex(item.cargo_type, true) << ": " << to_hex(item.acceptance, true) << "}";
    }
    os << " ];";
}


void CargoAcceptanceList::parse(TokenStream& is)
{

}


