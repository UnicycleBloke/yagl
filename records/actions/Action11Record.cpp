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
#include "Action11Record.h"
#include "ActionFFRecord.h"
#include "SpriteIndexRecord.h"
#include "StreamHelpers.h"


void Action11Record::read(std::istream& is, const GRFInfo& info)
{
    // This is used to tell us how many of the following records are the
    // children of this one, but not important after that
    m_num_binaries = read_uint16(is);
}


void Action11Record::write(std::ostream& os, const GRFInfo& info) const
{
    ContainerRecord::write(os, info);

    write_uint16(os, num_sprites_to_write());
}  


// sound_effects // Action11
// {
//     // Binaries are in the graphics section and referenced indirectly
//     sprite_id: 0x00004A72
//     {
//         binary("yagl/modern_electric_slow.wav");
//     }
//     sprite_id: 0x00004A73
//     {
//         binary("yagl/modern_electric_fast.wav");
//     }
//
//     // Binaries are in the data section directly following this record.
//     binary("yagl/modern_electric_slow.wav");
//     binary("yagl/modern_electric_fast.wav");
//
//     // Sounds effects imports from other GRFs.
//     import("GRFx", 0x0001);
//     import("GRFx", 0x0002);
// }


void Action11Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action11" << '\n';
    os << pad(indent) << "{" << '\n';

    for (uint16_t index = 0; index < m_num_binaries; ++index)
    {
        print_sprite(index, os, sprites, indent + 4);
    }

    os << pad(indent) << "}" << '\n';
}


void Action11Record::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        parse_sprite(is, sprites);
    }
    is.match(TokenType::CloseBrace);

    m_num_binaries = num_sprites_to_write();
}
