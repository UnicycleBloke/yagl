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
#include "TokenStream.h"
#include <cstdint>
#include <iostream>
#include <vector>


// This structure represents the Action02 sprite layout data used for Houses,
// IndustryTiles, Objects, AirportTiles and RoadStops. The same data structure
// is used for Action00 Stations property 1A (advanced layouts). It looks like 
// stations can have multiple layouts.
// See https://newgrf-specs.tt-wiki.net/wiki/Action2/Sprite_Layout 
// and https://newgrf-specs.tt-wiki.net/wiki/Action0/Stations.

// Houses/Airports/Objects/...
// ---------------------------
// <Sprite-number> * <Length> 02 07/09/0F/11 <set-id>
//     <0x40 + num-sprites>
//     <groundsprite> <flags> [<register/value>]...
//     [
//         <buildingsprite> <flags>
//         (<xoffset> <yoffset> <zoffset> <xextent> <yextent> <zextent> | <xpixeloffset> <ypixeloffset> 80)
//         [<register/value>]...
//     ]...

// Stations
// --------
// <Sprite-number> * <Length> 00 04 <num-props> <num-info> <id> 
// 1A <num-layouts>
//     <0x40 + num-sprites>
//     <groundsprite> <flags> [<register/value>]...
//     [
//         <buildingsprite> <flags>
//         (<xoffset> <yoffset> <zoffset> <xextent> <yextent> <zextent> | <xpixeloffset> <ypixeloffset> 80)
//         [<register/value>]...
//     ]...
class SpriteLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    void parse_ground_sprite(TokenStream& is);
    void parse_building_sprite(TokenStream& is);
    void parse_child_sprite(TokenStream& is);

private:
    enum class Format 
    { 
        Basic,    // Single building sprite
        Extended, // Multiple building sprites
        Advanced  // Multiple building sprites with flags/registers
    };

    // TODO Does the set of supported flags/registers differ with the feature type?
    struct Registers
    {
        // Interpret the flags as a bit field. Each bit present indicates a 
        // register value to read from the file.
        static constexpr uint16_t BIT0_SKIP_SPRITE     = 0x01; // Only draw sprite if value is non-zero.
        static constexpr uint16_t BIT1_SPRITE_OFFSET   = 0x02; // Add signed offset to sprite.
        static constexpr uint16_t BIT2_RECOLOUR_OFFSET = 0x04; // Add signed offset to recolour sprite.
        static constexpr uint16_t BIT3_RECOLOUR_ACT01  = 0x08; // Recolour sprite is from Action 1 (no registers)
        static constexpr uint16_t BIT4_BB_XY_OFFSET    = 0x10; // Add signed offset to bounding box X and Y positions (2 registers)
        static constexpr uint16_t BIT5_BB_Z_OFFSET     = 0x20; // Add signed offset to bounding box Z positions.
        static constexpr uint16_t BIT4_CHILD_X_OFFSET  = 0x10; // Add signed offset to child sprite X positions.
        static constexpr uint16_t BIT5_CHILD_Y_OFFSET  = 0x20; // Add signed offset to child sprite Y positions.
        static constexpr uint16_t BIT6_SPRITE_VAR10    = 0x40; // Resolve sprite with a specific value in variable 10.
        static constexpr uint16_t BIT7_RECOLOUR_VAR10  = 0x80; // Resolve recolour sprite with a specific value in variable 10.

        uint16_t flags{};
        uint8_t  skip_sprite{};       // bit 0 value is 0 (skip), 1 (draw)
        uint8_t  sprite_offset{};     // bit 1
        uint8_t  recolour_offset{};   // bit 2
        uint8_t  offset_x{};          // bit 4
        uint8_t  offset_y{};          // bit 4 (parent), bit 5 (child/ground)
        uint8_t  offset_z{};          // bit 5 (parent)
        uint8_t  sprite_var10{};      // bit 6
        uint8_t  recolour_var10{};    // bit 7

        void read(std::istream& is,  bool is_parent);
        void write(std::ostream& os, bool is_parent) const;
        void print(std::ostream& os, bool is_parent, uint16_t indent) const;
        void parse(TokenStream& is,  bool is_parent);
    };

    struct Sprite
    {
        uint32_t  sprite{}; // May be zero only in the simple case
        Registers regs{};
        int8_t    xofs{};
        int8_t    yofs{};
        uint8_t   zofs{}; // Absent for Basic case. Otherwise 0x80 when re-using previous bounding box
        uint8_t   xext{}; // Absent when re-using previous bounding box
        uint8_t   yext{}; // Absent when re-using previous bounding box
        uint8_t   zext{}; // Absent when re-using previous bounding box
        bool      new_bb{false};
    };
    // Number of building sprites implicit from size of vector.
    using Sprites = std::vector<Sprite>; 

    Format    m_format{Format::Basic};
    uint32_t  m_ground_sprite{};
    Registers m_ground_regs{};
    Sprites   m_building_sprites{};
};