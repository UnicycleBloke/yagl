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
            style_name.name    = read_string(is);
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
                text.text = read_string(is);
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
            write_string(os, style_name.name);
        }
        write_uint8(os, 0x00);
    }

    write_uint8(os, m_style_parts.size());

    for (const Part& part: m_style_parts) 
    {
        write_uint8(os, part.texts.size());
        write_uint8(os, part.first_bit);
        write_uint8(os, part.num_bits);

        for (const Text& text: part.texts)
        {
            write_uint8(os, text.probability | (text.is_string ? 0x00 : 0x80));
            if (text.is_string)
            {
                write_string(os, text.text);
            }
            else
            {
                write_uint8(os, text.action_0F_id);
            }
        }
    }
}  


// town_names<id>
// {
//     styles:
//     {
//         en_GB: "name";
//         fr_FR: "nom";
//     }
    
//     part<first, num>:
//     {
//         text("afadfaf", prob);
//         town_names(0x23, prob);
//     }
    
//     part<first, num>:
//     {
//         text("afadfaf", prob);
//         town_names(0x23, prob);
//     }
// }

void Action0FRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << RecordName(record_type()) << "<" << to_hex(m_id) << "> // Action0F\n";
    os << "{\n";

    // Names in various languages for this town names style.
    if (m_style_names.size() > 0)
    {
        os << pad(indent + 4) << "styles: // lang_id: style_name;\n";
        os << pad(indent + 4) << "{\n";

        for (const auto& style: m_style_names)
        {
            os << pad(indent + 8) << language_iso(style.lang_id) << ": ";
            os << "\"" << grf_string_to_readable_utf8(style.name) << "\";\n";
        }

        os << pad(indent + 4) << "}\n";
    }

    // Descriptions of the various parts of this town names record. Can refer to others.
    for (const auto& part: m_style_parts)
    {
        os << pad(indent + 4) << "part<" << uint16_t(part.first_bit) << ", ";
        os << uint16_t(part.num_bits) << ">: // <first_bit, num_bits>\n";
        os << pad(indent + 4) << "{\n";

        for (const auto& text: part.texts)
        {
            if (text.is_string)
            {
                os << pad(indent + 8) << "text(\"" << grf_string_to_readable_utf8(text.text) << "\"";
            } 
            else
            {
                os << pad(indent + 8) << "town_names(" << to_hex(text.action_0F_id);
            }
            os << ", " << uint16_t(text.probability) << ");\n";
        }

        os << pad(indent + 4) << "}\n";
    }

    os << "}\n";
}


void Action0FRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0FRecord::parse not implemented");
}
