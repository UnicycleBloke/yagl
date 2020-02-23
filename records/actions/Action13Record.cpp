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
#include "Action13Record.h"
#include "StreamHelpers.h"


void Action13Record::read(std::istream& is, const GRFInfo& info)
{
    m_grf_id.read(is);

    // Added for GRF8 
    m_language = 0x7F;
    if (info.version == GRFVersion::GRF8)
    {
        m_language = read_uint8(is);
    }

    uint8_t num_strings = read_uint8(is);
    m_first_string_id   = read_uint16(is);

    for (uint8_t i = 0; i < num_strings; ++i)
    {
        GRFString str;
        str.read(is);
        m_strings.push_back(str);
    }
}


void Action13Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    m_grf_id.write(os);

    // Added for GRF8
    if (info.version == GRFVersion::GRF8)
    {
        write_uint8(os, m_language);
    }

    write_uint8(os, uint8_t(m_strings.size()));
    write_uint16(os, m_first_string_id);

    for (const auto& str: m_strings)
    {
        str.write(os);
    }
}  


// grf_strings<"GRFX", en_US, 0xC504> // <grf_id, language, first_id> Action13, English (US)
// {
//     "Roofs"; // 0xC504
//     "Platform"; // 0xC505
//     "Benches"; // 0xC506
//     "Parking lot (front)"; // 0xC507
//     "Parking lot (back)"; // 0xC508
//     "Flat roofs"; // 0xC509
//     "Glass roofs"; // 0xC50A
//     "Overpass"; // 0xC50B
//     "Station Hall (small)"; // 0xC50C
//     "Station Hall (large)"; // 0xC50D
//     "Underpass"; // 0xC50E
//     "empty"; // 0xC50F
//     "void"; // 0xC510
// }


void Action13Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<\"" << m_grf_id.to_string() << "\", ";
    os << language_iso(m_language) << ", ";
    os << to_hex(m_first_string_id) << "> // <grf_id, language, first_id> Action13, " << language_name(m_language) << "\n";
    os << pad(indent) << "{\n";

    uint16_t string_id = m_first_string_id;
    for (const auto& str: m_strings)
    {
        os << pad(indent + 4) << "\"" << str.readable() << "\"; //" << to_hex(string_id++) << "\n";
    }

    os << pad(indent) << "}\n";

}


void Action13Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_grf_id.parse(is);
    is.match(TokenType::Comma);
    m_language = language_id(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_first_string_id = is.match_uint16();
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
