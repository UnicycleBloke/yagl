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
#include "Action0CRecord.h"
#include "StreamHelpers.h"


void Action0CRecord::read(std::istream& is, const GRFInfo& info)
{
    // This could be a text comment, or binary data (a commented pseudo-sprite).
    // UI needs to reflect this.
    m_comment.read(is, StringTerm::None);
}


void Action0CRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

     // Write an unterminated string. The record size implies the termination.
    m_comment.write(os, StringTerm::None);
}


// ignore // Action0C
// {
//     "m4nfo header r119 31.05.2019";
// }


void Action0CRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << RecordName(record_type()) << " // Action0C\n";
    os << "{\n";

    // TODO This is probably not a valid thing to do. Should more likely
    // print nothing, or convert to hex.
    os << pad(indent + 4) << "\"" << m_comment.readable() << "\";\n";

    os << "}\n";
}


void Action0CRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenBrace);
    m_comment.parse(is);
    is.match(TokenType::SemiColon);
    is.match(TokenType::CloseBrace);
}
