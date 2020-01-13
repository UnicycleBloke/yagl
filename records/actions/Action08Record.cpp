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
#include "Action08Record.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"
#include "Descriptors.h"


void Action08Record::read(std::istream& is, const GRFInfo& info)
{
    // This is the version of the GRF specification we are using.
    m_grf_version = static_cast<GRFVersion>(read_uint8(is));
    
    // Unique GRF ID. Can contain a version, or place a version in the Action14 record.
    m_grf_id.read(is);
        
    m_name = read_string(is);
    m_info = read_string(is);
}


void Action08Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_grf_version));
    m_grf_id.write(os);
    write_string(os, m_name);
    write_string(os, m_info);
}  


namespace {


constexpr const char* str_grf_id      = "grf_id";
constexpr const char* str_version     = "version";
constexpr const char* str_name        = "name";
constexpr const char* str_description = "description";


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices =
{
    { str_grf_id,      0x00 },
    { str_version,     0x01 },
    { str_name,        0x02 },
    { str_description, 0x03 },
};


constexpr GRFLabelDescriptor          desc_grf_id      = { 0x00, str_grf_id };
const     EnumDescriptorT<GRFVersion> desc_version     = { 0x01, str_version,
    { { 6, "GRF6" }, { 7, "GRF7" }, { 8, "GRF8" } } };
constexpr StringDescriptor            desc_name        = { 0x02, str_name };
constexpr StringDescriptor            desc_description = { 0x03, str_description };


} // namespace {


// grf // Action08
// {
//     grf_id: "\xFB\xFB\x06\x01";
//     version: 8;
//     name: "Dutch Trainset 2.1.0";
//     description: "{lt-gray}Dutch Trains for OpenTTD {new-line}{black}First vehicle: 1839.{new-line}{new-line}(c)Dutch Trainset Team {new-line}License: GPLv2 or higher. {new-line}See readme for details.";
// }


void Action08Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action08\n";
    os << pad(indent) << "{\n";

    desc_grf_id.print(m_grf_id, os, indent + 4); 
    desc_version.print(m_grf_version, os, indent + 4);
    desc_name.print(m_name, os, indent + 4);
    desc_description.print(m_info, os, indent + 4);

    os << pad(indent) << "}\n";
}


void Action08Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));

    // Could theoretically set the same members multiple times, but only the last will count.
    // Could maintain a bit field to check for this happening.
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
                case 0x00: desc_grf_id.parse(m_grf_id, is); break;
                case 0x01: desc_version.parse(m_grf_version, is); break;
                case 0x02: desc_name.parse(m_name, is); break;
                case 0x03: desc_description.parse(m_info, is); break;
            }

            is.match(TokenType::SemiColon);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: " + token.value, token);
        }
    }

    is.match(TokenType::CloseBrace);
}
