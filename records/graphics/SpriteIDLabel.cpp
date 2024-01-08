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
#include "SpriteIDLabel.h"


// _0123456789ABCDEF
const Character m_chars[17] =
{
    // Default
    {
        4, 5,
        0xF0, // 0000----
        0x90, // 0--0----
        0x90, // 0--0----
        0x90, // 0--0----
        0xF0, // 0000----
    },
    // Digit 0
    {
        4, 5,
        0x60, // -00-----
        0x90, // 0--0----
        0x90, // 0--0----
        0x90, // 0--0----
        0x60, // -00-----
    },
    // Digit 1
    {
        4, 5,
        0x20, // --0-----
        0x60, // -00-----
        0x20, // --0-----
        0x20, // --0-----
        0x70, // -000----
    },
    // Digit 2
    {
        4, 5,
        0xE0, // 000-----
        0x10, // ---0----
        0x60, // -00-----
        0x80, // 0-------
        0xF0, // 0000----
    },
    // Digit 3
    {
        4, 5,
        0xE0, // 000-----
        0x10, // ---0----
        0x60, // -00-----
        0x10, // ---0----
        0xE0, // 000-----
    },
    // Digit 4
    {
        4, 5,
        0x20, // --0-----
        0x60, // -00-----
        0xA0, // 0-0-----
        0xF0, // 0000----
        0x20, // --0-----
    },
    // Digit 5
    {
        4, 5,
        0xF0, // 0000----
        0x80, // 0-------
        0xE0, // 000-----
        0x10, // ---0----
        0xE0, // 000-----
    },
    // Digit 6
    {
        4, 5,
        0x60, // -00-----
        0x80, // 0-------
        0xE0, // 000-----
        0x90, // 0--0----
        0x60, // -00-----
    },
    // Digit 7
    {
        4, 5,
        0xF0, // 0000----
        0x10, // ---0----
        0x20, // --0-----
        0x40, // -0------
        0x40, // -0------
    },
    // Digit 8
    {
        4, 5,
        0x60, // -00-----
        0x90, // 0--0----
        0x60, // -00-----
        0x90, // 0--0----
        0x60, // -00-----
    },
    // Digit 9
    {
        4, 5,
        0x60, // -00-----
        0x90, // 0--0----
        0x70, // -000----
        0x10, // ---0----
        0x60, // -00-----
    },
    // Letter A
    {
        4, 5,
        0x60, // -00-----
        0x90, // 0--0----
        0xF0, // 0000----
        0x90, // 0--0----
        0x90, // 0--0----
    },
    // Letter B
    {
        4, 5,
        0xE0, // 000-----
        0x90, // 0--0----
        0xE0, // 000-----
        0x90, // 0--0----
        0xE0, // 000-----
    },
    // Letter C
    {
        4, 5,
        0x70, // -000----
        0x80, // 0-------
        0x80, // 0-------
        0x80, // 0-------
        0x70, // -000----
    },
    // Letter D
    {
        4, 5,
        0xE0, // 000-----
        0x90, // 0--0----
        0x90, // 0--0----
        0x90, // 0--0----
        0xE0, // 000-----
    },
    // Letter E
    {
        4, 5,
        0xF0, // 0000----
        0x80, // 0-------
        0xE0, // 000-----
        0x80, // 0-------
        0xF0, // 0000----
    },
    // Letter F
    {
        4, 5,
        0xF0, // 0000----
        0x80, // 0-------
        0xE0, // 000-----
        0x80, // 0-------
        0x80, // 0-------
    }
};


const Character& get_char(uint16_t index)
{
    return m_chars[index];
}
