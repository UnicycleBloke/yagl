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
#include "RecolourRecord.h"
#include "StreamHelpers.h"


void RecolourRecord::read(std::istream& is, const GRFInfo& info)
{
    for (auto& item: m_colour_map)
    {
        item = read_uint8(is);
    }
}


void RecolourRecord::write(std::ostream& os, const GRFInfo& info) const
{
    // The first byte in the record is literal zero. This makes it 
    // look like an Action00, but it can be disambiguated by context on
    // reading.
    write_uint8(os, 0x00);

    for (const auto& item: m_colour_map)
    {
        write_uint8(os, item);
    }
}  


static constexpr const char* str_recolour_sprite = "recolour_sprite";


void RecolourRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << str_recolour_sprite << '\n';
    os << pad(indent) << "{" << '\n';

    std::array<uint8_t, 256> diffs;
    for (uint16_t i = 0; i < 256; ++i)
    {
        diffs[i] = (i - m_colour_map[i]);
    }

    uint16_t state = 0;
    uint16_t start;
    for (uint16_t i = 0; i < 256; ++i)
    {        
        switch (state)
        {
            case 0:
                if (diffs[i] != 0)
                {
                    start = i;
                    state = 1;
                }
                break;

            case 1:
                if (diffs[i] != diffs[start])
                {
                    if (start != (i-1))
                    {
                        os << pad(indent + 4) << to_hex(uint8_t(start)) << ".." << to_hex(uint8_t(i-1)) << ": ";
                        os << to_hex(m_colour_map[start]) << ".." << to_hex(m_colour_map[i-1]) << ";" << "\n";
                    }                   
                    else
                    {
                        os << pad(indent + 4) << to_hex(uint8_t(start)) << ": ";
                        os << to_hex(m_colour_map[start]) << ";" << "\n";
                    }                     
                    state = 0;
                    if (diffs[i] != 0)
                    {
                        start = i;
                        state = 1;
                    }
                }
                break;
        }
    }

    os << pad(indent) << "}" << '\n';
}


void RecolourRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    // recolour_sprite
    // {
    //     0xE5: 0x01;
    //     0xE8..0xEA: 0x01..0x03;
    //     0xEB: 0x01;
    //     0xEC..0xEE: 0x04..0x06;
    // }

    for (uint16_t i = 0; i < 256; ++i)
    {
        m_colour_map[i] = uint8_t(i);
    }

    is.match_ident(str_recolour_sprite);
    is.match(TokenType::OpenBrace);

    while (is.peek().type != TokenType::CloseBrace)
    {
        uint16_t start = is.match_uint16();
        uint16_t end   = start;
        uint8_t  value;
        if (is.peek().type == TokenType::Colon)
        {
            is.match(TokenType::Colon);
            value = is.match_uint8();
        }
        else
        {
            is.match(TokenType::DoubleDot);
            end = is.match_uint16();

            is.match(TokenType::Colon);
            value = is.match_uint8();

            is.match(TokenType::DoubleDot);
            is.match_uint8();
        }
        is.match(TokenType::SemiColon);

        for (uint16_t index = start; index <= end; ++index)
        {
            m_colour_map[index] = value++;
        }
    }

    is.match(TokenType::CloseBrace);
}
