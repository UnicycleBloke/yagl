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
#include "GRFLabel.h"
#include "DateDescriptor.h"
#include "IntegerValue.h"
#include "PropertyMap.h"
#include "Vector.h"


using LongDateProperty     = Property<LongDate>;
using UInt32Property       = Property<UInt2<uint32_t>>;
using UInt16Property       = Property<UInt2<uint16_t>>;
using UInt8Property        = Property<UInt2<uint8_t>>;
using GRFLabelProperty     = Property<GRFLabel>;
using GRFLabelListProperty = Property<Vector<GRFLabel>>;


// TODO Add unit tests for this type.
class Action00RoadStops : public Action00Feature
{
public:
    Action00RoadStops() : Action00Feature(FeatureType::RoadStops) {}

private:
    GRFLabelProperty m_prop_08{m_properties, 0x08, "class_label"};
    UInt8Property    m_prop_09{m_properties, 0x09, "road_stop_type"};
    UInt16Property   m_prop_0A{m_properties, 0x0A, "road_stop_name_text_id"};
    UInt16Property   m_prop_0B{m_properties, 0x0B, "class_name_text_id"};
    UInt8Property    m_prop_0C{m_properties, 0x0C, "draw_mode"};
    UInt32Property   m_prop_0D{m_properties, 0x0D, "random_trigger_cargoes"}; 
    UInt16Property   m_prop_0E{m_properties, 0x0E, "animation_information"};
    UInt8Property    m_prop_0F{m_properties, 0x0F, "animation_speed"};
    UInt16Property   m_prop_10{m_properties, 0x10, "animation_triggers"}; 
    UInt8Property    m_prop_11{m_properties, 0x11, "callback_flags"};
    UInt32Property   m_prop_12{m_properties, 0x12, "general_flags"};
    UInt16Property   m_prop_15{m_properties, 0x15, "cost_multipliers"};
};

