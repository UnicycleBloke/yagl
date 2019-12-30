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
#include "Action00Feature.h"
#include <vector>


class Action00Stations : public Action00Feature
{
public:
    Action00Stations() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    struct SpriteTileData
    {
        bool     new_bb; ///< Indicates that this sprite has its own bounding box
        int8_t   x_off;
        int8_t   y_off;
        uint8_t  z_off;
        uint8_t  x_ext;
        uint8_t  y_ext;
        uint8_t  z_ext;
        uint32_t sprite;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

private:
    struct SpriteTile
    {
        uint32_t ground_sprite;
        std::vector<SpriteTileData> data;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

public:
    struct SpriteLayout
    {
        std::vector<SpriteTile> tiles;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

private:
    struct CustomLayout
    {
        enum class Platform 
        {
            Plain     = 0x00, 
            Building  = 0x02, 
            RoofLeft  = 0x04, 
            RoofRight = 0x06
        }; 

        uint8_t platform_length;
        uint8_t platform_count;
        // length * count, All bytes 00, 02, 04 or 06
        std::vector<Platform> platform_tiles; 

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

public:
    struct CustomStation
    {
        std::vector<CustomLayout> layouts;

        void read(std::istream& is);
        void write(std::ostream& os) const;
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
    };

private:
    uint32_t       m_08_class_id;
    SpriteLayout   m_09_sprite_layout;
    uint8_t        m_0A_copy_sprite_layout_id;
    uint8_t        m_0B_callback_flags;
    uint8_t        m_0C_disabled_platform_numbers;
    uint8_t        m_0D_disabled_platform_lengths;
    CustomStation  m_0E_custom_layout;
    uint8_t        m_0F_copy_custom_layout_id;
    uint16_t       m_10_little_lots_threshold;
    uint8_t        m_11_pylon_placement;
    uint32_t       m_12_cargo_type_triggers;
    uint8_t        m_13_general_flags;
    uint8_t        m_14_overhead_wire_placement;
    uint8_t        m_15_can_train_enter_tile;
    uint8_t        m_16_animation_frames;
    uint8_t        m_16_animation_type;
    uint8_t        m_17_animation_speed;
    uint16_t       m_18_animation_triggers;

    //Action00StationRouting  m_19_station_routing;
    //Action00StationAdvanced m_1A_station_advanced; - like Action02SpriteLayout?

};


