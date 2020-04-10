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


class Action00RailTypes : public Action00Feature
{
public:
    Action00RailTypes() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    GRFLabel     m_08_railtype_label{};
    uint16_t     m_09_toolbar_caption_id{};
    uint16_t     m_0A_dropdown_text_id{};
    uint16_t     m_0B_window_caption_id{};
    uint16_t     m_0C_autoreplace_text_id{};
    uint16_t     m_0D_new_engine_text_id{};
    GRFLabelList m_0E_compatible_railtypes{};
    GRFLabelList m_0F_powered_railtypes{};
    uint8_t      m_10_railtype_flags{};
    uint8_t      m_11_curve_speed_multiplier{};
    uint8_t      m_12_station_graphics{};
    uint16_t     m_13_construction_costs{};
    uint16_t     m_14_speed_limit{};
    uint8_t      m_15_acceleration_model{};
    uint8_t      m_16_minimap_colour{};
    Date         m_17_introduction_date{};
    GRFLabelList m_18_required_railtypes{};
    GRFLabelList m_19_introduced_railtypes{};
    uint8_t      m_1A_sort_order{};
    uint16_t     m_1B_rail_type_name_id{};
    uint16_t     m_1C_maintenance_cost_factor{};
    GRFLabelList m_1D_alternate_railtypes{};
};

