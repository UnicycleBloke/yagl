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


struct LanguageInfo
{
    uint8_t     code;  // Index for the language used within OTTD  
    const char* iso;   // Short string representation of the language_region  
    const char* name;  // Long string representation of the language 
};


static const std::map<uint8_t, LanguageInfo> g_language_names = 
{
    // The ISO codes have mostly been looked up. A few have guessed region codes.
    // The intention is just to allow users to enter language codes without less
    // concern over spelling errors.
    { 0x00, { 0x00, "en_US", "English (US)" } },
    { 0x01, { 0x01, "en_GB", "English (GB)" } },
    { 0x02, { 0x02, "de_DE", "German" } },
    { 0x03, { 0x03, "fr_FR", "French" } },
    { 0x04, { 0x04, "es_ES", "Spanish" } },
    { 0x05, { 0x05, "eo_XX", "Esperanto" } }, // Constructed language - no region.
    { 0x06, { 0x06, "io_XX", "Ido" } },       // Derived from reformed Esperanto.
    { 0x07, { 0x07, "ru_RU", "Russian" } },
    { 0x08, { 0x08, "ga_IE", "Irish" } },     // Is the region code reasonable?
    { 0x09, { 0x09, "mt_MT", "Maltese" } },
    { 0x0A, { 0x0A, "ta_IN", "Tamil" } },
    { 0x0B, { 0x0B, "cv_RU", "Chuvash" } }, 
    { 0x0C, { 0x0C, "zh_TW", "Chinese (Traditional)" } },
    { 0x0D, { 0x0D, "sr_BA", "Serbian" } },
    { 0x0E, { 0x0E, "nn_NO", "Norwegian (Nynorsk)" } },
    { 0x0F, { 0x0F, "cy_GB", "Welsh" } },
    { 0x10, { 0x10, "be_BY", "Belarusian" } },
    { 0x11, { 0x11, "mr_IN", "Marathi" } },
    { 0x12, { 0x12, "fo_FO", "Faroese" } },
    { 0x13, { 0x13, "gd_GB", "Scottish Gaelic" } }, 
    { 0x14, { 0x14, "ar_EG", "Arabic (Egypt)" } },
    { 0x15, { 0x15, "cs_CZ", "Czech" } },
    { 0x16, { 0x16, "sk_SK", "Slovak" } },
    { 0x18, { 0x18, "bg_BG", "Bulgarian" } },
    { 0x1B, { 0x1B, "af_ZA", "Afrikaans" } },
    { 0x1E, { 0x1E, "el_GR", "Greek" } },
    { 0x1F, { 0x1F, "nl_NL", "Dutch" } },
    { 0x21, { 0x21, "eu_ES", "Basque" } },
    { 0x22, { 0x22, "ca_ES", "Catalan" } },
    { 0x23, { 0x23, "de_LU", "Luxembourgish" } }, // ISO code is likely wrong
    { 0x24, { 0x24, "hu_HU", "Hungarian" } },
    { 0x26, { 0x26, "mk_MK", "Macedonian" } },
    { 0x27, { 0x27, "it_IT", "Italian" } },
    { 0x28, { 0x28, "ro_RO", "Romanian" } },
    { 0x29, { 0x29, "is_IS", "Icelandic" } },
    { 0x2A, { 0x2A, "lv_LV", "Latvian" } },
    { 0x2B, { 0x2B, "lt_LT", "Lithuanian" } },
    { 0x2C, { 0x2C, "sl_SI", "Slovenian" } },
    { 0x2D, { 0x2D, "da_DK", "Danish" } },
    { 0x2E, { 0x2E, "sv_SE", "Swedish" } },
    { 0x2F, { 0x2F, "nb_NO", "Norwegian (Bokmal)" } },
    { 0x30, { 0x30, "pl_PL", "Polish" } },
    { 0x31, { 0x31, "gl_ES", "Galician" } },
    { 0x32, { 0x32, "fy_NL", "Frisian" } }, // Is the region code correct?
    { 0x33, { 0x33, "uk_UA", "Ukrainian" } },
    { 0x34, { 0x34, "et_EE", "Estonian" } },
    { 0x35, { 0x35, "fi_FI", "Finnish" } },
    { 0x36, { 0x36, "pt_PT", "Portuguese" } },
    { 0x37, { 0x37, "pt_BR", "Brazilian Portuguese" } },
    { 0x38, { 0x38, "hr_HR", "Croatian" } },
    { 0x39, { 0x39, "ja_JP", "Japanese" } },
    { 0x3A, { 0x3A, "ko_KR", "Korean" } },
    { 0x3C, { 0x3C, "ms_MY", "Malay" } },
    { 0x3D, { 0x3D, "en_AU", "English (AU)" } },
    { 0x3E, { 0x3E, "tr_TR", "Turkish" } },
    { 0x42, { 0x42, "th_TH", "Thai" } },
    { 0x54, { 0x54, "vi_VN", "Vietnamese" } },
    { 0x55, { 0x55, "es_MX", "Mexican Spanish" } },
    { 0x56, { 0x56, "zh_CN", "Chinese (Simplified)" } },
    { 0x5A, { 0x5A, "id_ID", "Indonesian" } },
    { 0x5C, { 0x5C, "ur_PK", "Urdu" } },
    { 0x61, { 0x61, "he_IL", "Hebrew" } },
    { 0x62, { 0x62, "fa_IR", "Persian" } },
    { 0x66, { 0x66, "latin", "Latin" } }, // No ISO code?
    { 0x7F, { 0x7F, "default", "Default" } },
};


std::string language_name(uint8_t language_id)
{
    const auto& it = g_language_names.find(language_id);    
    if (it != g_language_names.end())
    {
        return it->second.name;
    }

    return "<unknown language>";
}


std::string language_iso(uint8_t language_id)
{
    const auto& it = g_language_names.find(language_id);    
    if (it != g_language_names.end())
    {
        return it->second.iso;
    }

    return "<unknown>";
}


uint8_t language_id(const std::string& iso)
{
    for (const auto& lang: g_language_names)
    {
        if (lang.second.iso == iso)
            return lang.second.code;
    }

    throw RUNTIME_ERROR("Unknown language: " + iso);
}


// Helpers.
static std::u16string grf_string_latin1_to_utf16(const std::string& str);
static std::u16string grf_string_utf8_to_utf16(const std::string& str);
static void process_utf16_extension_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos);
static void process_utf16_control_codes(uint16_t u16, std::basic_ostringstream<char16_t>& os, 
    const std::string& str, uint16_t& pos);


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

    uint16_t length = str.size();
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

    uint16_t length = str.size();
    for (uint16_t pos = 0; pos < length; ++pos)
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

    uint16_t length = str.size();
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
                    const auto& it2 = g_extension_codes.find(c);
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




