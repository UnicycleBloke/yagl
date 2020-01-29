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
#include "Action0FRecord.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"


void Action0FRecord::read(std::istream& is, const GRFInfo& info)
{
    m_id  = read_uint8(is);

    // Style names only exist if this is the final definition, the 
    // root. This is indicated by the high bit of the ID.
    if ((m_id & 0x80) == 0x80)
    {
        m_id = m_id & 0x7F;
        do
        {
            Name style_name;
            style_name.lang_id = read_uint8(is);
            style_name.name.read(is);
            m_style_names.push_back(style_name);
        }
        while (is.peek() != 0x00);
        read_uint8(is);
    }

    uint8_t num_parts = read_uint8(is);
    for (uint8_t i = 0; i < num_parts; ++i)
    {
        Part part;
        uint8_t text_count = read_uint8(is);
        part.first_bit     = read_uint8(is);
        part.num_bits      = read_uint8(is);
        for (uint8_t j = 0; j < text_count; ++j)
        {
            Text text;
            text.probability = read_uint8(is);
            text.is_string  = ((text.probability & 0x80) == 0x00);
            text.probability = text.probability & 0x7F;
            if (text.is_string)
            {
                text.text.read(is);
            }
            else
            {
                text.action_0F_id = read_uint8(is);
            }

            part.texts.push_back(text);
        }
        m_style_parts.push_back(part);
    }
}


void Action0FRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    bool is_final = (m_style_names.size() > 0);
    write_uint8(os, m_id | (is_final ? 0x80 : 0x00));

    if (is_final)
    {
        for (const Name& style_name: m_style_names) 
        {
            write_uint8(os, style_name.lang_id);
            style_name.name.write(os);
        }
        write_uint8(os, 0x00);
    }

    write_uint8(os, uint8_t(m_style_parts.size()));

    for (const Part& part: m_style_parts) 
    {
        write_uint8(os, uint8_t(part.texts.size()));
        write_uint8(os, part.first_bit);
        write_uint8(os, part.num_bits);

        for (const Text& text: part.texts)
        {
            write_uint8(os, text.probability | (text.is_string ? 0x00 : 0x80));
            if (text.is_string)
            {
                text.text.write(os);
            }
            else
            {
                write_uint8(os, text.action_0F_id);
            }
        }
    }
}  


namespace {
constexpr const char* str_styles     = "styles";
constexpr const char* str_part       = "part";
constexpr const char* str_text       = "text";
constexpr const char* str_town_names = "town_names";
} // namespace {


void Action0FRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << RecordName(record_type()) << "<" << to_hex(m_id) << "> // Action0F\n";
    os << "{\n";

    // Names in various languages for this town names style.
    if (m_style_names.size() > 0)
    {
        os << pad(indent + 4) << str_styles << ":\n";
        os << pad(indent + 4) << "{\n";
        os << pad(indent + 8) << "// lang_id: style_name;\n";

        for (const auto& style: m_style_names)
        {
            os << pad(indent + 8) << language_iso(style.lang_id) << ": ";
            os << "\"" << style.name.readable() << "\";\n";
        }

        os << pad(indent + 4) << "}\n";
    }

    // Descriptions of the various parts of this town names record. Can refer to others.
    for (const auto& part: m_style_parts)
    {
        os << pad(indent + 4) << str_part << "<" << uint16_t(part.first_bit) << ", ";
        os << uint16_t(part.num_bits) << ">: // <first_bit, num_bits>\n";
        os << pad(indent + 4) << "{\n";

        for (const auto& text: part.texts)
        {
            if (text.is_string)
            {
                os << pad(indent + 8) << str_text << "(\"" << text.text.readable() << "\"";
            } 
            else
            {
                os << pad(indent + 8) << str_town_names << "(" << to_hex(text.action_0F_id);
            }
            os << ", " << uint16_t(text.probability) << ");\n";
        }

        os << pad(indent + 4) << "}\n";
    }

    os << "}\n";
}

// town_names<0x00> // Action0F
// {
//     styles: // lang_id: style_name;
//     {
//         default: "UnicycleBloke's town names";
//     }
//     part<0, 3>: // <first_bit, num_bits>
//     {
//         text("Hemp", 1);
//         text("Thet", 1);
//         text("Fram", 1);
//         text("Gold", 1);
//         text("Hadden", 1);
//         text("Bolt", 1);
//         text("Wish", 1);
//         text("Grant", 1);
//         town_names(0x00, 1);
//         town_names(0x01, 1);
//         town_names(0x02, 1);
//     }
//     part<3, 3>: // <first_bit, num_bits>
//     {
//         text("ing", 1);
//         text("ham", 1);
//         ...
//     }
//     part<6, 2>: // <first_bit, num_bits>
//     {
//         text("ford", 1);
//         text("ton", 1);
//         ....
//     }
// }


void Action0FRecord::parse_styles(TokenStream& is)
{
    is.match(TokenType::Colon);
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        Name name;
        name.lang_id = language_id(is.match(TokenType::Ident));
        is.match(TokenType::Colon);
        name.name.parse(is);
        is.match(TokenType::SemiColon);
        m_style_names.push_back(name);
    }

    is.match(TokenType::CloseBrace);
}


void Action0FRecord::parse_part(TokenStream& is)
{
    Part part;
    is.match(TokenType::OpenAngle); 
    part.first_bit = is.match_integer();
    is.match(TokenType::Comma);
    part.num_bits = is.match_integer();
    is.match(TokenType::CloseAngle); 

    is.match(TokenType::Colon);
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        Text text;

        TokenValue token = is.peek();
        is.match(TokenType::Ident);
        is.match(TokenType::OpenParen);

        if (token.value == str_text)
        {
            text.text.parse(is);
            text.is_string = true;
        }
        else if (token.value == str_town_names)
        {
            text.action_0F_id = is.match_integer();
            text.is_string    = false;
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: " + token.value, token);
        }

        is.match(TokenType::Comma);
        text.probability = is.match_integer();
        is.match(TokenType::CloseParen);
        is.match(TokenType::SemiColon);

        part.texts.push_back(text);
    }

    m_style_parts.push_back(part);

    is.match(TokenType::CloseBrace);
}


void Action0FRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_id = is.match_integer();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        is.match(TokenType::Ident);
        if (token.value == str_styles)
        {
            parse_styles(is);
        }
        else if (token.value == str_part)
        {
            parse_part(is);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: " + token.value, token);
        }
    }

    is.match(TokenType::CloseBrace);
}
