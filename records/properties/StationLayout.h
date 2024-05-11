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


class StationTileData
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    bool     m_new_bb; ///< Indicates that this sprite has its own bounding box
    int8_t   m_x_off;
    int8_t   m_y_off;
    uint8_t  m_z_off;
    uint8_t  m_x_ext;
    uint8_t  m_y_ext;
    uint8_t  m_z_ext;
    uint32_t m_sprite;
};


class StationTile
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    uint32_t m_ground_sprite;
    std::vector<StationTileData> m_data;
};


class StationLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    std::vector<StationTile> m_tiles;
};
