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
#include "properties/IndustryMultipliers.h"
#include "properties/IndustryLayout.h"
#include "properties/IntegerValue.h"
#include "properties/PropertyMap.h"
#include "properties/Vector.h"
#include "properties/Array.h"


using IndustryLayoutsProperty = Property<IndustryLayouts>;
using MultipliersProperty     = Property<IndustryMultipliers>;


class Action00Industries : public Action00Feature
{
public:
    Action00Industries() : Action00Feature{FeatureType::Industries} {}

private:
    UInt8Property           m_prop_08{m_properties, 0x08, "substitute_industry_id"};
    UInt8Property           m_prop_09{m_properties, 0x09, "industry_type_override"};
    IndustryLayoutsProperty m_prop_0A{m_properties, 0x0A, "industry_layout"};
    UInt8Property           m_prop_0B{m_properties, 0x0B, "production_flags"};
    UInt16Property          m_prop_0C{m_properties, 0x0C, "closure_msg_id"};
    UInt16Property          m_prop_0D{m_properties, 0x0D, "production_up_id"};
    UInt16Property          m_prop_0E{m_properties, 0x0E, "production_down_id"};
    UInt8Property           m_prop_0F{m_properties, 0x0F, "fund_cost_multiplier"};
    UInt8ArrayProperty<2>   m_prop_10{m_properties, 0x10, "production_cargo_types"};
    UInt8ArrayProperty<4>   m_prop_11{m_properties, 0x11, "acceptance_cargo_types"};
    UInt8Property           m_prop_12{m_properties, 0x12, "production_multipliers_1"};
    UInt8Property           m_prop_13{m_properties, 0x13, "production_multipliers_2"};
    UInt8Property           m_prop_14{m_properties, 0x14, "minimum_distributed"};
    UInt8ListProperty       m_prop_15{m_properties, 0x15, "sound_effects"};
    UInt8ArrayProperty<3>   m_prop_16{m_properties, 0x16, "conflicting_industries"};
    UInt8Property           m_prop_17{m_properties, 0x17, "random_probability"};
    UInt8Property           m_prop_18{m_properties, 0x18, "gameplay_probability"};
    UInt8Property           m_prop_19{m_properties, 0x19, "map_colour"};
    UInt32Property          m_prop_1A{m_properties, 0x1A, "special_flags"};
    UInt16Property          m_prop_1B{m_properties, 0x1B, "new_industry_text_id"};
    UInt32Property          m_prop_1C{m_properties, 0x1C, "input_multipliers1"};
    UInt32Property          m_prop_1D{m_properties, 0x1D, "input_multipliers2"};
    UInt32Property          m_prop_1E{m_properties, 0x1E, "input_multipliers3"};
    UInt16Property          m_prop_1F{m_properties, 0x1F, "industry_name_id"};
    UInt32Property          m_prop_20{m_properties, 0x20, "prospecting_probability"};
    UInt8Property           m_prop_21{m_properties, 0x21, "callback_flags_1"};
    UInt8Property           m_prop_22{m_properties, 0x22, "callback_flags_2"};
    UInt32Property          m_prop_23{m_properties, 0x23, "destruction_cost_multiplier"};
    UInt16Property          m_prop_24{m_properties, 0x24, "nearby_station_text_id"};
    UInt8ListProperty       m_prop_25{m_properties, 0x25, "production_cargo_list"};
    UInt8ListProperty       m_prop_26{m_properties, 0x26, "acceptance_cargo_list"};
    UInt8ListProperty       m_prop_27{m_properties, 0x27, "production_multipliers"};
    MultipliersProperty     m_prop_28{m_properties, 0x28, "input_cargo_multipliers"};
};


