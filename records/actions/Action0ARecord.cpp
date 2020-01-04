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
#include "Action0ARecord.h"
#include "StreamHelpers.h"
#include "RecolourRecord.h"


void Action0ARecord::read(std::istream& is, const GRFInfo& info)
{
    uint8_t num_sets = read_uint8(is);
    m_sets.resize(num_sets);

    for (uint8_t i = 0; i < num_sets; ++i)
    {
        m_sets[i].num_sprites  = read_uint8(is);
        m_sets[i].first_sprite = read_uint16(is);
    }
}


void Action0ARecord::write(std::ostream& os, const GRFInfo& info) const
{
    ContainerRecord::write(os, info);

    uint8_t num_sets = m_sets.size();
    write_uint8(os, num_sets);

    for (const auto& set: m_sets)
    {
        write_uint8(os, set.num_sprites);
        write_uint16(os, set.first_sprite);
    }
}  


uint16_t Action0ARecord::num_sprites_to_read() const
{
    uint16_t total = 0;
    for (const auto& set: m_sets)
    {
        total += set.num_sprites;
    }
    return total;
}


// replacement_sprite_sets // Action0A 
// {
//     replacement_sprite_set // One or more sets
//     {
//         0x00000AB5: // Sprite ID being replaced
//         sprite_id: 0x00001061 // Sprite ID in this file of replacement
//         {
//             [8, 21, -3, -11], normal, 8bpp, "sprites/zbase_extra-8bpp-normal-0.png", [641, 7372];
//             [32, 34, -16, -23], normal, 32bpp|mask|chunked, "sprites/zbase_extra-32bpp-normal-0.png", [149, 6763], "sprites/zbase_extra-mask-normal-0.png", [372, 293];
//             [128, 135, -64, -91], zin4, 32bpp|mask|chunked, "sprites/zbase_extra-32bpp-zin4-5.png", [338, 9124], "sprites/zbase_extra-mask-zin4-0.png", [338, 3769];
//             [64, 68, -32, -46], zin2, 32bpp|mask|chunked, "sprites/zbase_extra-32bpp-zin2-1.png", [406, 5781], "sprites/zbase_extra-mask-zin2-0.png", [184, 982];
//         }
//         0x00000AB6:
//         sprite_id: 0x00001062
//         {
//             [20, 16, -14, -9], normal, 8bpp, "sprites/zbase_extra-8bpp-normal-0.png", [659, 7372];
//             [32, 24, -18, -13], normal, 32bpp|mask, "sprites/zbase_extra-32bpp-normal-0.png", [191, 6763], "sprites/zbase_extra-mask-normal-0.png", [414, 293];
//             [128, 96, -72, -52], zin4, 32bpp|mask|chunked, "sprites/zbase_extra-32bpp-zin4-5.png", [476, 9124], "sprites/zbase_extra-mask-zin4-0.png", [476, 3769];
//             [64, 48, -36, -26], zin2, 32bpp|mask|chunked, "sprites/zbase_extra-32bpp-zin2-1.png", [480, 5781], "sprites/zbase_extra-mask-zin2-0.png", [258, 982];
//         }
//         ...
//     }
//     replacement_sprite_set
//     {
//     ... 


static constexpr const char* str_replacement_sprite_set = "replacement_sprite_set";


void Action0ARecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0A\n";
    os << pad(indent) << "{\n";

    uint16_t index = 0;
    for (const auto& set: m_sets)
    {
        os << pad(indent + 4) << str_replacement_sprite_set;
        os << "<" << to_hex<uint16_t>(set.first_sprite) << "> // <first_sprite>\n";
        os << pad(indent + 4) << "{\n";

        for (uint16_t i = 0; i < set.num_sprites; ++i)
        {
            os << pad(indent + 8) << "// Replace sprite " << to_hex<uint16_t>(set.first_sprite + i) << ":\n";
            print_sprite(index, os, sprites, indent + 8);
            ++index;
        }

        os << pad(indent + 4) << "}\n";
    }

    os << pad(indent) << "}\n";
}


void Action0ARecord::parse(TokenStream& is)
{
    const std::string ident = RecordName(record_type());
    is.match_ident(ident);
    is.match(TokenType::OpenBrace);

    while (is.peek().type != TokenType::CloseBrace)
    {
        SpriteSet set{};
        is.match_ident(str_replacement_sprite_set); 
        is.match(TokenType::OpenAngle);
        set.first_sprite = is.match_integer();
        is.match(TokenType::CloseAngle);
        is.match(TokenType::OpenBrace);
    
        while (is.peek().type != TokenType::CloseBrace)
        {
            parse_sprite(is);
            ++set.num_sprites;
        }
    
        is.match(TokenType::CloseBrace);
        m_sets.push_back(set);
    }

    is.match(TokenType::CloseBrace);
}
