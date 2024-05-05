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
#include "IntegerDescriptor.h"
#include "BooleanDescriptor.h"
#include "GRFStrings.h"


class Action00Cargos : public Action00Feature
{
public:
    Action00Cargos() : Action00Feature{FeatureType::Cargos} {}

private:
    UInt8Property    m_prop_08{m_properties, 0x08, "bit_number"};
    UInt16Property   m_prop_09{m_properties, 0x09, "cargo_type_name_id"};
    UInt16Property   m_prop_0A{m_properties, 0x0A, "single_unit_name_id"};
    UInt16Property   m_prop_0B{m_properties, 0x0B, "single_unit_id"};
    UInt16Property   m_prop_0C{m_properties, 0x0C, "multiple_units_id"};
    UInt16Property   m_prop_0D{m_properties, 0x0D, "cargo_type_abbrev_id"};
    UInt16Property   m_prop_0E{m_properties, 0x0E, "cargo_sprite_id"};
    UInt8Property    m_prop_0F{m_properties, 0x0F, "single_unit_weight"};
    UInt8Property    m_prop_10{m_properties, 0x10, "penalty_time_1"};
    UInt8Property    m_prop_11{m_properties, 0x11, "penalty_time_2"};
    UInt32Property   m_prop_12{m_properties, 0x12, "base_price"};
    UInt8Property    m_prop_13{m_properties, 0x13, "station_list_colour"};
    UInt8Property    m_prop_14{m_properties, 0x14, "payment_list_colour"};
    BoolProperty     m_prop_15{m_properties, 0x15, "is_freight"};
    UInt16Property   m_prop_16{m_properties, 0x16, "cargo_classes"};
    GRFLabelProperty m_prop_17{m_properties, 0x17, "cargo_label"};
    UInt8Property    m_prop_18{m_properties, 0x18, "town_growth_effect"};
    UInt16Property   m_prop_19{m_properties, 0x19, "town_growth_multiplier"};
    UInt8Property    m_prop_1A{m_properties, 0x1A, "callback_flags"};
    UInt16Property   m_prop_1B{m_properties, 0x1B, "cargo_units_id"};
    UInt16Property   m_prop_1C{m_properties, 0x1C, "cargo_amount_id"};
    UInt16Property   m_prop_1D{m_properties, 0x1D, "capacity_multiplier"};
    UInt8Property    m_prop_1E{m_properties, 0x1E, "town_production_effect"};
    UInt16Property   m_prop_1F{m_properties, 0x1F, "town_production_multiplier"};
};


