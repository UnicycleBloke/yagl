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
#include "Descriptors.h"


namespace {


struct SnowLineDescriptor : PropertyDescriptor
{
    void print(const Action00GlobalSettings::SnowLine& snow_line, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        snow_line.print(os, indent);
    }

    void parse(Action00GlobalSettings::SnowLine& snow_line, TokenStream& is) const
    {
        snow_line.parse(is);
    }
};


struct GenderCaseDescriptor : PropertyDescriptor
{
    void print(const Action00GlobalSettings::GenderCase& gender_case, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        gender_case.print(os, indent);
    }

    void parse(Action00GlobalSettings::GenderCase& gender_case, TokenStream& is) const
    {
        gender_case.parse(is);
    }
};


constexpr const char* str_cost_base_multipliers      = "cost_base_multipliers";
constexpr const char* str_cargo_translation_table    = "cargo_translation_table";
constexpr const char* str_currency_display_names     = "currency_display_names";
constexpr const char* str_currency_multipliers       = "currency_multipliers";
constexpr const char* str_currency_separator         = "currency_separator";
constexpr const char* str_currency_is_postfix        = "currency_is_postfix";
constexpr const char* str_currency_symbols_prefix    = "currency_symbols_prefix";
constexpr const char* str_currency_symbols_suffix    = "currency_symbols_suffix";
constexpr const char* str_euro_introduction_dates    = "euro_introduction_dates";
constexpr const char* str_snow_line_table            = "snow_line_table";
constexpr const char* str_grf_overrides_source       = "grf_overrides_source";
constexpr const char* str_grf_overrides_target       = "grf_overrides_target";
constexpr const char* str_railtype_translation_table = "railtype_translation_table";
constexpr const char* str_gender_translation_table   = "gender_translation_table";
constexpr const char* str_case_translation_table     = "case_translation_table";
constexpr const char* str_plural_form                = "plural_form";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_cost_base_multipliers,      0x08'00 },
    { str_cargo_translation_table,    0x09'00 },
    { str_currency_display_names,     0x0A'00 },
    { str_currency_multipliers,       0x0B'00 },
    { str_currency_separator,         0x0C'00 },
    { str_currency_is_postfix,        0x0C'01 },
    { str_currency_symbols_prefix,    0x0D'00 },
    { str_currency_symbols_suffix,    0x0E'00 },
    { str_euro_introduction_dates,    0x0F'00 },
    { str_snow_line_table,            0x10'00 },
    { str_grf_overrides_source,       0x11'00 },
    { str_grf_overrides_target,       0x11'01 },
    { str_railtype_translation_table, 0x12'00 },
    { str_gender_translation_table,   0x13'00 },
    { str_case_translation_table,     0x14'00 },
    { str_plural_form,                0x15'00 },
};      


constexpr IntegerDescriptorT<uint8_t>  desc_08  = { 0x08, str_cost_base_multipliers,    PropFormat::Hex };
constexpr GRFLabelDescriptor           desc_09  = { 0x09, str_cargo_translation_table };
constexpr IntegerDescriptorT<uint16_t> desc_0A  = { 0x0A, str_currency_display_names,   PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_0B  = { 0x0B, str_currency_multipliers,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0C0 = { 0x0C, str_currency_separator,       PropFormat::Hex };
constexpr BooleanDescriptor            desc_0C1 = { 0x0C, str_currency_is_postfix };
constexpr GRFLabelDescriptor           desc_0D  = { 0x0D, str_currency_symbols_prefix };
constexpr GRFLabelDescriptor           desc_0E  = { 0x0E, str_currency_symbols_suffix };
constexpr IntegerDescriptorT<uint16_t> desc_0F  = { 0x0F, str_euro_introduction_dates,  PropFormat::Hex };
constexpr SnowLineDescriptor           desc_10  = { 0x10, str_snow_line_table };
constexpr GRFLabelDescriptor           desc_110 = { 0x11, str_grf_overrides_source };
constexpr GRFLabelDescriptor           desc_111 = { 0x11, str_grf_overrides_target };
constexpr GRFLabelDescriptor           desc_12  = { 0x12, str_railtype_translation_table };
constexpr GenderCaseDescriptor         desc_13  = { 0x13, str_gender_translation_table };
constexpr GenderCaseDescriptor         desc_14  = { 0x14, str_case_translation_table };
constexpr IntegerDescriptorT<uint8_t>  desc_15  = { 0x15, str_plural_form,              PropFormat::Hex };


} // namespace {


void Action00GlobalSettings::SnowLine::read(std::istream& is)
{
    for (uint8_t month = 0; month < 12; ++month)
    {
        for (uint8_t day = 0; day < 32; ++day)
        {
            m_snow_heights[month*32 + day] = read_uint8(is);
        }
    }
}


void Action00GlobalSettings::SnowLine::write(std::ostream& os) const
{
    for (uint8_t month = 0; month < 12; ++month)
    {
        for (uint8_t day = 0; day < 32; ++day)
        {
            write_uint8(os, m_snow_heights[month*32 + day]);
        }
    }
}


void Action00GlobalSettings::SnowLine::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "[\n";

    for (uint8_t month = 0; month < 12; ++month)
    {
        os << pad(indent + 4);
        for (uint8_t day = 0; day < 32; ++day)
        {
            os << to_hex(m_snow_heights[month*32 + day], true) << " ";
        }
        os << "\n";
    }

    os << pad(indent) << "];";
}


void Action00GlobalSettings::SnowLine::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);

    // TODO This is a bit crude - assumes that we have the correct 
    // number of values. Throws exception if we don't.
    for (uint8_t month = 0; month < 12; ++month)
    {
        for (uint8_t day = 0; day < 32; ++day)
        {
            uint8_t value = is.match_integer(); 
            m_snow_heights[month*32 + day] = value;
        }
    }

    is.match(TokenType::CloseBracket);
}


void Action00GlobalSettings::GenderCase::read(std::istream& is)
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


void Action00GlobalSettings::GenderCase::write(std::ostream& os) const
{
    for (const auto& item: m_items)
    {
        write_uint8(os, item.id);
        write_string(os, item.name);
    }
    write_uint8(os, 0x00);
}


void Action00GlobalSettings::GenderCase::print(std::ostream& os, uint16_t indent) const
{
    os << "[ ";

    for (const auto& item: m_items)
    {
        os << "{" << to_hex(item.id, true) << ": \"" << item.name << "\"} ";
    }

    os << " ];";
}


void Action00GlobalSettings::GenderCase::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);

    while (is.peek().type == TokenType::OpenBrace)
    {
        GenderCase::Item item;

        is.match(TokenType::OpenBrace);
        item.id = is.match_integer(); 
        is.match(TokenType::Colon);
        item.name = is.match(TokenType::String); 
        is.match(TokenType::CloseBrace);

        m_items.push_back(item);
    }

    is.match(TokenType::CloseBracket);
}


bool Action00GlobalSettings::read_property(std::istream& is, uint8_t property) 
{
    switch (property)
    {
        case 0x08: m_08_cost_base_multipliers      = read_uint8(is);  break;
        case 0x09: m_09_cargo_translation_table.read(is);             break;
        case 0x0A: m_0A_currency_display_names     = read_uint16(is); break;
        case 0x0B: m_0B_currency_multipliers       = read_uint32(is); break;
        case 0x0C: m_0C_currency_separator         = read_uint8(is);
                   m_0C_currency_is_postfix        = (read_uint8(is) == 0x01); break;
        case 0x0D: m_0D_currency_symbols_prefix.read(is);             break;
        case 0x0E: m_0E_currency_symbols_suffix.read(is);             break;
        case 0x0F: m_0F_euro_introduction_dates    = read_uint16(is); break;
        case 0x10: m_10_snow_line_table.read(is);                     break;
        case 0x11: m_11_grf_overrides_source.read(is);                
                   m_11_grf_overrides_target.read(is);                break;
        case 0x12: m_12_railtype_translation_table.read(is);          break;
        case 0x13: m_13_gender_translation_table.read(is);            break;
        case 0x14: m_14_case_translation_table.read(is);              break;
        case 0x15: m_15_plural_form                = read_uint8(is);  break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}   


bool Action00GlobalSettings::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint8(os, m_08_cost_base_multipliers);  break;
        case 0x09: m_09_cargo_translation_table.write(os); break;
        case 0x0A: write_uint16(os, m_0A_currency_display_names); break;
        case 0x0B: write_uint32(os, m_0B_currency_multipliers); break;
        case 0x0C: write_uint8(os, m_0C_currency_separator);
                   write_uint8(os, m_0C_currency_is_postfix ? 0x01 : 0x00); break;
        case 0x0D: m_0D_currency_symbols_prefix.write(os); break;
        case 0x0E: m_0E_currency_symbols_suffix.write(os); break;
        case 0x0F: write_uint16(os,m_0F_euro_introduction_dates); break;
        case 0x10: m_10_snow_line_table.write(os); break;
        case 0x11: m_11_grf_overrides_source.write(os);                
                   m_11_grf_overrides_target.write(os); break;
        case 0x12: m_12_railtype_translation_table.write(os); break;
        case 0x13: m_13_gender_translation_table.write(os); break;
        case 0x14: m_14_case_translation_table.write(os); break;
        case 0x15: write_uint8(os, m_15_plural_form); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    return true;
}


bool Action00GlobalSettings::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_cost_base_multipliers, os, indent); break;
        case 0x09: desc_09.print(m_09_cargo_translation_table, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_currency_display_names, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_currency_multipliers, os, indent); break;
        case 0x0C: desc_0C0.print(m_0C_currency_separator, os, indent); os << "\n";
                   desc_0C1.print(m_0C_currency_is_postfix, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_currency_symbols_prefix, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_currency_symbols_suffix, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_euro_introduction_dates, os, indent); break;
        case 0x10: desc_10.print(m_10_snow_line_table, os, indent); break;
        case 0x11: desc_110.print(m_11_grf_overrides_source, os, indent); os << "\n";
                   desc_111.print(m_11_grf_overrides_source, os, indent); break;
        case 0x12: desc_12.print(m_12_railtype_translation_table, os, indent); break;
        case 0x13: desc_13.print(m_13_gender_translation_table, os, indent); break;
        case 0x14: desc_14.print(m_14_case_translation_table, os, indent); break;
        case 0x15: desc_15.print(m_15_plural_form, os, indent); break;
        default:   throw RUNTIME_ERROR("Unknown property");
    }

    os << "\n";
    return true;
}   


bool Action00GlobalSettings::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_cost_base_multipliers, is); break;
            case 0x09'00: desc_09.parse(m_09_cargo_translation_table, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_currency_display_names, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_currency_multipliers, is); break;
            case 0x0C'00: desc_0C0.parse(m_0C_currency_separator, is); break;
            case 0x0C'01: desc_0C1.parse(m_0C_currency_is_postfix, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_currency_symbols_prefix, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_currency_symbols_suffix, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_euro_introduction_dates, is); break;
            case 0x10'00: desc_10.parse(m_10_snow_line_table, is); break;
            case 0x11'00: desc_110.parse(m_11_grf_overrides_source, is); break;
            case 0x11'01: desc_111.parse(m_11_grf_overrides_source, is); break;
            case 0x12'00: desc_12.parse(m_12_railtype_translation_table, is); break;
            case 0x13'00: desc_13.parse(m_13_gender_translation_table, is); break;
            case 0x14'00: desc_14.parse(m_14_case_translation_table, is); break;
            case 0x15'00: desc_15.parse(m_15_plural_form, is); break;
            default:      throw RUNTIME_ERROR("Unknown property");
        }

        return true;
    }

    throw RUNTIME_ERROR("Unknown property");
}

