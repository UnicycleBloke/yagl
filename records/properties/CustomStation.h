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
#include <cstdint>


class CustomLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

    bool terminator() const { return (m_platform_count == 0) && (m_platform_length == 0); }

public:
    enum class Platform
    {
        Plain     = 0x00,
        Building  = 0x02,
        RoofLeft  = 0x04,
        RoofRight = 0x06
    };

    enum class Direction
    {
        NE_SW = 0x00,
        NW_SE = 0x01
    };

private:
    uint8_t   m_platform_length;
    uint8_t   m_platform_count;
    Direction m_direction = Direction::NE_SW;
    // length * count, All bytes 00, 02, 04 or 06 - see Platform enum
    std::vector<Platform> m_platform_tiles;
};


class CustomStation
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    std::vector<CustomLayout> m_layouts;
};
