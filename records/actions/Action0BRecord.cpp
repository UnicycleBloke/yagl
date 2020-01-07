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


// error_message<Warning, default> // Action0B <severity, language>
// {
//     message_id: 0x05; // {substring1} must be loaded after {substring1}.
//     message_data: "NuTracks"; // Second substring
// }


namespace {


constexpr const char* str_message_id     = "message_id";
constexpr const char* str_custom_message = "custom_message";
constexpr const char* str_message_data   = "message_data";
constexpr const char* str_param1         = "param1";
constexpr const char* str_param2         = "param2";

// From the NewGRF specs:
constexpr const char* str_message_00 = "\x80 requires at least TTDPatch version \x80";
constexpr const char* str_message_01 = "\x80 is for the \x80 version of TTD."; // <data> should be "DOS" or "Windows"
constexpr const char* str_message_02 = "\x80 is designed to be used with \x80"; // <data> should be a switchname + value, e.g. "multihead 0"
constexpr const char* str_message_03 = "Invalid parameter for \x80: parameter \x80 ( \x7B )"; // <data> should be the switch number written out ("5")
constexpr const char* str_message_04 = "\x80 must be loaded before \x80.";
constexpr const char* str_message_05 = "\x80 must be loaded after \x80.";
constexpr const char* str_message_06 = "\x80 requires OpenTTD version \x80 or better."; 


// Fake property numbers to facilitate out of order parsing.
const std::map<std::string, uint8_t> g_indices =
{
    { str_message_id,     0x01 },
    { str_custom_message, 0x02 },
    { str_message_data,   0x03 },
    { str_param1,         0x04 },
    { str_param2,         0x05 },
};


const EnumDescriptorT<Action0BRecord::Severity> severity_desc = 
{ 
    0x00, "severity",                   
    {
        { 0, "Notice" },  // Severity::Notice },    
        { 1, "Warning" }, // Severity::Warning }, 
        { 2, "Error" },   // Severity::Error }, 
        { 3, "Fatal" },   // Severity::Fatal }, 
    }
};

const IntegerDescriptorT<uint8_t> desc_message_id  { 0x01, str_message_id, PropFormat::Hex };
const StringDescriptor            desc_message     { 0x02, str_custom_message };
const StringDescriptor            desc_message_data{ 0x03, str_message_data };
const IntegerDescriptorT<uint8_t> desc_param1      { 0x04, str_param1, PropFormat::Hex };
const IntegerDescriptorT<uint8_t> desc_param2      { 0x05, str_param2, PropFormat::Hex };


} // namespace {


void Action0BRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<";
    os << severity_desc.value(m_severity) << ", ";
    os << language_iso(m_language_id) << "> // Action0B <severity, language>\n";
    os << pad(indent) << "{\n";

    desc_message_id.print(m_message_id, os, indent + 4);
    // Indicate the standard message, if this is one.
    const char* std_message = nullptr;
    switch (m_message_id)
    {
        case 0x00: std_message = str_message_00; break;
        case 0x01: std_message = str_message_01; break;
        case 0x02: std_message = str_message_02; break;
        case 0x03: std_message = str_message_03; break;
        case 0x04: std_message = str_message_04; break;
        case 0x05: std_message = str_message_05; break;
        case 0x06: std_message = str_message_06; break;
    }
    if (std_message != nullptr)
    {
        os << pad(indent + 4) << "// " << grf_string_to_readable_utf8(std_message) << "\n";
    }

    if (m_message_id == 0xFF)
    {
        desc_message.print(m_custom_message, os, indent +4);
    }

    desc_message_data.print(m_message_data, os, indent +4);
    if (m_num_params > 0)
    {
        desc_param1.print(m_param1, os, indent + 4);
    }
    if (m_num_params > 1)
    {
        desc_param2.print(m_param2, os, indent + 4);
    }

    os << pad(indent) << "}\n";
}


void Action0BRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    severity_desc.parse(m_severity, is);
    is.match(TokenType::Comma);
    m_language_id = language_id(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices.find(token.value);
        if (it != g_indices.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x01: desc_message_id.parse(m_message_id, is); break;
                case 0x02: desc_message.parse(m_custom_message, is); break;
                case 0x03: desc_message_data.parse(m_message_data, is); break;
                case 0x04: desc_param1.parse(m_param1, is); break;
                case 0x05: desc_param2.parse(m_param2, is); break;
            }

            is.match(TokenType::SemiColon);
        }
        else
        {
            throw ParserError("Unexpected identifier: " + token.value, token);
        }
    }

    is.match(TokenType::CloseBrace);
}

