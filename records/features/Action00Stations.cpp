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
#include "EnumDescriptor.h"


namespace {


constexpr const char* str_class_id                  = "class_id";
constexpr const char* str_sprite_layouts            = "sprite_layouts";
constexpr const char* str_copy_sprite_layout_id     = "copy_sprite_layout_id";
constexpr const char* str_callback_flags            = "callback_flags";
constexpr const char* str_disabled_platform_numbers = "disabled_platform_numbers";
constexpr const char* str_disabled_platform_lengths = "disabled_platform_lengths";
constexpr const char* str_custom_layouts            = "custom_layouts";
constexpr const char* str_copy_custom_layout_id     = "copy_custom_layout_id";
constexpr const char* str_little_lots_threshold     = "little_lots_threshold";
constexpr const char* str_pylon_placement           = "pylon_placement";
constexpr const char* str_cargo_type_triggers       = "cargo_type_triggers";
constexpr const char* str_general_flags             = "general_flags";
constexpr const char* str_overhead_wire_placement   = "overhead_wire_placement";
constexpr const char* str_can_train_enter_tile      = "can_train_enter_tile";
constexpr const char* str_animation_info            = "animation_info";
constexpr const char* str_animation_speed           = "animation_speed";
constexpr const char* str_animation_triggers        = "animation_triggers";
constexpr const char* str_sprite                    = "sprite";
constexpr const char* str_tile                      = "tile";
constexpr const char* str_layout                    = "layout";
//constexpr const char* str_ground_sprite             = "ground_sprite";
//constexpr const char* str_sprite_data               = "sprite_data";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_class_id,                  0x08 },
    { str_sprite_layouts,            0x09 },
    { str_copy_sprite_layout_id,     0x0A },
    { str_callback_flags,            0x0B },
    { str_disabled_platform_numbers, 0x0C },
    { str_disabled_platform_lengths, 0x0D },
    { str_custom_layouts,            0x0E },
    { str_copy_custom_layout_id,     0x0F },
    { str_little_lots_threshold,     0x10 },
    { str_pylon_placement,           0x11 },
    { str_cargo_type_triggers,       0x12 },
    { str_general_flags,             0x13 },
    { str_overhead_wire_placement,   0x14 },
    { str_can_train_enter_tile,      0x15 },
    { str_animation_info,            0x16 },
    { str_animation_speed,           0x17 },
    { str_animation_triggers,        0x18 },
};


using StationLayoutDescriptor  = GenericDescriptor<StationLayout, true>;
using CustomStationsDescriptor = GenericDescriptor<CustomStation, true>;


constexpr GRFLabelDescriptor        desc_08  = { 0x08, str_class_id };
constexpr StationLayoutDescriptor   desc_09  = { 0x09, str_sprite_layouts };
constexpr UInt8Descriptor           desc_0A  = { 0x0A, str_copy_sprite_layout_id,     UIntFormat::Hex };
constexpr UInt8Descriptor           desc_0B  = { 0x0B, str_callback_flags,            UIntFormat::Hex };
constexpr UInt8Descriptor           desc_0C  = { 0x0C, str_disabled_platform_numbers, UIntFormat::Hex };
constexpr UInt8Descriptor           desc_0D  = { 0x0D, str_disabled_platform_lengths, UIntFormat::Hex };
constexpr CustomStationsDescriptor  desc_0E  = { 0x0E, str_custom_layouts };
constexpr UInt8Descriptor           desc_0F  = { 0x0F, str_copy_custom_layout_id,     UIntFormat::Hex };
constexpr UInt16Descriptor          desc_10  = { 0x10, str_little_lots_threshold,     UIntFormat::Hex };
constexpr UInt8Descriptor           desc_11  = { 0x11, str_pylon_placement,           UIntFormat::Hex };
constexpr UInt32Descriptor          desc_12  = { 0x12, str_cargo_type_triggers,       UIntFormat::Hex };
constexpr UInt8Descriptor           desc_13  = { 0x13, str_general_flags,             UIntFormat::Hex };
constexpr UInt8Descriptor           desc_14  = { 0x14, str_overhead_wire_placement,   UIntFormat::Hex };
constexpr UInt8Descriptor           desc_15  = { 0x15, str_can_train_enter_tile,      UIntFormat::Hex };
constexpr UInt16Descriptor          desc_16  = { 0x16, str_animation_info,            UIntFormat::Hex };
constexpr UInt8Descriptor           desc_17  = { 0x17, str_animation_speed,           UIntFormat::Hex };
constexpr UInt16Descriptor          desc_18  = { 0x18, str_animation_triggers,        UIntFormat::Hex };


const EnumDescriptorT<CustomLayout::Platform> platform_desc =
{
    0x00, "platform",
    {
        { 0, "P" }, // Platform::Plain },
        { 2, "B" }, // Platform::Building },
        { 4, "L" }, // Platform::RoofLeft },
        { 6, "R" }, // Platform::RoofRight },
    }
};

const EnumDescriptorT<CustomLayout::Direction> direction_desc =
{
    0x00, "direction",
    {
        { 0, "NE_SW" },
        { 1, "NW_SE" },
    }
};


} // namespace {


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationTileData::read(std::istream& is)
{
    m_x_off  = read_uint8(is);
    m_y_off  = read_uint8(is);
    m_z_off  = read_uint8(is);
    m_x_ext  = read_uint8(is);
    m_y_ext  = read_uint8(is);
    m_z_ext  = read_uint8(is);
    m_sprite = read_uint32(is);

    m_new_bb = (m_z_off != 0x80);
}


void StationTileData::write(std::ostream& os) const
{
    write_uint8(os, m_x_off);
    write_uint8(os, m_y_off);
    if (m_new_bb)
    {
        write_uint8(os, m_z_off);
        write_uint8(os, m_x_ext);
        write_uint8(os, m_y_ext);
        write_uint8(os, m_z_ext);
    }
    else
    {
        write_uint8(os, 0x80);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
        write_uint8(os, 0x00);
    }
    write_uint32(os, m_sprite);
}


void StationTileData::print(std::ostream& os, uint16_t indent) const
{
    if (m_new_bb)
    {
        os << pad(indent) << str_sprite << "(" << to_hex(m_sprite) << ", ";
        os << to_hex(m_x_off) << ", " << to_hex(m_y_off) << ", " << to_hex(m_z_off) << ", ";
        os << to_hex(m_x_ext) << ", " << to_hex(m_y_ext) << ", " << to_hex(m_z_ext);
        os << ");\n";
    }
    else
    {
        os << pad(indent) << str_sprite << "(" << to_hex(m_sprite) << ", ";
        os << to_hex(m_x_off) << ", " << to_hex(m_y_off);
        os << ");\n";
    }
}


void StationTileData::parse(TokenStream& is)
{
    is.match_ident(str_sprite);
    is.match(TokenType::OpenParen);
    m_sprite = is.match_uint32();
    is.match(TokenType::Comma);
    m_x_off = is.match_uint8();
    is.match(TokenType::Comma);
    m_y_off = is.match_uint8();

    m_new_bb = (is.peek().type == TokenType::Comma);
    if (m_new_bb)
    {
        is.match(TokenType::Comma);
        m_z_off = is.match_uint8();
        is.match(TokenType::Comma);
        m_x_ext = is.match_uint8();
        is.match(TokenType::Comma);
        m_y_ext = is.match_uint8();
        is.match(TokenType::Comma);
        m_z_ext = is.match_uint8();
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::SemiColon);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationTile::read(std::istream& is)
{
    m_ground_sprite = read_uint32(is);
    if (m_ground_sprite == 0x00000000)
    {
        return;
    }

    while (is.peek() != 0x80)
    {
        StationTileData datum;
        datum.read(is);
        m_data.push_back(datum);
    }
    read_uint8(is);
}


void StationTile::write(std::ostream& os) const
{
    write_uint32(os, m_ground_sprite);
    if (m_ground_sprite == 0x00000000)
    {
        return;
    }

    for (const auto& datum: m_data)
    {
        datum.write(os);
    }
    write_uint8(os, 0x80);
}


void StationTile::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_tile << "<" << to_hex(m_ground_sprite) << ">\n";
    os << pad(indent) << "{\n";

    if (m_ground_sprite != 0x0000'0000)
    {
        for (const auto& datum: m_data)
        {
            datum.print(os, indent + 4);
        }
    }

    os << pad(indent) << "}\n";

}


void StationTile::parse(TokenStream& is)
{
    is.match_ident(str_tile);
    is.match(TokenType::OpenAngle);
    m_ground_sprite = is.match_uint32();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        StationTileData datum{};
        datum.parse(is);
        m_data.push_back(datum);
    }

    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void StationLayout::read(std::istream& is)
{
    uint16_t num_tiles = read_uint8_ext(is);
    m_tiles.resize(num_tiles);
    for (uint16_t i = 0; i < num_tiles; ++i)
    {
        m_tiles[i].read(is);
    }
}


void StationLayout::write(std::ostream& os) const
{
    write_uint8_ext(os, uint16_t(m_tiles.size()));
    for (const auto& tile: m_tiles)
    {
        tile.write(os);
    }
}


void StationLayout::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "{\n";
    for (const auto& tile: m_tiles)
    {
        tile.print(os, indent + 4);
    }
    os << pad(indent) << "}";
}


void StationLayout::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        StationTile tile{};
        tile.parse(is);
        m_tiles.push_back(tile);
    }

    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CustomLayout::read(std::istream& is)
{
    m_platform_length = read_uint8(is);
    m_platform_count  = read_uint8(is);
    uint16_t dirs     = 0;
    uint16_t size     = m_platform_length * m_platform_count;
    for (uint16_t i = 0; i < size; ++i)
    {
        uint8_t temp = read_uint8(is);
        auto tile = static_cast<Platform>(temp & 0xFE);
        m_platform_tiles.push_back(tile);

        dirs += (temp & 0x01);
    }

    // Sum of directions should same as the number of platforms times length, or zero.
    // TODO check this.
    if (size > 0)
        m_direction = static_cast<Direction>(dirs / size);
}


void CustomLayout::write(std::ostream& os) const
{
    write_uint8(os, m_platform_length);
    write_uint8(os, m_platform_count);
    for (const auto tile: m_platform_tiles)
    {
        // Add back the orientation.
        write_uint8(os, static_cast<uint8_t>(tile) + static_cast<uint8_t>(m_direction));
    }

    // Assert that the vector has the correct length?
}


void CustomLayout::print(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_layout << "<";
    direction_desc.print_value(m_direction, os);
    os << ">\n";
    os << pad(indent) << "{\n";
    os << pad(indent + 4) << "// Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight\n";

    uint16_t index{};
    for (uint8_t c = 0; c < m_platform_count; ++c)
    {
        os << pad(indent + 4);
        for (uint8_t l = 0; l < m_platform_length; ++l)
        {
            // Assert that the vector has the correct length?
            const auto tile = m_platform_tiles[index++];
            switch (tile)
            {
                case Platform::Building:  os << "B "; break;
                case Platform::Plain:     os << "P "; break;
                case Platform::RoofLeft:  os << "L "; break;
                case Platform::RoofRight: os << "R "; break;
            }
        }
        os << ";\n";
    }

    os << pad(indent) << "}\n";
}


void CustomLayout::parse(TokenStream& is)
{
    is.match_ident(str_layout);
    is.match(TokenType::OpenAngle);
    direction_desc.parse(m_direction, is);
    is.match(TokenType::CloseAngle);

    m_platform_count = 0;
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        while (is.peek().type != TokenType::SemiColon)
        {
            Platform tile;
            platform_desc.parse(tile, is);
            m_platform_tiles.push_back(tile);
        }
        is.match(TokenType::SemiColon);
        ++m_platform_count;
    }
    is.match(TokenType::CloseBrace);

    m_platform_length = uint8_t(m_platform_tiles.size() / m_platform_count);
    if ((m_platform_tiles.size() % m_platform_count) != 0)
        throw PARSER_ERROR("Platform lengths do not match", is.peek());
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CustomStation::read(std::istream& is)
{
    while (true)
    {
        CustomLayout layout;
        layout.read(is);
        if (layout.terminator())
        {
            break;
        }
        m_layouts.push_back(layout);
    }
}


void CustomStation::write(std::ostream& os) const
{
    for (const auto& layout: m_layouts)
    {
        layout.write(os);
    }

    // Terminator
    write_uint8(os, 0x00);
    write_uint8(os, 0x00);
}


void CustomStation::print(std::ostream& os, uint16_t indent) const
{
    os << "\n" << pad(indent) << "{\n";
    for (const auto& layout: m_layouts)
    {
        layout.print(os, indent + 4);
    }
    os << pad(indent) << "}";
}


void CustomStation::parse(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        CustomLayout layout;
        layout.parse(is);
        m_layouts.push_back(layout);
    }
    is.match(TokenType::CloseBrace);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool Action00Stations::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_class_id.read(is);                  break;
        case 0x09: m_09_sprite_layout.read(is);             break;
        case 0x0A: m_0A_copy_sprite_layout_id.read(is);     break;
        case 0x0B: m_0B_callback_flags.read(is);            break;
        case 0x0C: m_0C_disabled_platform_numbers.read(is); break;
        case 0x0D: m_0D_disabled_platform_lengths.read(is); break;
        case 0x0E: m_0E_custom_layout.read(is);             break;
        case 0x0F: m_0F_copy_custom_layout_id.read(is);     break;
        case 0x10: m_10_little_lots_threshold.read(is);     break;
        case 0x11: m_11_pylon_placement.read(is);           break;
        case 0x12: m_12_cargo_type_triggers.read(is);       break;
        case 0x13: m_13_general_flags.read(is);             break;
        case 0x14: m_14_overhead_wire_placement.read(is);   break;
        case 0x15: m_15_can_train_enter_tile.read(is);      break;
        case 0x16: m_16_animation_info.read(is);            break;
        case 0x17: m_17_animation_speed.read(is);           break;
        case 0x18: m_18_animation_triggers.read(is);        break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00Stations::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_class_id.write(os);                  break;
        case 0x09: m_09_sprite_layout.write(os);             break;
        case 0x0A: m_0A_copy_sprite_layout_id.write(os);     break;
        case 0x0B: m_0B_callback_flags.write(os);            break;
        case 0x0C: m_0C_disabled_platform_numbers.write(os); break;
        case 0x0D: m_0D_disabled_platform_lengths.write(os); break;
        case 0x0E: m_0E_custom_layout.write(os);             break;
        case 0x0F: m_0F_copy_custom_layout_id.write(os);     break;
        case 0x10: m_10_little_lots_threshold.write(os);     break;
        case 0x11: m_11_pylon_placement.write(os);           break;
        case 0x12: m_12_cargo_type_triggers.write(os);       break;
        case 0x13: m_13_general_flags.write(os);             break;
        case 0x14: m_14_overhead_wire_placement.write(os);   break;
        case 0x15: m_15_can_train_enter_tile.write(os);      break;
        case 0x16: m_16_animation_info.write(os);            break;
        case 0x17: m_17_animation_speed.write(os);           break;
        case 0x18: m_18_animation_triggers.write(os);        break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00Stations::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_class_id, os, indent);                  break;
        case 0x09: desc_09.print(m_09_sprite_layout, os, indent);             break;
        case 0x0A: desc_0A.print(m_0A_copy_sprite_layout_id, os, indent);     break;
        case 0x0B: desc_0B.print(m_0B_callback_flags, os, indent);            break;
        case 0x0C: desc_0C.print(m_0C_disabled_platform_numbers, os, indent); break;
        case 0x0D: desc_0D.print(m_0D_disabled_platform_lengths, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_custom_layout, os, indent);             break;
        case 0x0F: desc_0F.print(m_0F_copy_custom_layout_id, os, indent);     break;
        case 0x10: desc_10.print(m_10_little_lots_threshold, os, indent);     break;
        case 0x11: desc_11.print(m_11_pylon_placement, os, indent);           break;
        case 0x12: desc_12.print(m_12_cargo_type_triggers, os, indent);       break;
        case 0x13: desc_13.print(m_13_general_flags, os, indent);             break;
        case 0x14: desc_14.print(m_14_overhead_wire_placement, os, indent);   break;
        case 0x15: desc_15.print(m_15_can_train_enter_tile, os, indent);      break;
        case 0x16: desc_16.print(m_16_animation_info, os, indent);            break;
        case 0x17: desc_17.print(m_17_animation_speed, os, indent);           break;
        case 0x18: desc_18.print(m_18_animation_triggers, os, indent);        break;
        default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
    }

    return true;
}


bool Action00Stations::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_class_id, is);                  break;
            case 0x09: desc_09.parse(m_09_sprite_layout, is);             break;
            case 0x0A: desc_0A.parse(m_0A_copy_sprite_layout_id, is);     break;
            case 0x0B: desc_0B.parse(m_0B_callback_flags, is);            break;
            case 0x0C: desc_0C.parse(m_0C_disabled_platform_numbers, is); break;
            case 0x0D: desc_0D.parse(m_0D_disabled_platform_lengths, is); break;
            case 0x0E: desc_0E.parse(m_0E_custom_layout, is);             break;
            case 0x0F: desc_0F.parse(m_0F_copy_custom_layout_id, is);     break;
            case 0x10: desc_10.parse(m_10_little_lots_threshold, is);     break;
            case 0x11: desc_11.parse(m_11_pylon_placement, is);           break;
            case 0x12: desc_12.parse(m_12_cargo_type_triggers, is);       break;
            case 0x13: desc_13.parse(m_13_general_flags, is);             break;
            case 0x14: desc_14.parse(m_14_overhead_wire_placement, is);   break;
            case 0x15: desc_15.parse(m_15_can_train_enter_tile, is);      break;
            case 0x16: desc_16.parse(m_16_animation_info, is);            break;
            case 0x17: desc_17.parse(m_17_animation_speed, is);           break;
            case 0x18: desc_18.parse(m_18_animation_triggers, is);        break;
            default:   throw PROPERTY_ERROR("Unknown property", feature(), property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", feature(), property);
}
