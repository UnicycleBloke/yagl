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
#include "Descriptors.h"


void Action0BRecord::read(std::istream& is, const GRFInfo& info)
{
    uint8_t severity = read_uint8(is);
    m_language_id    = read_uint8(is);
    m_message_id     = read_uint8(is);

    m_apply_during_init = (severity & 0x80) == 0x80;
    m_severity = static_cast<Severity>(severity & ~0x80);

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

    write_uint8(os, static_cast<uint8_t>(m_severity) | (m_apply_during_init ? 0x80 : 0x00));
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


static const EnumDescriptorT<Action0BRecord::Severity> severity_desc = 
{ 
    0x00, "severity",                   
    {
        { 0, "Notice" },  // Severity::Notice },    
        { 1, "Warning" }, // Severity::Warning }, 
        { 2, "Error" },   // Severity::Error }, 
        { 3, "Fatal" },   // Severity::Fatal }, 
    }
};


// From the NewGRF specs:
constexpr const char* str_message_00 = "\x80 requires at least TTDPatch version \x80";
constexpr const char* str_message_01 = "\x80 is for the \x80 version of TTD."; // <data> should be "DOS" or "Windows"
constexpr const char* str_message_02 = "\x80 is designed to be used with \x80"; // <data> should be a switchname + value, e.g. "multihead 0"
constexpr const char* str_message_03 = "Invalid parameter for \x80: parameter \x80 ( \x7B )"; // <data> should be the switch number written out ("5")
constexpr const char* str_message_04 = "\x80 must be loaded before \x80.";
constexpr const char* str_message_05 = "\x80 must be loaded after \x80.";
constexpr const char* str_message_06 = "\x80 equires OpenTTD version \x80 or better."; 


void Action0BRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action0B\n";
    os << pad(indent) << "{\n";

    severity_desc.print(m_severity, os, indent + 4);
    os << pad(indent + 4) << "language: " << to_hex(m_language_id) << "; // " << language_name(m_language_id) << "\n";
    os << pad(indent + 4) << "message_id: " << to_hex(m_message_id) << ";\n";

    if (m_message_id == 0xFF)
    {
        os << pad(indent + 4) << "custom_message: \"" << grf_string_to_readable_utf8(m_custom_message) << "\";\n";
    }
    else
    {
        os << pad(indent + 4) << "// standard_message: \"";
        switch (m_message_id)
        {
            case 0x00: os << grf_string_to_readable_utf8(str_message_00); break;
            case 0x01: os << grf_string_to_readable_utf8(str_message_01); break;
            case 0x02: os << grf_string_to_readable_utf8(str_message_02); break;
            case 0x03: os << grf_string_to_readable_utf8(str_message_03); break;
            case 0x04: os << grf_string_to_readable_utf8(str_message_04); break;
            case 0x05: os << grf_string_to_readable_utf8(str_message_05); break;
            case 0x06: os << grf_string_to_readable_utf8(str_message_06); break;
            default:   os << "<unknown>"; break;
        }
        os << "\";\n";
    }

    os << pad(indent + 4) << "message_data: \"" << grf_string_to_readable_utf8(m_message_data) << "\";\n";
    if (m_num_params > 0)
    {
        os << pad(indent + 4) << "param1: " << to_hex(m_param1) << ";\n";
    }
    if (m_num_params > 1)
    {
        os << pad(indent + 4) << "param2: " << to_hex(m_param2) << ";\n";
    }

    os << pad(indent) << "}\n";
}


void Action0BRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0BRecord::parse not implemented");
}

