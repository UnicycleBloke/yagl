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
#include "Action03Record.h"
#include "StreamHelpers.h"


void Action03Record::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));

    // This is the number of feature instances we are going to associate
    // graphics with. The livery override flag is used for wagons and means
    // that the graphics will be used for those wagons when they are attached
    // to a particular engine. The Action03 for the engine comes just before 
    // this one in that case.
    uint8_t num_ids   = read_uint8(is);
    m_livery_override = (num_ids & 0x80) == 0x80;
    num_ids           = (num_ids & 0x7F);

    // These are the IDs of the feature instances with which to associates the 
    // graphics. All the instances will get the same graphics. Can be zero instances.
    // Each feature type has a set of predefined IDs: you can modify existing 
    // instances or create new instancs with new IDs. 
    m_feature_ids.resize(num_ids);
    for (uint8_t i = 0; i < num_ids; ++i)
    {
        m_feature_ids[i] = read_uint8_ext(is);
    }

    // This only applies to vehicles and stations. These are specific graphics 
    // set IDs for various types of cargo. 
    // NOTE for RailTypes, the cargo IDs are re-purposed to give graphics IDs for
    // the various overlays and other parts.
    uint8_t num_cargo_types = read_uint8(is);
    m_cargo_types.resize(num_cargo_types);
    for (uint8_t i = 0; i < num_cargo_types; ++i)
    {
        m_cargo_types[i].cargo_type   = read_uint8(is);
        m_cargo_types[i].act02_set_id = read_uint16(is);
    }

    // This is the ID of the graphics set which is used unless there 
    m_default_act02_set_id = read_uint16(is);
}


void Action03Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));

    uint8_t num_ids = m_feature_ids.size();
    write_uint8(os, num_ids | (m_livery_override ? 0x80 : 0x00));

    for (const auto& id: m_feature_ids)
    {
        write_uint8_ext(os, id);
    }

    uint8_t num_cargo_types = m_cargo_types.size();
    write_uint8(os, num_cargo_types);
    for (const auto& cargo_type: m_cargo_types)
    {
        write_uint8(os, cargo_type.cargo_type);
        write_uint16(os, cargo_type.act02_set_id);
    }

    write_uint16(os, m_default_act02_set_id);
}  


static const char* LIVERY_OVERRIDE  = "livery_override";
static const char* DEFAULT_SET_ID   = "default_act02_set_id";
static const char* FEATURE_IDS      = "feature_ids";
static const char* CARGO_TYPES      = "cargo_types";
static const char* CARGO_TYPE       = "cargo_type";
static const char* SET_ID           = "act02_set_id";


void Action03Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action03" << '\n';
    os << pad(indent) << "{" << '\n';

    os << pad(indent + 4) << LIVERY_OVERRIDE << ": " << std::boolalpha << m_livery_override << ";\n";
    os << pad(indent + 4);
    os << DEFAULT_SET_ID << ": " << to_hex(m_default_act02_set_id) << ";\n";

    os << pad(indent + 4) << FEATURE_IDS << ": [ ";
    for (const auto& id: m_feature_ids)
    {
        os << to_hex(id);
    }
    os << " ]; // i.e. instances of '" << FeatureName(m_feature) << "'\n";

    // Make this conditional...
    os << pad(indent + 4) << CARGO_TYPES << ":\n";
    os << pad(indent + 4) << "[\n";
    for (const auto& c: m_cargo_types)
    {
        os << pad(indent + 8);
        os << "{ " << CARGO_TYPE << ": " << to_hex(c.cargo_type) << "; "; 
        os << SET_ID << ": " << to_hex(c.act02_set_id) << "; }\n";
    }
    os << pad(indent + 4) << "];\n";

    os << pad(indent) << "}" << '\n';
}


namespace {
const std::map<std::string, uint8_t> g_indices =
{
    { LIVERY_OVERRIDE, 0x00 },
    { DEFAULT_SET_ID,  0x01 },
    { FEATURE_IDS,     0x02 },
    { CARGO_TYPES,     0x03 },
};
} // namespace {


// feature_graphics<Trains> // Action03
// {
//     livery_override: false;
//     default_act02_set_id: 0x00FA;
//     feature_ids: [ 0x0094 ]; // i.e. instances of 'Trains'
//     cargo_types:
//     [
//         { cargo_type: 0xFF; act02_set_id: 0x00F4; }
//     ];
// }


Action03Record::CargoType Action03Record::parse_cargo_type(TokenStream& is)
{
    CargoType type = {};

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        std::string name = is.match(TokenType::Ident);
        is.match(TokenType::Colon);
        if (name == CARGO_TYPE)
        {
            type.cargo_type = is.match_integer();
        }
        else if (name == SET_ID)
        {
            type.act02_set_id = is.match_integer();
        }
        is.match(TokenType::SemiColon);
    }
    is.match(TokenType::CloseBrace);

    return type;
}


void Action03Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::OpenBrace)
    {
        const auto& it = g_indices.find(is.match(TokenType::Ident));
        is.match(TokenType::Colon);
        switch (it->second)
        {
            case 0x00:
                m_livery_override = is.match_bool(); 
                break;

            case 0x01:
                m_default_act02_set_id = is.match_integer();
                break;

            case 0x02:
                is.match(TokenType::OpenBracket);
                while (is.peek().type == TokenType::Number)
                {
                    m_feature_ids.push_back(is.match_integer());
                }
                is.match(TokenType::CloseBracket);                
                break;

            case 03:
                is.match(TokenType::OpenBracket);
                while (is.peek().type != TokenType::CloseBracket)
                {
                    CargoType type = parse_cargo_type(is);
                    m_cargo_types.push_back(type);
                }
                is.match(TokenType::CloseBracket);                
                break;
        }
        is.match(TokenType::SemiColon);
    }

    is.match(TokenType::OpenBrace);
}
