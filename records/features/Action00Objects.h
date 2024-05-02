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
#include "BitfieldValue.h"
#include "PropertyMap.h"
#include "ValueTypeVector.h"


using LongDateProperty     = Property<LongDate>;
using UInt16Property       = Property<UInt2<uint16_t>>;
using UInt8Property        = Property<UInt2<uint8_t>>;
using GRFLabelProperty     = Property<GRFLabel>;
using GRFLabelListProperty = Property<ValueTypeVector<GRFLabel>>;
using Bit8Property         = Property<BitfieldValue<uint8_t>>;


class Action00Objects : public Action00Feature
{
public:
    Action00Objects() : Action00Feature(FeatureType::Objects) {}

private:
    std::vector<BitfieldItem> m_climates{ {0, "Temperate"}, {1, "Arctic"}, {2, "Tropical"}, {3, "Toyland"} };

    GRFLabelProperty m_prop_08{m_properties, 0x08, "class_label"};
    UInt16Property   m_prop_09{m_properties, 0x09, "class_text_id"};
    UInt16Property   m_prop_0A{m_properties, 0x0A, "object_text_id"};
    Bit8Property     m_prop_0B{m_properties, 0x0B, "climate_availability", m_climates};
    UInt8Property    m_prop_0C{m_properties, 0x0C, "size_xy"};
    UInt8Property    m_prop_0D{m_properties, 0x0D, "cost_factor"};
    LongDateProperty m_prop_0E{m_properties, 0x0E, "introduction_date"};
    LongDateProperty m_prop_0F{m_properties, 0x0F, "end_of_life_date"};
    UInt16Property   m_prop_10{m_properties, 0x10, "object_flags"};
    UInt16Property   m_prop_11{m_properties, 0x11, "animation_info"};
    UInt8Property    m_prop_12{m_properties, 0x12, "animation_speed"};
    UInt16Property   m_prop_13{m_properties, 0x13, "animation_triggers"};
    UInt8Property    m_prop_14{m_properties, 0x14, "removal_cost_factor"};
    UInt16Property   m_prop_15{m_properties, 0x15, "callback_flags"};
    UInt8Property    m_prop_16{m_properties, 0x16, "building_height"};
    UInt8Property    m_prop_17{m_properties, 0x17, "number_of_views"};
    UInt8Property    m_prop_18{m_properties, 0x18, "number_on_creation"};
};

