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
#include "CargoAcceptanceDescriptor.h"


class Action00IndustryTiles : public Action00Feature
{
public:
    Action00IndustryTiles() : Action00Feature(FeatureType::IndustryTiles) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8               m_08_substitute_building_id{};
    UInt8               m_09_industry_tile_override{};
    UInt16              m_0A_tile_acceptance1{};
    UInt16              m_0B_tile_acceptance2{};
    UInt16              m_0C_tile_acceptance3{};
    UInt8               m_0D_land_shape_flags{};
    UInt8               m_0E_callback_flags{};
    UInt16              m_0F_animation_info{};
    UInt8               m_10_animation_speed{};
    UInt8               m_11_callback_25_triggers{};
    UInt8               m_12_special_flags{};
    CargoAcceptanceList m_13_cargo_acceptance_list{};
};


