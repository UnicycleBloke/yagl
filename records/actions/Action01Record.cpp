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
#include "StreamHelpers.h"
#include "Descriptors.h"


void Action01Record::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));
    
    // The record has a basic and an extended format. This is distinguished
    // by whether the number of sets in the basic format is zero. 
    m_num_sets = read_uint8(is);
    bool has_first_id = (m_num_sets == 0);

    if (has_first_id)
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

    write_uint8(os, static_cast<uint8_t>(m_feature));

    bool has_first_id = (m_first_set > 0) || (m_num_sets > 255);
    if (has_first_id)
    {
        write_uint8(os, 0x00);
        write_uint8_ext(os, m_first_set);
        write_uint8_ext(os, m_num_sets);
        write_uint8_ext(os, m_num_sprites);
    }
    else
    {
        write_uint8(os, uint8_t(m_num_sets));
        write_uint8_ext(os, m_num_sprites);
    }
}  


static constexpr const char* str_sprite_set = "sprite_set";


void Action01Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<";
    os << FeatureName(m_feature) << ", ";
    os << to_hex(m_first_set) << "> // <feature, first_set> Action01" << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t index = 0;    
    for (uint16_t set = 0; set < m_num_sets; ++set)
    {
        os << pad(indent + 4) << str_sprite_set << " // " << to_hex<uint16_t>(m_first_set + set) << "\n";
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


void Action01Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_first_set = is.match_uint16();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        // Count the sprites in this sprite set.
        uint16_t num_sprites = 0;

        is.match_ident(str_sprite_set);
        is.match(TokenType::OpenBrace);
        while (is.peek().type != TokenType::CloseBrace)
        {
            parse_sprite(is);
            ++num_sprites;
        }

        is.match(TokenType::CloseBrace);

        // Each set of sprites contains one or more sprites.
        // All the sets are the same size, so we only count
        // for the first one.
        ++m_num_sets;
        if (m_num_sets == 1)
        {
            m_num_sprites = num_sprites;
        }
        else
        {
            // Each set needs to be the same size.
            if (num_sprites != m_num_sprites)
            {
                std::ostringstream os;
                os << "The number of sprites in each spriteset should be the same for Action01. ";
                os << "Expected " << m_num_sprites << ", found " << num_sprites << ".";
                throw PARSER_ERROR(os.str(), is.peek());
            }
        }
    }

    is.match(TokenType::CloseBrace);
}
