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
#include "RealSpriteRecord.h"


class SpriteSheetGenerator
{
    public:
        SpriteSheetGenerator(const std::map<uint32_t, SpriteZoomVector>& sprites,
            const std::string& base_name, GRFFormat format);
        void generate();

    private:
        using SpriteVector = std::vector<RealSpriteRecord*>;
        using ZoomLevel    = RealSpriteRecord::ZoomLevel;

        // Palette and mask are both 8bpp images. Masks are used in conjunction with 32bpp of the same size.
        // The data for a mask is contained within the RealSpriteRecord. We need to split this into two parts.
        enum class ColourType { Palette, RGB, RGBA, Mask };

        // This is needed so that we can create a map of different colour depths and zoom levels.
        // These categories will be placed into different spritesheets.
        struct Category
        {
            ZoomLevel  zoom;
            ColourType colour;
            bool operator<(const Category& other) const
            {
                uint8_t z1 = static_cast<uint8_t>(zoom);
                uint8_t c1 = static_cast<uint8_t>(colour);
                uint8_t z2 = static_cast<uint8_t>(other.zoom);
                uint8_t c2 = static_cast<uint8_t>(other.colour);

                if (z1 == z2)
                    return c1 < c2;
                return z1 < z2;
            }
        };

    private:
        void partition_sprites();
        void partition_sprite(std::map<Category, SpriteVector>& partitions,
            Category cat, RealSpriteRecord* sprite);
        void layout_sprites(Category category, SpriteVector sprites);

        void create_sprite_sheet(Category category, SpriteVector sprites,
            uint32_t index, uint32_t width, uint32_t height);

        // png++ uses a template for different colour depths. This is not
        // dynamic polymorphism, so create methods to handle the cases we need.
        void create_sprite_sheet_8bpp(const std::string& image_path, SpriteVector sprites,
            uint32_t width, uint32_t height);
        //void create_sprite_sheet_24bpp(const std::string& image_path, SpriteVector sprites,
        //    uint32_t width, uint32_t height);
        void create_sprite_sheet_32bpp(const std::string& image_path, SpriteVector sprites,
            uint32_t width, uint32_t height);
        void create_sprite_sheet_mask(const std::string& image_path, SpriteVector sprites,
            uint32_t width, uint32_t height);

    private:
        const std::map<uint32_t, SpriteZoomVector>& m_sprites;
        std::string                                 m_base_name;
        GRFFormat                                   m_format;
};


