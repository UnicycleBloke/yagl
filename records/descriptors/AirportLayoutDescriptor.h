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


// Industry layouts and Airport layouts almost identical - the only difference 
// appears to be the rotation value that is included with Airports. Rather than
// templatise this lot, we pass a parameter to indicate whether we are dealing 
// with an Airport or an Industry.
enum class AirportType { Airport, Industry };


struct AirportTile
{
    enum class Type { OldTile, NewTile, Clearance };

    int8_t   x_off;  // 0x00 is terminator part 1
    int8_t   y_off;  // 0x80 is terminator part 2
    Type     type;
    uint16_t tile;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
};


struct AirportLayout
{
    enum class Rotation { North = 0, East = 2, South = 4, West = 6 }; 
    
    Rotation rotation;
    std::vector<AirportTile> tiles;

    void read(std::istream& is, AirportType type);
    void write(std::ostream& os, AirportType type) const;
    void print(std::ostream& os, uint16_t indent, AirportType type) const;
    const char* rotation_name(Rotation rotation) const;
};


struct AirportLayouts
{
    std::vector<AirportLayout> layouts;

    void read(std::istream& is, AirportType type);
    void write(std::ostream& os, AirportType type) const;
    void print(std::ostream& os, uint16_t indent, AirportType type) const;
    void parse(TokenStream& is, AirportType type);
};


struct AirportLayoutsDescriptor : PropertyDescriptor
{
    void print(const AirportLayouts& layouts, std::ostream& os, uint8_t indent, AirportType type) const
    {
        prefix(os, indent);
        layouts.print(os, indent, type);
    }

    void parse(AirportLayouts& layouts, TokenStream& is, AirportType type) const
    {
        layouts.parse(is, type);
    }
};


