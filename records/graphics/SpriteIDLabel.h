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
#include "png.hpp"
#include "StreamHelpers.h"


struct Character
{
    static constexpr uint8_t LENGTH = 5;

    uint8_t width;
    uint8_t height;
    uint8_t data[LENGTH];
};


const Character& get_char(uint16_t index);


// png++ uses template parameters to represent colour depth. 
// Not super convenient, but we do the same thing.
template <typename PixType>
class SpriteIDLabel
{
public:
    void draw(uint32_t id, uint32_t& xoff, uint32_t yoff,
        png::image<PixType>& image);

private:
    void draw_character(char c, uint32_t& xoff, uint32_t yoff, 
        png::image<PixType>& image);
};


template <typename PixType>
void SpriteIDLabel<PixType>::draw(uint32_t id, uint32_t& xoff, uint32_t yoff,
    png::image<PixType>& image)
{
    std::string value = to_hex(id, false);

    uint16_t length = uint16_t(value.length());
    uint16_t index  = 0;

    // Skip leading zeroes.
    while (value[index] == '0')
    {
        ++index;
    }

    // Handle the case where all the characters are zero.
    // Display one.
    if (index >= length)
    {
        --index;
    }
  
    // Display the remaining characters.
    while (index < length)
    {
        draw_character(value[index], xoff, yoff, image);
        ++index;
    }
}


template <typename PixType>
void SpriteIDLabel<PixType>::draw_character(char c, uint32_t& xoff, uint32_t yoff,
    png::image<PixType>& image)
{
    uint8_t index = 0;
    switch (c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': index = c - '0' + 1; break;
        case 'A': 
        case 'B': 
        case 'C': 
        case 'D': 
        case 'E': 
        case 'F': index = c - 'A' + 11; break;
    }

    const Character& character = get_char(index);

    for (uint16_t y = 0; y < character.height; ++y)
    {
        uint8_t char_row = character.data[y];
        for (uint16_t x = 0; x < character.width; ++x)
        {
            if ((char_row & 0x80) != 0x00)
            {
                if constexpr (std::is_same_v<PixType, png::index_pixel>)
                {
                    // Set to transparent blue colour.
                    image[yoff + y][xoff + x] = PixType{0x00};
                }
                if constexpr (std::is_same_v<PixType, png::rgb_pixel>)
                {
                    // Set to blue colour.
                    image[yoff + y][xoff + x] = PixType{0x00, 0x00, 0xFF};
                }
                if constexpr (std::is_same_v<PixType, png::rgba_pixel>)
                {
                    // Set to blue colour.
                    image[yoff + y][xoff + x] = PixType{0x00, 0x00, 0xFF, 0xFF};
                }
            }
            char_row <<= 1;
        }
    }
    xoff += (character.width + 1);
}


