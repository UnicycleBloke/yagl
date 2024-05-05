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
#include "TokenStream.h"
#include <vector>
#include <iostream>
#include <cstdint>


class GenderCase
{
public:
    // Binary serialisation
    void read(std::istream& is);
    void write(std::ostream& os) const;
    // Text serialisation
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    // The ID used for these translation tables is the Action 4 (GRF version 7 or higher)
    // language-id, i.e. this mapping only works with GRF version 7 or higher. Language-id 7F
    // (any) is not allowed. You can can define an ID multiple times in which case the new
    // mappings are simply appended to the already known mappings.
    struct Item
    {
        uint8_t     id;
        std::string name;
    };

    std::vector<Item> m_items;
};
