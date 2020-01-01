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
        std::string str = read_string(is);
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

    write_uint8(os,  m_strings.size());
    write_uint16(os, m_first_string_id);

    for (const auto& str: m_strings)
    {
        write_string(os, str);
    }
}  

// grf_strings<"GRFX", en_US> // Action04, English (US)
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


void Action13Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<\"" << m_grf_id.to_string() << "\", ";
    os << language_iso(m_language) << "> // Action13, " << language_name(m_language) <<  '\n';
    os << pad(indent) << "{" << '\n';

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


void Action13Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action13Record::parse not implemented");
}
