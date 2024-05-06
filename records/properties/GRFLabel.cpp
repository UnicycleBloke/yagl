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
#include "properties/GRFLabel.h"
#include "StreamHelpers.h"


void GRFLabel::read(std::istream& is)
{
    m_label = read_uint32(is);
}


void GRFLabel::write(std::ostream& os) const
{
    write_uint32(os, m_label);
}


void GRFLabel::print(std::ostream& os, uint16_t /*indent*/) const
{
    os << "\"" << to_string() << "\"";
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
    const TokenValue& token = is.peek();

    uint8_t b = 0;
    std::string value;
    switch (token.type)
    {
        case TokenType::Ident:
            value = is.match(TokenType::Ident);

            if (value.size() != 4)
            {
                throw PARSER_ERROR("Invalid GRF label: '" + token.value + "'", token);
            }

            m_label = 0;
            for (uint8_t i = 0; i < 4; ++i)
            {
                m_label |= (static_cast<uint8_t>(value[i]) << (i * 8));
            }
            break;

        case TokenType::String:
            value = is.match(TokenType::String);

            m_label = 0;
            for (uint8_t i = 0; (i < value.size()) && (b < 4); ++i)
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

            if (b != 4)
            {
                throw PARSER_ERROR("Invalid GRF label: '" + token.value + "'", token);
            }
            break;

        case TokenType::Number:
            m_label = is.match_uint32();
            break;

        default:
            throw PARSER_ERROR("Expected GRF label, got '" + token.value + "'", token);
    }
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


std::string GRFLabel::to_string_or_hex() const
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        uint8_t c = (m_label >> (i * 8)) & 0xFF;
        if (!std::isprint(c))
        {
            return ::to_hex(m_label);
        }
    }

    return to_string();
}

