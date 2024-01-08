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
#include "Languages.h"
#include "Exceptions.h"
#include <map>


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
