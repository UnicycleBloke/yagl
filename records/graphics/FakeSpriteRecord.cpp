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
#include "FakeSpriteRecord.h"
#include "StreamHelpers.h"


static constexpr const char* str_null_sprite = "null_sprite";


void FakeSpriteRecord::write(std::ostream& os, const GRFInfo& info) const
{
    write_uint8(os, 0x00);
}


void FakeSpriteRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << str_null_sprite << ";\n";
}


void FakeSpriteRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(str_null_sprite);
    is.match(TokenType::SemiColon);
}
