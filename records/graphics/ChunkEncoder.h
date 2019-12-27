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
#include "Record.h"
#include <vector>
#include <cstdint>


bool has_transparency(uint8_t compression, const GRFInfo& info);

std::vector<uint8_t> encode_tile(const std::vector<uint8_t>& pixels, uint16_t xdim, uint16_t ydim, 
    uint8_t compression, GRFFormat format);

std::vector<uint8_t> decode_tile(const std::vector<uint8_t>& chunks, uint16_t xdim, uint16_t ydim, 
    uint8_t compression, GRFFormat format);

void encode_tile_test();
