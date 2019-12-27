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


void Action08Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action08\n";
    os << pad(indent) << "{\n";
    os << pad(indent + 4) << "grf_id: \"" << m_grf_id.to_string() << "\";\n";
    os << pad(indent + 4) << "version: "<< (uint16_t)m_grf_version << ";\n";
    os << pad(indent + 4) << "name: \""<< grf_string_to_readable_utf8(m_name) << "\";\n";
    os << pad(indent + 4) << "description: \"" << grf_string_to_readable_utf8(m_info) << "\";\n";
    os << pad(indent) << "}\n";
}


void Action08Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenBrace);

    // TODO Could theoretically set the same members multiple times, but only the last will count.
    // Could maintain a bit field to check for this happening.
    while (is.peek().type == TokenType::Ident)
    {
        const TokenValue& ident = is.peek();
        is.match(TokenType::Ident);
        is.match(TokenType::Colon);
        
        // TODO maybe convert this to a map or use lambdas or something to make a little more 
        // data driven than procedural.
        if (ident.value == "grf_id")
        {
            m_grf_id.parse(is);
        }
        else if (ident.value == "version")
        {
            m_grf_version = static_cast<GRFVersion>(is.match_integer());
        }
        else if (ident.value == "name")
        {
            m_name = is.match(TokenType::String);
        }
        else if (ident.value == "description")
        {
            m_info = is.match(TokenType::String);
        }
        else
        {
            throw ParserError("Unexpected identifier: " + ident.value, ident);
        }        
    }

    is.match(TokenType::CloseBrace);
}
