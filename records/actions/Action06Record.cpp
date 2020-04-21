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
#include "Action06Record.h"
#include "StreamHelpers.h"


void Action06Record::read(std::istream& is, const GRFInfo& info)
{
    while (is.peek() != 0xFF)
    {
        Modification mod;
        mod.param_num  = read_uint8(is);
        mod.param_size = read_uint8(is);
        mod.add_bytes  = (mod.param_size & 0x80) != 0x00;
        mod.param_size = mod.param_size & 0x7F;

        mod.offset     = read_uint8_ext(is);
        m_modifications.push_back(mod);
    }

    // Not really necessary, but consume all the available bytes.
    read_uint8(is);
}


void Action06Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    for (const auto& mod: m_modifications)
    {
        write_uint8(os, mod.param_num);
        write_uint8(os, mod.param_size | (mod.add_bytes ? 0x80 : 0x00));
        write_uint8_ext(os, mod.offset);
    }
    write_uint8(os, 0xFF);
}  


// modify_next // Action06
// {
//     // modification(value, num_bytes, offset, add?);
//     modification(parameter[0x7E], 1, 8, false);
//     modification(parameter[0x7E], 1, 9, false);
//     modification(parameter[0x7E], 1, 10, false);
// }


static constexpr const char* str_modification = "modification";
static constexpr const char* str_parameter    = "parameter";


void Action06Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action06\n";
    os << pad(indent) << "{\n";
    os << pad(indent + 4) << "// modification(value, num_bytes, offset, add?);\n"; 

    for (const auto& mod: m_modifications)
    {
        os << pad(indent + 4) << str_modification << "("; 
        os << str_parameter << "[" << to_hex(mod.param_num) << "], ";
        os << static_cast<uint16_t>(mod.param_size) << ", ";
        os << mod.offset << ", ";
        os << (mod.add_bytes ? "true" : "false") << ");\n";
    }

    os << pad(indent) << "}\n";
}


void Action06Record::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        Modification mod;

        is.match_ident(str_modification);
        is.match(TokenType::OpenParen);

        is.match_ident(str_parameter);
        is.match(TokenType::OpenBracket);
        mod.param_num = is.match_uint8();
        is.match(TokenType::CloseBracket);
        is.match(TokenType::Comma);

        mod.param_size = is.match_uint8();
        is.match(TokenType::Comma);

        mod.offset = is.match_uint16();
        is.match(TokenType::Comma);

        mod.add_bytes = is.match_bool();
        is.match(TokenType::CloseParen);
        is.match(TokenType::SemiColon);

        m_modifications.push_back(mod);
    }

    is.match(TokenType::CloseBrace);
}
