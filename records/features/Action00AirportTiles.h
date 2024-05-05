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
#include "Action00Feature.h"
#include "IntegerDescriptor.h"


class Action00AirportTiles : public Action00Feature
{
public:
    Action00AirportTiles() : Action00Feature{FeatureType::AirportTiles} {}

private:
    UInt8Property   m_prop_08{m_properties, 0x08, "substitute_tile_id"};
    UInt8Property   m_prop_09{m_properties, 0x09, "aiport_tile_override"};
    UInt8Property   m_prop_0E{m_properties, 0x0E, "callback_flags"};
    // The low byte specifies the number of animation frames minus one, so 00 means 1 frame,
    // 01 means 2 frames etc. The maximum number of frames is 256, although you can have some
    // problems if your animation exceeds FD (253) frames. The high byte must be 0 for
    // on-looping animations and 01 for looping animations. Every other value is reserved for
    // future use. In addition, if the whole word contains FFFF, animation is turned off for
    // this tile (this is the default value).
    UInt16Property  m_prop_0F{m_properties, 0x0F, "animation_info"};
    UInt8Property   m_prop_10{m_properties, 0x10, "animation_speed"};
    UInt8Property   m_prop_11{m_properties, 0x11, "animation_triggers"};
};


