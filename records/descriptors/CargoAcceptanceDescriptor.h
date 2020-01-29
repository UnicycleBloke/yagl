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


struct CargoAcceptance
{
    uint8_t cargo_type; // Index into cargo translation table
    uint8_t acceptance; // In 1/8 units.
};


class CargoAcceptanceList
{
public:
    // Binary serialisation
    void read(std::istream& is);
    void write(std::ostream& os) const;
    // Text serialisation
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    std::vector<CargoAcceptance> m_items;
};


struct CargoAcceptanceDescriptor : PropertyDescriptor
{
    void print(const CargoAcceptanceList& list, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        list.print(os, indent);
    }

    void parse(CargoAcceptanceList& list, TokenStream& is) const
    {
        list.parse(is);
    }
};



