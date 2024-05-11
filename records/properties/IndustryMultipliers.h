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
#include <iostream>
#include <vector>
#include <cstdint>


// From the NewGRF specs:
// This property defines a matrix which described how input cargos from property 26 are processed 
// into output cargos from property 25.
//    03 04    // 3 input cargos, 4 output cargos
//    \wx0100 \wx0000 \wx0000 \wx0100    // input cargo 0 is converted to output cargos 0 and 3
//    \wx0000 \wx0100 \wx0000 \wx0100    // input cargo 1 is converted to output cargos 1 and 3
//    \wx0000 \wx0000 \wx0100 \wx0100    // input cargo 2 is converted to output cargos 2 and 3
class IndustryMultipliers
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    uint8_t               m_num_inputs{};
    uint8_t               m_num_outputs{};
    std::vector<uint16_t> m_items;
};
