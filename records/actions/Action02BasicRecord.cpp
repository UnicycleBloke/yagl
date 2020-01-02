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
#include "Descriptors.h"


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


// sprite_groups<Stations, 0xFF> // Action02 basic
// {
//     primary_spritesets: [ 0x0000 ];
//     secondary_spritesets: [ 0x0000 ];
// }


namespace {


constexpr const char* str_primary_spritesets = "primary_spritesets"; 
constexpr const char* str_secondary_spritesets = "secondary_spritesets"; 


const IntegerListDescriptorT<uint16_t> primary_desc  {0x00, str_primary_spritesets,   PropFormat::Hex };
const IntegerListDescriptorT<uint16_t> secondary_desc{0x00, str_secondary_spritesets, PropFormat::Hex };


} // namespace {


void Action02BasicRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_act02_set_id);
    os << "> // Action02 basic\n";
    os << pad(indent) << "{\n";

    if (m_act01_set_ids_1.size() > 0)
    {
        primary_desc.print(m_act01_set_ids_1, os, indent + 4);
    }

    if (m_act01_set_ids_2.size() > 0)
    {
        secondary_desc.print(m_act01_set_ids_2, os, indent + 4);
    }

    os << pad(indent) << "}\n";
}


// TODO convert this to using Descriptors...
void Action02BasicRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));    
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_act02_set_id = is.match_integer();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    
    while (is.peek().type != TokenType::CloseBrace)
    {
        std::string name = is.match(TokenType::String);
        if (name == str_primary_spritesets)
        {
            primary_desc.parse(m_act01_set_ids_1, is);
        }
        if (name == str_primary_spritesets)
        {
            secondary_desc.parse(m_act01_set_ids_2, is);
        }
    }

    is.match(TokenType::CloseBrace);
}
