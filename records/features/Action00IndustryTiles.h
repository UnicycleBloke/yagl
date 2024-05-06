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
#include "properties/CargoAcceptance.h"


class Action00IndustryTiles : public Action00Feature
{
public:
    Action00IndustryTiles() : Action00Feature(FeatureType::IndustryTiles) {}

private:
    UInt8Property     m_prop_08{m_properties, 0x08, "substitute_building_id"};
    UInt8Property     m_prop_09{m_properties, 0x09, "industry_tile_override"};
    UInt16Property    m_prop_0A{m_properties, 0x0A, "tile_acceptance1"};
    UInt16Property    m_prop_0B{m_properties, 0x0B, "tile_acceptance2"};
    UInt16Property    m_prop_0C{m_properties, 0x0C, "tile_acceptance3"};
    UInt8Property     m_prop_0D{m_properties, 0x0D, "land_shape_flags"};
    UInt8Property     m_prop_0E{m_properties, 0x0E, "callback_flags"};
    UInt16Property    m_prop_0F{m_properties, 0x0F, "animation_info"};
    UInt8Property     m_prop_10{m_properties, 0x10, "animation_speed"};
    UInt8Property     m_prop_11{m_properties, 0x11, "callback_25_triggers"};
    UInt8Property     m_prop_12{m_properties, 0x12, "special_flags"};
    CargoListProperty m_prop_13{m_properties, 0x13, "cargo_acceptance_list"};
};


