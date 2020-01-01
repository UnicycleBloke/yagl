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
// }


void Action11Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action11" << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t num_binaries = num_sprites_to_write(); 
    for (uint16_t index = 0; index < num_binaries; ++index)
    {
        print_sprite(index, os, sprites, indent + 4);
    }

    os << pad(indent) << "}" << '\n';
}


void Action11Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action11Record::parse not implemented");
}


void Action11Record::write_binary_files(const std::map<uint32_t, SpriteZoomVector>& sprites, const std::string& binary_dir) const
{
    for (uint16_t i = 0; i < num_sprites_to_write(); ++i)
    {
        const auto record = get_sprite(i);
        switch (record->record_type())
        {
            case RecordType::ACTION_FF:
            {
                const auto actionFF = std::dynamic_pointer_cast<ActionFFRecord>(record);
                actionFF->write_binary_file(binary_dir);
                break;
            }

            case RecordType::SPRITE_INDEX:
            {
                const auto index = std::dynamic_pointer_cast<SpriteIndexRecord>(record);
                const auto& it = sprites.find(index->sprite_id()); 
                if (it != sprites.end())
                {
                    const auto actionFF = std::dynamic_pointer_cast<ActionFFRecord>(it->second[0]);
                    actionFF->write_binary_file(binary_dir);
                }
            }
        }
    }
}
