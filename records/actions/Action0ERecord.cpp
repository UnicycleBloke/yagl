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
#include "Action0ERecord.h"
#include "StreamHelpers.h"


void Action0ERecord::read(std::istream& is, const GRFInfo& info)
{
    m_grf_ids.read(is);
}


void Action0ERecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);
    m_grf_ids.write(os);
}  


// disable_grfs
// {
//     grf_ids: [ "XXXX" "YYYY" ];
// }


namespace {


constexpr const char* str_grf_ids = "grf_ids";


constexpr GRFLabelListDescriptor grf_desc = { 0x00, str_grf_ids };


} // namespace {


void Action0ERecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0E\n";
    os << pad(indent) << "{\n";

    grf_desc.print(m_grf_ids, os, indent + 4);
    
    os << pad(indent) << "}\n";
}


void Action0ERecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenBrace);
    is.match_ident(str_grf_ids);
    is.match(TokenType::Colon);
    m_grf_ids.parse(is);
    is.match(TokenType::SemiColon);
    is.match(TokenType::CloseBrace);
}
