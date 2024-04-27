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
#include "Action00Vehicles.h"
#include "IntegerDescriptor.h"
#include "DateDescriptor.h"
#include "BooleanDescriptor.h"
#include "VisualEffect.h"


class Action00Ships : public Action00Vehicles
{
public:
    Action00Ships() : Action00Vehicles(FeatureType::Ships) {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8        m_08_sprite_id{};
    Bool         m_09_is_refittable{};
    UInt8        m_0A_cost_factor{};
    UInt8        m_0B_speed_2_kmh{};
    UInt8        m_0C_cargo_type{};
    UInt16       m_0D_cargo_capacity{};
    UInt8        m_0F_running_cost_factor{};
    UInt8        m_10_sound_effect_type{};
    UInt32       m_11_refit_cargo_types{};
    UInt8        m_12_callback_flags_mask{};
    UInt8        m_13_refit_cost{};
    UInt8        m_14_ocean_speed_fraction{};
    UInt8        m_15_canal_speed_fraction{};
    UInt8        m_16_retire_vehicle_early{};
    UInt8        m_17_miscellaneous_flags{};
    UInt16       m_18_refittable_cargo_classes{};
    UInt16       m_19_non_refittable_cargo_classes{};
    LongDate     m_1A_long_introduction_date{};
    UInt8Ext     m_1B_sort_purchase_list{};
    VisualEffect m_1C_visual_effect{};
    UInt16       m_1D_custom_cargo_aging_period{};
    CargoList    m_1E_always_refittable_cargos{};
    CargoList    m_1F_never_refittable_cargos{};
    UInt16       m_20_variant_group{};
    UInt32       m_21_extra_flags{};
    UInt8        m_22_extra_callback_flags_mask{};
    UInt16       m_23_speed_3_2_mph{};
    UInt8        m_24_acceleration_3_2_mph{};
};


