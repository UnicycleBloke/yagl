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
#include "SpriteLayout.h"
#include "StationLayout.h"
#include "CustomStation.h"
#include "IntegerDescriptor.h"
#include "IntegerValue.h"
#include "PropertyMap.h"
#include "GRFLabel.h"
#include <vector>


using StationLayoutProperty = Property<StationLayout>;
using CustomStationProperty = Property<CustomStation>;
using SpriteLayoutsProperty = Property<Vector<SpriteLayout>>;


class Action00Stations : public Action00Feature
{
public:
    Action00Stations() : Action00Feature{FeatureType::Stations} {}

private:
    GRFLabelProperty      m_prop_08{m_properties, 0x08, "class_id"};
    StationLayoutProperty m_prop_09{m_properties, 0x09, "sprite_layouts"};
    UInt8Property         m_prop_0A{m_properties, 0x0A, "copy_sprite_layout_id"};
    UInt8Property         m_prop_0B{m_properties, 0x0B, "callback_flags"};
    UInt8Property         m_prop_0C{m_properties, 0x0C, "disabled_platform_numbers"};
    UInt8Property         m_prop_0D{m_properties, 0x0D, "disabled_platform_lengths"};
    CustomStationProperty m_prop_0E{m_properties, 0x0E, "custom_layouts"};
    UInt8Property         m_prop_0F{m_properties, 0x0F, "copy_custom_layout_id"};
    UInt16Property        m_prop_10{m_properties, 0x10, "little_lots_threshold"};
    UInt8Property         m_prop_11{m_properties, 0x11, "pylon_placement"};
    UInt32Property        m_prop_12{m_properties, 0x12, "cargo_type_triggers"};
    UInt8Property         m_prop_13{m_properties, 0x13, "general_flags"};
    UInt8Property         m_prop_14{m_properties, 0x14, "overhead_wire_placement"};
    UInt8Property         m_prop_15{m_properties, 0x15, "can_train_enter_tile"};
    UInt16Property        m_prop_16{m_properties, 0x16, "animation_info"};
    UInt8Property         m_prop_17{m_properties, 0x17, "animation_speed"};
    UInt16Property        m_prop_18{m_properties, 0x18, "animation_triggers"};
    SpriteLayoutsProperty m_prop_1A{m_properties, 0x1A, "advanced_sprite_layout"};
    UInt8ArrayProperty<8> m_prop_1B{m_properties, 0x1B, "minimum_bridge_height"};
    UInt16Property        m_prop_1C{m_properties, 0x1C, "station_name_id"};
    UInt16Property        m_prop_1D{m_properties, 0x1D, "station_class_name_id"};
};


