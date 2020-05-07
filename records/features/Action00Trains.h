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
#include "Action00Common.h"
#include "BooleanDescriptor.h"
#include "IntegerDescriptor.h"
#include "DateDescriptor.h"
#include "VisualEffect.h"


class Action00Trains : public Action00Common
{
public:
    Action00Trains() : Action00Common() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8        m_05_track_type{};
    Bool         m_08_ai_special_flag{};
    UInt16       m_09_speed_kmh{};
    UInt16       m_0B_power{};
    UInt8        m_0D_running_cost_factor{};
    UInt32       m_0E_running_cost_base{};
    UInt8        m_12_sprite_id{};
    Bool         m_13_is_dual_headed{};
    UInt8        m_14_cargo_capacity{};
    UInt8        m_15_cargo_type{};
    UInt8        m_16_weight_tons{};
    UInt8        m_17_cost_factor{};
    UInt8        m_18_ai_engine_rank{};
    UInt8        m_19_engine_traction_type{};
    UInt8Ext     m_1A_sort_purchase_list{};
    UInt16       m_1B_power_from_each_wagon{};
    UInt8        m_1C_refit_cost{};
    UInt32       m_1D_refit_cargo_types{};
    UInt8        m_1E_callback_flags_mask{};
    UInt8        m_1F_coeff_of_tractive_effort{};
    UInt8        m_20_coeff_of_air_drag{};
    UInt8        m_21_shorten_vehicle{};
    VisualEffect m_22_visual_effect{};
    UInt8        m_23_weight_from_wagons{};
    UInt8        m_24_weight_high_byte{};
    UInt8        m_25_mask_for_var_42{};
    UInt8        m_26_retire_vehicle_early{};
    UInt8        m_27_miscellaneous_flags{};
    UInt16       m_28_refittable_cargo_classes{};
    UInt16       m_29_non_refittable_cargo_classes{};
    LongDate     m_2A_long_introduction_date{};
    UInt16       m_2B_custom_cargo_aging_period{};   
    CargoList    m_2C_always_refittable_cargos{};
    CargoList    m_2D_never_refittable_cargos{};
};


