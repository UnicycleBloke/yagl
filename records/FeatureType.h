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
#include <cstdint>
#include <string>


enum class FeatureType : uint8_t
{
    Trains          = 0x00,
    RoadVehicles    = 0x01,
    Ships           = 0x02,
    Aircraft        = 0x03,
    Stations        = 0x04,
    Canals          = 0x05,
    Bridges         = 0x06,
    Houses          = 0x07,
    GlobalSettings  = 0x08,
    IndustryTiles   = 0x09,
    Industries      = 0x0A,
    Cargos          = 0x0B,
    SoundEffects    = 0x0C,
    Airports        = 0x0D,
    Signals         = 0x0E,
    Objects         = 0x0F,
    RailTypes       = 0x10,
    AirportTiles    = 0x11,
    RoadTypes       = 0x12,
    TramTypes       = 0x13,
    RoadStops       = 0x14,
    OriginalStrings = 0x48,
};


std::string FeatureName(FeatureType type);
FeatureType FeatureFromName(const std::string& name);
bool feature_is_vehicle(FeatureType type);
