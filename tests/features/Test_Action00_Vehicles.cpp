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
#include "catch.hpp"
#include "Test_Shared.h"
#include "Action00Record.h"


namespace {

// Add all properties, even with silly values, so that we can confirm the 
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "properties<Vehicles, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        speed_2_kmh: 0x23;\n"
    "        running_cost_factor: 0x35;\n"
    "        running_cost_base: 0x01020304;\n"
    "        sprite_id: 0x24;\n"
    "        cargo_capacity: 0x25;\n"
    "        cargo_type: 0x35;\n"
    "        cost_factor: 0x36;\n"
    "        sound_effect_type: 0x37;\n"
    "        power_10_hp: 0x38;\n"
    "        weight_quarter_tons: 0x39;\n"
    "        speed_half_kmh: 0x3A;\n"
    "        refit_cargo_types: 0x12345678;\n"
    "        callback_flags_mask: 0x3B;\n"
    "        coeff_of_tractive_effort: 0x3C;\n"
    "        coeff_of_air_drag: 0x3D;\n"
    "        refit_cost: 0x41;\n"
    "        retire_vehicle_early: 0x42;\n"
    "        miscellaneous_flags: 0x43;\n"
    "        refittable_cargo_classes: 0x4412;\n"
    "        non_refittable_cargo_classes: 0x4413;\n"
    "        long_introduction_date: date(1911/3/13);\n"
    "        sort_purchase_list: 0x0102;\n"
    "        visual_effect: effect(SteamPuffs, 0x07, Disable);\n"
    "        custom_cargo_aging_period: 0x1237;\n"
    "        shorten_vehicle: 0x46;\n"
    "        always_refittable_cargos: [ 0x02 0x03 0x04 0x11 ];\n"
    "        never_refittable_cargos: [ 0x05 0x06 0x07 ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "                // Action00
    "01 "                // Vehicles
    "1B "                // 27 properties...
    "01 "                // ... for 1 item
    "FF 23 00 "          // First ID 0x23 (extended byte)
    "08 23 "             // speed_2_kmh
    "09 35 "             // running_cost_factor
    "0A 04 03 02 01 "    // running_cost_base
    "0E 24 "             // sprite_id
    "0F 25 "             // cargo_capacity
    "10 35 "             // cargo_type
    "11 36 "             // cost_factor
    "12 37 "             // sound_effect_type
    "13 38 "             // power_10_hp
    "14 39 "             // weight_quarter_tons
    "15 3A "             // speed_half_kmh
    "16 78 56 34 12 "    // refit_cargo_types
    "17 3B "             // callback_flags_mask
    "18 3C "             // coeff_of_tractive_effort
    "19 3D "             // coeff_of_air_drag
    "1A 41 "             // refit_cost
    "1B 42 "             // retire_vehicle_early
    "1C 43 "             // miscellaneous_flags
    "1D 12 44 "          // refittable_cargo_classes
    "1E 13 44 "          // non_refittable_cargo_classes
    "1F C1 A6 0A 00 "    // long_introduction_date
    "20 FF 02 01 "       // sort_purchase_list
    "21 97 "             // visual_effect
    "22 37 12 "          // custom_cargo_aging_period
    "23 46 "             // shorten_vehicle
    "24 04 02 03 04 11 " // always_refittable_cargos
    "25 03 05 06 07 ";   // never_refittable_cargos

} // namespace {


TEST_CASE("Action00Vehicles", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
