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
#include "properties2/GRFLabel.h"
#include "properties2/DateValue.h"
#include "properties2/Boolean.h"
#include "properties2/IntegerValue.h"
#include "properties2/PropertyMap.h"
#include "properties2/Vector.h"
#include "properties2/Array.h"
#include "properties2/VisualEffect.h"
#include "CargoAcceptanceDescriptor.h"


using CargosProperty       = UInt8ListProperty;
using VisualEffectProperty = Property<VisualEffect>;
using CargoListProperty    = Property<Vector<CargoAcceptance>>;


class Action00RoadVehicles : public Action00Vehicles

{
public:
    Action00RoadVehicles() : Action00Vehicles{FeatureType::RoadVehicles} {}

private:
    UInt8Property         m_prop_05{m_properties, 0x05, "roadtype_tramtype"};
    UInt8Property         m_prop_08{m_properties, 0x08, "speed_2_kmh"};
    UInt8Property         m_prop_09{m_properties, 0x09, "running_cost_factor"};
    UInt32Property        m_prop_0A{m_properties, 0x0A, "running_cost_base"};
    UInt8Property         m_prop_0E{m_properties, 0x0E, "sprite_id"};
    UInt8Property         m_prop_0F{m_properties, 0x0F, "cargo_capacity"};
    UInt8Property         m_prop_10{m_properties, 0x10, "cargo_type"};
    UInt8Property         m_prop_11{m_properties, 0x11, "cost_factor"};
    UInt8Property         m_prop_12{m_properties, 0x12, "sound_effect_type"};
    UInt8Property         m_prop_13{m_properties, 0x13, "power_10_hp"};
    UInt8Property         m_prop_14{m_properties, 0x14, "weight_quarter_tons"};
    UInt8Property         m_prop_15{m_properties, 0x15, "speed_half_kmh"};
    UInt32Property        m_prop_16{m_properties, 0x16, "refit_cargo_types"};
    UInt8Property         m_prop_17{m_properties, 0x17, "callback_flags_mask"};
    UInt8Property         m_prop_18{m_properties, 0x18, "coeff_of_tractive_effort"};
    UInt8Property         m_prop_19{m_properties, 0x19, "coeff_of_air_drag"};
    UInt8Property         m_prop_1A{m_properties, 0x1A, "refit_cost"};
    UInt8Property         m_prop_1B{m_properties, 0x1B, "retire_vehicle_early"};
    UInt8Property         m_prop_1C{m_properties, 0x1C, "miscellaneous_flags"};
    UInt16Property        m_prop_1D{m_properties, 0x1D, "refittable_cargo_classes"};
    UInt16Property        m_prop_1E{m_properties, 0x1E, "non_refittable_cargo_classes"};
    LongDateProperty      m_prop_1F{m_properties, 0x1F, "long_introduction_date"};
    UInt8XProperty        m_prop_20{m_properties, 0x20, "sort_purchase_list"};
    VisualEffectProperty  m_prop_21{m_properties, 0x21, "visual_effect"};
    UInt16Property        m_prop_22{m_properties, 0x22, "custom_cargo_aging_period"};
    UInt8Property         m_prop_23{m_properties, 0x23, "shorten_vehicle"};
    UInt8ListProperty     m_prop_24{m_properties, 0x24, "always_refittable_cargos"};
    UInt8ListProperty     m_prop_25{m_properties, 0x25, "never_refittable_cargos"};
    UInt16Property        m_prop_26{m_properties, 0x26, "variant_group"};
    UInt32Property        m_prop_27{m_properties, 0x27, "extra_flags"};
    UInt8Property         m_prop_28{m_properties, 0x28, "extra_callback_flags_mask"};
};



