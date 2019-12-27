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
#include "Action05Record.h"
#include "StreamHelpers.h"


void Action05Record::read(std::istream& is, const GRFInfo& info)
{
    // NewFeatureType - this is a type of sprite for new features 
    // not in TTD.
    uint8_t sprite_type = read_uint8(is);
    m_num_sprites = read_uint8_ext(is);

    // Some of the new features optional support an offset so that only
    // a subset of the graphics can to be provided, not starting with the 
    // first sprite for the feature.
    bool has_offset = (sprite_type & 0x80) != 0x00;
    m_sprite_type   = static_cast<NewFeatureType>(sprite_type & 0x7F);
    m_offset        = has_offset ? read_uint8_ext(is) : 0;
}


void Action05Record::write(std::ostream& os, const GRFInfo& info) const
{
    ContainerRecord::write(os, info);

    bool has_offset = (m_offset != 0);
    write_uint8(os, static_cast<uint8_t>(m_sprite_type) | (has_offset ? 0x80 : 0x00));
    // TODO get the number of sprites from the contained records.
    write_uint8_ext(os, m_num_sprites);

    if (has_offset)
    {
        write_uint8_ext(os, m_offset);
    }
}  


void Action05Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << NewFeatureName(m_sprite_type) << "> // Action05" << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t num_sprites = num_sprites_to_write();
    for (uint16_t index = 0; index < num_sprites; ++index)
    {
        print_sprite(index, os, sprites, indent + 4);
    }

    os << pad(indent) << "}" << '\n';
}


void Action05Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action05Record::parse not implemented");
}
