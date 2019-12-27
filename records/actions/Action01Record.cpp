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
#include "Action01Record.h"
#include "SpriteIndexRecord.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


void Action01Record::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));
    
    // The record has a basic and an extended format. This is distinguished
    // by whether the number of sets in the basic format is zero. 
    m_num_sets = read_uint8(is);
    m_has_first_id = (m_num_sets == 0);

    if (m_has_first_id)
    {
        // Extended format
        m_first_set    = read_uint8_ext(is);
        m_num_sets     = read_uint8_ext(is);
        m_num_sprites  = read_uint8_ext(is);
    }
    else
    {
        // Basic format
        m_first_set    = 0;
        m_num_sprites  = read_uint8_ext(is);
    }
}


void Action01Record::write(std::ostream& os, const GRFInfo& info) const
{
    ContainerRecord::write(os, info);

    // TODO get the number of sprites from the contained records.
    write_uint8(os, static_cast<uint8_t>(m_feature));
    if (m_has_first_id)
    {
        write_uint8(os, 0x00);
        write_uint8_ext(os, m_first_set);
        write_uint8_ext(os, m_num_sets);
        write_uint8_ext(os, m_num_sprites);
    }
    else
    {
        write_uint8(os, m_num_sets);
        write_uint8_ext(os, m_num_sprites);
    }
}  


void Action01Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    // TODO reorganise the sprites into blocks with the same zoom level.
    // TODO what about missing sprites in that case? What is a null sprite
    //      in NML? What does it output?

    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action01" << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t index = 0;    
    for (uint16_t set = 0; set < m_num_sets; ++set)
    {
        os << pad(indent + 4) << "set_id: " << to_hex(m_first_set + set, true) << "\n";
        os << pad(indent + 4) << "{" << '\n';
        
        for (uint16_t i = 0; i < m_num_sprites; ++i)
        {
            // This is a SpriteIndexRecord or a RecolourRecord
            print_sprite(index, os, sprites, indent + 8);
            ++index;
        }

        os << pad(indent + 4) << "}" << '\n';
    }

    os << pad(indent) << "}" << '\n';
}


// sprite_sets<Trains> // Action01
// {
//     set_id: 0x00000000
//     {
//         sprite_id: 0x00003D3D
//         {
//             [8, 24, -3, -12] normal palette "base_name.normal.1.png" [299, 2490];
//         }
//         sprite_id: 0x00003D3E
//         {
//             [22, 18, -14, -10] normal palette "base_name.normal.1.png" [317, 2490];
//         }
//     }
//     set_id: 0x00000001
//     {
//         sprite_id: 0x00003D3D
//         {
//             [8, 24, -3, -12] normal palette "base_name.normal.1.png" [299, 2490];
//         }
//         sprite_id: 0x00003D3E
//         {
//             [22, 18, -14, -10] normal palette "base_name.normal.1.png" [317, 2490];
//         }
//     }
// }


void Action01Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type == TokenType::Ident)
    {
        // The record represents one or more sets of sprites.
        ++m_num_sets;
        is.match_ident("set_id");
        is.match(TokenType::Colon);

        if (m_num_sets == 1)
        {
            // We don't care about this value. It was added to be helpful 
            // in the output.
            is.match_integer();
        }
        else
        {
            m_first_set    = is.match_integer();
            m_has_first_id = (m_first_set > 0); 
        }

        is.match(TokenType::OpenBrace);
        while (is.peek().type == TokenType::Ident)
        {
            // Each set of sprites contains one or more sprites.
            // All the sets are the same size, so we only count
            // for the first one.
            if (m_num_sets == 1)
            {
                ++m_num_sprites;
            }

            // The sprite itself contains one or more different version of the 
            // image, in different zooms and/or colour depths. 
            auto sprite = std::make_shared<SpriteIndexRecord>();
            append_sprite(sprite);
            sprite->parse(is);
        }

        is.match(TokenType::CloseBrace);
    }

    is.match(TokenType::CloseBrace);
}
