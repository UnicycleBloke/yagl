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


struct AirportTile
{
    enum class Type { OldTile, NewTile, Clearance };

    int8_t   x_off;  // 0x00 is terminator part 1
    int8_t   y_off;  // 0x80 is terminator part 2
    uint16_t tile;
    Type     type;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


struct AirportLayout
{
    enum class Rotation { North = 0, East = 2, South = 4, West = 6 }; 
    
    Rotation rotation;
    std::vector<AirportTile> tiles;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


struct AirportLayouts
{
    std::vector<AirportLayout> layouts;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


using AirportLayoutsDescriptor = GenericDescriptor<AirportLayouts, true>;

