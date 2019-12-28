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


struct IndustryLayoutDescriptor : PropertyDescriptor
{
    void print(const Action00Industries::IndustryLayouts& layouts, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Industries::IndustryLayouts& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


struct IndustrySFXListDescriptor : PropertyDescriptor
{
    void print(const Action00Industries::IndustrySFXList& layouts, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Industries::IndustrySFXList& layouts, TokenStream& is) const
    {
        layouts.parse(is);
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
};


constexpr IntegerDescriptorT<uint8_t>  desc_08  = { 0x08, str_substitute_industry_id,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_09  = { 0x09, str_industry_type_override,      PropFormat::Hex };
constexpr IndustryLayoutDescriptor     desc_0A  = { 0x0A, str_industry_layout };
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
constexpr IndustrySFXListDescriptor    desc_15  = { 0x15, str_sound_effects };
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


} // namespace {


void Action00Industries::IndustryTile::read(std::istream& is)
{
    x_off = read_uint8(is);
    y_off = read_uint8(is);

    // Nasty! Sign extension of the signed int8 causes test against 0x80 to fail.
    if (x_off == 0x00 && uint8_t(y_off) == 0x80)
        return;

    type = read_uint8(is);
    switch (type)
    {
        case 0xFF:
            break;
        case 0xFE:
            tile = read_uint16(is);
            break;
        default:
            tile = type;
            type = 0x00;
    }
}


void Action00Industries::IndustryTile::write(std::ostream& os) const
{
    write_uint8(os, x_off);
    write_uint8(os, y_off);
 
    //if (x_off == 0x00 && uint8_t(y_off) == 0x80)
    //    return;
 
    switch (type)
    {
        case 0xFF:
            write_uint8(os, 0xFF);
            break;
        case 0xFE:
            write_uint8(os, 0xFE);
            write_uint16(os, tile);
            break;
        default:
            write_uint8(os, tile);
    }
}


void Action00Industries::IndustryTile::print(std::ostream& os, uint16_t indent) const
{

}


void Action00Industries::IndustryTile::parse(TokenStream& is)
{

}


void Action00Industries::IndustryLayout::read(std::istream& is)
{
    // This could go wrong if the first tile is for clearance and has the x-value of -2.
    is_reference = (is.peek() == 0xFE);
    if (is_reference)
    {
        // Special case in which the layout is a reference to another industry
        // Waste the 0xFE byte we just peeked.
        read_uint8(is);
        industry     = read_uint8(is);
        layout_index = read_uint8(is);
    }
    else
    {
        while (true)
        {
            IndustryTile tile;
            tile.read(is);
            // Nasty! Sign extension of the signed int8 causes test against 0x80 to fail.
            if (tile.x_off == 0x00 && uint8_t(tile.y_off) == 0x80)
                break;
            tiles.push_back(tile);
        }
    }
}


void Action00Industries::IndustryLayout::write(std::ostream& os) const
{
    if (is_reference)
    {
        // Special case in which the layout is a reference to another industry
        write_uint8(os, 0xFE);
        write_uint8(os, industry);
        write_uint8(os, layout_index);
    }
    else
    {
        for (const auto& tile: tiles)
        {
            tile.write(os);
        }
        write_uint8(os, 0x00);
        write_uint8(os, 0x80);
    }
}


void Action00Industries::IndustryLayout::print(std::ostream& os, uint16_t indent) const
{

}


void Action00Industries::IndustryLayout::parse(TokenStream& is)
{

}


void Action00Industries::IndustryLayouts::read(std::istream& is)
{
    uint8_t num_layouts = read_uint8(is);

    // Size not used for anything.
    read_uint32(is);

    layouts.resize(num_layouts);
    for (uint8_t i = 0; i < num_layouts; ++i)
    {
        layouts[i].read(is);
    }
}


void Action00Industries::IndustryLayouts::write(std::ostream& os) const
{
    write_uint8(os, layouts.size());

    // Size needs to be calculated... 
    std::ostringstream ss;
    for (const auto& layout: layouts)
    {
        layout.write(ss);
    }

    write_uint32(os, ss.str().length());
    for (const auto& layout: layouts)
    {
        layout.write(os);
    }
}


void Action00Industries::IndustryLayouts::print(std::ostream& os, uint16_t indent) const
{

}


void Action00Industries::IndustryLayouts::parse(TokenStream& is)
{

}


void Action00Industries::IndustrySFXList::read(std::istream& is)
{
    uint8_t num_effects = read_uint8(is);
    for (uint8_t i = 0; i < num_effects; ++i)
    {
        effects.push_back(read_uint8(is));
    }
}


void Action00Industries::IndustrySFXList::write(std::ostream& os) const
{
    write_uint8(os, effects.size());
    for (uint8_t effect: effects)
    {
        write_uint8(os, effect);
    }
}


void Action00Industries::IndustrySFXList::print(std::ostream& os, uint16_t indent) const
{

}


void Action00Industries::IndustrySFXList::parse(TokenStream& is)
{

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
        default:   throw RUNTIME_ERROR("Unknown property");
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
        default:   throw RUNTIME_ERROR("Unknown property");
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
        case 0x16: desc_160.print(m_16_conflicting_industries, os, indent); os << "\n";
                   desc_161.print(m_16_conflicting_old_new, os, indent); break;
        case 0x17: desc_17.print(m_17_random_probability, os, indent); break;
        case 0x18: desc_18.print(m_18_gameplay_probability, os, indent); break;
        case 0x19: desc_19.print(m_19_map_colour, os, indent); break;
        case 0x1A: desc_1A.print(m_1A_special_flags, os, indent); break;
        case 0x1B: desc_1B.print(m_1B_new_industry_text_id, os, indent); break;
        case 0x1C: desc_1C0.print(m_1C_input_multipliers_A1, os, indent); os << "\n";
                   desc_1C1.print(m_1C_input_multipliers_B1, os, indent); break;
        case 0x1D: desc_1D0.print(m_1D_input_multipliers_A2, os, indent); os << "\n";
                   desc_1D1.print(m_1D_input_multipliers_B2, os, indent); break;
        case 0x1E: desc_1E0.print(m_1E_input_multipliers_A3, os, indent); os << "\n";
                   desc_1E1.print(m_1E_input_multipliers_B3, os, indent); break;
        case 0x1F: desc_1F.print(m_1F_industry_name_id, os, indent); break;
        case 0x20: desc_20.print(m_20_prospecting_probability, os, indent); break;
        case 0x21: desc_21.print(m_21_callback_flags_1, os, indent); break;
        case 0x22: desc_22.print(m_22_callback_flags_2, os, indent); break;
        case 0x23: desc_23.print(m_23_destruction_cost_multiplier, os, indent); break;
        case 0x24: desc_24.print(m_24_nearby_station_text_id, os, indent); break;

        default:   throw RUNTIME_ERROR("Unknown property");
    }

    os << "\n";
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
            default:      throw RUNTIME_ERROR("Unknown property");
        }

        return true;
    }

    throw RUNTIME_ERROR("Unknown property");
}

