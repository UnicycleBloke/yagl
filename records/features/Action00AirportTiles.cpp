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
#include "Action00AirportTiles.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_substitute_tile_id   = "substitute_tile_id";
constexpr const char* str_aiport_tile_override = "aiport_tile_override";
constexpr const char* str_callback_flags       = "callback_flags";
constexpr const char* str_animation_info       = "animation_info";
constexpr const char* str_animation_speed      = "animation_speed";
constexpr const char* str_animation_triggers   = "animation_triggers";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_substitute_tile_id,   0x08 },
    { str_aiport_tile_override, 0x09 },
    { str_callback_flags,       0x0E },
    { str_animation_info,       0x0F },
    { str_animation_speed,      0x10 },
    { str_animation_triggers,   0x11 },
};


constexpr UInt8Descriptor  desc_08 = { 0x08, str_substitute_tile_id,   UIntFormat::Hex };
constexpr UInt8Descriptor  desc_09 = { 0x09, str_aiport_tile_override, UIntFormat::Hex };
constexpr UInt8Descriptor  desc_0E = { 0x0E, str_callback_flags,       UIntFormat::Hex };
constexpr UInt16Descriptor desc_0F = { 0x0F, str_animation_info,       UIntFormat::Hex };
constexpr UInt8Descriptor  desc_10 = { 0x10, str_animation_speed,      UIntFormat::Hex };
constexpr UInt8Descriptor  desc_11 = { 0x11, str_animation_triggers,   UIntFormat::Hex };


} // namespace {


bool Action00AirportTiles::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_substitute_tile_id.read(is);   break;
        case 0x09: m_09_aiport_tile_override.read(is); break;
        case 0x0E: m_0E_callback_flags.read(is);       break;
        // The low byte specifies the number of animation frames minus one, so 00 means 1 frame,
        // 01 means 2 frames etc. The maximum number of frames is 256, although you can have some
        // problems if your animation exceeds FD (253) frames. The high byte must be 0 for
        // on-looping animations and 01 for looping animations. Every other value is reserved for
        // future use. In addition, if the whole word contains FFFF, animation is turned off for
        // this tile (this is the default value).
        case 0x0F: m_0F_animation_info.read(is);       break;
        case 0x10: m_10_animation_speed.read(is);      break;
        case 0x11: m_11_animation_triggers.read(is);   break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00AirportTiles::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_substitute_tile_id.write(os);   break;
        case 0x09: m_09_aiport_tile_override.write(os); break;
        case 0x0E: m_0E_callback_flags.write(os);       break;
        case 0x0F: m_0F_animation_info.write(os);       break;
        case 0x10: m_10_animation_speed.write(os);      break;
        case 0x11: m_11_animation_triggers.write(os);   break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00AirportTiles::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_substitute_tile_id, os, indent);   break;
        case 0x09: desc_09.print(m_09_aiport_tile_override, os, indent); break;
        case 0x0E: desc_0E.print(m_0E_callback_flags, os, indent);       break;
        case 0x0F: desc_0F.print(m_0F_animation_info, os, indent);       break;
        case 0x10: desc_10.print(m_10_animation_speed, os, indent);      break;
        case 0x11: desc_11.print(m_11_animation_triggers, os, indent);   break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00AirportTiles::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_substitute_tile_id, is);   break;
            case 0x09: desc_09.parse(m_09_aiport_tile_override, is); break;
            case 0x0E: desc_0E.parse(m_0E_callback_flags, is);       break;
            case 0x0F: desc_0F.parse(m_0F_animation_info, is);       break;
            case 0x10: desc_10.parse(m_10_animation_speed, is);      break;
            case 0x11: desc_11.parse(m_11_animation_triggers, is);   break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}

