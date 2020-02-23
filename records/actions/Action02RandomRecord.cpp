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
#include "Action02RandomRecord.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


void Action02RandomRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));
    m_set_id  = read_uint8(is);
    
    m_type = static_cast<RandomType>(read_uint8(is));
    if (m_type == RandomType::Consist)
    {
        // For type 84, the count byte specifies which vehicle's random bits this vehicle 
        // will be using and/or modifying.
        // - The low nibble (bits 0-3) specifies how far to count from the starting vehicle. 
        //   If it is zero, the count will be read from GRF register 100h instead.
        // - The high nibble (bits 6-7, actually) specifies which vehicle is the starting 
        //   vehicle, and which direction to count.     
        m_count   = read_uint8(is);
        m_method  = static_cast<ConsistType>(m_count & 0xC0);
        m_count  &= 0x0F;
    }
    
    m_triggers = read_uint8(is);
    m_randbit  = read_uint8(is);

    // This must be a power of two.
    uint8_t nrand = read_uint8(is);
    for (uint8_t i = 0; i < nrand; ++i)
    {
        uint16_t set_id = read_uint16(is);
        if (m_set_ids.find(set_id) == m_set_ids.end())
        {
            m_set_ids[set_id] = 1;
        }
        else
        {
            m_set_ids[set_id] = m_set_ids[set_id] + 1;
        }        
    }
}


void Action02RandomRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_set_id);
    
    write_uint8(os, static_cast<uint8_t>(m_type));
    if (m_type == RandomType::Consist)
    {
        write_uint8(os, m_count | static_cast<uint8_t>(m_method));
    }

    write_uint8(os, m_triggers);
    write_uint8(os, m_randbit);

    // Total number of set IDs.
    uint16_t nrand = 0;
    for (const auto it: m_set_ids)
    {
        nrand += it.second;
    }
    write_uint8(os, uint8_t(nrand));
 
    for (const auto it: m_set_ids)
    {
        for (uint16_t i = 0; i < it.second; ++i)
        {
            write_uint16(os, it.first);
        }
    }
}  


// random_switch<Trains, 0xFD> // Action02 random
// {
//     random_type: Object;
//     triggers: 0x00;
//     rand_bit: 0x00;
//     set_ids:
//     [
//         0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 
//         0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00FA 0x00FA 
//     ];
// }


namespace {


constexpr const char* str_random_type  = "random_type";
constexpr const char* str_consist_type = "consist_type";
constexpr const char* str_triggers     = "triggers";
constexpr const char* str_rand_bit     = "rand_bit";
constexpr const char* str_set_ids      = "set_ids";


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices =
{
    //{ str_random_type,  0x00 },
    //{ str_consist_type, 0x01 },
    { str_triggers,     0x02 },
    { str_rand_bit,     0x03 },
    { str_set_ids,      0x04 },
};


const EnumDescriptorT<Action02RandomRecord::RandomType> random_desc = 
{ 
    0x00, str_random_type,                   
    {
        { 0x80, "Object" },  // RandomType::Object },    
        { 0x83, "Related" }, // RandomType::Related }, 
        { 0x84, "Consist" }, // RandomType::Consist }, 
    }
};


const EnumDescriptorT<Action02RandomRecord::ConsistType> consist_desc = 
{ 
    0x01, str_consist_type,                   
    {
        { 0x00, "BackwardFromVehicle" }, // ConsistType::BackwardFromVehicle: 
        { 0x40, "ForwardFromVehicle" },  // ConsistType::ForwardFromVehicle: 
        { 0x80, "BackwardFromEngine" },  // ConsistType::BackwardFromEngine: 
        { 0xC0, "BackwardFromSameID" },  // ConsistType::BackwardFromSameID: 
    }
};

const IntegerDescriptorT<uint8_t> triggers_desc{ 0x02, str_triggers, PropFormat::Hex };
const IntegerDescriptorT<uint8_t> randbit_desc { 0x03, str_rand_bit, PropFormat::Hex };


} // namespace {


void Action02RandomRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_set_id);
    os << ", " << random_desc.value(m_type);

    if (m_type == RandomType::Consist)
    {
        os << ", " << consist_desc.value(m_method);
        os << "[" << to_hex(m_count) << "]";
    }

    os << "> // Action02 random\n";
    os << pad(indent) << "{\n";

    triggers_desc.print(m_triggers, os, indent + 4); 
    randbit_desc.print(m_randbit, os, indent + 4); 

    // TODO create descripter for this?
    os << pad(indent + 4) << str_set_ids << ": // set_id: probability;\n";
    os << pad(indent + 4) << "{\n";
    for (const auto& it: m_set_ids)
    {
        os << pad(indent + 8) << to_hex(it.first) << ": " << it.second << ";\n";
    }
    os << pad(indent + 4) << "};\n";

    os << pad(indent) << "}" << '\n';
}


void Action02RandomRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));    
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_set_id = is.match_uint8();
    is.match(TokenType::Comma);
    random_desc.parse(m_type, is);

    if (m_type == RandomType::Consist)
    {
        consist_desc.parse(m_method, is);
        is.match(TokenType::OpenBracket);
        m_count = is.match_uint8();
        is.match(TokenType::CloseBracket);
    }

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
                case 0x02: triggers_desc.parse(m_triggers, is); break;
                case 0x03: randbit_desc.parse(m_randbit, is); break;
                case 0x04:
                    is.match(TokenType::OpenBrace);
                    while (is.peek().type != TokenType::CloseBrace)
                    {
                        uint16_t set_id = is.match_uint16();
                        is.match(TokenType::Colon);
                        uint16_t count  = is.match_uint16();
                        is.match(TokenType::SemiColon);
                        m_set_ids[set_id] = count;
                    }
                    is.match(TokenType::CloseBrace);
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
