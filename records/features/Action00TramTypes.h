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
#include "DateDescriptor.h"
#include "GRFLabel.h"


// Note that this is currently identical in content to the RoadTypes, which is
// similar but not quite the same as RailTypes.
class Action00TramTypes : public Action00Feature
{
public:
    Action00TramTypes() : Action00Feature(FeatureType::TramTypes) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    GRFLabel     m_08_tramtype_label{};
    UInt16       m_09_toolbar_caption_id{};
    UInt16       m_0A_dropdown_text_id{};
    UInt16       m_0B_window_caption_id{};
    UInt16       m_0C_autoreplace_text_id{};
    UInt16       m_0D_new_engine_text_id{};
    GRFLabelList m_0F_powered_tramtypes{};
    UInt8        m_10_tramtype_flags{};
    UInt16       m_13_construction_costs{};
    UInt16       m_14_speed_limit{};
    UInt8        m_16_minimap_colour{};
    LongDate     m_17_introduction_date{};
    GRFLabelList m_18_required_tramtypes{};
    GRFLabelList m_19_introduced_tramtypes{};
    UInt8        m_1A_sort_order{};
    UInt16       m_1B_tram_type_name_id{};
    UInt16       m_1C_maintenance_cost_factor{};
    GRFLabelList m_1D_alternate_tramtypes{};
};


