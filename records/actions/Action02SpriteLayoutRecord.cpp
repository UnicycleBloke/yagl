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
#include "IntegerDescriptor.h"
#include "BooleanDescriptor.h"


void Action02SpriteLayoutRecord::SpriteRegisters::read(std::istream& is, bool is_parent)
{
    if (flags & BIT0_SKIP_SPRITE)     skip_sprite     = read_uint8(is);
    if (flags & BIT1_SPRITE_OFFSET)   sprite_offset   = read_uint8(is);
    if (flags & BIT2_RECOLOUR_OFFSET) recolour_offset = read_uint8(is);

    if (is_parent)
    {
        if (flags & BIT4_BB_XY_OFFSET) offset_x = read_uint8(is);
        if (flags & BIT4_BB_XY_OFFSET) offset_y = read_uint8(is);
        if (flags & BIT5_BB_Z_OFFSET)  offset_z = read_uint8(is);
    }
    else
    {
        if (flags & BIT4_CHILD_X_OFFSET) offset_x = read_uint8(is);
        if (flags & BIT5_CHILD_Y_OFFSET) offset_y = read_uint8(is);
    }

    if (flags & BIT6_SPRITE_VAR10)   sprite_var10   = read_uint8(is);
    if (flags & BIT7_RECOLOUR_VAR10) recolour_var10 = read_uint8(is);
}


void Action02SpriteLayoutRecord::SpriteRegisters::write(std::ostream& os, bool is_parent) const
{
    if (flags & BIT0_SKIP_SPRITE)       write_uint8(os, skip_sprite);
    if (flags & BIT1_SPRITE_OFFSET)     write_uint8(os, sprite_offset);
    if (flags & BIT2_RECOLOUR_OFFSET)   write_uint8(os, recolour_offset);

    if (is_parent)
    {
        if (flags & BIT4_BB_XY_OFFSET) write_uint8(os, offset_x);
        if (flags & BIT4_BB_XY_OFFSET) write_uint8(os, offset_y);
        if (flags & BIT5_BB_Z_OFFSET)  write_uint8(os, offset_z);
    }
    else
    {
        if (flags & BIT4_CHILD_X_OFFSET) write_uint8(os, offset_x);
        if (flags & BIT5_CHILD_Y_OFFSET) write_uint8(os, offset_y);
    }

    if (flags & BIT6_SPRITE_VAR10)   write_uint8(os, sprite_var10);
    if (flags & BIT7_RECOLOUR_VAR10) write_uint8(os, recolour_var10);
}


namespace {


constexpr const char* str_registers       = "registers";
constexpr const char* str_hide_sprite     = "hide_sprite";
constexpr const char* str_sprite_offset   = "sprite_offset";
constexpr const char* str_palette_offset  = "palette_offset";
constexpr const char* str_palette_act01   = "palette_act01";
constexpr const char* str_offset_x        = "offset_x";
constexpr const char* str_offset_y        = "offset_y";
constexpr const char* str_offset_z        = "offset_z";
constexpr const char* str_sprite_var10    = "sprite_var10";
constexpr const char* str_palette_var10   = "palette_var10";
constexpr const char* str_ground_sprite   = "ground_sprite";
constexpr const char* str_building_sprite = "building_sprite";
constexpr const char* str_child_sprite    = "child_sprite";
constexpr const char* str_offset          = "offset";
constexpr const char* str_extent          = "extent";


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices =
{
    { str_hide_sprite,    0x00 },
    { str_sprite_offset,  0x01 },
    { str_palette_offset, 0x02 },
    { str_palette_act01,  0x03 },
    { str_offset_x,       0x04 },
    { str_offset_y,       0x05 },
    { str_offset_z,       0x06 },
    { str_sprite_var10,   0x07 },
    { str_palette_var10,  0x08 },
};


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices2 =
{
    { str_ground_sprite,   0x01 },
    { str_building_sprite, 0x02 },
    { str_child_sprite,    0x03 },
};


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices3 =
{
    { str_offset,    0x01 },
    { str_extent,    0x02 },
    { str_registers, 0x03 },
};


const IntegerDescriptorT<uint8_t> desc_hide_sprite    { 0x00, str_hide_sprite,    UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_sprite_offset  { 0x01, str_sprite_offset,  UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_palette_offset { 0x02, str_palette_offset, UIntFormat::Hex };
const RawBoolDescriptor           desc_palette_act01  { 0x03, str_palette_act01 };
const IntegerDescriptorT<uint8_t> desc_offset_x       { 0x04, str_offset_x,       UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_offset_y       { 0x05, str_offset_y,       UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_offset_z       { 0x06, str_offset_z,       UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_sprite_var10   { 0x07, str_sprite_var10,   UIntFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_palette_var10  { 0x08, str_palette_var10,  UIntFormat::Hex };


} // namespace {


void Action02SpriteLayoutRecord::SpriteRegisters::print(std::ostream& os, bool is_parent, uint16_t indent) const
{
    if ((flags & 0xFF) > 0)
    {
        os << pad(indent) << str_registers << ": \n";
        os << pad(indent) << "{\n";

        if (flags & BIT0_SKIP_SPRITE)     desc_hide_sprite.print(skip_sprite, os, indent + 4);
        if (flags & BIT1_SPRITE_OFFSET)   desc_sprite_offset.print(sprite_offset, os, indent + 4);
        if (flags & BIT2_RECOLOUR_OFFSET) desc_palette_offset.print(recolour_offset, os, indent + 4);
        if (flags & BIT3_RECOLOUR_ACT01)
        {
            bool flag = true;
            desc_palette_act01.print(flag, os, indent + 4);
        }

        if (is_parent)
        {
            if (flags & BIT4_BB_XY_OFFSET) desc_offset_x.print(offset_x, os, indent + 4);
            if (flags & BIT4_BB_XY_OFFSET) desc_offset_y.print(offset_y, os, indent + 4);
            if (flags & BIT5_BB_Z_OFFSET)  desc_offset_z.print(offset_z, os, indent + 4);
        }
        else
        {
            if (flags & BIT4_CHILD_X_OFFSET) desc_offset_x.print(offset_x, os, indent + 4);
            if (flags & BIT5_CHILD_Y_OFFSET) desc_offset_y.print(offset_y, os, indent + 4);
        }

        if (flags & BIT6_SPRITE_VAR10)   desc_sprite_var10.print(sprite_var10, os, indent + 4);
        if (flags & BIT7_RECOLOUR_VAR10) desc_palette_var10.print(recolour_var10, os, indent + 4);

        os << pad(indent) << "}\n";
    }
}


void Action02SpriteLayoutRecord::SpriteRegisters::parse(TokenStream& is, bool is_parent)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices.find(token.value);
        if (it != g_indices.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            bool flag = false;
            switch (it->second)
            {
                case 0x00:
                    desc_hide_sprite.parse(skip_sprite, is);
                    flags |= BIT0_SKIP_SPRITE;
                    break;
                case 0x01:
                    desc_sprite_offset.parse(sprite_offset, is);
                    flags |= BIT1_SPRITE_OFFSET;
                    break;
                case 0x02:
                    desc_palette_offset.parse(recolour_offset, is);
                    flags |= BIT2_RECOLOUR_OFFSET;
                    break;
                case 0x03:
                    desc_palette_act01.parse(flag, is);
                    flags |= BIT3_RECOLOUR_ACT01;
                    break;
                case 0x04:
                    desc_offset_x.parse(offset_x, is);
                    flags |= BIT4_BB_XY_OFFSET;
                    break;
                case 0x05:
                    desc_offset_y.parse(offset_y, is);
                    flags |= (is_parent ? BIT4_BB_XY_OFFSET : BIT5_CHILD_Y_OFFSET);
                    break;
                case 0x06:
                    desc_offset_z.parse(offset_z, is);
                    flags |= BIT5_BB_Z_OFFSET;
                    break;
                case 0x07:
                    desc_sprite_var10.parse(sprite_var10, is);
                    flags |= BIT6_SPRITE_VAR10;
                    break;
                case 0x08:
                    desc_palette_var10.parse(recolour_var10, is);
                    flags |= BIT7_RECOLOUR_VAR10;
                    break;
            }

            is.match(TokenType::SemiColon);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);
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
    m_format = Format::Basic;
    if ((num_sprites & 0x3F) > 0)
    {
        m_format = Format::Extended;
    }
    if (num_sprites & 0x40)
    {
        m_format = Format::Advanced;
    }
    num_sprites &= 0x3F;

    if (m_format == Format::Basic)
    {
        m_ground_sprite = read_uint32(is);

        BuildingSprite sprite;
        sprite.sprite = read_uint32(is);
        sprite.xofs   = read_uint8(is);
        sprite.yofs   = read_uint8(is);
        sprite.zofs   = 0;
        sprite.new_bb = true;
        sprite.xext   = read_uint8(is);
        sprite.yext   = read_uint8(is);
        sprite.zext   = read_uint8(is);
        m_building_sprites.push_back(sprite);
    }
    else // Both Extended and Advanced handled here.
    {
        m_ground_sprite = read_uint32(is);
        if (m_format == Format::Advanced)
        {
            m_ground_regs.flags = read_uint16(is);
            m_ground_regs.read(is, true);
        }

        for (uint8_t i = 0; i < num_sprites; ++i)
        {
            BuildingSprite sprite;
            sprite.sprite = read_uint32(is);
            if (m_format == Format::Advanced)
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

            if (m_format == Format::Advanced)
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
    uint8_t num_sprites     = uint8_t(m_building_sprites.size());
    bool    extended_format = (m_format != Format::Basic) ||
                              (num_sprites > 1) ||
                              (m_building_sprites[0].new_bb == false) ||
                              (m_building_sprites[0].zofs != 0);
    if (extended_format)
    {
        write_uint8(os, num_sprites | ((m_format == Format::Advanced) ? 0x40 : 0x00));
    }
    else
    {
        write_uint8(os, 0x00);
    }
    write_uint32(os, m_ground_sprite);

    if (m_format == Format::Advanced)
    {
        write_uint16(os, m_ground_regs.flags);
        m_ground_regs.write(os, true); // this is a parent sprite.
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
            if (m_format == Format::Advanced)
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

            if (m_format == Format::Advanced)
            {
                // bool parameter indicates whether this is a parent bounding box
                sprite.regs.write(os, sprite.new_bb);
            }
        }
    }
}


void Action02SpriteLayoutRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_set_id);
    os << "> // Action02 sprite layout\n";
    os << pad(indent) << "{\n";

    os << pad(indent + 4) << str_ground_sprite << "<" << to_hex(m_ground_sprite) << ">\n";
    os << pad(indent + 4) << "{" << '\n';
    m_ground_regs.print(os, true, indent + 8); // true = is a parent
    os << pad(indent + 4) << "}" << '\n';

    for (const auto& sprite: m_building_sprites)
    {
        if (sprite.new_bb)
        {
            os << pad(indent + 4) << str_building_sprite << "<" << to_hex(sprite.sprite) << ">\n";
            os << pad(indent + 4) << "{\n";
            os << pad(indent + 8) << str_offset << ": " << to_hex(sprite.xofs) << ", " << to_hex(sprite.yofs) << ", " << to_hex(sprite.zofs) << ";\n";
            os << pad(indent + 8) << str_extent << ": " << to_hex(sprite.xext) << ", " << to_hex(sprite.yext) << ", " << to_hex(sprite.zext) << ";\n";
            sprite.regs.print(os, true, indent + 8); // true = is a parent
            os << pad(indent + 4) << "}\n";
        }
        else
        {
            os << pad(indent + 4) << str_child_sprite << "<" << to_hex(sprite.sprite) << ">\n";
            os << pad(indent + 4) << "{\n";
            os << pad(indent + 8) << str_offset << ": " << to_hex(sprite.xofs) << ", " << to_hex(sprite.yofs) << ";\n";
            sprite.regs.print(os, false, indent + 8); // false = not a parent
            os << pad(indent + 4) << "}\n";
        }
    }

    os << pad(indent) << "}\n";
}


void Action02SpriteLayoutRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_set_id = is.match_uint8();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices2.find(token.value);
        if (it != g_indices2.end())
        {
            is.match(TokenType::Ident);
            switch (it->second)
            {
                case 0x01: parse_ground_sprite(is); break;
                case 0x02: parse_building_sprite(is); break;
                case 0x03: parse_child_sprite(is); break;
            }
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);

    if ((m_format == Format::Basic) && (m_building_sprites.size() > 1))
    {
        m_format = Format::Extended;
    }
}


void Action02SpriteLayoutRecord::parse_ground_sprite(TokenStream& is)
{
    is.match(TokenType::OpenAngle);
    m_ground_sprite = is.match_uint32();
    m_ground_regs   = {};
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices3.find(token.value);
        if (it != g_indices3.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x03:
                    m_ground_regs.parse(is, true); // parent
                    m_format = (m_ground_regs.flags != 0x00) ? Format::Advanced : m_format;
                    break;

                default:
                    throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
            }
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);
}


void Action02SpriteLayoutRecord::parse_building_sprite(TokenStream& is)
{
    BuildingSprite sprite;
    sprite.new_bb = true;
    is.match(TokenType::OpenAngle);
    sprite.sprite = is.match_uint32();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices3.find(token.value);
        if (it != g_indices3.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x01:
                    sprite.xofs = is.match_uint8();
                    is.match(TokenType::Comma);
                    sprite.yofs = is.match_uint8();
                    is.match(TokenType::Comma);
                    sprite.zofs = is.match_uint8();
                    is.match(TokenType::SemiColon);
                    break; // offset

                case 0x02:
                    sprite.xext = is.match_uint8();
                    is.match(TokenType::Comma);
                    sprite.yext = is.match_uint8();
                    is.match(TokenType::Comma);
                    sprite.zext = is.match_uint8();
                    is.match(TokenType::SemiColon);
                    break; // extent

                case 0x03:
                    sprite.regs.parse(is, true); // Parent
                    m_format = (sprite.regs.flags != 0x00) ? Format::Advanced : m_format;
                    break;

                default:
                    throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
            }
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);

    m_building_sprites.push_back(sprite);
}


void Action02SpriteLayoutRecord::parse_child_sprite(TokenStream& is)
{
    BuildingSprite sprite;
    sprite.new_bb = false;
    is.match(TokenType::OpenAngle);
    sprite.sprite = is.match_uint32();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices3.find(token.value);
        if (it != g_indices3.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x01:
                    sprite.xofs = is.match_uint8();
                    is.match(TokenType::Comma);
                    sprite.yofs = is.match_uint8();
                    is.match(TokenType::SemiColon);
                    break; // offset

                case 0x03:
                    sprite.regs.parse(is, false); // Not a parent
                    m_format = (sprite.regs.flags != 0x00) ? Format::Advanced : m_format;
                    break;

                default:
                    throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
            }
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);

    m_building_sprites.push_back(sprite);
}

