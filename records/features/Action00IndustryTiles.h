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
#include "Descriptors.h"


class Action00IndustryTiles : public Action00Feature
{
public:
    Action00IndustryTiles() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    uint8_t             m_08_substitute_building_id{};
    uint8_t             m_09_industry_tile_override{};
    uint16_t            m_0A_tile_acceptance1{};
    uint16_t            m_0B_tile_acceptance2{};
    uint16_t            m_0C_tile_acceptance3{};
    uint8_t             m_0D_land_shape_flags{};
    uint8_t             m_0E_callback_flags{};
    uint16_t            m_0F_animation_info{};
    uint8_t             m_10_animation_speed{};
    uint8_t             m_11_callback_25_triggers{};
    uint8_t             m_12_special_flags{};
    CargoAcceptanceList m_13_cargo_acceptance_list{};
};


