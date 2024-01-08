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
#include "SpriteSheetReader.h"


// class RGBSpriteSheet : public SpriteSheet
// {
// public:
//     RGBSpriteSheet(const std::string& file_name);
//     Pixel pixel(uint32_t x, uint32_t y) const override;

// private:
//     png::image<png::rgb_pixel> m_image;
// };


int SpriteSheet::alloc_count = 0;


class RGBASpriteSheet : public SpriteSheet
{
public:
    RGBASpriteSheet(const std::string& file_name);
    Pixel pixel(uint32_t x, uint32_t y) const override;

private:
    png::image<png::rgba_pixel> m_image;
};


class PaletteSpriteSheet : public SpriteSheet
{
public:
    PaletteSpriteSheet(const std::string& file_name);
    Pixel pixel(uint32_t x, uint32_t y) const override;

private:
    png::image<png::index_pixel> m_image;
};


RGBASpriteSheet::RGBASpriteSheet(const std::string& file_name)
: m_image{file_name, png::require_color_space<png::rgba_pixel>()}
{
}


SpriteSheet::Pixel RGBASpriteSheet::pixel(uint32_t x, uint32_t y) const
{
    png::rgba_pixel in = m_image[y][x];

    Pixel out = {};
    out.red   = in.red;
    out.green = in.green;
    out.blue  = in.blue;
    out.alpha = in.alpha;

    return out;
}


PaletteSpriteSheet::PaletteSpriteSheet(const std::string& file_name)
: m_image{file_name, png::require_color_space<png::index_pixel>()}
{
}


SpriteSheet::Pixel PaletteSpriteSheet::pixel(uint32_t x, uint32_t y) const
{
    Pixel out = {};
    out.index = m_image[y][x];
    return out;
}


SpriteSheetPool& SpriteSheetPool::pool()
{
    static SpriteSheetPool instance;
    return instance;
}


SpriteSheet& SpriteSheetPool::get_sprite_sheet(const std::string file_name, SpriteSheet::Colour colour)
{
    auto it = m_sheets.find(file_name);
    if (it == m_sheets.end())
    {
        using Colour = SpriteSheet::Colour;

        std::cout << "Opening sprite sheet: " << file_name << "..." << std::endl;

        std::unique_ptr<SpriteSheet> sheet;
        switch (colour)
        {
            case Colour::Palette:
                sheet = std::make_unique<PaletteSpriteSheet>(file_name);
                break;
            // case Colour::RGB:
            //     sheet = std::make_unique<RGBSpriteSheet>(file_name);
            //     break;
            case Colour::RGBA:
                sheet = std::make_unique<RGBASpriteSheet>(file_name);
                break;
        }

        m_sheets[file_name] = std::move(sheet);
    }

    return *m_sheets[file_name];
}
