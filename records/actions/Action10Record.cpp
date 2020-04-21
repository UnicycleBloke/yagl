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
#include "Action10Record.h"
#include "StreamHelpers.h"


void Action10Record::read(std::istream& is, const GRFInfo& info)
{
    m_label = read_uint8(is);
    m_comment.read(is, StringTerm::None);
}


void Action10Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_label);
    // Write an unterminated string. The record size implies the termination.
    m_comment.write(os, StringTerm::None);
}  


void Action10Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << RecordName(record_type()) << "<" << to_hex(m_label) << "> // Action10 - target for Action07 or Action09\n";
    os << "{\n";

    // TODO This is probably not a valid thing to do. Should more likely
    // print nothing, or convert to hex. 
    if (m_comment.length() > 1)
    {
        os << pad(indent + 4) << "\"" << m_comment.readable() << "\";\n";
    }
    
    os << "}\n";
}


void Action10Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_label = is.match_uint8();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);

    if (is.peek().type == TokenType::String)
    {
        m_comment.parse(is);
        is.match(TokenType::SemiColon);
    }

    is.match(TokenType::CloseBrace);
}
