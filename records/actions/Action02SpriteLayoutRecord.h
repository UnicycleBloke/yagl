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
#include "Record.h"


class Action02SpriteLayoutRecord : public ActionRecord
{
public:
    Action02SpriteLayoutRecord()
    : ActionRecord{RecordType::ACTION_02_SPRITE_LAYOUT}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;    
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

private:    
    void parse_ground_sprite(TokenStream& is);
    void parse_building_sprite(TokenStream& is);
    void parse_child_sprite(TokenStream& is);

private:
    struct SpriteRegisters
    {
        static constexpr uint8_t BIT0_SKIP_SPRITE     = 0x01; // Only draw sprite if value is non-zero.
        static constexpr uint8_t BIT1_SPRITE_OFFSET   = 0x02; // Add signed offset to sprite.
        static constexpr uint8_t BIT2_RECOLOUR_OFFSET = 0x04; // Add signed offset to recolour sprite.
        static constexpr uint8_t BIT3_RECOLOUR_ACT01  = 0x08; // Recolour sprite is from Action 1 (no registers)
        static constexpr uint8_t BIT4_BB_XY_OFFSET    = 0x10; // Add signed offset to bounding box X and Y positions (2 registers) 
        static constexpr uint8_t BIT5_BB_Z_OFFSET     = 0x20; // Add signed offset to bounding box Z positions.
        static constexpr uint8_t BIT4_CHILD_X_OFFSET  = 0x10; // Add signed offset to child sprite X positions.
        static constexpr uint8_t BIT5_CHILD_Y_OFFSET  = 0x20; // Add signed offset to child sprite Y positions.
        static constexpr uint8_t BIT6_SPRITE_VAR10    = 0x40; // Resolve sprite with a specific value in variable 10.
        static constexpr uint8_t BIT7_RECOLOUR_VAR10  = 0x80; // Resolve recolour sprite with a specific value in variable 10.

        uint16_t flags = 0x00;
        uint8_t  skip_sprite;       // bit 0 value is 0 (skip), 1 (draw)
        uint8_t  sprite_offset;     // bit 1
        uint8_t  recolour_offset;   // bit 2
        uint8_t  offset_x;          // bit 4
        uint8_t  offset_y;          // bit 4 (parent), bit 5 (child/ground)
        uint8_t  offset_z;          // bit 5 (parent)
        uint8_t  sprite_var10;      // bit 6 
        uint8_t  recolour_var10;    // bit 7

        void read(std::istream& is, bool is_parent);
        void write(std::ostream& os, bool is_parent) const;
        void print(std::ostream& os, bool is_parent, uint16_t indent) const;
        void parse(TokenStream& is, bool is_parent);
    };

private:
    struct BuildingSprite
    {
        uint32_t        sprite = 0x00; // May be zero only in the simple case 
        SpriteRegisters regs   = {};
        int8_t          xofs   = 0x00;
        int8_t          yofs   = 0x00;
        uint8_t         zofs   = 0x80; // Absent for simple case. 0x80 when re-using previous bounding box 
        uint8_t         xext   = 0x00; // Absent when re-using previous bounding box 
        uint8_t         yext   = 0x00; // Absent when re-using previous bounding box 
        uint8_t         zext   = 0x00; // Absent when re-using previous bounding box 
        bool            new_bb = false;
    };

private:    
    enum class Format { Basic, Extended, Advanced };

private:
    FeatureType     m_feature       = FeatureType::Trains;
    uint8_t         m_set_id        = 0x00;
    uint32_t        m_ground_sprite = 0x00;
    SpriteRegisters m_ground_regs   = {};
    Format          m_format        = Format::Basic;

    std::vector<BuildingSprite> m_building_sprites;
};

