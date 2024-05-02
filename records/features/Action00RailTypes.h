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
#include "GRFLabel.h"
#include "IntegerDescriptor.h"
#include "DateDescriptor.h"


class Action00RailTypes : public Action00Feature
{
public:
    Action00RailTypes() : Action00Feature(FeatureType::RailTypes) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    GRFLabel     m_08_railtype_label{};
    UInt16       m_09_toolbar_caption_id{};
    UInt16       m_0A_dropdown_text_id{};
    UInt16       m_0B_window_caption_id{};
    UInt16       m_0C_autoreplace_text_id{};
    UInt16       m_0D_new_engine_text_id{};
    GRFLabelList m_0E_compatible_railtypes{};
    GRFLabelList m_0F_powered_railtypes{};
    UInt8        m_10_railtype_flags{};
    UInt8        m_11_curve_speed_multiplier{};
    UInt8        m_12_station_graphics{};
    UInt16       m_13_construction_costs{};
    UInt16       m_14_speed_limit{};
    UInt8        m_15_acceleration_model{};
    UInt8        m_16_minimap_colour{};
    LongDate     m_17_introduction_date{};
    GRFLabelList m_18_required_railtypes{};
    GRFLabelList m_19_introduced_railtypes{};
    UInt8        m_1A_sort_order{};
    UInt16       m_1B_rail_type_name_id{};
    UInt16       m_1C_maintenance_cost_factor{};
    GRFLabelList m_1D_alternate_railtypes{};
};


#include "IntegerValue.h"
#include "PropertyMap.h"
#include "ValueTypeVector.h"


// For now.
using LongDateProperty     = Property<LongDate>;
using UInt16Property       = Property<UInt2<uint16_t>>;
using UInt8Property        = Property<UInt2<uint8_t>>;
using GRFLabelProperty     = Property<GRFLabel>;
using GRFLabelListProperty = Property<ValueTypeVector<GRFLabel>>;

class Action00RailTypes2 : public Action00Feature
{
public:
    Action00RailTypes2() : Action00Feature(FeatureType::RailTypes) {}

private:
    GRFLabelProperty      m_prop_08{m_properties, 0x08, "railtype_label"};
    UInt16Property        m_prop_09{m_properties, 0x09, "toolbar_caption_id"};
    UInt16Property        m_prop_0A{m_properties, 0x0A, "dropdown_text_id"};
    UInt16Property        m_prop_0B{m_properties, 0x0B, "window_caption_id"};
    UInt16Property        m_prop_0C{m_properties, 0x0C, "autoreplace_text_id"};
    UInt16Property        m_prop_0D{m_properties, 0x0D, "new_engine_text_id"};
    GRFLabelListProperty  m_prop_0E{m_properties, 0x0E, "compatible_railtypes"};
    GRFLabelListProperty  m_prop_0F{m_properties, 0x0F, "powered_railtypes"};
    UInt8Property         m_prop_10{m_properties, 0x10, "railtype_flags"};
    UInt8Property         m_prop_11{m_properties, 0x11, "curve_speed_multiplier"};
    UInt8Property         m_prop_12{m_properties, 0x12, "station_graphics"};
    UInt16Property        m_prop_13{m_properties, 0x13, "construction_costs"};
    UInt16Property        m_prop_14{m_properties, 0x14, "speed_limit"};
    UInt8Property         m_prop_15{m_properties, 0x15, "acceleration_model"};
    UInt8Property         m_prop_16{m_properties, 0x16, "minimap_colour"};
    LongDateProperty      m_prop_17{m_properties, 0x17, "introduction_date"};
    GRFLabelListProperty  m_prop_18{m_properties, 0x18, "required_railtypes"};
    GRFLabelListProperty  m_prop_19{m_properties, 0x19, "introduced_railtypes"};
    UInt8Property         m_prop_1A{m_properties, 0x1A, "sort_order"};
    UInt16Property        m_prop_1B{m_properties, 0x1B, "rail_type_name_id"};
    UInt16Property        m_prop_1C{m_properties, 0x1C, "maintenance_cost_factor"};
    GRFLabelListProperty  m_prop_1D{m_properties, 0x1D, "alternate_railtypes"};
};