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
#include "properties/BitfieldValue.h"
#include "properties/VisualEffect.h"
#include "CargoAcceptanceDescriptor.h"


using CargosProperty       = UInt8ListProperty;
using VisualEffectProperty = Property<VisualEffect>;
using CargoListProperty    = Property<Vector<CargoAcceptance>>;
using Bit8Property         = Property<BitfieldValue<uint8_t>>;


class Action00Ships : public Action00Vehicles
{
public:
    Action00Ships() : Action00Vehicles{FeatureType::Ships} {}

private:
    UInt8Property        m_prop_08{m_properties, 0x08, "sprite_id"};
    BoolProperty         m_prop_09{m_properties, 0x09, "is_refittable"};
    UInt8Property        m_prop_0A{m_properties, 0x0A, "cost_factor"};
    UInt8Property        m_prop_0B{m_properties, 0x0B, "speed_2_kmh"};
    UInt8Property        m_prop_0C{m_properties, 0x0C, "cargo_type"};
    UInt16Property       m_prop_0D{m_properties, 0x0D, "cargo_capacity"};
    UInt8Property        m_prop_0F{m_properties, 0x0F, "running_cost_factor"};
    UInt8Property        m_prop_10{m_properties, 0x10, "sound_effect_type"};
    UInt32Property       m_prop_11{m_properties, 0x11, "refit_cargo_types"};
    UInt8Property        m_prop_12{m_properties, 0x12, "callback_flags_mask"};
    UInt8Property        m_prop_13{m_properties, 0x13, "refit_cost"};
    UInt8Property        m_prop_14{m_properties, 0x14, "ocean_speed_fraction"};
    UInt8Property        m_prop_15{m_properties, 0x15, "canal_speed_fraction"};
    UInt8Property        m_prop_16{m_properties, 0x16, "retire_vehicle_early"};
    UInt8Property        m_prop_17{m_properties, 0x17, "miscellaneous_flags"};
    UInt16Property       m_prop_18{m_properties, 0x18, "refittable_cargo_classes"};
    UInt16Property       m_prop_19{m_properties, 0x19, "non_refittable_cargo_classes"};
    LongDateProperty     m_prop_1A{m_properties, 0x1A, "long_introduction_date"};
    UInt8XProperty       m_prop_1B{m_properties, 0x1B, "sort_purchase_list"};
    VisualEffectProperty m_prop_1C{m_properties, 0x1C, "visual_effect"};
    UInt16Property       m_prop_1D{m_properties, 0x1D, "custom_cargo_aging_period"};
    UInt8ListProperty    m_prop_1E{m_properties, 0x1E, "always_refittable_cargos"};
    UInt8ListProperty    m_prop_1F{m_properties, 0x1F, "never_refittable_cargos"};
    UInt16Property       m_prop_20{m_properties, 0x20, "variant_group"};
    UInt32Property       m_prop_21{m_properties, 0x21, "extra_flags"};
    UInt8Property        m_prop_22{m_properties, 0x22, "extra_callback_flags_mask"};
    UInt16Property       m_prop_23{m_properties, 0x23, "speed_3_2_mph"};
    UInt8Property        m_prop_24{m_properties, 0x24, "acceleration_3_2_mph"};
};


