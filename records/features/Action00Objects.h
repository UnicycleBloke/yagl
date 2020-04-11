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
#include "DateDescriptor.h"


class Action00Objects : public Action00Feature
{
public:
    Action00Objects() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt32   m_08_class_label{};
    UInt16   m_09_class_text_id{};
    UInt16   m_0A_object_text_id{};
    UInt8    m_0B_climate_availability{};
    UInt8    m_0C_size_xy{};
    UInt8    m_0D_cost_factor{};
    LongDate m_0E_introduction_date{};
    LongDate m_0F_end_of_life_date{};
    UInt16   m_10_object_flags{};
    UInt16   m_11_animation_info{};
    UInt8    m_12_animation_speed{};
    UInt16   m_13_animation_triggers{};
    UInt8    m_14_removal_cost_factor{};
    UInt16   m_15_callback_flags{};
    UInt8    m_16_building_height{};
    UInt8    m_17_number_of_views{};
    UInt8    m_18_number_on_creation{};
};


