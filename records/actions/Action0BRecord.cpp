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
#include "Action0BRecord.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"


void Action0BRecord::read(std::istream& is, const GRFInfo& info)
{
    m_severity    = read_uint8(is);
    m_language_id = read_uint8(is);
    m_message_id  = read_uint8(is);

    m_apply_during_init = (m_severity & 0x80) == 0x80;
    m_severity = m_severity & ~0x80;

    if (m_message_id == 0xFF)
    {
        // The message may have up to two 0x80s in it, and then up to two 0x7Bs.
        // - The first 0x80 is the filename of the GRF.
        // - The second 0x80 is the message data string below.
        // - The first 0x7B is the first parameter byte below.
        // - The second 0x7B is the second parameter byte below.
        // No other combinations are permitted.
        m_custom_message = read_string(is);
    }

    // // Can either scan the string or just check for end of input. The 
    // // built in strings can't be scanned, so...
    if (is.peek() != EOF)
    {
        m_message_data = read_string(is);
    }

    m_num_params = 0;
    m_param1     = 0xFF;
    m_param2     = 0xFF;
    if (is.peek() != EOF)
    {
        m_num_params = 1;
        m_param1     = read_uint8(is);
    }
    if (is.peek() != EOF)
    {
        m_num_params = 2;
        m_param2     = read_uint8(is);
    }
}


void Action0BRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_severity | (m_apply_during_init ? 0x80 : 0x00));
    write_uint8(os, m_language_id);
    write_uint8(os, m_message_id);

    if (m_message_id == 0xFF)
    {
        write_string(os, m_custom_message);
    }

    // Bug - what if the string that was read was just a 0 termination byte?
    if (m_message_data.length() > 0)
    {
        write_string(os, m_message_data);
    }

    if (m_num_params > 0) write_uint8(os, m_param1);
    if (m_num_params > 1) write_uint8(os, m_param2);
}  


void Action0BRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0B" << '\n';
    os << pad(indent) << "{" << '\n';

    os << pad(indent + 4) << "severity: " << to_hex(m_severity) << '\n';
    os << pad(indent + 4) << "language: " << to_hex(m_language_id) << " // " << language_name(m_language_id) << '\n';
    os << pad(indent + 4) << "message_id: " << to_hex(m_message_id) << '\n';

    if (m_message_id == 0xFF)
    {
        os << pad(indent + 4) << "custom_message: \"" << grf_string_to_readable_utf8(m_custom_message) << "\"\n";
    }

    os << pad(indent + 4) << "message_data: \"" << grf_string_to_readable_utf8(m_message_data) << "\"\n";
    if (m_num_params > 0)
        os << pad(indent + 4) << "param1: " << to_hex(m_param1) << '\n';
    if (m_num_params > 1)
        os << pad(indent + 4) << "param2: " << to_hex(m_param2) << '\n';

    os << pad(indent) << "}" << '\n';
}


void Action0BRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0BRecord::parse not implemented");
}

