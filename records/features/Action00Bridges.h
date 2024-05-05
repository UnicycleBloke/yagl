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
#include "IntegerValue.h"
#include "YearDescriptor.h"
#include "PropertyMap.h"
#include "BridgeLayout.h"
#include <vector>
#include <array>


using Year8Property  = Property<Year8>;
using Year16Property = Property<Year16>;
using Year32Property = Property<Year32>;
using BridgeLayoutProperty = Property<BridgeLayout>;


class Action00Bridges : public Action00Feature
{
public:
    Action00Bridges() : Action00Feature(FeatureType::Bridges) {}

private:
    UInt8Property        m_prop_00{m_properties, 0x00, "fallback_type_id"};
    Year8Property        m_prop_08{m_properties, 0x08, "year_available"};
    UInt8Property        m_prop_09{m_properties, 0x09, "minimum_length"};
    UInt8Property        m_prop_0A{m_properties, 0x0A, "maximum_length"};
    UInt8Property        m_prop_0B{m_properties, 0x0B, "cost_factor"};
    UInt16Property       m_prop_0C{m_properties, 0x0C, "maximum_speed"};
    BridgeLayoutProperty m_prop_0D{m_properties, 0x0D, "bridge_layout"};
    UInt8Property        m_prop_0E{m_properties, 0x0E, "various_flags"};
    Year32Property       m_prop_0F{m_properties, 0x0F, "long_year_available"};
    UInt16Property       m_prop_10{m_properties, 0x10, "purchase_text"};
    UInt16Property       m_prop_11{m_properties, 0x11, "description_rail"};
    UInt16Property       m_prop_12{m_properties, 0x12, "description_road"};
    UInt16Property       m_prop_13{m_properties, 0x13, "cost_factor_word"};
};


