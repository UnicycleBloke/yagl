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

public:
    SpriteSheet() = default;
    virtual ~SpriteSheet() {}

    //virtual void read(const std::string& file_path) = 0;    
    //virtual void write(const std::string& file_path) = 0;

    virtual Pixel pixel(uint32_t x, uint32_t y) = 0;
    //virtual void set_pixel(uint32_t x, uint32_t y, const Pixel& pixel) = 0;
};


// Maintains a pool of open sprite sheets so that sprites can read their 
// pixels without opening and closing files a bazillion times.
class SpriteSheetPool
{
public: 
    static SpriteSheetPool& pool();

public:  
    std::shared_ptr<SpriteSheet> get_sprite_sheet(const std::string file_name, SpriteSheet::Colour colour);

private:
    std::map<std::string, std::shared_ptr<SpriteSheet>> m_sheets;
};