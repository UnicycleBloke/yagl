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
#include "CommandLineOptions.h"


void SpriteIndexRecord::read(std::istream& is, const GRFInfo& info)
{
    m_sprite_id = read_uint32(is);
}


void SpriteIndexRecord::write(std::ostream& os, const GRFInfo& info) const
{
    write_uint32(os, m_sprite_id);
}  


void SpriteIndexRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    // TODO this needs to be changed to split the indices as used in 
    // sprite sets and so on, from the actual list of sprites for each 
    // index (different zoom-levels and 'compression').

    os << pad(indent) << "sprite_id: " << to_hex(m_sprite_id, true) << "\n";
    os << pad(indent) << "{" << '\n';

    if (sprites.find(m_sprite_id) != sprites.end())
    {
        const SpriteZoomVector& sprite_list = sprites.at(m_sprite_id);
        for (auto sprite: sprite_list)
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


void SpriteIndexRecord::parse(TokenStream& is)
{
    // sprite_id: 0x00000679
    // {
    //     // Different zoom levels and colour depths.    
    //     [8, 21, -3, -9] normal M "base_name.normal.0.png" [168, 778];
    //     [8, 21, -3, -9] normal M "base_name.normal.0.png" [168, 778];
    //     [8, 21, -3, -9] normal M "base_name.normal.0.png" [168, 778];
    //     Could be recolour sprites, too.
    // }

    is.match_ident("sprite_id");
    is.match(TokenType::Colon);

    m_sprite_id = is.match_integer();
    is.match(TokenType::OpenBrace);

    // TODO these need to be passed back to the main structure somehow.
    std::vector<std::shared_ptr<Record>> sprite_list;
    while (is.peek().type != TokenType::CloseBrace)
    {
        if (is.peek().type == TokenType::OpenBracket)
        {
            // Need to create RealSpriteRecord     
            // Size and compression are place holder values to be read from the token stream.
            auto sprite = std::make_shared<RealSpriteRecord>(m_sprite_id, 0, 0);
            sprite_list.push_back(sprite);
            sprite->parse(is);
        }
        else
        {
            auto sprite = std::make_shared<RecolourRecord>();
            sprite_list.push_back(sprite);
            sprite->parse(is);
        }
    }

    is.match(TokenType::CloseBrace);
}
