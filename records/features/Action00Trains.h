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
#include "GRFLabel.h"
#include "DateDescriptor.h"
#include "YearDescriptor.h"
#include "BooleanDescriptor.h"
#include "IntegerValue.h"
#include "PropertyMap.h"
#include "Vector.h"
#include "Array.h"
#include "CargoAcceptanceDescriptor.h"
#include "VisualEffect.h"


using LongDateProperty     = Property<LongDate>;
using ShortDateProperty    = Property<ShortDate>;
using GRFLabelProperty     = Property<GRFLabel>;
using GRFLabelListProperty = Property<Vector<GRFLabel>>;
using Year8PairProperty    = Property<Array<Year8, 2>>;
using Year16Property       = Property<Year16>;
using BoolProperty         = Property<Bool>;
using BoolHeliProperty     = Property<BoolHeli>;
using VisualEffectProperty = Property<VisualEffect>;
using CargoListProperty    = Property<Vector<CargoAcceptance>>;

using UInt8PropertyDec     = Property<UIntNew<uint8_t, false, UIntFormat::Dec>>;
using UInt16PropertyDec    = Property<UIntNew<uint16_t, false, UIntFormat::Dec>>;


class Action00Trains : public Action00Vehicles
{
public:
    Action00Trains() : Action00Vehicles{FeatureType::Trains} {}

private:
    UInt8PropertyDec     m_prop_05{m_properties, 0x05, "track_type"};
    BoolProperty         m_prop_08{m_properties, 0x08, "ai_special_flag"};
    UInt16PropertyDec    m_prop_09{m_properties, 0x09, "speed_kmh"};
    UInt16PropertyDec    m_prop_0B{m_properties, 0x0B, "power"};
    UInt8Property        m_prop_0D{m_properties, 0x0D, "running_cost_factor"};
    UInt32Property       m_prop_0E{m_properties, 0x0E, "running_cost_base"};
    UInt8Property        m_prop_12{m_properties, 0x12, "sprite_id"};
    BoolProperty         m_prop_13{m_properties, 0x13, "is_dual_headed"};
    UInt8PropertyDec     m_prop_14{m_properties, 0x14, "cargo_capacity"};
    UInt8Property        m_prop_15{m_properties, 0x15, "cargo_type"};
    UInt8PropertyDec     m_prop_16{m_properties, 0x16, "weight_tons"};
    UInt8Property        m_prop_17{m_properties, 0x17, "cost_factor"};
    UInt8Property        m_prop_18{m_properties, 0x18, "ai_engine_rank"};
    UInt8Property        m_prop_19{m_properties, 0x19, "engine_traction_type"};
    UInt8XProperty       m_prop_1A{m_properties, 0x1A, "sort_purchase_list"};
    UInt16Property       m_prop_1B{m_properties, 0x1B, "power_from_each_wagon"};
    UInt8Property        m_prop_1C{m_properties, 0x1C, "refit_cost"};
    UInt32Property       m_prop_1D{m_properties, 0x1D, "refit_cargo_types"};
    UInt8Property        m_prop_1E{m_properties, 0x1E, "callback_flags_mask"};
    UInt8Property        m_prop_1F{m_properties, 0x1F, "coeff_of_tractive_effort"};
    UInt8Property        m_prop_20{m_properties, 0x20, "coeff_of_air_drag"};
    UInt8Property        m_prop_21{m_properties, 0x21, "shorten_vehicle"};
    VisualEffectProperty m_prop_22{m_properties, 0x22, "visual_effect"};
    UInt8Property        m_prop_23{m_properties, 0x23, "weight_from_wagons"};
    UInt8Property        m_prop_24{m_properties, 0x24, "weight_high_byte"};
    UInt8Property        m_prop_25{m_properties, 0x25, "mask_for_var_42"};
    UInt8Property        m_prop_26{m_properties, 0x26, "retire_vehicle_early"};
    UInt8Property        m_prop_27{m_properties, 0x27, "miscellaneous_flags"};
    UInt16Property       m_prop_28{m_properties, 0x28, "refittable_cargo_classes"};
    UInt16Property       m_prop_29{m_properties, 0x29, "non_refittable_cargo_classes"};
    LongDateProperty     m_prop_2A{m_properties, 0x2A, "long_introduction_date"};
    UInt16Property       m_prop_2B{m_properties, 0x2B, "custom_cargo_aging_period"};
    UInt8ListProperty    m_prop_2C{m_properties, 0x2C, "always_refittable_cargos"};
    UInt8ListProperty    m_prop_2D{m_properties, 0x2D, "never_refittable_cargos"};
    UInt16Property       m_prop_2E{m_properties, 0x2E, "maximum_curve_speed_modifier"};
    UInt16Property       m_prop_2F{m_properties, 0x2F, "variant_group"};
    UInt32Property       m_prop_30{m_properties, 0x30, "extra_flags"};
    UInt8Property        m_prop_31{m_properties, 0x31, "extra_callback_flags_mask"};
};


