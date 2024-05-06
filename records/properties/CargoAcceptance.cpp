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
#include "properties/CargoAcceptance.h"
#include "StreamHelpers.h"


void CargoAcceptance::read(std::istream& is)
{
    m_cargo_type = read_uint8(is);
    m_acceptance = read_uint8(is);
}


void CargoAcceptance::write(std::ostream& os) const
{
    write_uint8(os, m_cargo_type);
    write_uint8(os, m_acceptance);
}


void CargoAcceptance::print(std::ostream& os, uint16_t indent) const
{
    os << "{" << to_hex(m_cargo_type) << ": " << to_hex(m_acceptance) << "}";
}


void CargoAcceptance::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    m_cargo_type = is.match_uint8();
    is.match(TokenType::Colon);
    m_acceptance = is.match_uint8();
    is.match(TokenType::CloseBrace);
}
