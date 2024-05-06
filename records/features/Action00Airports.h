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
#include "properties/YearValue.h"
#include "properties/AirportLayout.h"


using AirportLayoutsProperty = Property<AirportLayouts>;


class Action00Airports : public Action00Feature
{
public:
    Action00Airports() : Action00Feature(FeatureType::Airports) {}

private:
    UInt8Property          m_prop_08{m_properties, 0x08, "airport_override_id"};
    AirportLayoutsProperty m_prop_0A{m_properties, 0x0A, "airport_layouts"};
    Year16PairProperty     m_prop_0C{m_properties, 0x0C, "years_available"};
    UInt8Property          m_prop_0D{m_properties, 0x0D, "compatible_ttd_airport"};
    UInt8Property          m_prop_0E{m_properties, 0x0E, "catchment_area"};
    UInt8Property          m_prop_0F{m_properties, 0x0F, "noise_level"};
    UInt16Property         m_prop_10{m_properties, 0x10, "airport_name_id"};
    UInt16Property         m_prop_11{m_properties, 0x11, "maintenance_cost_factor"};
};


