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
#include <cstdint>
#include <array>


constexpr uint16_t PALETTE_SIZE = 256U * 3U;


// Contains R G B components for 256 palette entries.
using PaletteArray = std::array<uint8_t, PALETTE_SIZE>;


enum class PaletteType
{
    Default,
    DOS,
    Windows,
    DOSToyland,
    WindowsToyland
};


const PaletteArray& get_palette_data(PaletteType type);


