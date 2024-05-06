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
#include "properties/GRFLabel.h"
#include "properties/DateValue.h"
#include "properties/IntegerValue.h"
#include "properties/PropertyMap.h"
#include "properties/Vector.h"
#include "properties/Array.h"
#include "CargoAcceptanceDescriptor.h"


using CargoListProperty = Property<Vector<CargoAcceptance>>;


class Action00Houses : public Action00Feature
{
public:
    Action00Houses() : Action00Feature(FeatureType::Houses) {}

private:
    UInt8Property         m_prop_08{m_properties, 0x08, "substitute_building_id"};
    UInt8Property         m_prop_09{m_properties, 0x09, "building_flags"};
    Year8PairProperty     m_prop_0A{m_properties, 0x0A, "years_available"};
    UInt8Property         m_prop_0B{m_properties, 0x0B, "population"};
    UInt8Property         m_prop_0C{m_properties, 0x0C, "mail_multiplier"};
    UInt8Property         m_prop_0D{m_properties, 0x0D, "passenger_acceptance"};
    UInt8Property         m_prop_0E{m_properties, 0x0E, "mail_acceptance"};
    UInt8Property         m_prop_0F{m_properties, 0x0F, "goods_etc_acceptance"};
    UInt16Property        m_prop_10{m_properties, 0x10, "la_rating_decrease"};
    UInt8Property         m_prop_11{m_properties, 0x11, "removal_cost_multiplier"};
    UInt16Property        m_prop_12{m_properties, 0x12, "building_name_id"};
    UInt16Property        m_prop_13{m_properties, 0x13, "availability_mask"};
    UInt8Property         m_prop_14{m_properties, 0x14, "callback_flags"};
    UInt8Property         m_prop_15{m_properties, 0x15, "override_byte"};
    UInt8Property         m_prop_16{m_properties, 0x16, "refresh_multiplier"};
    UInt8ArrayProperty<4> m_prop_17{m_properties, 0x17, "four_random_colours"};
    UInt8Property         m_prop_18{m_properties, 0x18, "appearance_probability"};
    UInt8Property         m_prop_19{m_properties, 0x19, "extra_flags"};
    UInt8Property         m_prop_1A{m_properties, 0x1A, "animation_frames"};
    UInt8Property         m_prop_1B{m_properties, 0x1B, "animation_speed"};
    UInt8Property         m_prop_1C{m_properties, 0x1C, "building_class"};
    UInt8Property         m_prop_1D{m_properties, 0x1D, "callback_flags_2"};
    UInt8ArrayProperty<4> m_prop_1E{m_properties, 0x1E, "accepted_cargo_types"};
    UInt16Property        m_prop_1F{m_properties, 0x1F, "minimum_life_years"};
    UInt8ListProperty     m_prop_20{m_properties, 0x20, "accepted_cargo_list"};
    Year16Property        m_prop_21{m_properties, 0x21, "long_minimum_year"};
    Year16Property        m_prop_22{m_properties, 0x22, "long_maximum_year"};
    CargoListProperty     m_prop_23{m_properties, 0x23, "tile_acceptance_list"};
};


