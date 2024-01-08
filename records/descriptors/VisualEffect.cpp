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
#include "VisualEffect.h"
#include "EnumDescriptor.h"
#include "StreamHelpers.h"


namespace {

using Effect = VisualEffect::Type;
const EnumDescriptorT<Effect> desc_effect
{
    0x00, "effect",
    {
        { static_cast<uint8_t>(Effect::Default),          "Default" },
        { static_cast<uint8_t>(Effect::SteamPuffs),       "SteamPuffs" },
        { static_cast<uint8_t>(Effect::DieselFumes),      "DieselFumes" },
        { static_cast<uint8_t>(Effect::ElectricSparks),   "ElectricSparks" },
        { static_cast<uint8_t>(Effect::DisableEffect),    "DisableEffect" },
        { static_cast<uint8_t>(Effect::SteamCallback),    "SteamCallback" },
        { static_cast<uint8_t>(Effect::DieselCallback),   "DieselCallback" },
        { static_cast<uint8_t>(Effect::ElectricCallback), "ElectricCallback" },
    }
};

using Power = VisualEffect::WagonPower;
const EnumDescriptorT<Power> desc_power
{
    0x00, "power",
    {
        { static_cast<uint8_t>(Power::Enable),  "Enable" },
        { static_cast<uint8_t>(Power::Disable), "Disable" },
    }
};

static constexpr const char* str_effect = "effect";

} // namespace {


void VisualEffect::read(std::istream& is)
{
    uint8_t value = read_uint8(is);
    if (value & 0x40)
    {
        m_effect   = static_cast<Type>(value & 0x43);
        m_position = 0;
    }
    else
    {
        m_effect   = static_cast<Type>(value & 0x30);
        m_position = (value & 0x0F);
    }
    m_wagon_power = (value & 0x80) ? WagonPower::Disable : WagonPower::Enable;
}


void VisualEffect::write(std::ostream& os) const
{
    uint8_t value = m_position;
    value |= static_cast<uint8_t>(m_effect);
    value |= static_cast<uint8_t>(m_wagon_power);
    write_uint8(os, value);
}


void VisualEffect::print(std::ostream& os) const
{
    os << str_effect << "(";
    os << desc_effect.value(m_effect) << ", ";
    os << to_hex(m_position) << ", ";
    os << desc_power.value(m_wagon_power) << ")";
}


void VisualEffect::parse(TokenStream& is)
{
    is.match_ident(str_effect);
    is.match(TokenType::OpenParen);
    desc_effect.parse(m_effect, is);

    // Optional feature.
    m_position = 0;
    if (is.peek().type == TokenType::Comma)
    {
        is.match(TokenType::Comma);
        m_position = is.match_uint8();
    }

    // Optional feature.
    m_wagon_power = WagonPower::Enable;
    if (is.peek().type == TokenType::Comma)
    {
        is.match(TokenType::Comma);
        desc_power.parse(m_wagon_power, is);
    }

    is.match(TokenType::CloseParen);
}

