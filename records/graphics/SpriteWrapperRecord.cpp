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
#include "SpriteWrapperRecord.h"
#include "StreamHelpers.h"
#include <sstream>


void SpriteWrapperRecord::read(std::istream& is, const GRFInfo& info)
{
    // Nothing to read as this is done externally.
}


void SpriteWrapperRecord::write(std::ostream& os, const GRFInfo& info) const
{
    // sprite ID
    std::ostringstream ss;
    m_sprite->write(ss, info);
    uint32_t size = static_cast<uint32_t>(ss.str().size());
    
    write_uint32(os, m_sprite_id);
    write_uint32(os, size + 1); // +1 to account for the extra 0xFF.
    write_uint8(os, 0xFF);

    m_sprite->write(os, info);
}  


void SpriteWrapperRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    m_sprite->print(os, sprites, indent);
}


void SpriteWrapperRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    m_sprite->parse(is, sprites);
}
