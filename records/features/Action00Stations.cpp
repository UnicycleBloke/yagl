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
#include "Action00Stations.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


namespace {


struct SpriteLayoutsDescriptor : PropertyDescriptor
{
    void print(const Action00Stations::SpriteLayout& layouts, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Stations::SpriteLayout& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


struct CustomStationsDescriptor : PropertyDescriptor
{
    void print(const Action00Stations::CustomStation& layouts, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        layouts.print(os, indent);
    }

    void parse(Action00Stations::CustomStation& layouts, TokenStream& is) const
    {
        layouts.parse(is);
    }
};


constexpr const char* str_class_id                  = "class_id";
constexpr const char* str_sprite_layout             = "sprite_layout";
constexpr const char* str_copy_sprite_layout_id     = "copy_sprite_layout_id";
constexpr const char* str_callback_flags            = "callback_flags";
constexpr const char* str_disabled_platform_numbers = "disabled_platform_numbers";
constexpr const char* str_disabled_platform_lengths = "disabled_platform_lengths";
constexpr const char* str_custom_layout             = "custom_layout";
constexpr const char* str_copy_custom_layout_id     = "copy_custom_layout_id";
constexpr const char* str_little_lots_threshold     = "little_lots_threshold";
constexpr const char* str_pylon_placement           = "pylon_placement";
constexpr const char* str_cargo_type_triggers       = "cargo_type_triggers";
constexpr const char* str_general_flags             = "general_flags";
constexpr const char* str_overhead_wire_placement   = "overhead_wire_placement";
constexpr const char* str_can_train_enter_tile      = "can_train_enter_tile";
constexpr const char* str_animation_frames          = "animation_frames";
constexpr const char* str_animation_type            = "animation_type";
constexpr const char* str_animation_speed           = "animation_speed";
constexpr const char* str_animation_triggers        = "animation_triggers";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_class_id,                  0x08'00 },
    { str_sprite_layout,             0x09'00 },
    { str_copy_sprite_layout_id,     0x0A'00 },
    { str_callback_flags,            0x0B'00 },
    { str_disabled_platform_numbers, 0x0C'00 },
    { str_disabled_platform_lengths, 0x0D'00 },
    { str_custom_layout,             0x0E'00 },
    { str_copy_custom_layout_id,     0x0F'00 },
    { str_little_lots_threshold,     0x10'00 },
    { str_pylon_placement,           0x11'00 },
    { str_cargo_type_triggers,       0x12'00 },
    { str_general_flags,             0x13'00 },
    { str_overhead_wire_placement,   0x14'00 },
    { str_can_train_enter_tile,      0x15'00 },
    { str_animation_frames,          0x16'00 },
    { str_animation_type,            0x16'01 },
    { str_animation_speed,           0x17'00 },
    { str_animation_triggers,        0x18'00 },
};


constexpr IntegerDescriptorT<uint32_t> desc_08  = { 0x08, str_class_id,                  PropFormat::Hex };
constexpr SpriteLayoutsDescriptor      desc_09  = { 0x09, str_sprite_layout };
constexpr IntegerDescriptorT<uint8_t>  desc_0A  = { 0x0A, str_copy_sprite_layout_id,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0B  = { 0x0B, str_callback_flags,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0C  = { 0x0C, str_disabled_platform_numbers, PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_0D  = { 0x0D, str_disabled_platform_lengths, PropFormat::Hex };
constexpr CustomStationsDescriptor     desc_0E  = { 0x0E, str_custom_layout };
constexpr IntegerDescriptorT<uint8_t>  desc_0F  = { 0x0F, str_copy_custom_layout_id,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_10  = { 0x10, str_little_lots_threshold,     PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_11  = { 0x11, str_pylon_placement,           PropFormat::Hex };
constexpr IntegerDescriptorT<uint32_t> desc_12  = { 0x12, str_cargo_type_triggers,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_13  = { 0x13, str_general_flags,             PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_14  = { 0x14, str_overhead_wire_placement,   PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_15  = { 0x15, str_can_train_enter_tile,      PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_160 = { 0x16, str_animation_frames,          PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_161 = { 0x16, str_animation_type,            PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_17  = { 0x17, str_animation_speed,           PropFormat::Hex };
constexpr IntegerDescriptorT<uint16_t> desc_18  = { 0x18, str_animation_triggers,        PropFormat::Hex };


} // namespace {


void Action00Stations::SpriteTileData::read(std::istream& is)
{
    x_off  = read_uint8(is);
    y_off  = read_uint8(is);
    z_off  = read_uint8(is);
    x_ext  = read_uint8(is);
    y_ext  = read_uint8(is);
    z_ext  = read_uint8(is);
    sprite = read_uint32(is);

    new_bb = (z_off != 0x80);
}


void Action00Stations::SpriteTileData::write(std::ostream& os) const
{
    write_uint8(os, x_off);
    write_uint8(os, y_off);
    if (new_bb)
    {
        write_uint8(os, z_off);
        write_uint8(os, x_ext);
        write_uint8(os, y_ext);
        write_uint8(os, z_ext);
    }
    else
    {
        write_uint8(os, 0x80);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
    }
    write_uint32(os, sprite);
}


void Action00Stations::SpriteTileData::print(std::ostream& os, uint16_t indent) const
{
    if (new_bb)
    {
        os << pad(indent) << "tile(" << to_hex(sprite) << ", ";
        os << to_hex(x_off) << ", " << to_hex(y_off) << ", " << to_hex(z_off) << ", "; 
        os << to_hex(x_ext) << ", " << to_hex(y_ext) << ", " << to_hex(z_ext); 
        os << ");\n"; 
    }
    else
    {
        os << pad(indent) << "tile(" << to_hex(sprite) << ", ";
        os << to_hex(x_off) << ", " << to_hex(y_off); 
        os << ");\n"; 
    }
}


void Action00Stations::SpriteTileData::parse(TokenStream& is)
{

}


void Action00Stations::SpriteTile::read(std::istream& is)
{
    ground_sprite = read_uint32(is);
    if (ground_sprite == 0x00000000)
    {
        return;
    }

    while (is.peek() != 0x80)
    {
        SpriteTileData datum;
        datum.read(is);
        data.push_back(datum);
    }
    read_uint8(is);
}


void Action00Stations::SpriteTile::write(std::ostream& os) const
{
    write_uint32(os, ground_sprite);
    if (ground_sprite == 0x00000000)
    {
        return;
    }

    for (const auto& datum: data)
    {
        datum.write(os);
    }
    write_uint8(os, 0x80);
}


void Action00Stations::SpriteTile::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "layout\n"; 
    os << pad(indent) << "{\n"; 

    os << pad(indent + 4) << "ground_sprite: " << to_hex(ground_sprite) << "\n"; 

    if (ground_sprite != 0x00000000)
    {
        os << pad(indent + 4) << "sprite_data:\n"; 
        os << pad(indent + 4) << "{\n"; 
     
        for (const auto& datum: data)
        {
            datum.print(os, indent + 8);
        }

        os << pad(indent + 4) << "}\n"; 
    }

    os << pad(indent) << "}\n"; 

}


void Action00Stations::SpriteTile::parse(TokenStream& is)
{

}


void Action00Stations::SpriteLayout::read(std::istream& is)
{
    uint16_t num_tiles = read_uint8_ext(is);
    tiles.resize(num_tiles);
    for (uint16_t i = 0; i < num_tiles; ++i)
    {
        tiles[i].read(is);
    }
}


void Action00Stations::SpriteLayout::write(std::ostream& os) const
{
    write_uint8_ext(os, tiles.size());
    for (const auto& tile: tiles)
    {
        tile.write(os);
    }
}


void Action00Stations::SpriteLayout::print(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << "layouts\n"; 
    os << "layouts\n"; 
    os << pad(indent) << "{\n"; 
    for (const auto& tile: tiles)
    {
        tile.print(os, indent + 4);
    }
    os << pad(indent) << "};\n"; 
}


void Action00Stations::SpriteLayout::parse(TokenStream& is)
{

}


void Action00Stations::CustomLayout::read(std::istream& is)
{
    platform_length = read_uint8(is);
    platform_count  = read_uint8(is);
    for (uint16_t i = 0; i < platform_length * platform_count; ++i)
    {
        auto tile = static_cast<Platform>(read_uint8(is));
        platform_tiles.push_back(tile);
    }
}


void Action00Stations::CustomLayout::write(std::ostream& os) const
{
    write_uint8(os, platform_length);
    write_uint8(os, platform_count);
    // Assert that the vector has the correct length?
    for (const auto tile: platform_tiles)
    {
        write_uint8(os, static_cast<uint8_t>(tile));
    }
}


void Action00Stations::CustomLayout::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << "layout\n"; 
    os << pad(indent) << "{\n"; 
    os << pad(indent + 4) << "// Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight \n"; 

    for (uint8_t c = 0; c < platform_count; ++c)
    {
        os << pad(indent + 4); 
        for (uint8_t l = 0; l < platform_length; ++l)
        {
            // Assert that the vector has the correct length?
            const auto tile = platform_tiles[c * platform_length + l];
            os << " "; // << to_hex(static_cast<uint8_t>(tile));
            switch (tile)
            {
                // case Platform::Building:  os << "Building"; break;
                // case Platform::Plain:     os << "Plain"; break;
                // case Platform::RoofLeft:  os << "RoofLeft"; break;
                // case Platform::RoofRight: os << "RoofRight"; break;
                case Platform::Building:  os << "B"; break;
                case Platform::Plain:     os << "P"; break;
                case Platform::RoofLeft:  os << "L"; break;
                case Platform::RoofRight: os << "R"; break;
            }
        }
        os << "\n"; 
    }

    os << pad(indent) << " }\n"; 
}


void Action00Stations::CustomLayout::parse(TokenStream& is)
{

}


void Action00Stations::CustomStation::read(std::istream& is)
{
    while (true)
    {
        CustomLayout layout;
        layout.read(is);
        if (layout.platform_count == 0 && layout.platform_length == 0)
        {
            break;
        }
        layouts.push_back(layout);
    }
}


void Action00Stations::CustomStation::write(std::ostream& os) const
{
    for (const auto& layout: layouts)
    {
        layout.write(os);
    }
    write_uint8(os, 0x00);
    write_uint8(os, 0x00);
}


void Action00Stations::CustomStation::print(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << "layouts\n"; 
    os << "layouts\n"; 
    os << pad(indent) << "{\n"; 
    for (const auto& layout: layouts)
    {
        layout.print(os, indent + 4);
    }
    os << pad(indent) << "};\n"; 
}


void Action00Stations::CustomStation::parse(TokenStream& is)
{

}


bool Action00Stations::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_class_id                  = read_uint32(is); break;
        case 0x09: m_09_sprite_layout.read(is);                      break;
        case 0x0A: m_0A_copy_sprite_layout_id     = read_uint8(is);  break;
        case 0x0B: m_0B_callback_flags            = read_uint8(is);  break;
        case 0x0C: m_0C_disabled_platform_numbers = read_uint8(is);  break;
        case 0x0D: m_0D_disabled_platform_lengths = read_uint8(is);  break;
        case 0x0E: m_0E_custom_layout.read(is);                      break;
        case 0x0F: m_0F_copy_custom_layout_id     = read_uint8(is);  break;
        case 0x10: m_10_little_lots_threshold     = read_uint16(is); break;
        case 0x11: m_11_pylon_placement           = read_uint8(is);  break;
        case 0x12: m_12_cargo_type_triggers       = read_uint32(is); break;
        case 0x13: m_13_general_flags             = read_uint8(is);  break;
        case 0x14: m_14_overhead_wire_placement   = read_uint8(is);  break;
        case 0x15: m_15_can_train_enter_tile      = read_uint8(is);  break;
        case 0x16: m_16_animation_frames          = read_uint8(is);
                   m_16_animation_type            = read_uint8(is);  break;
        case 0x17: m_17_animation_speed           = read_uint8(is);  break;
        case 0x18: m_18_animation_triggers        = read_uint16(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}   


bool Action00Stations::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: write_uint32(os, m_08_class_id); break;
        case 0x09: m_09_sprite_layout.write(os); break;
        case 0x0A: write_uint8(os, m_0A_copy_sprite_layout_id); break;
        case 0x0B: write_uint8(os, m_0B_callback_flags); break;
        case 0x0C: write_uint8(os, m_0C_disabled_platform_numbers); break;
        case 0x0D: write_uint8(os, m_0D_disabled_platform_lengths); break;
        case 0x0E: m_0E_custom_layout.write(os); break;
        case 0x0F: write_uint8(os, m_0F_copy_custom_layout_id); break;
        case 0x10: write_uint16(os, m_10_little_lots_threshold); break;
        case 0x11: write_uint8(os, m_11_pylon_placement); break;
        case 0x12: write_uint32(os, m_12_cargo_type_triggers); break;
        case 0x13: write_uint8(os, m_13_general_flags); break;
        case 0x14: write_uint8(os, m_14_overhead_wire_placement); break;
        case 0x15: write_uint8(os, m_15_can_train_enter_tile); break;
        case 0x16: write_uint8(os, m_16_animation_frames);
                   write_uint8(os, m_16_animation_type); break;
        case 0x17: write_uint8(os, m_17_animation_speed); break;
        case 0x18: write_uint16(os, m_18_animation_triggers); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00Stations::print_property(std::ostream& os, uint8_t property, uint16_t indent) const 
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_class_id, os, indent); break;
        case 0x09: desc_09.print(m_09_sprite_layout, os, indent); break;
        case 0x0A: desc_0A.print(m_0A_copy_sprite_layout_id, os, indent); break;
        case 0x0B: desc_0B.print(m_0B_callback_flags, os, indent); break;
        case 0x0C: desc_0C.print(m_0C_disabled_platform_numbers, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_disabled_platform_lengths, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_custom_layout, os, indent); break;
        case 0x0F: desc_0F.print(m_0F_copy_custom_layout_id, os, indent); break;
        case 0x10: desc_10.print(m_10_little_lots_threshold, os, indent); break;
        case 0x11: desc_11.print(m_11_pylon_placement, os, indent); break;
        case 0x12: desc_12.print(m_12_cargo_type_triggers, os, indent); break;
        case 0x13: desc_13.print(m_13_general_flags, os, indent); break;
        case 0x14: desc_14.print(m_14_overhead_wire_placement, os, indent); break;
        case 0x15: desc_15.print(m_15_can_train_enter_tile, os, indent); break;
        case 0x16: desc_160.print(m_16_animation_frames, os, indent); os << "\n";
                   desc_161.print(m_16_animation_type, os, indent); break;
        case 0x17: desc_17.print(m_17_animation_speed, os, indent); break;
        case 0x18: desc_18.print(m_18_animation_triggers, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    os << "\n";
    return true;
}


bool Action00Stations::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x08'00: desc_08.parse(m_08_class_id, is); break;
            case 0x09'00: desc_09.parse(m_09_sprite_layout, is); break;
            case 0x0A'00: desc_0A.parse(m_0A_copy_sprite_layout_id, is); break;
            case 0x0B'00: desc_0B.parse(m_0B_callback_flags, is); break;
            case 0x0C'00: desc_0C.parse(m_0C_disabled_platform_numbers, is); break;
            case 0x0D'00: desc_0D.parse(m_0D_disabled_platform_lengths, is); break;
            case 0x0E'00: desc_0E.parse(m_0E_custom_layout, is); break;
            case 0x0F'00: desc_0F.parse(m_0F_copy_custom_layout_id, is); break;
            case 0x10'00: desc_10.parse(m_10_little_lots_threshold, is); break;
            case 0x11'00: desc_11.parse(m_11_pylon_placement, is); break;
            case 0x12'00: desc_12.parse(m_12_cargo_type_triggers, is); break;
            case 0x13'00: desc_13.parse(m_13_general_flags, is); break;
            case 0x14'00: desc_14.parse(m_14_overhead_wire_placement, is); break;
            case 0x15'00: desc_15.parse(m_15_can_train_enter_tile, is); break;
            case 0x16'00: desc_160.parse(m_16_animation_frames, is); break;
            case 0x16'01: desc_161.parse(m_16_animation_type, is); break;
            case 0x17'00: desc_17.parse(m_17_animation_speed, is); break;
            case 0x18'00: desc_18.parse(m_18_animation_triggers, is); break;
            default:      throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
