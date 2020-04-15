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

    // Read the ID of the first string here. The format is bizarrely conditional. From the specs:
    // - When language - id bit 7 is clear, this is a byte value; For OpenTTD since r13482, where it is an extended byte 
    //   value for vehicles.
    // - When language - id bit 7 is set, this is a word value in little endian notation, e.g. 8134 becomes 34 81.
    // - The 8 bit version is only allowed for vehicles to set their name, in which case the text ID is just the vehicle ID.
    // - To replace original texts, or to define texts for usage in callbacks or properties of vehicles, stations, houses or 
    //   industries you have to use the 16 bit version.
    if (m_uint16_ids)
    {
        m_first_string_id = read_uint16(is);
    }
    else if (feature_is_vehicle(m_feature)) // Is a vehicle (road, rail, ship, air)
    {
        m_first_string_id = read_uint8_ext(is);
    }
    else
    {
        // "only allowed for vehicles to set their name" - but we have already dealt with vehicles above.
        m_first_string_id = read_uint8(is);
    }

    // Finally read the strings.
    for (uint8_t i = 0; i < num_strings; ++i)
    {
        GRFString str;
        str.read(is);
        m_strings.push_back(str);
    }
}


void Action04Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_language | (m_uint16_ids ? 0x80 : 0x00));

    uint8_t num_strings = uint8_t(m_strings.size());
    write_uint8(os, num_strings);

    // Force word IDs
    if (m_uint16_ids)
    {
        write_uint16(os, m_first_string_id);
    }
    // OR vehicles have extended byte IDs
    else if ((m_first_string_id > 0xFF) || feature_is_vehicle(m_feature))
    {
        write_uint8_ext(os, m_first_string_id);
    }
    // OR we just have byte IDs.
    else
    {
        // "only allowed for vehicles to set their name" - but we have already dealt with vehicles above.
        write_uint8(os, uint8_t(m_first_string_id));
    }

    for (const auto& str: m_strings)
    {
        str.write(os);
    }
}  


void Action04Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << ", ";
    os << language_iso(m_language) << ", ";
    // Come up with something better than this. The * is used to disambiguate whether the original
    // GRF used 16-bit IDs. Or we could ignore that and just go off the actual value of the ID.
    os << to_hex(m_first_string_id) << (m_uint16_ids ? "*" : "");
    os << "> // <feature, language, first_id> Action04, " << language_name(m_language) << "\n";
    os << pad(indent) << "{\n";

    uint16_t string_id = m_first_string_id;
    for (const auto& s: m_strings)
    {
        os << pad(indent + 4) << "/* " << to_hex(string_id++) << " */ "; 
        os << "\"" << s.readable() << "\";\n";
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
    is.match(TokenType::Comma);
    m_first_string_id = is.match_uint16();

    m_uint16_ids = false;
    if (is.peek().type == TokenType::OpMultiply)
    {
        is.match(TokenType::OpMultiply);
        m_uint16_ids = true;
    }

    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        GRFString str;
        str.parse(is);
        m_strings.push_back(str);
        is.match(TokenType::SemiColon);
    }

    is.match(TokenType::CloseBrace);
}
