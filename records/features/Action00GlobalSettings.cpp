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
#include "Action00GlobalSettings.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_cost_base_multipliers      = "cost_base_multipliers";
constexpr const char* str_cargo_translation_table    = "cargo_translation_table";
constexpr const char* str_currency_display_names     = "currency_display_names";
constexpr const char* str_currency_multipliers       = "currency_multipliers";
constexpr const char* str_currency_options           = "currency_options";
constexpr const char* str_currency_symbols_prefix    = "currency_symbols_prefix";
constexpr const char* str_currency_symbols_suffix    = "currency_symbols_suffix";
constexpr const char* str_euro_introduction_dates    = "euro_introduction_dates";
constexpr const char* str_snow_line_table            = "snow_line_table";
constexpr const char* str_grf_overrides              = "grf_overrides";
constexpr const char* str_railtype_translation_table = "railtype_translation_table";
constexpr const char* str_gender_translation_table   = "gender_translation_table";
constexpr const char* str_case_translation_table     = "case_translation_table";
constexpr const char* str_plural_form                = "plural_form";
constexpr const char* str_roadtype_translation_table = "roadtype_translation_table";
constexpr const char* str_tramtype_translation_table = "tramtype_translation_table";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_cost_base_multipliers,      0x08 },
    { str_cargo_translation_table,    0x09 },
    { str_currency_display_names,     0x0A },
    { str_currency_multipliers,       0x0B },
    { str_currency_options,           0x0C },
    { str_currency_symbols_prefix,    0x0D },
    { str_currency_symbols_suffix,    0x0E },
    { str_euro_introduction_dates,    0x0F },
    { str_snow_line_table,            0x10 },
    { str_grf_overrides,              0x11 },
    { str_railtype_translation_table, 0x12 },
    { str_gender_translation_table,   0x13 },
    { str_case_translation_table,     0x14 },
    { str_plural_form,                0x15 },
    { str_roadtype_translation_table, 0x16 },
    { str_tramtype_translation_table, 0x17 },
};


using SnowLineDescriptor   = GenericDescriptor<SnowLine, true>; // Forward the indent
using GenderCaseDescriptor = GenericDescriptor<GenderCase>;


constexpr UInt8Descriptor        desc_08  = { 0x08, str_cost_base_multipliers,    UIntFormat::Hex };
constexpr GRFLabelDescriptor     desc_09  = { 0x09, str_cargo_translation_table };
constexpr UInt16Descriptor       desc_0A  = { 0x0A, str_currency_display_names,   UIntFormat::Hex };
constexpr UInt32Descriptor       desc_0B  = { 0x0B, str_currency_multipliers,     UIntFormat::Hex };
constexpr UInt16Descriptor       desc_0C  = { 0x0C, str_currency_options,         UIntFormat::Hex };
constexpr GRFLabelDescriptor     desc_0D  = { 0x0D, str_currency_symbols_prefix };
constexpr GRFLabelDescriptor     desc_0E  = { 0x0E, str_currency_symbols_suffix };
constexpr UInt16Descriptor       desc_0F  = { 0x0F, str_euro_introduction_dates,  UIntFormat::Hex };
constexpr SnowLineDescriptor     desc_10  = { 0x10, str_snow_line_table };
constexpr GRFLabelPairDescriptor desc_11  = { 0x11, str_grf_overrides };
constexpr GRFLabelDescriptor     desc_12  = { 0x12, str_railtype_translation_table };
constexpr GenderCaseDescriptor   desc_13  = { 0x13, str_gender_translation_table };
constexpr GenderCaseDescriptor   desc_14  = { 0x14, str_case_translation_table };
constexpr UInt8Descriptor        desc_15  = { 0x15, str_plural_form,              UIntFormat::Hex };
constexpr GRFLabelDescriptor     desc_16  = { 0x16, str_roadtype_translation_table };
constexpr GRFLabelDescriptor     desc_17  = { 0x17, str_tramtype_translation_table };


} // namespace {


void SnowLine::read(std::istream& is)
{
    for (auto& value: m_snow_heights)
    {
        value = read_uint8(is);
    }
}


void SnowLine::write(std::ostream& os) const
{
    for (auto value : m_snow_heights)
    {
        write_uint8(os, value);
    }
}


void SnowLine::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "[\n";

    uint16_t index = 0;
    for (uint8_t month = 0; month < 12; ++month)
    {
        os << pad(indent + 4);
        for (uint8_t day = 0; day < 32; ++day)
        {
            os << to_hex(m_snow_heights[index++]) << " ";
        }
        os << "\n";
    }

    os << pad(indent) << "]";
}


void SnowLine::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);

    // This is a bit crude - assumes that we have the correct
    // number of values. Throws exception if we don't.
    for (auto& value : m_snow_heights)
    {
        value = is.match_uint8();
    }

    is.match(TokenType::CloseBracket);
}


void GenderCase::read(std::istream& is)
{
    while (is.peek() != 0x00)
    {
        Item item;
        item.id   = read_uint8(is);
        item.name = read_string(is);
        m_items.push_back(item);
    }
    read_uint8(is);
}


void GenderCase::write(std::ostream& os) const
{
    for (const auto& item: m_items)
    {
        write_uint8(os, item.id);
        write_string(os, item.name);
    }
    write_uint8(os, 0x00);
}


void GenderCase::print(std::ostream& os) const
{
    os << "[ ";

    for (const auto& item: m_items)
    {
        os << to_hex(item.id) << ":\"" << item.name << "\" ";
    }

    os << "]";
}


void GenderCase::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        GenderCase::Item item;

        item.id = is.match_uint8();
        is.match(TokenType::Colon);
        item.name = is.match(TokenType::String);

        m_items.push_back(item);
    }

    is.match(TokenType::CloseBracket);
}


bool Action00GlobalSettings::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_cost_base_multipliers.read(is);      break;
        case 0x09: m_09_cargo_translation_table.read(is);    break;
        case 0x0A: m_0A_currency_display_names.read(is);     break;
        case 0x0B: m_0B_currency_multipliers.read(is);       break;
        case 0x0C: m_0C_currency_options.read(is);           break;
        case 0x0D: m_0D_currency_symbols_prefix.read(is);    break;
        case 0x0E: m_0E_currency_symbols_suffix.read(is);    break;
        case 0x0F: m_0F_euro_introduction_dates.read(is);    break;
        case 0x10: m_10_snow_line_table.read(is);            break;
        case 0x11: m_11_grf_overrides.read(is);              break;
        case 0x12: m_12_railtype_translation_table.read(is); break;
        case 0x13: m_13_gender_translation_table.read(is);   break;
        case 0x14: m_14_case_translation_table.read(is);     break;
        case 0x15: m_15_plural_form.read(is);                break;
        case 0x16: m_16_roadtype_translation_table.read(is); break;
        case 0x17: m_17_tramtype_translation_table.read(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00GlobalSettings::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_cost_base_multipliers.write(os);      break;
        case 0x09: m_09_cargo_translation_table.write(os);    break;
        case 0x0A: m_0A_currency_display_names.write(os);     break;
        case 0x0B: m_0B_currency_multipliers.write(os);       break;
        case 0x0C: m_0C_currency_options.write(os);           break;
        case 0x0D: m_0D_currency_symbols_prefix.write(os);    break;
        case 0x0E: m_0E_currency_symbols_suffix.write(os);    break;
        case 0x0F: m_0F_euro_introduction_dates.write(os);    break;
        case 0x10: m_10_snow_line_table.write(os);            break;
        case 0x11: m_11_grf_overrides.write(os);              break;
        case 0x12: m_12_railtype_translation_table.write(os); break;
        case 0x13: m_13_gender_translation_table.write(os);   break;
        case 0x14: m_14_case_translation_table.write(os);     break;
        case 0x15: m_15_plural_form.write(os);                break;
        case 0x16: m_16_roadtype_translation_table.write(os); break;
        case 0x17: m_17_tramtype_translation_table.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00GlobalSettings::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_cost_base_multipliers, os, indent);      break;
        case 0x09: desc_09.print(m_09_cargo_translation_table, os, indent);    break;
        case 0x0A: desc_0A.print(m_0A_currency_display_names, os, indent);     break;
        case 0x0B: desc_0B.print(m_0B_currency_multipliers, os, indent);       break;
        case 0x0C: desc_0C.print(m_0C_currency_options, os, indent);           break;
        case 0x0D: desc_0D.print(m_0D_currency_symbols_prefix, os, indent);    break;
        case 0x0E: desc_0E.print(m_0E_currency_symbols_suffix, os, indent);    break;
        case 0x0F: desc_0F.print(m_0F_euro_introduction_dates, os, indent);    break;
        case 0x10: desc_10.print(m_10_snow_line_table, os, indent);            break;
        case 0x11: desc_11.print(m_11_grf_overrides, os, indent);              break;
        case 0x12: desc_12.print(m_12_railtype_translation_table, os, indent); break;
        case 0x13: desc_13.print(m_13_gender_translation_table, os, indent);   break;
        case 0x14: desc_14.print(m_14_case_translation_table, os, indent);     break;
        case 0x15: desc_15.print(m_15_plural_form, os, indent);                break;
        case 0x16: desc_16.print(m_16_roadtype_translation_table, os, indent); break;
        case 0x17: desc_17.print(m_17_tramtype_translation_table, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00GlobalSettings::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_cost_base_multipliers, is);      break;
            case 0x09: desc_09.parse(m_09_cargo_translation_table, is);    break;
            case 0x0A: desc_0A.parse(m_0A_currency_display_names, is);     break;
            case 0x0B: desc_0B.parse(m_0B_currency_multipliers, is);       break;
            case 0x0C: desc_0C.parse(m_0C_currency_options, is);           break;
            case 0x0D: desc_0D.parse(m_0D_currency_symbols_prefix, is);    break;
            case 0x0E: desc_0E.parse(m_0E_currency_symbols_suffix, is);    break;
            case 0x0F: desc_0F.parse(m_0F_euro_introduction_dates, is);    break;
            case 0x10: desc_10.parse(m_10_snow_line_table, is);            break;
            case 0x11: desc_11.parse(m_11_grf_overrides, is);              break;
            case 0x12: desc_12.parse(m_12_railtype_translation_table, is); break;
            case 0x13: desc_13.parse(m_13_gender_translation_table, is);   break;
            case 0x14: desc_14.parse(m_14_case_translation_table, is);     break;
            case 0x15: desc_15.parse(m_15_plural_form, is);                break;
            case 0x16: desc_16.parse(m_16_roadtype_translation_table, is); break;
            case 0x17: desc_17.parse(m_17_tramtype_translation_table, is); break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}

