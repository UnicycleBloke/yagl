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
#include "properties/DateValue.h"
#include "properties/IntegerValue.h"
#include "properties/BitfieldValue.h"
#include "properties/PropertyMap.h"
#include "properties/Vector.h"
#include "properties/Array.h"
#include "CargoAcceptanceDescriptor.h"


using UInt8PropertyDec     = Property<UIntNew<uint8_t, false, UIntFormat::Dec>>;
using Bit8Property         = Property<BitfieldValue<uint8_t>>;


class Action00Vehicles : public Action00Feature
{
public:
    Action00Vehicles(FeatureType feature) : Action00Feature(feature) {}

private:
    std::vector<BitfieldItem> m_climates{ {0, "Temperate"}, {1, "Arctic"}, {2, "Tropical"}, {3, "Toyland"} };

    ShortDateProperty m_prop_00{m_properties, 0x00, "introduction_date"};
    UInt8PropertyDec  m_prop_02{m_properties, 0x02, "reliability_decay_speed"};
    UInt8PropertyDec  m_prop_03{m_properties, 0x03, "vehicle_life_years"};
    UInt8PropertyDec  m_prop_04{m_properties, 0x04, "model_life_years"};
    Bit8Property      m_prop_06{m_properties, 0x06, "climate_availability", m_climates};
    UInt8Property     m_prop_07{m_properties, 0x07, "loading_speed"};
};
