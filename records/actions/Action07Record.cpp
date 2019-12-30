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
#include "Action07Record.h"
#include "StreamHelpers.h"


void Action07Record::read(std::istream& is, const GRFInfo& info)
{
    m_variable  = read_uint8(is);
    m_varsize   = read_uint8(is);
    m_condition = read_uint8(is);

    if (m_condition < 2)
    {
        /* Always 1 for bit tests, the given value should be ignored. */
        m_varsize = 1;
    }

    switch (m_varsize)
    {
        case 8: m_value = read_uint32(is); m_mask = read_uint32(is); break;
        case 4: m_value = read_uint32(is); m_mask = 0xFFFFFFFF;      break;
        case 2: m_value = read_uint16(is); m_mask = 0x0000FFFF;      break;
        case 1: m_value = read_uint8(is);  m_mask = 0x000000FF;      break;
        default: break;
    }

    m_sprites = read_uint8(is);
}


void Action07Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_variable);
    write_uint8(os, m_varsize);
    write_uint8(os, m_condition);

    switch (m_varsize)
    {
        case 8: write_uint32(os, m_value); 
                write_uint32(os, m_mask); break;
        case 4: write_uint32(os, m_value); break;
        case 2: write_uint16(os, m_value); break;
        case 1: write_uint8(os, m_value);  break;
        default: break;
    }

    write_uint8(os, m_sprites);
}  


void Action07Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " (";

    // Variable type:
    if      (m_variable < 0x80) os << "param[" << to_hex(m_variable) << "] ";
    else if (m_variable < 0xC0) os << "global_var[" << to_hex(m_variable) << "] "; 
    else                        os << "<error?>reserved[" << to_hex(m_variable) << "] ";

    // Operation/condition
    switch (m_condition)
    {
        case 0x00: os << "bit_is_set "; break;    
        case 0x01: os << "bit_is_clear "; break;  

        case 0x02: os << "== "; break;
        case 0x03: os << "!= "; break;
        case 0x04: os << "< "; break;
        case 0x05: os << "> "; break;

        case 0x06: os << "grf_is_active "; break;
        case 0x07: os << "grf_not_active "; break;
        case 0x08: os << "grf_will_be_active "; break;
        case 0x09: os << "grf_is_active_or_will_be "; break;
        case 0x0A: os << "grf_will_not_be_active "; break;

        case 0x0B: os << "cargo_type_not_available "; break;
        case 0x0C: os << "cargo_type_is_available "; break;
        case 0x0D: os << "rail_type_not_defined "; break;
        case 0x0e: os << "rail_type_is_defined "; break;

        default:   os << "<error> "; 
    }

    // Value to compare or operate with
    switch (m_varsize)
    {
        case 8:  os << to_hex(m_value) << ", " << to_hex(m_mask); break;
        case 4:  os << to_hex(m_value); break;
        case 2:  os << to_hex(m_value); break;
        case 1:  os << to_hex(m_value); break;
        default: os << "<error>"; 
    }

    os << ")";
    if (record_type() == RecordType::ACTION_07)
        os << " // Action07";
    else
        os << " // Action09";
    os << pad(indent) << "\n{\n";
    os << pad(indent + 4) <<  "skip " << to_hex(m_sprites) << " sprite(s)";
    os << pad(indent) << "\n}\n";
}    


void Action07Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action07Record::parse not implemented");
}
