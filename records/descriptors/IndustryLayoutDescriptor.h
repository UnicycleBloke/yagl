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


struct IndustryTile
{
    enum class Type { OldTile, NewTile, Clearance };

    int8_t   x_off;  // 0x00 is terminator part 1
    int8_t   y_off;  // 0x80 is terminator part 2
    Type     type;
    uint16_t tile;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


struct IndustryLayout
{
    // The layout is a reference is the first byte is 0xFE.
    bool is_reference;
    
    // These are used if this layout is a reference to another 
    // industry layout, in which case there are zero tiles.
    uint8_t industry_num;
    uint8_t layout_num;

    // List of tiles when this is not a reference to another layout.
    std::vector<IndustryTile> tiles;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


struct IndustryLayouts
{
    std::vector<IndustryLayout> layouts;

    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);
};


struct IndustryLayoutsDescriptor : PropertyDescriptor
{
    void print(const IndustryLayouts& layouts, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(IndustryLayouts& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


