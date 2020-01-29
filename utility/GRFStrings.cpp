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
#include "GRFStrings.h"
#include "StreamHelpers.h"
#include <map>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>


struct ControlCode
{
    uint8_t         code;         // The numeric value of the code as it appears in the string.
    const char16_t* name;         // The human readable name used to represent the code. 
    uint8_t         data_size;    // The number of bytes in the string used as arguments 
    const char*     description;
};


static const std::map<uint8_t, ControlCode> g_control_codes =
{
    { 0x01, { 0x01, u"x-off",       1, "X offset in next byte of string (variable space)" } },
    { 0x0D, { 0x0D, u"new-line",    0, "New line" } },
    { 0x0E, { 0x0E, u"small-font",  0, "Set small font size" } },
    { 0x0F, { 0x0F, u"large-font",  0, "Set large font size" } },
    { 0x1F, { 0x1F, u"xy-offs",     2, "X and Y offsets in next two bytes of string" } },
    { 0x22, { 0x22, u"dq",          0, "Double quote" } },
    { 0x7B, { 0x7B, u"sd",          0, "Print signed dword" } },
    { 0x7C, { 0x7C, u"sw",          0, "Print signed word" } },
    { 0x7D, { 0x7D, u"sb",          0, "Print signed byte" } },
    { 0x7E, { 0x7E, u"uw",          0, "Print unsigned word" } },
    { 0x7F, { 0x7F, u"sd-currency", 0, "Print dword in currency units" } },
    { 0x80, { 0x80, u"substring1",  0, "Print substring (text ID from stack)" } },
    { 0x81, { 0x81, u"substring2",  2, "Print substring (text ID in next 2 bytes of string)" } },
    { 0x82, { 0x82, u"d-m-year",    0, "Print date (day, month, year) (based on year 1920)" } },
    { 0x83, { 0x83, u"m-year",      0, "Print short date (month and year) (based on year 1920)" } },
    { 0x84, { 0x84, u"sw-speed",    0, "Print signed word in speed units" } },
    { 0x85, { 0x85, u"discard",     0, "Discard next word from stack" } },
    { 0x86, { 0x86, u"rotate",      0, "Rotate down top 4 words on stack" } },
    { 0x87, { 0x87, u"sw-litres",   0, "Print signed word in litres" } },
    { 0x88, { 0x88, u"blue",        0, "Blue" } },
    { 0x89, { 0x89, u"lt-gray",     0, "Light Gray" } },
    { 0x8A, { 0x8A, u"gold",        0, "Light Orange ('Gold')" } },
    { 0x8B, { 0x8B, u"red",         0, "Red" } },
    { 0x8C, { 0x8C, u"purple",      0, "Purple" } },
    { 0x8D, { 0x8D, u"gray-green",  0, "Gray-Green" } },
    { 0x8E, { 0x8E, u"orange",      0, "Orange" } },
    { 0x8F, { 0x8F, u"green",       0, "Green" } },
    { 0x90, { 0x90, u"yellow",      0, "Yellow" } },
    { 0x91, { 0x91, u"lt-green",    0, "Light Green" } },
    { 0x92, { 0x92, u"red-brown",   0, "Red-Brown" } },
    { 0x93, { 0x93, u"brown",       0, "Brown" } },
    { 0x94, { 0x94, u"white",       0, "White" } },
    { 0x95, { 0x95, u"lt-blue",     0, "Light Blue" } },
    { 0x96, { 0x96, u"dk-gray",     0, "Dark Gray" } },
    { 0x97, { 0x97, u"mauve",       0, "Mauve (grayish purple)" } },
    { 0x98, { 0x98, u"black",       0, "Black" } },
    { 0x99, { 0x99, u"switch-cc",   1, "Switch to company colour that follows in next byte (enabled by enhancegui)" } },
    { 0x9A, { 0x9A, u"ext",         0, "Extended format code in next byte:" } },
    { 0x9E, { 0x9E, u"euro",        0, "Euro character '�'" } },
    { 0x9F, { 0x9F, u"Y-umlaut",    0, "Capital Y umlaut '�'" } },
    { 0xA0, { 0xA0, u"scroll-up",   0, "Scroll button up" } },
    { 0xAA, { 0xAA, u"scroll-down", 0, "Scroll button down" } },
    { 0xAC, { 0xAC, u"tick",        0, "Tick mark" } },
    { 0xAD, { 0xAD, u"x",           0, "X mark" } },
    { 0xAF, { 0xAF, u"scroll-right", 0, "Scroll button right" } },
    { 0xB4, { 0xB4, u"train",       0, "Train symbol" } },
    { 0xB5, { 0xB5, u"truck",       0, "Truck symbol" } },
    { 0xB6, { 0xB6, u"bus",         0, "Bus symbol" } },
    { 0xB7, { 0xB7, u"plane",       0, "Plane symbol" } },
    { 0xB8, { 0xB8, u"ship",        0, "Ship symbol" } },
    { 0xB9, { 0xB9, u"super-1",     0, "Superscript -1" } },
    { 0xBC, { 0xBC, u"small-up",    0, "Small scroll button up" } },
    { 0xBD, { 0xBD, u"small-down",  0, "Small scroll button down"  } },
};


static const std::map<uint8_t, ControlCode> g_extension_codes =
{
    { 0x00, { 0x00, u"64-currency0",       0, "Display 64-bit value from stack in currency units" } },
    { 0x01, { 0x01, u"64-currency1",       0, "Display 64-bit value from stack in currency units" } },
    { 0x02, { 0x02, u"ignore-colour",      0, "Ignore next colour byte. Multiple instances will skip multiple colour bytes." } },
    { 0x03, { 0x03, u"push-w",             2, "WORD Push WORD onto the textref stack" } },
    { 0x04, { 0x04, u"unprint-b",          1, "BYTE Un-print the previous BYTE characters." } }, 
    { 0x05, { 0x05, u"internal5",          0, "For internal use only. Not valid in GRF files." } },
    { 0x06, { 0x06, u"b-hex",              0, "Print byte in hex" } },
    { 0x07, { 0x07, u"w-hex",              0, "Print word in hex" } },
    { 0x08, { 0x08, u"d-hex",              0, "Print dword in hex" } },
    { 0x09, { 0x09, u"internal9",          0, "For internal use only. Usage in NewGRFs will most likely crash TTDPatch." } },
    { 0x0A, { 0x0A, u"internalA",          0, "For internal use only. Usage in NewGRFs will most likely crash TTDPatch." } },
    { 0x0B, { 0x0B, u"64-hex",             0, "Print 64-bit value in hex" } },
    { 0x0C, { 0x0C, u"station",            0, "Print name of station with id in next textrefstack word" } },
    { 0x0D, { 0x0D, u"uw-tonnes",          0, "Print unsigned word in tonnes" } },
    { 0x0E, { 0x0E, u"gender",             1, "Set gender of string, NewGRF internal ID in next byte. Must be first in a string." } },
    { 0x0F, { 0x0F, u"case",               1, "Select case for next substring, NewGRF internal ID in next byte" } },
    { 0x10, { 0x10, u"list-value",         1, "Begin choice list value, NewGRF internal ID in next byte" } },
    { 0x11, { 0x11, u"list-default",       0, "Begin choice list default" } },
    { 0x12, { 0x12, u"end-list",           0, "End choice list" } },
    { 0x13, { 0x13, u"gender-list",        1, "Begin gender choice list, stack offset of substring to get gender from in next byte" } },
    { 0x14, { 0x14, u"case-list",          0, "Begin case choice list" } },
    { 0x15, { 0x15, u"plural-list",        1, "Begin plural choice list, stack offset of value to get plural for in next byte" } },
    { 0x16, { 0x16, u"dw-date",            0, "Print dword as date (day, month, year) (based on year 0)" } },
    { 0x17, { 0x17, u"dw-short-date",      0, "Print dword as short date (month and year) (based on year 0)" } },
    { 0x18, { 0x18, u"uw-hp",              0, "Print unsigned word in horse power" } },
    { 0x19, { 0x19, u"uw-volume",          0, "Print unsigned word as short volume" } },
    { 0x1A, { 0x1A, u"uw-weight",          0, "Print unsigned word as short weight" } },
};


// Helpers.
static std::u16string grf_string_latin1_to_utf16(const std::string& str);
static std::u16string grf_string_utf8_to_utf16(const std::string& str);
static void process_utf16_extension_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos);
static void process_utf16_control_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos);
static std::u16string grf_string_to_utf16(const std::string& str);
static std::u16string grf_string_utf16_to_readable_utf16(const std::u16string& str);


std::string grf_string_to_readable_utf8(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::u16string str16  = grf_string_to_utf16(str);
    std::u16string read16 = grf_string_utf16_to_readable_utf16(str16);
    return converter.to_bytes(read16);
}


// GRF strings are stored in one of two formats: Latin1 (sort of), or UTF8 (also sort of).
// We first determine which one before proceeding. Decoding is then in two phases. First
// we convert both formats to a common UTF16 format. This makes life a little easier. Second
// we convert the UTF16 string into readable UTF8 
std::u16string grf_string_to_utf16(const std::string& str)
{
    // We are UTF8 if the string begins with an upper case 'Thorn' character.
    if (str.size() >= 2)
    {
        uint8_t c0 = str[0];
        uint8_t c1 = str[1];
        if ((c0 == 0xC3) && (c1 == 0x9E))
        {
            return grf_string_utf8_to_utf16(str);
        }
    }

    return grf_string_latin1_to_utf16(str);
}


// Convert a Latin-1 string to UTF-16. The control codes are placed into the 
// private 0xE0xx area to disambiguate them.
std::u16string grf_string_latin1_to_utf16(const std::string& str)
{
    std::basic_ostringstream<char16_t> os;

    uint16_t length = uint16_t(str.size());
    for (uint16_t pos = 0; pos < length; ++pos)
    {
        uint8_t u16 = str[pos];
        process_utf16_control_codes(u16, os, str, pos);
    }

    return os.str();
}


// Convert a UTF-8 string to UTF-16. The control codes are placed into the 
// private 0xE0xx area to disambiguate them.
//
// Basically there are three possibilities:
//
// 1. Characters U+E020..U+E0FF in the E0xx Private Use Area do what their respective 
//    character xx would do in TTD, as do the control characters below U+0020
// 2. All other valid UTF-8 sequences display actual glyphs, if they are available
// 3. All invalid UTF-8 sequences do what their individual bytes would do in TTD
//
// To summarize, here's a handy table:
//
// Character   Encoding in UTF-8 mode 	Meaning
// ---------   ----------------------   -------
// 7E          7E                       Unicode Character 'TILDE' (~)
// 82          82 (invalid UTF-8)       Print date (day, month, year)
// 82          C2 82                    Display glyph for U+0082
// AC          AC (invalid UTF-8)       Tick mark
// AC          C2 AC                    Unicode Character 'NOT SIGN' (¬)
// E07E        EE 81 BE                 Print unsigned word
// E082        EE 82 82                 Print date (day, month, year)
// E0AC        EE 82 AC                 Tick mark 
//
// These examples go wrong on the first byte, what about going wrong on the second
// or subsequent bytes?
std::u16string grf_string_utf8_to_utf16(const std::string& str)
{
    // Constants to help avoid issues with unicode encoding. 
    static constexpr uint8_t MASK_2_BYTE = 0xE0;
    static constexpr uint8_t LEAD_2_BYTE = 0xC0;
    static constexpr uint8_t MASK_3_BYTE = 0xF0;
    static constexpr uint8_t LEAD_3_BYTE = 0xE0;
    static constexpr uint8_t MASK_4_BYTE = 0xF8;
    static constexpr uint8_t LEAD_4_BYTE = 0xF0;
    static constexpr uint8_t MASK_NEXT   = 0xC0;
    static constexpr uint8_t LEAD_NEXT   = 0x80;

    std::basic_ostringstream<char16_t> os;

    uint16_t length = uint16_t(str.size());
    //for (uint16_t pos = 0; pos < length; ++pos)
    // Skip the leading thorn character - we don't actually need this.
    for (uint16_t pos = 2; pos < length; ++pos)
    {  
        // Extract the next character from the UTF8 string. This is either a valid 
        // multibyte sequence, or we take the first byte.
        uint8_t  c0      = str[pos];
        uint16_t u16     = c0;
        bool     invalid = true;
        if (c0 < 0x80)
        {
            // One byte value
            u16 = c0;
        }
        else if ((c0 & MASK_4_BYTE) == LEAD_4_BYTE)
        {
            // Four byte value
            uint8_t c1 = str[pos+1];
            uint8_t c2 = str[pos+2];
            uint8_t c3 = str[pos+3];
            if (((c1 & MASK_NEXT) == LEAD_NEXT) && ((c2 & MASK_NEXT) == LEAD_NEXT) && ((c3 & MASK_NEXT) == LEAD_NEXT))
            {
                // We should encode this as UTF16. Very unlikely that we will see any of these.
                throw RUNTIME_ERROR("Character from outside the basic multilingual plane");

                //uint32_t u32;
                //u32   = (c0 & ~MASK_4_BYTE) << 18;
                //u32  |= (c1 & ~MASK_NEXT)   << 12;
                //u32  |= (c2 & ~MASK_NEXT)   << 6;
                //u32  |= (c3 & ~MASK_NEXT);
                //pos += 3;
                //invalid = false;
            } 
        }
        else if ((c0 & MASK_3_BYTE) == LEAD_3_BYTE)
        {
            // Three byte value
            uint8_t c1 = str[pos+1];
            uint8_t c2 = str[pos+2];
            if (((c1 & MASK_NEXT) == LEAD_NEXT) && ((c2 & MASK_NEXT) == LEAD_NEXT))
            {
                u16   = (c0 & ~MASK_3_BYTE) << 12;
                u16  |= (c1 & ~MASK_NEXT) << 6;
                u16  |= (c2 & ~MASK_NEXT);
                pos  += 2;
                invalid = false;
            } 
        }
        else if ((c0 & MASK_2_BYTE) == LEAD_2_BYTE)
        { 
            // Two byte value
            uint8_t c1 = str[pos+1];
            if ((c1 & MASK_NEXT) == LEAD_NEXT)
            {
                u16   = (c0 & ~MASK_2_BYTE) << 6;
                u16  |= (c1 & ~MASK_NEXT);
                pos  += 1;
                invalid = false;
            } 
        }

        // Check for control codes. This really just to extract any arguments that
        // might be in the string for some of the codes.
        if (invalid || (u16 <= 0x20) || ((u16 & 0xFF00) == 0xE000))
        {
            process_utf16_control_codes(u16, os, str, pos);
        }
        else
        {
            // This is not a control code.
            os << static_cast<char16_t>(u16);
        }            
    }

    return os.str();
}


// Convert a UTF-16 string with control codes into a UTF-16 string with human readable 
// versions of the codes. This can be converted to UTF-8 for display or printing.
// Could probably short circuit some of the conversion steps, but it seems wise to be 
// clear about what is going on.
std::u16string grf_string_utf16_to_readable_utf16(const std::u16string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::basic_ostringstream<char16_t> os;

    uint16_t length = uint16_t(str.size());
    for (uint16_t pos = 0; pos < length; ++pos)
    {
        char16_t c = str[pos];
        uint16_t u = c;
        if ((c & 0xFF00) == 0xE000)
        {
            // This is a control code.
            const auto& it = g_control_codes.find(c & 0x00FF);
            if (it != g_control_codes.end())
            {
                const ControlCode& code = it->second;
                os << u"{" << code.name;
                for (uint8_t d = 0; d < code.data_size; ++d)
                {
                    os << u" 0x" << converter.from_bytes(to_hex(str[++pos], false));
                }

                if ((c & 0xFF) == 0x9A)
                {
                    // This is an extension control code.
                    c = str[++pos];
                    const auto& it2 = g_extension_codes.find(c & 0x00FF);
                    if (it2 != g_extension_codes.end())
                    {
                        const ControlCode& code = it2->second;
                        os << u" " << code.name;
                        for (uint8_t d = 0; d < code.data_size; ++d)
                        {
                            os << u" 0x" << converter.from_bytes(to_hex(str[++pos], false));
                        }
                    }
                    else
                    {
                        throw RUNTIME_ERROR("unexpected extension code");
                    }                    
                }

                os << u"}";
            }
            else
            {
                // Shouldn't get here. It means the string contains a control code we don't know about. 
                // But the string was processed with the list that we do know about already...
                os <<  u"{<unknown> 0x" << converter.from_bytes(to_hex(str[++pos], false)) << u"}";
            }
        }
        else if (c == '"')
        {
            // This is a bit of a pain. We need to escape double quotes because these are 
            // important when parsing the YAGL in again. Also going to need an escape for '{'
            // in case anyone wants to use it in strings.
            os << u"{dq}";
        }
        else
        {
            os << c;
        }
    }

    return os.str();
}


// Insert the given character into the stream and any arguments for the control codes.
static void process_utf16_extension_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos)
{
    // This may be an extension code. 
    if ((u16 & 0xFF) == 0x9A)
    {
        uint8_t c = str[++pos];
        const auto& it2 = g_extension_codes.find(c);
        if (it2 != g_extension_codes.end())
        {
            // This is an extension control code. No need to place 
            // this extension sub-code into 0xE0xx.
            const ControlCode& code = it2->second;
            os << static_cast<char16_t>(c);

            // Arguments for the code, if any.
            for (uint8_t d = 0; d < code.data_size; ++d)
            {
                os << static_cast<char16_t>(str[++pos]);
            }
        }
        else
        {
            throw RUNTIME_ERROR("unexpected extension code");
        }                    
    }
}


// Insert the given character into the stream and any arguments for the control codes.
static void process_utf16_control_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos)
{
    // This may be a control code.
    const auto& it = g_control_codes.find(u16 & 0xFF);
    if (it != g_control_codes.end())
    {
        // This is a control code. Store in the range 0xE0xx.
        const ControlCode& code = it->second;
        os << static_cast<char16_t>(u16 | 0xE000);

        // Arguments for the code, if any.
        for (uint8_t d = 0; d < code.data_size; ++d)
        {
            os << static_cast<char16_t>(str[++pos]);
        }

        process_utf16_extension_codes(u16, os, str, pos);
    }
    else
    {
        // This is not a control code.
        os << static_cast<char16_t>(u16);
    }            
}


std::string read_string(std::istream& is)
{
    std::string result;
    std::getline(is, result, char(0));

    if (is.fail())
    {
        throw RUNTIME_ERROR("read_string failed");
    }

    return result;
}


void write_string(std::ostream& os, const std::string& value, StringTerm term)
{
    // Optionally write without a 0 terminator - one or two strings in the GRF are terminated by end of record or whatever.
    os.write(value.c_str(), value.length() + ((term == StringTerm::None) ? 0 : 1));
}


void write_string(std::ostream& os, const std::string& value)
{
    write_string(os, value, StringTerm::Null);
}


// TODO this is such bloaty code... the goal was to make sure to get something working.
// Can always refactor later once some tests added.


static const ControlCode& find_extension_code(const std::u16string& name)
{
    for (const auto& item: g_extension_codes)
    {
        const ControlCode& control = item.second;
        if (control.name == name)
        {
            return control;
        }
    }
    throw RUNTIME_ERROR("Unknown extension code");
}


static void parse_extension_code(std::vector<std::u16string>& args, std::basic_ostringstream<char16_t>& os)
{
    const ControlCode& extension = find_extension_code(args[1]);
    os << char16_t(extension.code);

    if (args.size() == (extension.data_size + 2))
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    
        for (uint8_t i = 0; i < extension.data_size; ++i)
        {
            std::string arg = converter.to_bytes(args[i + 2]);
            char16_t value = strtol(arg.c_str() + 2, nullptr, 16);
            os << value;
        }
    }
    else 
    {
        throw RUNTIME_ERROR("Incorrect number of control arguments");
    }
}


static const ControlCode& find_control_code(const std::u16string& name)
{
    for (const auto& item: g_control_codes)
    {
        const ControlCode& control = item.second;
        if (control.name == name)
        {
            return control;
        }
    }
    throw RUNTIME_ERROR("Unknown control code");
}


static void parse_control_code(std::vector<std::u16string>& args, std::basic_ostringstream<char16_t>& os)
{
    const ControlCode& control = find_control_code(args[0]);
    os << char16_t(control.code | 0xE000);

    if (control.code == 0x9A)
    {
        parse_extension_code(args, os);
    }
    else
    {
        if (args.size() == (control.data_size + 1))
        {
           std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
      
           for (uint8_t i = 0; i < control.data_size; ++i)
            {
                std::string arg = converter.to_bytes(args[i + 1]);
                char16_t value = char16_t(strtol(arg.c_str() + 2, nullptr, 16));
                os << value;
            }
        }
        else
        {
            throw RUNTIME_ERROR("Incorrect number of extension arguments");
        }
    }
}


static std::u16string readable_utf16_to_binary_utf16(const std::u16string& str)
{
    enum class State { Normal, Brace, Control };    
    State state = State::Normal;

    // Output string with text control codes converted to binary equivalents.
    std::basic_ostringstream<char16_t> os;

    // Probably not the most efficient method of finding the control codes within the string, 
    // but we can refactor later. At least this is simple to understand. The vector of strings 
    // is to deal with extension codes and arguments.
    std::basic_ostringstream<char16_t> cs;
    std::vector<std::u16string> args;

    uint16_t length = uint16_t(str.length());
    for (uint16_t pos = 0; pos < length; ++pos)
    {
        char16_t u16 = str[pos];
        switch (state)
        {
            case State::Normal:
                // '{' is the escape sequence which potentially begins a control code,
                // or just accept the character.
                if (u16 == u'{')
                {
                    state = State::Brace;
                }
                else
                {
                    os << u16;
                }
                break;

            case State::Brace:
                // '{{' is the escape sequence meaning a literal '{',
                // or we have started a control code sequence.
                if (u16 == u'{')
                {
                    state = State::Normal;
                    os << u'{';
                    break;
                }
                state = State::Control;
                // Fall through

            case State::Control:
                if (u16 == u'}')
                {
                    args.push_back(cs.str());
                    cs.str(u"");

                    // Process control codes we found.
                    parse_control_code(args, os);

                    args.clear();
                    state = State::Normal;
                }
                else if (std::isspace(u16))    
                {
                    // This seems to ignore empty strings, which is what we want.
                    args.push_back(cs.str());
                    cs.str(u"");
                }
                else
                {
                    // This is a control code character. 
                    cs << u16;
                }
                break;
        }
    }

    auto result = os.str();
    auto c = result[0];
    return result;
}


static void convert_utf32_utf8(uint32_t value, std::ostringstream& os)
{
    if (value < 0x80)
    {
        os << uint8_t(value);
    }
    else if (value < 0x800)
    {
        os << uint8_t(((value >> 6) & 0x1F) | 0xC0);
        os << uint8_t((value & 0x3F) | 0x80);
    }
    else if (value < 0x10000)
    {
        os << uint8_t(((value >> 12) & 0x0F) | 0xE0);
        os << uint8_t(((value >> 6) & 0x3F) | 0x80);
        os << uint8_t((value & 0x3F) | 0x80);
    }
    else if (value < 0x110000)
    {
        value = value - 0x10000;
        os << uint8_t(((value >> 18) & 0x07) | 0xF0);
        os << uint8_t(((value >> 12) & 0x3F) | 0x80);
        os << uint8_t(((value >> 6) & 0x3F) | 0x80);
        os << uint8_t((value & 0x3F) | 0x80);
    }
}


static bool binary_utf16_is_utf8(const std::u16string& str)
{
    uint16_t length = uint16_t(str.length());
    uint16_t pos = 0;
    while (pos < length)
    {
        // Items are mostly converted to UTF8 ...
        auto c = str[pos++];

        // ... but control codes get further treatment.   
        if ((c & 0xFF00) == 0xE000)
        {
            // If this is an extension, the index is placed into the string without special encoding.
            auto it = g_control_codes.find(c & 0xFF);
            if (it->second.code == 0x9A)
            {
                c = str[pos++];
                it = g_extension_codes.find(c & 0xFF);
            }

            // If the control or extension code has arguments, these are places into the string 
            // without special encoding.
            for (uint8_t arg = 0; arg < it->second.data_size; ++arg)
            {
                c = str[pos++];
            }
        }
        else if (c >= 0x80)
        {
            return true;
        }
    }

    return false;
}


static std::string binary_utf16_to_utf8(const std::u16string& str)
{
    std::ostringstream os;
    // Thorn indicates that this string is encoded with UTF8.
    convert_utf32_utf8(0x00DE, os);

    uint16_t length = uint16_t(str.length());
    uint16_t pos = 0;
    while (pos < length)
    {
        // Items are mostly converted to UTF8 ...
        auto c = str[pos++];

        // ... but control codes get further treatment.   
        if ((c & 0xFF00) == 0xE000)
        {
            if (c == 0xE00D)
            {
                os << uint8_t(c);
            } 
            else
            {
                convert_utf32_utf8(c, os);
            }        

            // If this is an extension, the index is placed into the string without special encoding.
            auto it = g_control_codes.find(c & 0xFF);
            if (it->second.code == 0x9A)
            {
                c = str[pos++];
                os << uint8_t(c);

                it = g_extension_codes.find(c & 0xFF);
            }

            // If the control or extension code has arguments, these are places into the string 
            // without special encoding.
            for (uint8_t arg = 0; arg < it->second.data_size; ++arg)
            {
                c = str[pos++];
                os << uint8_t(c);
            }
        }
        else
        {
            convert_utf32_utf8(c, os);
        }
        
    }

    return os.str();
}


static std::string binary_utf16_to_latin1(const std::u16string& str)
{
    std::ostringstream os;

    uint16_t length = uint16_t(str.length());
    uint16_t pos = 0;
    while (pos < length)
    {
        // Items are mostly converted to UTF8 ...
        auto c = str[pos++];

        // ... but control codes get further treatment.   
        if ((c & 0xFF00) == 0xE000)
        {
            os << uint8_t(c);

            // If this is an extension, the index is placed into the string without special encoding.
            auto it = g_control_codes.find(c & 0xFF);
            if (it->second.code == 0x9A)
            {
                c = str[pos++];
                os << uint8_t(c);

                it = g_extension_codes.find(c & 0xFF);
            }

            // If the control or extension code has arguments, these are places into the string 
            // without special encoding.
            for (uint8_t arg = 0; arg < it->second.data_size; ++arg)
            {
                c = str[pos++];
                os << uint8_t(c);
            }
        }
        else
        {
            os << uint8_t(c);
        }
        
    }

    return os.str();
}


static std::string readable_utf8_to_grf_string(const std::string& str)
{
    // Input is the string that comes from the YAGL.

    // Convert to UTF16 as an intermediate (all characters are single elements).
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::u16string readable_utf16 = converter.from_bytes(str);

    // Translate the control codes into their UTF16 binary form.
    std::u16string binary_u16 = readable_utf16_to_binary_utf16(readable_utf16);

    // Test for non-Latin characters, other than control codes and their arguments.
    std::string result;
    if (binary_utf16_is_utf8(binary_u16))
    {
        result = binary_utf16_to_utf8(binary_u16);
    }
    else
    {
        result = binary_utf16_to_latin1(binary_u16);
    }
    return result;
}


void GRFString::read(std::istream& is, StringTerm term)
{
    if (term == StringTerm::None)
    {
        m_value = {};
        while (is.peek() != EOF)
        {
            m_value.push_back(read_uint8(is));
        }        
    }
    else
    {
        m_value = read_string(is);
    }
}


void GRFString::write(std::ostream& os, StringTerm term) const
{
    write_string(os, m_value, term);
}


void GRFString::print(std::ostream& os) const
{
    os << "\"" << readable() << "\"";
}


void string_to_hex(const std::string& str)
{
    for (uint8_t c: str)
    {
        std::cout << to_hex(c, false) << ' ';
    }
    std::cout << "\n\n";
}


std::string GRFString::readable() const
{
    std::string readable = grf_string_to_readable_utf8(m_value);

    // This is just for testing. Convert the string to readable
    // and back again, and see if it matches. There are some 
    // discrepancies, but these are so far due to the fact that
    // the GRF string encoding is non-unique.
    std::string binary   = readable_utf8_to_grf_string(readable);    
    if (m_value != binary)
    {
        string_to_hex(m_value);
        string_to_hex(binary);
    }

    return readable;
}


void GRFString::parse(TokenStream& is)
{
    std::string readable = is.match(TokenType::String);
    m_value = readable_utf8_to_grf_string(readable);
}

