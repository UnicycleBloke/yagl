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


class IndustryTile
{
public:
    enum class Type { OldTile, NewTile, Clearance };

public:
    bool read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    int8_t   m_x_off;  // 0x00 is terminator part 1
    int8_t   m_y_off;  // 0x80 is terminator part 2
    Type     m_type;
    uint16_t m_tile;
};


struct IndustryLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    // The layout is a reference is the first byte is 0xFE.
    bool m_is_reference;

    // These are used if this layout is a reference to another
    // industry layout, in which case there are zero tiles.
    uint8_t m_industry_num;
    uint8_t m_layout_num;

    // List of tiles when this is not a reference to another layout.
    std::vector<IndustryTile> m_tiles;
};


struct IndustryLayouts
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    std::vector<IndustryLayout> m_layouts;
};

