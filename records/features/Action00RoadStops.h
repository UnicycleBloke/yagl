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


// TODO Add unit tests for this type.
class Action00RoadStops : public Action00Feature
{
public:
    Action00RoadStops() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    GRFLabel m_08_class_label{};
    UInt8    m_09_road_stop_type{};
    UInt16   m_0A_road_stop_name_text_id{};
    UInt16   m_0B_class_name_text_id{};
    UInt8    m_0C_draw_mode{};
    UInt32   m_0D_random_trigger_cargoes{}; 
    UInt16   m_0E_animation_information{};
    UInt8    m_0F_animation_speed{};
    UInt16   m_10_animation_triggers{}; 
    UInt8    m_11_callback_flags{};
    UInt32   m_12_general_flags{};
    UInt16   m_15_cost_multipliers{};
};


