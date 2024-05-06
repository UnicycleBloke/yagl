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
#include "properties/IntegerValue.h"
#include "properties/PropertyMap.h"


class Action00Canals : public Action00Feature
{
public:
    Action00Canals() : Action00Feature(FeatureType::Canals) {}

private:
    UInt8Property m_prop_08{m_properties, 0x08, "callback_flags"};
    UInt8Property m_prop_09{m_properties, 0x09, "graphics_flags"};
};


