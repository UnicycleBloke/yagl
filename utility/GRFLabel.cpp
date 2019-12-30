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
#include "GRFLabel.h"
#include "StreamHelpers.h"


void GRFLabel::read(std::istream& is)
{
    m_label = read_uint32(is);
}


void GRFLabel::write(std::ostream& os) const
{
    write_uint32(os, m_label);
}


static uint8_t hex_to_dec(uint8_t h)
{
    if ((h >= '0') && (h <= '9'))
        return h - '0';

    if ((h >= 'A') && (h <= 'F'))
        return 10 + h - 'A';

    if ((h >= 'a') && (h <= 'f'))
        return 10 + h - 'a';

    // The lexer should have already validated the number.
    return 0;
}


void GRFLabel::parse(TokenStream& is)
{
    uint8_t b = 0;
    std::string value;
    switch (is.peek().type)
    {
        case TokenType::Ident:
            value   = is.match(TokenType::Ident);
            if (value.size() != 4) throw ParserError("Invalid GRF label", is.peek());
            m_label = 0;
            for (uint8_t i = 0; i < 4; ++i)
            {
                m_label |= (static_cast<uint8_t>(value[i]) << (i * 8));
            }
            break;

        case TokenType::String:
            value   = is.match(TokenType::String);
            // TODO we need to handle the escapes \xHH. "\xFB\xFB\x06\x01"
            //if (value.size() != 4) throw ParserError("Invalid GRF label", is.peek());
            m_label = 0;
            for (uint8_t i = 0; i < value.size(); ++i)
            {
                uint8_t c = value[i];
                if (c == '\\')
                {
                    i += 2;
                    c  = hex_to_dec(value[i++]) << 4;
                    c |= hex_to_dec(value[i]);
                }
                m_label |= (c << (b++ * 8));
            }
            break;

        case TokenType::Number:
            m_label = is.match_integer();
            break;

        default:
            throw ParserError("Expected GRF label", is.peek());
    }
}


uint32_t GRFLabel::to_integer() const
{
    return m_label;
}


std::string GRFLabel::to_string() const
{
    std::ostringstream os;
    for (uint8_t i = 0; i < 4; ++i)
    {
        uint8_t c = (m_label >> (i * 8)) & 0xFF;
        if (std::isprint(c))
        {
            os << c;
        }
        else
        {
            os << "\\x" << ::to_hex(c, false);
        }
    }
    return os.str();
}


std::string GRFLabel::to_hex() const
{
    return ::to_hex(m_label);
}


std::string GRFLabel::to_string_or_hex() const
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        uint8_t c = (m_label >> (i * 8)) & 0xFF;
        if (!std::isprint(c))
        {
            return to_hex(); 
        }
    }

    return to_string();
}

