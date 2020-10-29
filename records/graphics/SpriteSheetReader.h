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
#include <map>
#include <memory>
#include "png.hpp"
#include "RealSpriteRecord.h"


class SpriteSheet
{
public:
    enum class Colour { Palette, RGB, RGBA }; 
    using Pixel = RealSpriteRecord::Pixel;       

    // Overloaded for testing purposes only
    static int alloc_count;
    static void* operator new(std::size_t size);
    static void operator delete(void* ptr);

public:
    SpriteSheet() = default;
    virtual ~SpriteSheet() {}
    virtual Pixel pixel(uint32_t x, uint32_t y) const = 0;
};


// Maintains a pool of open sprite sheets so that sprites can read their 
// pixels without opening and closing files a bazillion times.
class SpriteSheetPool
{
public: 
    static SpriteSheetPool& pool();

public:  
    SpriteSheet& get_sprite_sheet(const std::string file_name, SpriteSheet::Colour colour);

private:
    std::map<std::string, std::unique_ptr<SpriteSheet>> m_sheets;
};