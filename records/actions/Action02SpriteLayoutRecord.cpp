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
#include "Action02SpriteLayoutRecord.h"
#include "StreamHelpers.h"


void Action02SpriteLayoutRecord::SpriteRegisters::read(std::istream& is, bool is_parent)
{
    if (flags & HIDE_SPRITE)    hide_sprite    = read_uint8(is);
    if (flags & SPRITE_OFFSET)  sprite_offset  = read_uint8(is);
    if (flags & PALETTE_OFFSET) palette_offset = read_uint8(is);
    if (flags & PALETTE_ACT01)  palette_act01  = read_uint8(is);
    
    if (is_parent)
    {
        if (flags & BB_XY_OFFSET) offset_x = read_uint8(is);
        if (flags & BB_XY_OFFSET) offset_y = read_uint8(is);
        if (flags & BB_Z_OFFSET)  offset_z = read_uint8(is);
    }
    else
    {
        if (flags & CHILD_X_OFFSET) offset_x = read_uint8(is);
        if (flags & CHILD_Y_OFFSET) offset_y = read_uint8(is);
    }

    if (flags & SPRITE_VAR10) sprite_var10   = read_uint8(is);
    if (flags & PALETTE_VAR10) palette_var10 = read_uint8(is);
}


void Action02SpriteLayoutRecord::SpriteRegisters::write(std::ostream& os, bool is_parent) const
{
    if (flags & HIDE_SPRITE)    write_uint8(os, hide_sprite);
    if (flags & SPRITE_OFFSET)  write_uint8(os, sprite_offset);
    if (flags & PALETTE_OFFSET) write_uint8(os, palette_offset);
    if (flags & PALETTE_ACT01)  write_uint8(os, palette_act01);
    
    if (is_parent)
    {
        if (flags & BB_XY_OFFSET) write_uint8(os, offset_x);
        if (flags & BB_XY_OFFSET) write_uint8(os, offset_y);
        if (flags & BB_Z_OFFSET)  write_uint8(os, offset_z);
    }
    else
    {
        if (flags & CHILD_X_OFFSET) write_uint8(os, offset_x);
        if (flags & CHILD_Y_OFFSET) write_uint8(os, offset_y);
    }

    if (flags & SPRITE_VAR10)  write_uint8(os, sprite_var10);
    if (flags & PALETTE_VAR10) write_uint8(os, palette_var10);
}


void Action02SpriteLayoutRecord::SpriteRegisters::print(std::ostream& os, bool is_parent, uint16_t indent) const
{
    if ((flags & 0xFF) > 0)
    {
        os << pad(indent) << "registers: \n";
        os << pad(indent) << "{\n";

        if (flags & HIDE_SPRITE)    os << pad(indent + 4) << "hide_sprite: "    << to_hex(hide_sprite) << ";\n";
        if (flags & SPRITE_OFFSET)  os << pad(indent + 4) << "sprite_offset: "  << to_hex(sprite_offset) << ";\n";
        if (flags & PALETTE_OFFSET) os << pad(indent + 4) << "palette_offset: " << to_hex(palette_offset) << ";\n";
        if (flags & PALETTE_ACT01)  os << pad(indent + 4) << "palette_act01: "  << to_hex(palette_act01) << ";\n";
        
        if (is_parent)
        {
            if (flags & BB_XY_OFFSET) os << pad(indent + 4) << "offset_x: " << to_hex(offset_x) << ";\n";
            if (flags & BB_XY_OFFSET) os << pad(indent + 4) << "offset_y: " << to_hex(offset_y) << ";\n";
            if (flags & BB_Z_OFFSET)  os << pad(indent + 4) << "offset_z: " << to_hex(offset_z) << ";\n";
        }
        else
        {
            if (flags & CHILD_X_OFFSET) os << pad(indent + 4) << "offset_x: " << to_hex(offset_x) << ";\n";
            if (flags & CHILD_Y_OFFSET) os << pad(indent + 4) << "offset_y: " << to_hex(offset_y) << ";\n";
        }

        if (flags & SPRITE_VAR10)  os << pad(indent + 4) << "sprite_var10: "  << to_hex(sprite_var10) << ";\n";
        if (flags & PALETTE_VAR10) os << pad(indent + 4) << "palette_var10: " << to_hex(palette_var10) << ";\n";

        os << pad(indent) << "}\n";
    }
}


void Action02SpriteLayoutRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));
    m_set_id  = read_uint8(is);
    
    // This will be zero for the basic format - meaning only one building sprite.
    // If the value has bit 6 (0x40) set, it is advanced format rather than 
    // extended format. Presumably byte read will be 0x41 rather 0x40 for a single 
    // building sprite.
    uint8_t num_sprites = read_uint8(is);
    m_advanced    = (num_sprites & 0x40) == 0x40;
    num_sprites   =  num_sprites & 0x3F;
    bool extended = (num_sprites > 0) | m_advanced;
    
    m_ground_sprite = read_uint32(is);
    if (m_advanced)
    {
        m_ground_regs.flags = read_uint16(is);
        m_ground_regs.read(is, false);
    }

    if (!extended)
    {
        BuildingSprite sprite;
        sprite.sprite = read_uint32(is);
        sprite.xofs   = read_uint8(is);
        sprite.yofs   = read_uint8(is);
        // We don't have these for the simple format, but keep them consistent for later.
        sprite.zofs   = 0;
        sprite.new_bb = true;
        sprite.xext   = read_uint8(is);
        sprite.yext   = read_uint8(is);
        sprite.zext   = read_uint8(is);
        m_building_sprites.push_back(sprite);
    }
    else
    {
        for (uint8_t i = 0; i < num_sprites; ++i)
        {
            BuildingSprite sprite;
            sprite.sprite = read_uint32(is);
            if (m_advanced)
            {
                sprite.regs.flags = read_uint16(is);
            }
            sprite.xofs   = read_uint8(is);
            sprite.yofs   = read_uint8(is);
            sprite.zofs   = read_uint8(is);
            sprite.new_bb = (uint8_t(sprite.zofs) != 0x80);

            // New BB means this is a building sprite, otherwise a child sprite.
            if (sprite.new_bb)
            {
                sprite.xext   = read_uint8(is);
                sprite.yext   = read_uint8(is);
                sprite.zext   = read_uint8(is);
            }

            if (m_advanced)
            {
                // bool parameter indicates whether this is a parent bounding box
                sprite.regs.read(is, sprite.new_bb);
            }

            m_building_sprites.push_back(sprite);
        }
    }
}


void Action02SpriteLayoutRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_set_id);

    // Not sure I've fully understood the relationships here. It appears that
    // you can only have advanced (registers) in extended format. Presumably
    // byte written will be 0x41 rather 0x40 for a single building sprite.
    uint8_t num_sprites     = m_building_sprites.size();
    bool    extended_format = m_advanced ||
                              (num_sprites != 1) ||
                              (m_building_sprites[0].new_bb == false) ||
                              (m_building_sprites[0].zofs != 0);
    if (extended_format)
    {
        write_uint8(os, num_sprites | (m_advanced ? 0x40 : 0x00));
    }
    else
    {
        write_uint8(os, 0x00);
    }
    write_uint32(os, m_ground_sprite);

    if (m_advanced)
    {
        write_uint16(os, m_ground_regs.flags);
        m_ground_regs.write(os, false);
    }

    if (!extended_format)
    {
        const BuildingSprite& sprite = m_building_sprites[0];
        write_uint32(os, sprite.sprite);
        write_uint8(os, sprite.xofs);
        write_uint8(os, sprite.yofs);
        write_uint8(os, sprite.xext);
        write_uint8(os, sprite.yext);
        write_uint8(os, sprite.zext);
    }
    else
    {
        for (uint8_t i = 0; i < num_sprites; ++i)
        {
            const BuildingSprite& sprite = m_building_sprites[i];
            write_uint32(os, sprite.sprite);
            if (m_advanced)
            {
                write_uint16(os, sprite.regs.flags);
            }
            write_uint8(os, sprite.xofs);
            write_uint8(os, sprite.yofs);
            write_uint8(os, sprite.new_bb ? sprite.zofs : 0x80);

            if (sprite.new_bb)
            {
                write_uint8(os, sprite.xext);
                write_uint8(os, sprite.yext);
                write_uint8(os, sprite.zext);
            }

            if (m_advanced)
            {
                // bool parameter indicates whether this is a parent bounding box
                sprite.regs.write(os, sprite.new_bb);
            }
        }
    }
}  


// some bits in the sprite ID are important for other reasons.
// If any registers are set, the action is advanced.
// If more than one building sprite, the action is extended.
// If more than zero child sprite, the action is extended.
// ground_sprite<id>:
// {
//     registers:
//     {
//         skip_bb: 0x01; // hide_sprite   bit 0
//         sprite:  0x02;
//         palette: 0x04;
//         palette_act01: 0x08;
//         xoffset: 0x10;
//         yoffset: 0x10;
//         zoffset: 0x20;
//         sprite_var10: 0x40;
//         palette_var10: 0x80;
//     }
// }
// building_sprite<id>:
// {
//     xoffset: 0;
//     yoffset: 0;
//     zoffset: 0;
//     xextent: 0;
//     yextent: 0;
//     zextent: 0;   

//     offset(x, y, z);
//     extent(x, y, z);

//     registers:
//     {
//     }
// }
// child_sprite<id>:
// {
//     xoffset: 0; // pixel offset
//     yoffset: 0;
//     registers:
//     {
//     }
// }

//     uint32_t        m_ground_sprite;
//     SpriteRegisters m_ground_regs;
//     bool            m_advanced;
//     std::vector<BuildingSprite> m_building_sprites;
// };


void Action02SpriteLayoutRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action02 basic" << '\n';
    os << pad(indent) << "{" << '\n';
    os << pad(indent + 4) << "this_set_id: " << to_hex(m_set_id) << ";\n";

    os << pad(indent + 4) << "ground_sprite: " << to_hex(m_ground_sprite) << ";\n";
    os << pad(indent + 4) << "{" << '\n';
    m_ground_regs.print(os, true, indent + 8);
    os << pad(indent + 4) << "}" << '\n';

    for (const auto& sprite: m_building_sprites)
    {
        if (sprite.new_bb)
        {
            os << pad(indent + 4) << "building_sprite: " << to_hex(sprite.sprite) << '\n';
            os << pad(indent + 4) << "{\n";
            os << pad(indent + 8) << "offset: " << to_hex(sprite.xofs) << ", " << to_hex(sprite.yofs) << ", " << to_hex(sprite.zofs) << ";\n";
            os << pad(indent + 8) << "extent: " << to_hex(sprite.xext) << ", " << to_hex(sprite.yext) << ", " << to_hex(sprite.zext) << ";\n";
            sprite.regs.print(os, true, indent + 8);
            os << pad(indent + 4) << "}\n";
        }
        else
        {
            os << pad(indent + 4) << "child_sprite: " << to_hex(sprite.sprite) << '\n';
            os << pad(indent + 4) << "{\n";
            os << pad(indent + 8) << "offset: " << to_hex(sprite.xofs) << ", " << to_hex(sprite.yofs) << ";\n";
            sprite.regs.print(os, true, indent + 8);
            os << pad(indent + 4) << "}\n";
        }
    }

    os << pad(indent) << "}\n";
}


void Action02SpriteLayoutRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action02SpriteLayoutRecord::parse not implemented");
}
