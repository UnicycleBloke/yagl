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
#include "CustomStation.h"
#include "StreamHelpers.h"
#include "EnumDescriptor.h"


namespace {


constexpr const char* str_layout = "layout";


const EnumDescriptorT<CustomLayout::Platform> platform_desc =
{
    0x00, "platform",
    {
        { 0, "P" }, // Platform::Plain },
        { 2, "B" }, // Platform::Building },
        { 4, "L" }, // Platform::RoofLeft },
        { 6, "R" }, // Platform::RoofRight },
    }
};


const EnumDescriptorT<CustomLayout::Direction> direction_desc =
{
    0x00, "direction",
    {
        { 0, "NE_SW" },
        { 1, "NW_SE" },
    }
};


} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CustomLayout::read(std::istream& is)
{
    m_platform_length = read_uint8(is);
    m_platform_count  = read_uint8(is);
    uint16_t dirs     = 0;
    uint16_t size     = m_platform_length * m_platform_count;
    for (uint16_t i = 0; i < size; ++i)
    {
        uint8_t temp = read_uint8(is);
        auto tile = static_cast<Platform>(temp & 0xFE);
        m_platform_tiles.push_back(tile);

        dirs += (temp & 0x01);
    }

    // Sum of directions should same as the number of platforms times length, or zero.
    // TODO check this.
    if (size > 0)
        m_direction = static_cast<Direction>(dirs / size);
}


void CustomLayout::write(std::ostream& os) const
{
    write_uint8(os, m_platform_length);
    write_uint8(os, m_platform_count);
    for (const auto tile: m_platform_tiles)
    {
        // Add back the orientation.
        write_uint8(os, static_cast<uint8_t>(tile) + static_cast<uint8_t>(m_direction));
    }

    // Assert that the vector has the correct length?
}


void CustomLayout::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_layout << "<";
    direction_desc.print_value(m_direction, os);
    os << ">\n";
    os << pad(indent) << "{\n";
    os << pad(indent + 4) << "// Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight\n";

    uint16_t index{};
    for (uint8_t c = 0; c < m_platform_count; ++c)
    {
        os << pad(indent + 4);
        for (uint8_t l = 0; l < m_platform_length; ++l)
        {
            // Assert that the vector has the correct length?
            const auto tile = m_platform_tiles[index++];
            switch (tile)
            {
                case Platform::Building:  os << "B "; break;
                case Platform::Plain:     os << "P "; break;
                case Platform::RoofLeft:  os << "L "; break;
                case Platform::RoofRight: os << "R "; break;
            }
        }
        os << ";\n";
    }

    os << pad(indent) << "}\n";
}


void CustomLayout::parse(TokenStream& is)
{
    is.match_ident(str_layout);
    is.match(TokenType::OpenAngle);
    direction_desc.parse(m_direction, is);
    is.match(TokenType::CloseAngle);

    m_platform_count = 0;
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        while (is.peek().type != TokenType::SemiColon)
        {
            Platform tile;
            platform_desc.parse(tile, is);
            m_platform_tiles.push_back(tile);
        }
        is.match(TokenType::SemiColon);
        ++m_platform_count;
    }
    is.match(TokenType::CloseBrace);

    m_platform_length = uint8_t(m_platform_tiles.size() / m_platform_count);
    if ((m_platform_tiles.size() % m_platform_count) != 0)
        throw PARSER_ERROR("Platform lengths do not match", is.peek());
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CustomStation::read(std::istream& is)
{
    while (true)
    {
        CustomLayout layout;
        layout.read(is);
        if (layout.terminator())
        {
            break;
        }
        m_layouts.push_back(layout);
    }
}


void CustomStation::write(std::ostream& os) const
{
    for (const auto& layout: m_layouts)
    {
        layout.write(os);
    }

    // Terminator
    write_uint8(os, 0x00);
    write_uint8(os, 0x00);
}


void CustomStation::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "[\n";
    for (const auto& layout: m_layouts)
    {
        layout.print(os, indent + 4);
    }
    os << pad(indent) << "]";
}


void CustomStation::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        CustomLayout layout;
        layout.parse(is);
        m_layouts.push_back(layout);
    }
    is.match(TokenType::CloseBracket);
}

