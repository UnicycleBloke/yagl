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
#include "ActionFERecord.h"
#include "StreamHelpers.h"


void ActionFERecord::read(std::istream& is, const GRFInfo& info)
{
    m_import_code = read_uint8(is);
    m_grf_id.read(is);
    m_sound_index = read_uint16(is);
}


void ActionFERecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_import_code);
    m_grf_id.write(os);
    write_uint16(os, m_sound_index);
}  


void ActionFERecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    // This need only distinguish itself from an ActionFF record.
    os << pad(indent) << "import(\"" << m_grf_id.to_string() << "\", ";
    os << to_hex(m_sound_index) << ");\n";
}


void ActionFERecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("ActionFERecord::parse not implemented");
}
