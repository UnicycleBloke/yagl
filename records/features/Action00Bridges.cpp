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
#include "Action00Bridges.h"
#include "Descriptors.h"


namespace {


struct BridgeLayoutDescriptor : PropertyDescriptor
{
    void print(const Action00Bridges::BridgeLayout& layouts, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Bridges::BridgeLayout& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


constexpr const char* str_fallback_type_id    = "fallback_type_id";
constexpr const char* str_year_available      = "year_available";
constexpr const char* str_minimum_length      = "minimum_length";
constexpr const char* str_maximum_length      = "maximum_length";
constexpr const char* str_cost_factor         = "cost_factor";
constexpr const char* str_maximum_speed       = "maximum_speed";
constexpr const char* str_bridge_layout       = "bridge_layout";
constexpr const char* str_various_flags       = "various_flags";
constexpr const char* str_long_year_available = "long_year_available";
constexpr const char* str_purchase_text       = "purchase_text";
constexpr const char* str_description_rail    = "description_rail";
constexpr const char* str_description_road    = "description_road";
constexpr const char* str_cost_factor_word    = "cost_factor_word";
constexpr const char* str_layout              = "layout";
constexpr const char* str_table               = "table";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_fallback_type_id,    0x00'00 },
    { str_year_available,      0x08'00 },
    { str_minimum_length,      0x09'00 },
    { str_maximum_length,      0x0A'00 },
    { str_cost_factor,         0x0B'00 },
    { str_maximum_speed,       0x0C'00 },
    { str_bridge_layout,       0x0D'00 },
    { str_various_flags,       0x0E'00 },
    { str_long_year_available, 0x0F'00 },
    { str_purchase_text,       0x10'00 },
    { str_description_rail,    0x11'00 },
    { str_description_road,    0x12'00 },
    { str_cost_factor_word,    0x13'00 },
};


constexpr IntegerDescriptorT<uint8_t>  desc_00 = { 0x00, str_fallback_type_id,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_08 = { 0x08, str_year_available,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_09 = { 0x09, str_minimum_length,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0A = { 0x0A, str_maximum_length,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0B = { 0x0B, str_cost_factor,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0C = { 0x0C, str_maximum_speed,       PropFormat::Hex };
constexpr BridgeLayoutDescriptor       desc_0D = { 0x0D, str_bridge_layout };
constexpr IntegerDescriptorT<uint8_t>  desc_0E = { 0x0E, str_various_flags,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_0F = { 0x0F, str_long_year_available, PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_10 = { 0x10, str_purchase_text,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_11 = { 0x11, str_description_rail,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_12 = { 0x12, str_description_road,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_13 = { 0x13, str_cost_factor_word,    PropFormat::Hex };


} // namespace {


void Action00Bridges::BridgeTable::read(std::istream& is)
{
    for (uint32_t& sprite: m_sprites)
    {
        sprite = read_uint32(is);
    }
}


void Action00Bridges::BridgeTable::write(std::ostream& os) const
{
    for (const uint32_t& sprite: m_sprites)
    {
        write_uint32(os, sprite);
    }
}


void Action00Bridges::BridgeTable::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "{\n"; 
 
    uint16_t index = 0;
    for (const uint32_t& sprite: m_sprites)
    {
        if ((index % 8) == 0) 
        {
            os << pad(indent + 4);
        }
        
        os << to_hex(sprite) << " ";
        
        ++index;
        if ((index % 8) == 0) 
        {
            os << "\n"; 
        }
    }

    os << pad(indent) << "}\n"; 
}


void Action00Bridges::BridgeTable::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    for (uint32_t& sprite: m_sprites)
    {
        // We expect precisely 32 sprite indices. Anything else will lead to an exception.
        // More than 32 will throw when matching a brace chokes on an integer.
        // Fewer than 32 will throw when matching an integer chokes on a brace.
        sprite = is.match_integer();
    }
    is.match(TokenType::CloseBrace);
}


void Action00Bridges::BridgeLayout::read(std::istream& is)
{
    m_first_table_id = read_uint8(is);

    uint8_t num_tables = read_uint8(is);
    m_tables.resize(num_tables);
    for (uint8_t i = 0; i < num_tables; ++i)
    {
        m_tables[i].read(is);
    }
}


void Action00Bridges::BridgeLayout::write(std::ostream& os) const
{
    write_uint8(os, m_first_table_id);

    write_uint8(os, uint8_t(m_tables.size()));
    for (const auto& table: m_tables)
    {
        table.write(os);
    }
}


void Action00Bridges::BridgeLayout::print(std::ostream& os, uint16_t indent) const
{
    uint8_t table_id = m_first_table_id;

    //os << pad(indent) << str_layout << "\n"; 
    os << "\n"; 
    os << pad(indent) << "{\n"; 
    for (const auto& table: m_tables)
    {
        os << pad(indent + 4) << str_table << "<" << to_hex(table_id++) << ">\n"; 
        table.print(os, indent + 4);
    }
    os << pad(indent) << "};\n"; 
}


void Action00Bridges::BridgeLayout::parse(TokenStream& is)
{
    //is.match_ident(str_layout);
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        is.match_ident(str_table);
        is.match(TokenType::OpenAngle);
        uint8_t table_id = is.match_integer();
        is.match(TokenType::CloseAngle);

        BridgeTable table = {};
        table.parse(is);

        m_tables.push_back(table);
        if (m_tables.size() == 1)
        {
            m_first_table_id = table_id;
        }
    }
    is.match(TokenType::CloseBrace);
}


bool Action00Bridges::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x00: m_00_fallback_type_id    = read_uint8(is); break;
        case 0x08: m_08_year_available      = read_uint8(is); break;
        case 0x09: m_09_minimum_length      = read_uint8(is); break;
        case 0x0A: m_0A_maximum_length      = read_uint8(is); break;
        case 0x0B: m_0B_cost_factor         = read_uint8(is); break;
        case 0x0C: m_0C_maximum_speed       = read_uint16(is); break;
        case 0x0D: m_0D_bridge_layout.read(is); break;
        case 0x0E: m_0E_various_flags       = read_uint8(is); break;
        case 0x0F: m_0F_long_year_available = read_uint32(is); break;
        case 0x10: m_10_purchase_text       = read_uint16(is); break;
        case 0x11: m_11_description_rail    = read_uint16(is); break;
        case 0x12: m_12_description_road    = read_uint16(is); break;
        case 0x13: m_13_cost_factor_word    = read_uint16(is); break;
        case 0x14: break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Bridges::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x00: write_uint8(os, m_00_fallback_type_id); break;
        case 0x08: write_uint8(os, m_08_year_available); break;
        case 0x09: write_uint8(os, m_09_minimum_length); break;
        case 0x0A: write_uint8(os, m_0A_maximum_length); break;
        case 0x0B: write_uint8(os, m_0B_cost_factor); break;
        case 0x0C: write_uint16(os, m_0C_maximum_speed); break;
        case 0x0D: m_0D_bridge_layout.write(os); break;
        case 0x0E: write_uint8(os, m_0E_various_flags); break;
        case 0x0F: write_uint32(os, m_0F_long_year_available); break;
        case 0x10: write_uint16(os, m_10_purchase_text); break;
        case 0x11: write_uint16(os, m_11_description_rail); break;
        case 0x12: write_uint16(os, m_12_description_road); break;
        case 0x13: write_uint16(os, m_13_cost_factor_word); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Bridges::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x00: desc_00.print(m_00_fallback_type_id, os, indent); break;
        case 0x08: desc_08.print(m_08_year_available, os, indent); break;
        case 0x09: desc_09.print(m_09_minimum_length, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_maximum_length, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_cost_factor, os, indent); break;
        case 0x0C: desc_0C.print(m_0C_maximum_speed, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_bridge_layout, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_various_flags, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_long_year_available, os, indent); break;
        case 0x10: desc_10.print(m_10_purchase_text, os, indent); break;
        case 0x11: desc_11.print(m_11_description_rail, os, indent); break;
        case 0x12: desc_12.print(m_12_description_road, os, indent); break;
        case 0x13: desc_13.print(m_13_cost_factor_word, os, indent); break;
        case 0x14: os << pad(indent) << "// unknown property 0x14" << "\n"; break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Bridges::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x00'00: desc_00.parse(m_00_fallback_type_id, is); break;
            case 0x08'00: desc_08.parse(m_08_year_available, is); break;
            case 0x09'00: desc_09.parse(m_09_minimum_length, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_maximum_length, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_cost_factor, is); break;
            case 0x0C'00: desc_0C.parse(m_0C_maximum_speed, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_bridge_layout, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_various_flags, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_long_year_available, is); break;
            case 0x10'00: desc_10.parse(m_10_purchase_text, is); break;
            case 0x11'00: desc_11.parse(m_11_description_rail, is); break;
            case 0x12'00: desc_12.parse(m_12_description_road, is); break;
            case 0x13'00: desc_13.parse(m_13_cost_factor_word, is); break;
            default:      throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}

