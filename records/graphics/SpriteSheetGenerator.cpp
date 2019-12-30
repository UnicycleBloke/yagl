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
#include "SpriteSheetGenerator.h"
#include "Palettes.h"
#include "RealSpriteRecord.h"
#include "CommandLineOptions.h"
#include "SpriteIDLabel.h"
#include "png.hpp"
#include <sstream>
#include "FileSystem.h"


SpriteSheetGenerator::SpriteSheetGenerator(const std::map<uint32_t, SpriteZoomVector>& sprites, 
    const std::string& base_name, GRFFormat format)
: m_sprites{sprites}
, m_base_name{base_name}
, m_format{format}
{
}


void SpriteSheetGenerator::generate()
{
    partition_sprites();
}


void SpriteSheetGenerator::partition_sprite(std::map<Category, SpriteVector>& partitions, 
    Category cat, std::shared_ptr<RealSpriteRecord> sprite)
{
    if (partitions.find(cat) == partitions.end())
    {
        SpriteVector ids{ sprite }; 
        partitions[cat] = ids; 
    }
    else
    {
        partitions[cat].push_back(sprite); 
    }
}


void SpriteSheetGenerator::partition_sprites()
{
    // First work out what the different colour classes are that we have.
    // This map is used to count the number of sprites in each class. 
    std::map<Category, SpriteVector> partitions;
    for (const auto& it: m_sprites)
    {
        for (const auto record: it.second)
        {
            // We only care about real sprites. We might also encounter 
            // recolour sprites here.
            if (record->record_type() == RecordType::REAL_SPRITE)
            {
                // We need to downcast the pointer to access particular members.
                auto sprite = std::static_pointer_cast<RealSpriteRecord>(record);
                Category cat;

                cat.zoom       = sprite->zoom();
                uint8_t colour = sprite->colour();
                switch (colour)
                {
                    case RealSpriteRecord::HAS_PALETTE:
                        cat.colour = ColourType::Palette;
                        partition_sprite(partitions, cat, sprite); 
                        break;
                    
                    case RealSpriteRecord::HAS_RGB | RealSpriteRecord::HAS_ALPHA:
                        cat.colour = ColourType::RGBA;
                        partition_sprite(partitions, cat, sprite); 
                        break;

                    case RealSpriteRecord::HAS_RGB | RealSpriteRecord::HAS_ALPHA | RealSpriteRecord::HAS_PALETTE:
                        cat.colour = ColourType::RGBA;
                        partition_sprite(partitions, cat, sprite);
                        // The sprite contains two images 
                        cat.colour = ColourType::Mask;
                        partition_sprite(partitions, cat, sprite); 
                        break;

                    // Do these ever occur?
                    case RealSpriteRecord::HAS_RGB:
                        cat.colour = ColourType::RGB;
                        partition_sprite(partitions, cat, sprite); 
                        break;

                    // Do these ever occur?
                    case RealSpriteRecord::HAS_RGB | RealSpriteRecord::HAS_PALETTE:
                        cat.colour = ColourType::RGB;
                        partition_sprite(partitions, cat, sprite); 
                        cat.colour = ColourType::Mask;
                        partition_sprite(partitions, cat, sprite); 
                        break;

                    default:  throw RUNTIME_ERROR("Invalid colour depth");
                }   
            }
        }
    }

    for (const auto& p: partitions)
    {
        layout_sprites(p.first, p.second);
    }
}


void SpriteSheetGenerator::layout_sprites(Category category, SpriteVector sprites)
{
    // Constants
    const uint32_t max_width  = CommandLineOptions::options().width();
    const uint32_t max_height = CommandLineOptions::options().height();
    const uint32_t xmargin    = 10; 
    const uint32_t ymargin    = 10;

    // Image file index
    uint16_t index = 0;

    // Size of the current image
    uint32_t image_width  = 0;
    uint32_t image_height = 0;

    // Working variables
    uint32_t row_height = 0;
    uint32_t xoffset    = xmargin;  
    uint32_t yoffset    = ymargin;

    SpriteVector layout;
    for (const auto sprite: sprites)
    {
        if ((xoffset + sprite->xdim() + xmargin) > max_width)
        {
            yoffset     += (row_height + ymargin);
            xoffset      = xmargin;
            row_height   = 0;
            image_height = std::max(image_height, yoffset);

            // This will result in image a little taller than image_height because the
            // sprites in the final row are laid out before making the decision. Is it 
            // problem? Nah.
            if (image_height > max_height)
            {
                create_sprite_sheet(category, layout, index, image_width, image_height);
                layout.clear(); 

                image_width  = 0;
                image_height = 0;
                yoffset      = ymargin;
                ++index;
            }
        }

        // Distinguish mask from regular file offsets. This is only relevant for 
        // RGB[A]P sprites.
        if (category.colour == ColourType::Mask)
        {
            sprite->set_mask_xoff(xoffset);
            sprite->set_mask_yoff(yoffset);
        }
        else
        {
            sprite->set_xoff(xoffset);
            sprite->set_yoff(yoffset);
        }
        
        layout.push_back(sprite);

        row_height   = std::max<uint32_t>(row_height, sprite->ydim());
        xoffset     += sprite->xdim() + xmargin;
        image_width  = std::max(image_width, xoffset);
    }  

    image_height = std::max(image_height, yoffset + row_height + ymargin);
    create_sprite_sheet(category, layout, index, image_width, image_height);
}


void SpriteSheetGenerator::create_sprite_sheet(Category category, SpriteVector sprites, 
    uint32_t index, uint32_t width, uint32_t height)
{
    // Manufacture a file name for the sprite sheet. Maybe it makes
    // no sense to partition the sprites by zoom level, but let's do it 
    // for now.
    std::ostringstream os;
    os << m_base_name;

    switch (category.colour)
    {
        case ColourType::Palette:  
            os << "-8bpp"; 
            break;
        case ColourType::RGBA:  
            os << "-32bpp"; 
            break;
        // Do these ever occur?
        case ColourType::RGB:  
            os << "-24bpp"; 
            break;
        case ColourType::Mask:  
            os << "-mask"; 
            break;
        default:  throw RUNTIME_ERROR("Invalid colour depth");   
    }

    switch (category.zoom)
    {
        case ZoomLevel::Normal:    os << "-normal-"; break;
        case ZoomLevel::ZoomInX2:  os << "-zin2-";   break;
        case ZoomLevel::ZoomInX4:  os << "-zin4-";   break;
        case ZoomLevel::ZoomOutX2: os << "-zout2-";  break;
        case ZoomLevel::ZoomOutX4: os << "-zout4-";  break;
        case ZoomLevel::ZoomOutX8: os << "-zout8-";  break;
    }
    os << index << ".png";
    const std::string image_path = os.str();

    if (CommandLineOptions::options().debug()) 
    {
        std::cout << "Creating sprite sheet: " << image_path << '\n';
    }

    // Deal with different colour depths.
    switch (category.colour)
    {
        case ColourType::Palette: 
            create_sprite_sheet_8bpp(image_path, sprites, width, height);
            break;
        case ColourType::RGB: 
            create_sprite_sheet_24bpp(image_path, sprites, width, height);
            break;
        case ColourType::RGBA:
            create_sprite_sheet_32bpp(image_path, sprites, width, height);
            break;
        case ColourType::Mask:
            create_sprite_sheet_mask(image_path, sprites, width, height);
            break;
    }
}


// The functions below are horribly repetitive. png++ sort of forced
// this on us. Until we think of something neater. 


void SpriteSheetGenerator::create_sprite_sheet_8bpp(const std::string& image_path,
    SpriteVector sprites, uint32_t width, uint32_t height)
{    
    // Since this is a paletted image, we need to create the palette. Copy in
    // one of the standard palettes. This is set on the command line, or by an
    // Action14 PALS section.
    const PaletteArray& data = get_palette_data(CommandLineOptions::options().palette());
    png::palette palette(256);
    for (size_t i = 0; i < palette.size(); ++i)
    {
        palette[i] = png::color(data[3*i], data[3*i+1], data[3*i+2]);
    }

    // Finally create the actual image.
    png::image<png::index_pixel> image;
    image.set_palette(palette);
    image.resize(width, height);

    // Set all the pixels to brilliant white - this is the background.
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            image[y][x] = 0xFF;
        }
    }

    // Get the relative path for the spritesheet. This is used in the YAGL.
    fs::path absolute{image_path};
    fs::path relative{CommandLineOptions::options().yagl_dir()};
    relative.append(absolute.filename().string());

    // Copy the pixels for each sprite into the sprite sheet.
    uint32_t xlabel = 0;
    for (const auto& sprite: sprites)
    {
        // Each sprite needs to know the file name of its sprite sheet so that we 
        // can put this into the YAGL and read back the pixels later.
        sprite->set_filename(relative);

        const uint32_t xdim = sprite->xdim(); 
        const uint32_t ydim = sprite->ydim(); 
        const uint32_t xoff = sprite->xoff(); 
        const uint32_t yoff = sprite->yoff(); 

        if (xoff <= 10)
        {
            xlabel = 0;
        }
        if ((xoff > xlabel) && ((xoff + 40) < width))
        {
            uint32_t xtemp = xoff;
            SpriteIDLabel<png::index_pixel> label;
            label.draw(sprite->sprite_id(), xtemp, yoff - 7, image);
            xlabel = xtemp;
        }

        for (uint32_t y = 0; y < ydim; ++y)
        {
            for (uint32_t x = 0; x < xdim; ++x)
            {
                RealSpriteRecord::Pixel p = sprite->pixel(x, y);
                image[y + yoff][x + xoff] = p.index; 
            }
        }
    }

    image.write(image_path);
}


void SpriteSheetGenerator::create_sprite_sheet_24bpp(const std::string& image_path, 
    SpriteVector sprites, uint32_t width, uint32_t height)
{    
    // Finally create the actual image.
    png::image<png::rgb_pixel> image;
    image.resize(width, height);

    // Set all the pixels to brilliant white - this is the background.
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            image[y][x] = png::rgb_pixel{ 0xFF, 0xFF, 0xFF };
        }
    }

    // Get the relative path for the spritesheet. This is used in the YAGL.
    fs::path absolute{image_path};
    fs::path relative{CommandLineOptions::options().yagl_dir()};
    relative.append(absolute.filename().string());

    // Copy the pixels for each sprite into the sprite sheet.
    uint32_t xlabel = 0;
    for (const auto& sprite: sprites)
    {
        // Each sprite needs to know the file name of its sprite sheet so that we 
        // can put this into the YAGL and read back the pixels later.
        sprite->set_filename(relative);

        const uint32_t xdim = sprite->xdim(); 
        const uint32_t ydim = sprite->ydim(); 
        const uint32_t xoff = sprite->xoff(); 
        const uint32_t yoff = sprite->yoff(); 

        if (xoff <= 10)
        {
            xlabel = 0;
        }
        if ((xoff > xlabel) && ((xoff + 40) < width))
        {
            uint32_t xtemp = xoff;
            SpriteIDLabel<png::rgb_pixel> label;
            label.draw(sprite->sprite_id(), xtemp, yoff - 7, image);
            xlabel = xtemp;
        }

        for (uint32_t y = 0; y < ydim; ++y)
        {
            for (uint32_t x = 0; x < xdim; ++x)
            {
                RealSpriteRecord::Pixel p = sprite->pixel(x, y);
                image[y + yoff][x + xoff] = png::rgb_pixel{ p.red, p.green, p.blue };
            }
        }
    }

    image.write(image_path);
}


void SpriteSheetGenerator::create_sprite_sheet_32bpp(const std::string& image_path, 
    SpriteVector sprites, uint32_t width, uint32_t height)
{    
    // Finally create the actual image.
    png::image<png::rgba_pixel> image;
    image.resize(width, height);

    // Set all the pixels to brilliant white - this is the background.
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            image[y][x] = png::rgba_pixel{ 0xFF, 0xFF, 0xFF, 0xFF };
        }
    }

    // Get the relative path for the spritesheet. This is used in the YAGL.
    fs::path absolute{image_path};
    fs::path relative{CommandLineOptions::options().yagl_dir()};
    relative.append(absolute.filename().string());

    // Copy the pixels for each sprite into the sprite sheet.
    uint32_t xlabel = 0;
    for (const auto& sprite: sprites)
    {
        // Each sprite needs to know the file name of its sprite sheet so that we 
        // can put this into the YAGL and read back the pixels later.
        sprite->set_filename(relative);

        const uint32_t xdim = sprite->xdim(); 
        const uint32_t ydim = sprite->ydim(); 
        const uint32_t xoff = sprite->xoff(); 
        const uint32_t yoff = sprite->yoff(); 

        if (xoff <= 10)
        {
            xlabel = 0;
        }
        if ((xoff > xlabel) && ((xoff + 40) < width))
        {
            uint32_t xtemp = xoff;
            SpriteIDLabel<png::rgba_pixel> label;
            label.draw(sprite->sprite_id(), xtemp, yoff - 7, image);
            xlabel = xtemp;
        }

        for (uint32_t y = 0; y < ydim; ++y)
        {
            for (uint32_t x = 0; x < xdim; ++x)
            {
                RealSpriteRecord::Pixel p = sprite->pixel(x, y);
                image[y + yoff][x + xoff] = png::rgba_pixel{ p.red, p.green, p.blue, p.alpha };
            }
        }
    }

    image.write(image_path);
}


void SpriteSheetGenerator::create_sprite_sheet_mask(const std::string& image_path,
    SpriteVector sprites, uint32_t width, uint32_t height)
{    
    // Since this is a paletted image, we need to create the palette. Copy in
    // one of the standard palettes. This is set on the command line, or by an
    // Action14 PALS section.
    const PaletteArray& data = get_palette_data(CommandLineOptions::options().palette());
    png::palette palette(256);
    for (size_t i = 0; i < palette.size(); ++i)
    {
        palette[i] = png::color(data[3*i], data[3*i+1], data[3*i+2]);
    }

    // Finally create the actual image.
    png::image<png::index_pixel> image;
    image.set_palette(palette);
    image.resize(width, height);

    // Set all the pixels to brilliant white - this is the background.
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            image[y][x] = 0xFF;
        }
    }

    // Get the relative path for the spritesheet. This is used in the YAGL.
    fs::path absolute{image_path};
    fs::path relative{CommandLineOptions::options().yagl_dir()};
    relative.append(absolute.filename().string());

    // Copy the pixels for each sprite into the sprite sheet.
    uint32_t xlabel = 0;
    for (const auto& sprite: sprites)
    {
        // Each sprite needs to know the file name of its sprite sheet so that we 
        // can put this into the YAGL and read back the pixels later.
        sprite->set_mask_filename(relative);

        const uint32_t xdim = sprite->xdim(); 
        const uint32_t ydim = sprite->ydim(); 
        const uint32_t xoff = sprite->mask_xoff(); 
        const uint32_t yoff = sprite->mask_yoff(); 

        if (xoff <= 10)
        {
            xlabel = 0;
        }
        if ((xoff > xlabel) && ((xoff + 40) < width))
        {
            uint32_t xtemp = xoff;
            SpriteIDLabel<png::index_pixel> label;
            label.draw(sprite->sprite_id(), xtemp, yoff - 7, image);
            xlabel = xtemp;
        }

        for (uint32_t y = 0; y < ydim; ++y)
        {
            for (uint32_t x = 0; x < xdim; ++x)
            {
                RealSpriteRecord::Pixel p = sprite->pixel(x, y);
                image[y + yoff][x + xoff] = p.index; 
            }
        }
    }

    image.write(image_path);
}


