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
#include "IntegerDescriptor.h"
#include "BooleanDescriptor.h"


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

    uint8_t num_ids = uint8_t(m_feature_ids.size());
    write_uint8(os, num_ids | (m_livery_override ? 0x80 : 0x00));

    for (auto id: m_feature_ids)
    {
        // In OpenTTD since r13482, each ID is an extended byte for vehicles,
        // otherwise the ID is a regular byte. Use short format where possible.
        if (feature_is_vehicle(m_feature))
            write_uint8_ext(os, id, ExtByteFormat::Short);
        else
            write_uint8(os, static_cast<uint8_t>(id));
    }

    uint8_t num_cargo_types = uint8_t(m_cargo_types.size());
    write_uint8(os, num_cargo_types);
    for (const auto& cargo_type: m_cargo_types)
    {
        write_uint8(os, cargo_type.cargo_type);
        write_uint16(os, cargo_type.act02_set_id);
    }

    write_uint16(os, m_default_act02_set_id);
}


namespace {


constexpr const char* str_livery_override = "livery_override";
constexpr const char* str_default_set_id  = "default_set_id";
constexpr const char* str_feature_ids     = "feature_ids";
constexpr const char* str_cargo_types     = "cargo_types";


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices =
{
    { str_livery_override, 0x01 },
    { str_default_set_id,  0x02 },
    { str_feature_ids,     0x03 },
    { str_cargo_types,     0x04 },
};


const RawBoolDescriptor                desc_livery_override { 0x01, str_livery_override };
const IntegerDescriptorT<uint16_t>     desc_default_set_id  { 0x02, str_default_set_id, UIntFormat::Hex };
const IntegerListDescriptorT<uint16_t> desc_feature_ids     { 0x03, str_feature_ids, UIntFormat::Hex };


} // namespace {


void Action03Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action03" << '\n';
    os << pad(indent) << "{" << '\n';

    desc_livery_override.print(m_livery_override, os, indent + 4);
    desc_default_set_id.print(m_default_act02_set_id, os, indent + 4);
    desc_feature_ids.print(m_feature_ids, os, indent + 4);

    os << pad(indent + 4) << str_cargo_types << ":\n";
    os << pad(indent + 4) << "{\n";
    os << pad(indent + 8) << "// <cargo_type>: <cargo_id>;\n";
    for (const auto& c: m_cargo_types)
    {
        os << pad(indent + 8) << to_hex(c.cargo_type) << ": " << to_hex(c.act02_set_id) << ";\n";
    }
    os << pad(indent + 4) << "};\n";

    os << pad(indent) << "}" << '\n';
}


void Action03Record::parse_cargo_types(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        CargoType c;
        c.cargo_type = is.match_uint8();
        is.match(TokenType::Colon);
        c.act02_set_id = is.match_uint16();
        is.match(TokenType::SemiColon);

        m_cargo_types.push_back(c);
    }

    is.match(TokenType::CloseBrace);
}


void Action03Record::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices.find(token.value);
        if (it != g_indices.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x01: desc_livery_override.parse(m_livery_override, is); break;
                case 0x02: desc_default_set_id.parse(m_default_act02_set_id, is); break;
                case 0x03: desc_feature_ids.parse(m_feature_ids, is); break;
                case 0x04: parse_cargo_types(is); break;
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
