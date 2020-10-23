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
#include "SpriteIndexRecord.h"
#include "RealSpriteRecord.h"
#include "RecolourRecord.h"
#include "StreamHelpers.h"
#include "ActionFFRecord.h"
#include "SpriteWrapperRecord.h"
#include "NewGRFData.h"
#include "CommandLineOptions.h"


void SpriteIndexRecord::read(std::istream& is, const GRFInfo& info)
{
    m_sprite_id = read_uint32(is);
}


void SpriteIndexRecord::write(std::ostream& os, const GRFInfo& info) const
{
    write_uint32(os, m_sprite_id);
}  


static constexpr const char* str_sprite_id = "sprite_id";


void SpriteIndexRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << str_sprite_id << "<" << to_hex(m_sprite_id) << ">\n";
    os << pad(indent) << "{" << '\n';

    if (sprites.find(m_sprite_id) != sprites.end())
    {
        const SpriteZoomVector& sprite_list = sprites.at(m_sprite_id);
        for (const auto& sprite: sprite_list)
        {
            sprite->print(os, sprites, indent + 4);
        }
    }
    else
    {
        if (CommandLineOptions::options().debug())
        {
            std::cout << pad(indent + 4) << "Missing sprites.\n";
        }
    }

    os << pad(indent) << "}" << '\n';
}


void SpriteIndexRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(str_sprite_id);
    is.match(TokenType::OpenAngle);
    m_sprite_id = is.match_uint32();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        std::unique_ptr<Record> record;

        // SpriteIndexRecords generally "contain" one or several real sprite images (zoom levels),
        // But can also contain a single binary sound effect, which is itself found in the graphics
        // section of the GRF.
        if (is.peek().type == TokenType::OpenBracket)
        {
            // Need to create RealSpriteRecord     
            // Size and compression are place holder values to be read from the token stream.
            record = std::make_unique<RealSpriteRecord>(m_sprite_id, 0, 0);
            record->parse(is, sprites);
        }
        else
        {
            // ActionFF is a sound effect. The SpriteWrapper is needed to write the sprite index value
            // into the binary file, which the ActionFF does not do. This is only important for Container
            // version 2 files. RealSprites this for themselves. ActionFF can also be used directly in the 
            // data section, whether Container 2 or not.
            std::unique_ptr<Record> effect = std::make_unique<ActionFFRecord>();
            effect->parse(is, sprites);
            record = std::make_unique<SpriteWrapperRecord>(m_sprite_id, std::move(effect));
        }

        // This adds the sprite to the m_sprites map inside NewGRFData.
        //append_real_sprite(m_sprite_id, record);
        // Sprites with the same ID are stored in map indexed by zoom level. 
        // These maps are stored in a map index by the sprite ID.
        if (sprites.find(m_sprite_id) == sprites.end())
        {
            sprites[m_sprite_id] = SpriteZoomVector{};
        }
        sprites[m_sprite_id].push_back(std::move(record));
    }

    is.match(TokenType::CloseBrace);
}
