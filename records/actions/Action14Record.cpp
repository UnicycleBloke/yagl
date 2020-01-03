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
#include "Action14Record.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"
#include <memory>
#include <cctype>


void Action14Record::read_chunks(std::istream& is, std::vector<Chunk>& chunks)
{
    while (is.peek() != 0x00)
    {
        Chunk chunk;
        chunk.type = read_uint8(is);
        chunk.label.read(is);
        switch (chunk.type)
        {
            case 'C':
                read_chunks(is, chunk.chunks);
                break;

            case 'T':
                chunk.language = read_uint8(is);
                chunk.text     = read_string(is);
                break;

            case 'B':
                {
                    uint16_t length = read_uint16(is);
                    chunk.data.resize(length);
                    for (uint16_t i = 0; i < length; ++i)
                    {
                        chunk.data[i] = read_uint8(is);
                    }
                }
                break;
        }

        chunks.push_back(chunk);
    }
    read_uint8(is);

}


void Action14Record::read(std::istream& is, const GRFInfo& info)
{
    read_chunks(is, m_chunks);
}


void Action14Record::write_chunks(std::ostream& os, const std::vector<Chunk>& chunks) const
{
    for (const auto& chunk: chunks)
    {
        write_uint8(os, chunk.type);
        chunk.label.write(os);
        switch (chunk.type)
        {
            case 'C':
                {
                    write_chunks(os, chunk.chunks);
                }
                break;
            case 'T':
                {
                    write_uint8(os, chunk.language);
                    write_string(os, chunk.text);
                }
                break;
            case 'B':
                {
                    uint16_t length = chunk.data.size();
                    write_uint16(os, length);
                    for (const auto& byte : chunk.data)
                    {
                        write_uint8(os, byte);
                    }
                }
                break;
        }
    }
    write_uint8(os, 0x00);
}


void Action14Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);
    
    write_chunks(os, m_chunks);
}  


void Action14Record::print_chunks(std::ostream& os, const std::vector<Chunk>& chunks, uint16_t indent) const
{
    for (const auto& chunk: chunks)
    {
        // No need to write out the type as it can be discerned from the other output.
        // os << pad(indent) << chunk.type << ' ' << chunk.label.to_string_or_hex() << ": ";
        os << pad(indent) << chunk.label.to_string_or_hex() << ": ";

        switch (chunk.type)
        {
            case 'C':
                os << "\n" << pad(indent) << "{\n";
                print_chunks(os, chunk.chunks, indent + 4);
                os << pad(indent) << "}\n";
                break;

            case 'T':
                os << language_iso(chunk.language) << ",";
                os << " \"" << grf_string_to_readable_utf8(chunk.text) << "\";";
                os << " // " << language_name(chunk.language) << "\n";
                break;

            case 'B':
                os << "[ ";
                for (const auto& d: chunk.data)
                {
                    os << to_hex(d) << ' ';
                }
                os << "];\n";
                break;
        }
    }
}


// optional_info // Action14
// {
//     INFO: 
//     {
//         NPAR: [ 0x01 ];
//         NAME: default, "NewStations v0.6 14.08.2014"; // Default
//         DESC: default, "{new-line}{lt-gray}©2004 .. 2014 Michael Blunck. All rights reserved.{new-line}Visit www.ttdpatch.de"; // Default
//         URL_: default, "http://www.ttdpatch.de"; // Default
//         PALS: [ 0x44 ];
//         VRSN: [ 0x03 0x00 0x00 0x00 ];
//         MINV: [ 0x00 0x00 0x00 0x00 ];
//         PARA: 
//         {
//             0x00000000: // Index instead of GRF label
//             {
//                 TYPE: [ 0x01 ];
//                 MASK: [ 0x00 0x00 0x01 ];
//                 NAME: default, "No green glass"; // Default
//                 NAME: de_DE, "ÞKein grünes Glas"; // German
//                 NAME: fr_FR, "ÞPas de verre teinté vert"; // French
//                 ... // More languages 
//                 DESC: default, "{lt-gray}Disables the tinted green glass for modern roofed platforms."; // Default
//                 DESC: de_DE, "Þ{lt-gray}Grünes Glas für die modernen überdachten Bahnsteige wird deaktiviert."; // German
//                 DESC: fr_FR, "Þ{lt-gray}Désactive le verre teinté vert pour les quais modernes couvertes."; // French
//                 ... // More languages 
//             }
//         }
//     }
// }


void Action14Record::parse_chunks(TokenStream& is, std::vector<Chunk>& chunks)
{
    is.match(TokenType::OpenBrace);

    while (is.peek().type != TokenType::CloseBrace)
    {
        Chunk chunk;

        // Read the GRFLabel
        chunk.label.parse(is);
        is.match(TokenType::Colon);
 
        switch(is.peek().type)
        {
            case TokenType::OpenBrace:
                chunk.type = 'C';
                parse_chunks(is, chunk.chunks);
                break;

            case TokenType::Ident:
                chunk.type = 'T';
                chunk.language = language_id(is.match(TokenType::Ident));
                is.match(TokenType::Comma);
                // TODO convert the readable string to the GRF format. This is either UTF8 (sort of), or Latin-1 (sort of).
                chunk.text = is.match(TokenType::String);
                is.match(TokenType::SemiColon);
                break;

            case TokenType::OpenBracket:
                chunk.type = 'B';
                is.match(TokenType::OpenBracket);
                while (is.peek().type == TokenType::Number)
                {
                    chunk.data.push_back(is.match_integer());
                }
                is.match(TokenType::CloseBracket);
                is.match(TokenType::SemiColon);
                break;

            default:
                throw ParserError("Invalid chunk type:", is.peek());
        }

        chunks.push_back(chunk);
    }

    is.match(TokenType::CloseBrace);
}


void Action14Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << RecordName(record_type()) << " // Action14\n";
    os << "{\n";
    print_chunks(os, m_chunks, indent + 4);
    os << "}\n";
}


void Action14Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    //is.match(TokenType::OpenBrace);
    parse_chunks(is, m_chunks);
    //is.match(TokenType::CloseBrace);
}
