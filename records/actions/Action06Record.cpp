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


void Action06Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action06" << '\n';
    os << pad(indent) << "{" << '\n';

    for (const auto& mod: m_modifications)
    {
        os << pad(indent + 4) << "value: parameter[" << to_hex(mod.param_num, true) << "]; ";
        os << "size: " << static_cast<uint16_t>(mod.param_size) << "; ";
        os << "offset: " << mod.offset << "; ";
        os << "add: " << (mod.add_bytes ? "true" : "false") << ";\n";
    }

    os << pad(indent) << "}" << '\n';
}


void Action06Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action06Record::parse not implemented");
}
