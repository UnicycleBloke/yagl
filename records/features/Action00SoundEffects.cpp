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
#include "Action00SoundEffects.h"
#include "StreamHelpers.h"


namespace {


constexpr const char* str_relative_volume    = "relative_volume";
constexpr const char* str_priority           = "priority";
constexpr const char* str_override_old_sound = "override_old_sound";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint8_t> g_indices =
{
    { str_relative_volume,    0x08 },
    { str_priority,           0x09 },
    { str_override_old_sound, 0x0A },
};


constexpr UInt8Descriptor desc_08 = { 0x08, str_relative_volume,    UIntFormat::Hex };
constexpr UInt8Descriptor desc_09 = { 0x09, str_priority,           UIntFormat::Hex };
constexpr UInt8Descriptor desc_0A = { 0x0A, str_override_old_sound, UIntFormat::Hex };


} // namespace {


bool Action00SoundEffects::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x08: m_08_relative_volume.read(is);    break;
        case 0x09: m_09_priority.read(is);           break;
        case 0x0A: m_0A_override_old_sound.read(is); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00SoundEffects::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x08: m_08_relative_volume.write(os);    break;
        case 0x09: m_09_priority.write(os);           break;
        case 0x0A: m_0A_override_old_sound.write(os); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00SoundEffects::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x08: desc_08.print(m_08_relative_volume, os, indent);    break;
        case 0x09: desc_09.print(m_09_priority, os, indent);           break;
        case 0x0A: desc_0A.print(m_0A_override_old_sound, os, indent); break;
        default:   throw PROPERTY_ERROR("Unknown property", property);
    }

    return true;
}


bool Action00SoundEffects::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        property = it->second;
        switch (property)
        {
            case 0x08: desc_08.parse(m_08_relative_volume, is);    break;
            case 0x09: desc_09.parse(m_09_priority, is);           break;
            case 0x0A: desc_0A.parse(m_0A_override_old_sound, is); break;
            default:   throw PROPERTY_ERROR("Unknown property", property);
        }

        return true;
    }

    throw PROPERTY_ERROR("Unknown property", property);
}
