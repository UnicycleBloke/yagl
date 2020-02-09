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
#include "Action00Industries.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


namespace {


struct VectorU8Descriptor : PropertyDescriptor
{
    void print(const Action00Industries::VectorU8& items, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        items.print(os, indent);
    }

    void parse(Action00Industries::VectorU8& items, TokenStream& is) const
    {
        items.parse(is);
    }
};


struct MultipliersDescriptor : PropertyDescriptor
{
    void print(const Action00Industries::Multipliers& items, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        items.print(os, indent);
    }

    void parse(Action00Industries::Multipliers& items, TokenStream& is) const
    {
        items.parse(is);
    }
};


constexpr const char* str_substitute_industry_id      = "substitute_industry_id";
constexpr const char* str_industry_type_override      = "industry_type_override";
constexpr const char* str_industry_layout             = "industry_layout";
constexpr const char* str_production_flags            = "production_flags";
constexpr const char* str_closure_msg_id              = "closure_msg_id";
constexpr const char* str_production_up_id            = "production_up_id";
constexpr const char* str_production_down_id          = "production_down_id";
constexpr const char* str_fund_cost_multiplier        = "fund_cost_multiplier";
constexpr const char* str_production_cargo_types      = "production_cargo_types";
constexpr const char* str_acceptance_cargo_types      = "acceptance_cargo_types";
constexpr const char* str_production_multipliers_1    = "production_multipliers_1";
constexpr const char* str_production_multipliers_2    = "production_multipliers_2";
constexpr const char* str_minimum_distributed         = "minimum_distributed";
constexpr const char* str_sound_effects               = "sound_effects";
constexpr const char* str_conflicting_industries      = "conflicting_industries";
constexpr const char* str_conflicting_old_new         = "conflicting_old_new";
constexpr const char* str_random_probability          = "random_probability";
constexpr const char* str_gameplay_probability        = "gameplay_probability";
constexpr const char* str_map_colour                  = "map_colour";
constexpr const char* str_special_flags               = "special_flags";
constexpr const char* str_new_industry_text_id        = "new_industry_text_id";
constexpr const char* str_input_multipliers_A1        = "input_multipliers_A1";
constexpr const char* str_input_multipliers_B1        = "input_multipliers_B1";
constexpr const char* str_input_multipliers_A2        = "input_multipliers_A2";
constexpr const char* str_input_multipliers_B2        = "input_multipliers_B2";
constexpr const char* str_input_multipliers_A3        = "input_multipliers_A3";
constexpr const char* str_input_multipliers_B3        = "input_multipliers_B3";
constexpr const char* str_industry_name_id            = "industry_name_id";
constexpr const char* str_prospecting_probability     = "prospecting_probability";
constexpr const char* str_callback_flags_1            = "callback_flags_1";
constexpr const char* str_callback_flags_2            = "callback_flags_2";
constexpr const char* str_destruction_cost_multiplier = "destruction_cost_multiplier";
constexpr const char* str_nearby_station_text_id      = "nearby_station_text_id";
constexpr const char* str_production_cargo_list       = "production_cargo_list";
constexpr const char* str_acceptance_cargo_list       = "acceptance_cargo_list";
constexpr const char* str_production_multipliers      = "production_multipliers";
constexpr const char* str_input_cargo_multipliers     = "input_cargo_multipliers";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_substitute_industry_id,      0x08'00 },
    { str_industry_type_override,      0x09'00 },
    { str_industry_layout,             0x0A'00 },
    { str_production_flags,            0x0B'00 },
    { str_closure_msg_id,              0x0C'00 },
    { str_production_up_id,            0x0D'00 },
    { str_production_down_id,          0x0E'00 },
    { str_fund_cost_multiplier,        0x0F'00 },
    { str_production_cargo_types,      0x10'00 },
    { str_acceptance_cargo_types,      0x11'00 },
    { str_production_multipliers_1,    0x12'00 },
    { str_production_multipliers_2,    0x13'00 },
    { str_minimum_distributed,         0x14'00 },
    { str_sound_effects,               0x15'00 },
    { str_conflicting_industries,      0x16'00 },
    { str_conflicting_old_new,         0x16'01 },
    { str_random_probability,          0x17'00 },
    { str_gameplay_probability,        0x18'00 },
    { str_map_colour,                  0x19'00 },
    { str_special_flags,               0x1A'00 },
    { str_new_industry_text_id,        0x1B'00 },
    { str_input_multipliers_A1,        0x1C'00 },
    { str_input_multipliers_B1,        0x1C'01 },
    { str_input_multipliers_A2,        0x1D'00 },
    { str_input_multipliers_B2,        0x1D'01 },
    { str_input_multipliers_A3,        0x1E'00 },
    { str_input_multipliers_B3,        0x1E'01 },
    { str_industry_name_id,            0x1F'00 },
    { str_prospecting_probability,     0x20'00 },
    { str_callback_flags_1,            0x21'00 },
    { str_callback_flags_2,            0x22'00 },
    { str_destruction_cost_multiplier, 0x23'00 },
    { str_nearby_station_text_id,      0x24'00 },
    { str_production_cargo_list,       0x25'00 },
    { str_acceptance_cargo_list,       0x26'00 },
    { str_production_multipliers,      0x27'00 },
    { str_input_cargo_multipliers,     0x28'00 },
};


constexpr IntegerDescriptorT<uint8_t>  desc_08  = { 0x08, str_substitute_industry_id,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_09  = { 0x09, str_industry_type_override,      PropFormat::Hex };
constexpr IndustryLayoutsDescriptor    desc_0A  = { 0x0A, str_industry_layout };
constexpr IntegerDescriptorT<uint8_t>  desc_0B  = { 0x0B, str_production_flags,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0C  = { 0x0C, str_closure_msg_id,              PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0D  = { 0x0D, str_production_up_id,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_0E  = { 0x0E, str_production_down_id,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0F  = { 0x0F, str_fund_cost_multiplier,        PropFormat::Hex };
constexpr ArrayDescriptorT<uint8_t, 2> desc_10  = { 0x10, str_production_cargo_types,      PropFormat::Hex };
constexpr ArrayDescriptorT<uint8_t, 4> desc_11  = { 0x11, str_acceptance_cargo_types,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_12  = { 0x12, str_production_multipliers_1,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_13  = { 0x13, str_production_multipliers_2,    PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_14  = { 0x14, str_minimum_distributed,         PropFormat::Hex };
constexpr VectorU8Descriptor           desc_15  = { 0x15, str_sound_effects };
constexpr ArrayDescriptorT<uint8_t, 3> desc_160 = { 0x16, str_conflicting_industries,      PropFormat::Hex };
constexpr ArrayDescriptorT<uint8_t, 3> desc_161 = { 0x16, str_conflicting_old_new,         PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_17  = { 0x17, str_random_probability,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_18  = { 0x18, str_gameplay_probability,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_19  = { 0x19, str_map_colour,                  PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_1A  = { 0x1A, str_special_flags,               PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1B  = { 0x1B, str_new_industry_text_id,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1C0 = { 0x1C, str_input_multipliers_A1,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1C1 = { 0x1C, str_input_multipliers_B1,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1D0 = { 0x1D, str_input_multipliers_A2,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1D1 = { 0x1D, str_input_multipliers_B2,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1E0 = { 0x1E, str_input_multipliers_A3,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1E1 = { 0x1E, str_input_multipliers_B3,        PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_1F  = { 0x1F, str_industry_name_id,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_20  = { 0x20, str_prospecting_probability,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_21  = { 0x21, str_callback_flags_1,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_22  = { 0x22, str_callback_flags_2,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_23  = { 0x23, str_destruction_cost_multiplier, PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_24  = { 0x24, str_nearby_station_text_id,      PropFormat::Hex };
constexpr VectorU8Descriptor           desc_25  = { 0x25, str_production_cargo_list };
constexpr VectorU8Descriptor           desc_26  = { 0x26, str_acceptance_cargo_list };
constexpr VectorU8Descriptor           desc_27  = { 0x27, str_production_multipliers };
constexpr MultipliersDescriptor        desc_28  = { 0x28, str_input_cargo_multipliers };


} // namespace {


void Action00Industries::VectorU8::read(std::istream& is)
{
    uint8_t num_items = read_uint8(is);
    for (uint8_t i = 0; i < num_items; ++i)
    {
        items.push_back(read_uint8(is));
    }
}


void Action00Industries::VectorU8::write(std::ostream& os) const
{
    write_uint8(os, uint8_t(items.size()));
    for (uint8_t item: items)
    {
        write_uint8(os, item);
    }
}


void Action00Industries::VectorU8::print(std::ostream& os, uint16_t indent) const
{
    os << "[";
    for (const auto& item: items)
    {
        os << " " << to_hex(item);
    }
    os << " ];\n";
}


void Action00Industries::VectorU8::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        uint8_t value = is.match_integer();
        items.push_back(value);
    }

    is.match(TokenType::CloseBracket);
}


void Action00Industries::Multipliers::read(std::istream& is)
{
    num_inputs  = read_uint8(is);
    num_outputs = read_uint8(is);
    uint16_t num_items = num_inputs * num_outputs;
    for (uint16_t i = 0; i < num_items; ++i)
    {
        items.push_back(read_uint16(is));
    }
}


void Action00Industries::Multipliers::write(std::ostream& os) const
{
    write_uint8(os, num_inputs);
    write_uint8(os, num_outputs);
    // TODO assert size of vector matches.
    for (uint16_t item: items)
    {
        write_uint16(os, item);
    }
}


void Action00Industries::Multipliers::print(std::ostream& os, uint16_t indent) const
{
    os << "[";
    for (uint8_t i = 0; i < num_inputs; ++i)
    {
        os << " [";
        for (uint8_t o = 0; o < num_outputs; ++o)
        {
            os << " " << to_hex(items[i * num_outputs + o]);
        }
        os << " ]";
    }
    os << " ];\n";
}


void Action00Industries::Multipliers::parse(TokenStream& is)
{
    uint16_t inputs  = 0;
    uint16_t outputs = 0;

    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        ++inputs;
        is.match(TokenType::OpenBracket);
        while (is.peek().type != TokenType::CloseBracket)
        {
            ++outputs;
            uint8_t value = is.match_integer();
            items.push_back(value);
        }
        is.match(TokenType::CloseBracket);
    }
    is.match(TokenType::CloseBracket);

    num_inputs  = static_cast<uint8_t>(inputs);
    num_outputs = static_cast<uint8_t>(outputs / inputs);
    // TODO Assert values < 256. Assert that outputs % inputs == 0.
}


bool Action00Industries::read_property(std::istream& is, uint8_t property)
{
    uint8_t byte;

    switch (property)
    {
        case 0x08: m_08_substitute_industry_id      = read_uint8(is); break;
        case 0x09: m_09_industry_type_override      = read_uint8(is); break;
        case 0x0A: m_0A_industry_layout.read(is); break;
        case 0x0B: m_0B_production_flags            = read_uint8(is); break;
        case 0x0C: m_0C_closure_msg_id              = read_uint16(is); break;
        case 0x0D: m_0D_production_up_id            = read_uint16(is); break;
        case 0x0E: m_0E_production_down_id          = read_uint16(is); break;
        case 0x0F: m_0F_fund_cost_multiplier        = read_uint8(is); break;
        case 0x10: m_10_production_cargo_types[0]   = read_uint8(is);
                   m_10_production_cargo_types[1]   = read_uint8(is); break;
        case 0x11: m_11_acceptance_cargo_types[0]   = read_uint8(is);
                   m_11_acceptance_cargo_types[1]   = read_uint8(is);
                   m_11_acceptance_cargo_types[2]   = read_uint8(is);
                   m_11_acceptance_cargo_types[3]   = read_uint8(is); break;
        case 0x12: m_12_production_multipliers_1    = read_uint8(is); break;
        case 0x13: m_13_production_multipliers_2    = read_uint8(is); break;
        case 0x14: m_14_minimum_distributed         = read_uint8(is); break;
        case 0x15: m_15_sound_effects.read(is); break;
        case 0x16: byte                             = read_uint8(is);
                   m_16_conflicting_industries[0]   = byte & 0x7F;
                   m_16_conflicting_old_new[0]      = (byte != 0xFF) ? (byte & 0x80) : 0xFF;
                   byte                             = read_uint8(is);
                   m_16_conflicting_industries[1]   = byte & 0x7F;
                   m_16_conflicting_old_new[1]      = (byte != 0xFF) ? (byte & 0x80) : 0xFF;
                   byte                             = read_uint8(is);
                   m_16_conflicting_industries[2]   = byte & 0x7F;
                   m_16_conflicting_old_new[2]      = (byte != 0xFF) ? (byte & 0x80) : 0xFF; break;
        case 0x17: m_17_random_probability          = read_uint8(is); break;
        case 0x18: m_18_gameplay_probability        = read_uint8(is); break;
        case 0x19: m_19_map_colour                  = read_uint8(is); break;
        case 0x1A: m_1A_special_flags               = read_uint32(is); break;
        case 0x1B: m_1B_new_industry_text_id        = read_uint16(is); break;
        case 0x1C: m_1C_input_multipliers_A1        = read_uint16(is);
                   m_1C_input_multipliers_B1        = read_uint16(is); break;
        case 0x1D: m_1D_input_multipliers_A2        = read_uint16(is);
                   m_1D_input_multipliers_B2        = read_uint16(is); break;
        case 0x1E: m_1E_input_multipliers_A3        = read_uint16(is);
                   m_1E_input_multipliers_B3        = read_uint16(is); break;
        case 0x1F: m_1F_industry_name_id            = read_uint16(is); break;
        case 0x20: m_20_prospecting_probability     = read_uint32(is); break;
        case 0x21: m_21_callback_flags_1            = read_uint8(is); break;
        case 0x22: m_22_callback_flags_2            = read_uint8(is); break;
        case 0x23: m_23_destruction_cost_multiplier = read_uint32(is); break;
        case 0x24: m_24_nearby_station_text_id      = read_uint16(is); break;
        case 0x25: m_25_production_cargo_list.read(is); break;
        case 0x26: m_26_acceptance_cargo_list.read(is); break;
        case 0x27: m_27_production_multipliers.read(is); break;
        case 0x28: m_28_input_cargo_multipliers.read(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Industries::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint8(os, m_08_substitute_industry_id); break;
        case 0x09: write_uint8(os, m_09_industry_type_override); break;
        case 0x0A: m_0A_industry_layout.write(os); break;
        case 0x0B: write_uint8(os, m_0B_production_flags); break;
        case 0x0C: write_uint16(os, m_0C_closure_msg_id); break;
        case 0x0D: write_uint16(os, m_0D_production_up_id); break;
        case 0x0E: write_uint16(os, m_0E_production_down_id); break;
        case 0x0F: write_uint8(os, m_0F_fund_cost_multiplier); break;
        case 0x10: write_uint8(os, m_10_production_cargo_types[0]);
                   write_uint8(os, m_10_production_cargo_types[1]); break;
        case 0x11: write_uint8(os, m_11_acceptance_cargo_types[0]);
                   write_uint8(os, m_11_acceptance_cargo_types[1]);
                   write_uint8(os, m_11_acceptance_cargo_types[2]);
                   write_uint8(os, m_11_acceptance_cargo_types[3]); break;
        case 0x12: write_uint8(os, m_12_production_multipliers_1); break;
        case 0x13: write_uint8(os, m_13_production_multipliers_2); break;
        case 0x14: write_uint8(os, m_14_minimum_distributed); break;
        case 0x15: m_15_sound_effects.write(os); break;
        case 0x16: write_uint8(os, m_16_conflicting_industries[0] | m_16_conflicting_old_new[0]);
                   write_uint8(os, m_16_conflicting_industries[1] | m_16_conflicting_old_new[1]);
                   write_uint8(os, m_16_conflicting_industries[2] | m_16_conflicting_old_new[2]); break;
        case 0x17: write_uint8(os, m_17_random_probability); break;
        case 0x18: write_uint8(os, m_18_gameplay_probability); break;
        case 0x19: write_uint8(os, m_19_map_colour); break;
        case 0x1A: write_uint32(os, m_1A_special_flags); break;
        case 0x1B: write_uint16(os, m_1B_new_industry_text_id); break;
        case 0x1C: write_uint16(os, m_1C_input_multipliers_A1);
                   write_uint16(os, m_1C_input_multipliers_B1); break;
        case 0x1D: write_uint16(os, m_1D_input_multipliers_A2);
                   write_uint16(os, m_1D_input_multipliers_B2); break;
        case 0x1E: write_uint16(os, m_1E_input_multipliers_A3);
                   write_uint16(os, m_1E_input_multipliers_B3); break;
        case 0x1F: write_uint16(os, m_1F_industry_name_id); break;
        case 0x20: write_uint32(os, m_20_prospecting_probability); break;
        case 0x21: write_uint8(os, m_21_callback_flags_1); break;
        case 0x22: write_uint8(os, m_22_callback_flags_2); break;
        case 0x23: write_uint32(os, m_23_destruction_cost_multiplier); break;
        case 0x24: write_uint16(os, m_24_nearby_station_text_id); break;
        case 0x25: m_25_production_cargo_list.write(os); break;
        case 0x26: m_26_acceptance_cargo_list.write(os); break;
        case 0x27: m_27_production_multipliers.write(os); break;
        case 0x28: m_28_input_cargo_multipliers.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Industries::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_substitute_industry_id, os, indent); break;
        case 0x09: desc_09.print(m_09_industry_type_override, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_industry_layout, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_production_flags, os, indent); break;
        case 0x0C: desc_0C.print(m_0C_closure_msg_id, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_production_up_id, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_production_down_id, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_fund_cost_multiplier, os, indent); break;
        case 0x10: desc_10.print(m_10_production_cargo_types, os, indent); break;
        case 0x11: desc_11.print(m_11_acceptance_cargo_types, os, indent); break;
        case 0x12: desc_12.print(m_12_production_multipliers_1, os, indent); break;
        case 0x13: desc_13.print(m_13_production_multipliers_2, os, indent); break;
        case 0x14: desc_14.print(m_14_minimum_distributed, os, indent); break;
        case 0x15: desc_15.print(m_15_sound_effects, os, indent); break;
        case 0x16: desc_160.print(m_16_conflicting_industries, os, indent); 
                   desc_161.print(m_16_conflicting_old_new, os, indent); break;
        case 0x17: desc_17.print(m_17_random_probability, os, indent); break;
        case 0x18: desc_18.print(m_18_gameplay_probability, os, indent); break;
        case 0x19: desc_19.print(m_19_map_colour, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_special_flags, os, indent); break;
        case 0x1B: desc_1B.print(m_1B_new_industry_text_id, os, indent); break;
        case 0x1C: desc_1C0.print(m_1C_input_multipliers_A1, os, indent); 
                   desc_1C1.print(m_1C_input_multipliers_B1, os, indent); break;
        case 0x1D: desc_1D0.print(m_1D_input_multipliers_A2, os, indent); 
                   desc_1D1.print(m_1D_input_multipliers_B2, os, indent); break;
        case 0x1E: desc_1E0.print(m_1E_input_multipliers_A3, os, indent); 
                   desc_1E1.print(m_1E_input_multipliers_B3, os, indent); break;
        case 0x1F: desc_1F.print(m_1F_industry_name_id, os, indent); break;
        case 0x20: desc_20.print(m_20_prospecting_probability, os, indent); break;
        case 0x21: desc_21.print(m_21_callback_flags_1, os, indent); break;
        case 0x22: desc_22.print(m_22_callback_flags_2, os, indent); break;
        case 0x23: desc_23.print(m_23_destruction_cost_multiplier, os, indent); break;
        case 0x24: desc_24.print(m_24_nearby_station_text_id, os, indent); break;
        case 0x25: desc_25.print(m_25_production_cargo_list, os, indent); break;
        case 0x26: desc_26.print(m_26_acceptance_cargo_list, os, indent); break;
        case 0x27: desc_27.print(m_27_production_multipliers, os, indent); break;
        case 0x28: desc_28.print(m_28_input_cargo_multipliers, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Industries::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_substitute_industry_id, is); break;
            case 0x09'00: desc_09.parse(m_09_industry_type_override, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_industry_layout, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_production_flags, is); break;
            case 0x0C'00: desc_0C.parse(m_0C_closure_msg_id, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_production_up_id, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_production_down_id, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_fund_cost_multiplier, is); break;
            case 0x10'00: desc_10.parse(m_10_production_cargo_types, is); break;
            case 0x11'00: desc_11.parse(m_11_acceptance_cargo_types, is); break;
            case 0x12'00: desc_12.parse(m_12_production_multipliers_1, is); break;
            case 0x13'00: desc_13.parse(m_13_production_multipliers_2, is); break;
            case 0x14'00: desc_14.parse(m_14_minimum_distributed, is); break;
            case 0x15'00: desc_15.parse(m_15_sound_effects, is); break;
            case 0x16'00: desc_160.parse(m_16_conflicting_industries, is); break;
            case 0x16'01: desc_161.parse(m_16_conflicting_old_new, is); break;
            case 0x17'00: desc_17.parse(m_17_random_probability, is); break;
            case 0x18'00: desc_18.parse(m_18_gameplay_probability, is); break;
            case 0x19'00: desc_19.parse(m_19_map_colour, is); break;
            case 0x1A'00: desc_1A.parse(m_1A_special_flags, is); break;
            case 0x1B'00: desc_1B.parse(m_1B_new_industry_text_id, is); break;
            case 0x1C'00: desc_1C0.parse(m_1C_input_multipliers_A1, is); break;
            case 0x1C'01: desc_1C1.parse(m_1C_input_multipliers_B1, is); break;
            case 0x1D'00: desc_1D0.parse(m_1D_input_multipliers_A2, is); break;
            case 0x1D'01: desc_1D1.parse(m_1D_input_multipliers_B2, is); break;
            case 0x1E'00: desc_1E0.parse(m_1E_input_multipliers_A3, is); break;
            case 0x1E'01: desc_1E1.parse(m_1E_input_multipliers_B3, is); break;
            case 0x1F'00: desc_1F.parse(m_1F_industry_name_id, is); break;
            case 0x20'00: desc_20.parse(m_20_prospecting_probability, is); break;
            case 0x21'00: desc_21.parse(m_21_callback_flags_1, is); break;
            case 0x22'00: desc_22.parse(m_22_callback_flags_2, is); break;
            case 0x23'00: desc_23.parse(m_23_destruction_cost_multiplier, is); break;
            case 0x24'00: desc_24.parse(m_24_nearby_station_text_id, is); break;
            case 0x25'00: desc_25.parse(m_25_production_cargo_list, is); break;
            case 0x26'00: desc_26.parse(m_26_acceptance_cargo_list, is); break;
            case 0x27'00: desc_27.parse(m_27_production_multipliers, is); break;
            case 0x28'00: desc_28.parse(m_28_input_cargo_multipliers, is); break;
            default:      throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}

