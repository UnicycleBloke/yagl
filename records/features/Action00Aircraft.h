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
#include "Action00Vehicles.h"
#include "properties/GRFLabel.h"
#include "properties/DateValue.h"
#include "properties/Boolean.h"
#include "properties/IntegerValue.h"
#include "properties/PropertyMap.h"
#include "properties/Vector.h"
#include "properties/Array.h"


using CargosProperty = UInt8ListProperty;


class Action00Aircraft : public Action00Vehicles
{
public:
    Action00Aircraft() : Action00Vehicles{FeatureType::Aircraft} {}

private:
    UInt8Property     m_prop_08{m_properties, 0x08, "sprite_id"};
    BoolHeliProperty  m_prop_09{m_properties, 0x09, "is_helicopter"};
    BoolProperty      m_prop_0A{m_properties, 0x0A, "is_large"};
    UInt8Property     m_prop_0B{m_properties, 0x0B, "cost_factor"};
    UInt8Property     m_prop_0C{m_properties, 0x0C, "speed_8_mph"};
    UInt8Property     m_prop_0D{m_properties, 0x0D, "acceleration"};
    UInt8Property     m_prop_0E{m_properties, 0x0E, "running_cost_factor"};
    UInt16Property    m_prop_0F{m_properties, 0x0F, "passenger_capacity"};
    UInt8Property     m_prop_11{m_properties, 0x11, "mail_capacity"};
    UInt8Property     m_prop_12{m_properties, 0x12, "sound_effect_type"};
    UInt32Property    m_prop_13{m_properties, 0x13, "refit_cargo_types"};   // GRFv >= 6
    UInt8Property     m_prop_14{m_properties, 0x14, "callback_flags_mask"}; // GRFv >= 6
    UInt8Property     m_prop_15{m_properties, 0x15, "refit_cost"};
    UInt8Property     m_prop_16{m_properties, 0x16, "retire_vehicle_early"};
    UInt8Property     m_prop_17{m_properties, 0x17, "miscellaneous_flags"};
    UInt16Property    m_prop_18{m_properties, 0x18, "refittable_cargo_classes"};
    UInt16Property    m_prop_19{m_properties, 0x19, "non_refittable_cargo_classes"};
    LongDateProperty  m_prop_1A{m_properties, 0x1A, "long_introduction_date"};
    UInt8XProperty    m_prop_1B{m_properties, 0x1B, "sort_purchase_list"};
    UInt16Property    m_prop_1C{m_properties, 0x1C, "custom_cargo_aging_period"};
    CargosProperty    m_prop_1D{m_properties, 0x1D, "always_refittable_cargos"};
    CargosProperty    m_prop_1E{m_properties, 0x1E, "never_refittable_cargos"};
    UInt16Property    m_prop_1F{m_properties, 0x1F, "aircraft_range"};
    UInt16Property    m_prop_20{m_properties, 0x20, "variant_group"};
    UInt32Property    m_prop_21{m_properties, 0x21, "extra_flags"};
    UInt8Property     m_prop_22{m_properties, 0x22, "extra_callback_flags_mask"};
};
