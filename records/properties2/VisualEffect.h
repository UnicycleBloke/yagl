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
#pragma once
#include "properties2/PropertyMap.h"


class VisualEffect
{
public:
    enum class Type : uint8_t
    {
        Default          = 0x00,
        SteamPuffs       = 0x10,
        DieselFumes      = 0x20,
        ElectricSparks   = 0x30,
        DisableEffect    = 0x40,
        SteamCallback    = 0x41,
        DieselCallback   = 0x42,
        ElectricCallback = 0x43
    };

    enum class WagonPower : uint8_t
    {
        Enable  = 0x00,
        Disable = 0x80
    };

public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent = 0) const;
    void parse(TokenStream& is);

private:
    Type       m_effect{Type::Default};
    uint8_t    m_position{};
    WagonPower m_wagon_power{WagonPower::Enable};
};
