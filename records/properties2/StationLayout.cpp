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
#include "StationLayout.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_sprite = "sprite";
constexpr const char* str_tile   = "tile";


} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationTileData::read(std::istream& is)
{
    m_x_off  = read_uint8(is);
    m_y_off  = read_uint8(is);
    m_z_off  = read_uint8(is);
    m_x_ext  = read_uint8(is);
    m_y_ext  = read_uint8(is);
    m_z_ext  = read_uint8(is);
    m_sprite = read_uint32(is);

    m_new_bb = (m_z_off != 0x80);
}


void StationTileData::write(std::ostream& os) const
{
    write_uint8(os, m_x_off);
    write_uint8(os, m_y_off);
    if (m_new_bb)
    {
        write_uint8(os, m_z_off);
        write_uint8(os, m_x_ext);
        write_uint8(os, m_y_ext);
        write_uint8(os, m_z_ext);
    }
    else
    {
        write_uint8(os, 0x80);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
    }
    write_uint32(os, m_sprite);
}


void StationTileData::print(std::ostream& os, uint16_t indent) const
{
    if (m_new_bb)
    {
        os << pad(indent) << str_sprite << "(" << to_hex(m_sprite) << ", ";
        os << to_hex(m_x_off) << ", " << to_hex(m_y_off) << ", " << to_hex(m_z_off) << ", ";
        os << to_hex(m_x_ext) << ", " << to_hex(m_y_ext) << ", " << to_hex(m_z_ext);
        os << ");\n";
    }
    else
    {
        os << pad(indent) << str_sprite << "(" << to_hex(m_sprite) << ", ";
        os << to_hex(m_x_off) << ", " << to_hex(m_y_off);
        os << ");\n";
    }
}


void StationTileData::parse(TokenStream& is)
{
    is.match_ident(str_sprite);
    is.match(TokenType::OpenParen);
    m_sprite = is.match_uint32();
    is.match(TokenType::Comma);
    m_x_off = is.match_uint8();
    is.match(TokenType::Comma);
    m_y_off = is.match_uint8();

    m_new_bb = (is.peek().type == TokenType::Comma);
    if (m_new_bb)
    {
        is.match(TokenType::Comma);
        m_z_off = is.match_uint8();
        is.match(TokenType::Comma);
        m_x_ext = is.match_uint8();
        is.match(TokenType::Comma);
        m_y_ext = is.match_uint8();
        is.match(TokenType::Comma);
        m_z_ext = is.match_uint8();
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationTile::read(std::istream& is)
{
    m_ground_sprite = read_uint32(is);
    if (m_ground_sprite == 0x00000000)
    {
        return;
    }

    while (is.peek() != 0x80)
    {
        StationTileData datum;
        datum.read(is);
        m_data.push_back(datum);
    }
    read_uint8(is);
}


void StationTile::write(std::ostream& os) const
{
    write_uint32(os, m_ground_sprite);
    if (m_ground_sprite == 0x00000000)
    {
        return;
    }

    for (const auto& datum: m_data)
    {
        datum.write(os);
    }
    write_uint8(os, 0x80);
}


void StationTile::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_tile << "<" << to_hex(m_ground_sprite) << ">\n";
    os << pad(indent) << "{\n";

    if (m_ground_sprite != 0x0000'0000)
    {
        for (const auto& datum: m_data)
        {
            datum.print(os, indent + 4);
        }
    }

    os << pad(indent) << "}\n";

}


void StationTile::parse(TokenStream& is)
{
    is.match_ident(str_tile);
    is.match(TokenType::OpenAngle);
    m_ground_sprite = is.match_uint32();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        StationTileData datum{};
        datum.parse(is);
        m_data.push_back(datum);
    }

    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationLayout::read(std::istream& is)
{
    uint16_t num_tiles = read_uint8_ext(is);
    m_tiles.resize(num_tiles);
    for (uint16_t i = 0; i < num_tiles; ++i)
    {
        m_tiles[i].read(is);
    }
}


void StationLayout::write(std::ostream& os) const
{
    write_uint8_ext(os, uint16_t(m_tiles.size()));
    for (const auto& tile: m_tiles)
    {
        tile.write(os);
    }
}


void StationLayout::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "[\n";
    for (const auto& tile: m_tiles)
    {
        tile.print(os, indent + 4);
    }
    os << pad(indent) << "]";
}


void StationLayout::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        StationTile tile{};
        tile.parse(is);
        m_tiles.push_back(tile);
    }

    is.match(TokenType::CloseBracket);
}

