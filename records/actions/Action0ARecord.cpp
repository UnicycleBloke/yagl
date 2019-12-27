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
#include "Action0ARecord.h"
#include "StreamHelpers.h"


void Action0ARecord::read(std::istream& is, const GRFInfo& info)
{
    uint8_t num_sets = read_uint8(is);
    m_sets.resize(num_sets);

    for (uint8_t i = 0; i < num_sets; ++i)
    {
        m_sets[i].num_sprites  = read_uint8(is);
        m_sets[i].first_sprite = read_uint16(is);
    }
}


void Action0ARecord::write(std::ostream& os, const GRFInfo& info) const
{
    ContainerRecord::write(os, info);

    uint8_t num_sets = m_sets.size();
    write_uint8(os, num_sets);

    // TODO get the number of sprites from the contained records.
    for (const auto& set: m_sets)
    {
        write_uint8(os, set.num_sprites);
        write_uint16(os, set.first_sprite);
    }
}  


uint16_t Action0ARecord::num_sprites_to_read() const
{
    uint16_t total = 0;
    for (const auto& set: m_sets)
    {
        total += set.num_sprites;
    }
    return total;
}


void Action0ARecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0A " << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t index = 0;
    for (const auto& set: m_sets)
    {
        os << pad(indent + 4) << "replacement_sprite_set" << '\n';
        os << pad(indent + 4) << "{" << '\n';

        for (uint16_t i = 0; i < set.num_sprites; ++i)
        {
            os << pad(indent + 8) << to_hex(set.first_sprite + i) << ":\n";
            print_sprite(index, os, sprites, indent + 8);
            ++index;
        }

        os << pad(indent + 4) << "}" << '\n';
    }

    os << pad(indent) << "}" << '\n';
}


void Action0ARecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0ARecord::parse not implemented");
}
