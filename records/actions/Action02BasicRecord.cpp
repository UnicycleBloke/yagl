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
#include "Action02BasicRecord.h"
#include "StreamHelpers.h"


void Action02BasicRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature             = static_cast<FeatureType>(read_uint8(is));
    m_act02_set_id        = read_uint8(is);
    uint8_t num_entries_1 = read_uint8(is);
    uint8_t num_entries_2 = read_uint8(is);

    for (uint8_t i = 0; i < num_entries_1; ++i)
    {
        m_act01_set_ids_1.push_back(read_uint16(is));
    }

    for (uint8_t i = 0; i < num_entries_2; ++i)
    {
        m_act01_set_ids_2.push_back(read_uint16(is));
    }
}


void Action02BasicRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_act02_set_id);
    write_uint8(os, m_act01_set_ids_1.size());
    write_uint8(os, m_act01_set_ids_2.size());

    for (const auto& set_id: m_act01_set_ids_1)
    {
        write_uint16(os, set_id);
    }

    for (const auto& set_id: m_act01_set_ids_2)
    {
        write_uint16(os, set_id);
    }
}  


void Action02BasicRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action02 basic" << '\n';
    os << pad(indent) << "{" << '\n';
    os << pad(indent + 4) << "this_set_id: " << to_hex(m_act02_set_id) << ";\n";

    os << pad(indent + 4) << "primary_spritesets: [ ";
    for (uint8_t i = 0; i < m_act01_set_ids_1.size(); ++i)
    {
        os << to_hex(m_act01_set_ids_1[i]) << " ";
    }
    os << "];\n";

    os << pad(indent + 4) << "secondary_spritesets: [ ";
    for (uint8_t i = 0; i < m_act01_set_ids_2.size(); ++i)
    {
        os << to_hex(m_act01_set_ids_2[i]) << " ";
    }
    os << "];\n";

    os << pad(indent) << "}" << '\n';
}


// spritegroup<Trains> // Action02 basic
// {
//     this_set_id: 0xF4;
//     primary_spritesets: [ 0x0000 ];
//     secondary_spritesets: [ 0x0000 ];
// }


void Action02BasicRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));    
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);

    is.match_ident("this_set_id");
    is.match(TokenType::Colon);
    m_act02_set_id = is.match_integer();
    is.match(TokenType::SemiColon);

    is.match_ident("this_set_id");
    is.match(TokenType::Colon);
    is.match(TokenType::OpenBracket);
    while (is.peek().type == TokenType::Number)
    {
        m_act01_set_ids_1.push_back(is.match_integer());
    }
    is.match(TokenType::CloseBracket);
    is.match(TokenType::SemiColon);

    is.match_ident("this_set_id");
    is.match(TokenType::Colon);
    is.match(TokenType::OpenBracket);
    while (is.peek().type == TokenType::Number)
    {
        m_act01_set_ids_2.push_back(is.match_integer());
    }
    is.match(TokenType::CloseBracket);
    is.match(TokenType::SemiColon);

    is.match(TokenType::CloseBrace);
}
