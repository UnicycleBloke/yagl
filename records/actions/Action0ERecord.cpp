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
    uint8_t count = read_uint8(is);
    for (uint8_t i = 0; i < count; ++i)
    {
        GRFLabel grf_id;
        grf_id.read(is);
        m_grf_ids.push_back(grf_id);
    }
}


void Action0ERecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    uint8_t count = m_grf_ids.size();
    write_uint8(os, count);
    for (const GRFLabel& grf_id: m_grf_ids)
    {
        grf_id.write(os);
    }
}  


// disable_grfs
// {
//     grf_ids: [ "XXXX" "YYYY" ];
// }


void Action0ERecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0E\n";
    os << pad(indent) << "{\n";

    os << pad(indent + 4) << "grf_ids: [";
    for (const auto& grf_id: m_grf_ids)
    {
        os << " \"" << grf_id.to_string() << "\"";
    }
    os << " ];\n";
    
    os << pad(indent) << "}\n";
}


void Action0ERecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0ERecord::parse not implemented");
}
