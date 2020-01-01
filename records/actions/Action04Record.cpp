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
#include "Action04Record.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"


void Action04Record::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));

    // The interpretation of the language ID depends on the GRF version.
    // Version <= 6 is a bit mask, version >= 7 is a value from a list.
    m_language   = read_uint8(is);
    m_uint16_ids = (m_language & 0x80) != 0x00;
    m_language   = m_language & 0x7F;

    uint8_t num_strings = read_uint8(is);

    // Read the ID of the first string here. The format is bizarrely 
    // conditional. 
    if (m_uint16_ids)
    {
        m_first_string_id = read_uint16(is);
    }
    else if (static_cast<uint8_t>(m_feature) <= 3) // Is a vehicle (road, rail, ship, air)
    {
        m_first_string_id = read_uint8_ext(is);
    }
    else
    {
        m_first_string_id = read_uint8(is);
    }

    // Finally read the strings.
    for (uint8_t i = 0; i < num_strings; ++i)
    {
        std::string str = read_string(is);
        m_strings.push_back(str);
    }
}


void Action04Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_language | (m_uint16_ids ? 0x80 : 0x00));

    uint8_t num_strings = m_strings.size();
    write_uint8(os, num_strings);

    if (m_uint16_ids)
    {
        write_uint16(os, m_first_string_id);
    }
    else if (static_cast<uint8_t>(m_feature) <= 3) // Is a vehicle
    {
        write_uint8_ext(os, m_first_string_id);
    }
    else
    {
        write_uint8(os, m_first_string_id);
    }

    for (const auto& str: m_strings)
    {
        write_string(os, str);
    }
}  


// strings<Stations, en_US> // Action04, English (US)
// {
//     // Only the first ID is meaningful - others included as an aid.     
//     0xC504: "Roofs";
//     0xC505: "Platform";
//     0xC506: "Benches";
//     0xC507: "Parking lot (front)";
//     0xC508: "Parking lot (back)";
//     0xC509: "Flat roofs";
//     0xC50A: "Glass roofs";
//     0xC50B: "Overpass";
//     0xC50C: "Station Hall (small)";
//     0xC50D: "Station Hall (large)";
//     0xC50E: "Underpass";
//     0xC50F: "empty";
//     0xC510: "void";
// }


void Action04Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << ", ";
    os << language_iso(m_language) << "> // Action04, " << language_name(m_language) << "\n";
    os << pad(indent) << "{\n";

    if (m_strings.size() > 1)
    {
        os << pad(indent + 4) << "// Only the first ID is meaningful - others included as an aid.\n";     
    }

    uint16_t string_id = m_first_string_id;
    for (const auto& s: m_strings)
    {
        os << pad(indent + 4) << to_hex(string_id++) << ": \"" << grf_string_to_readable_utf8(s) << "\";\n";
    }

    os << pad(indent) << "}\n";
}


void Action04Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_language = language_id(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);

    bool first = true;
    while (is.peek().type != TokenType::CloseBrace)
    {
        if (first)
        {
            m_first_string_id = is.match_integer();
        }
        else
        {
            is.match(TokenType::Number);
        }
    
        is.match(TokenType::Colon);
        m_strings.push_back(is.match(TokenType::String));
        is.match(TokenType::SemiColon);
    }

    is.match(TokenType::CloseBrace);
}
