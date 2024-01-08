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

// TODO Add all properties, even with silly values, so that we can confirm the
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "properties<Trains, 0x0086> // Action00\n"
    "{\n"
    "    // instance_id: 0x0086\n"
    "    {\n"
    "        track_type: 18;\n"
    "        ai_special_flag: false;\n"
    "        speed_kmh: 4098;\n"
    "        power: 4099;\n"
    "        running_cost_factor: 0x31;\n"
    "        running_cost_base: 0x12345678;\n"
    "        sprite_id: 0x32;\n"
    "        is_dual_headed: false;\n"
    "        cargo_capacity: 52;\n"
    "        cargo_type: 0x35;\n"
    "        weight_tons: 54;\n"
    "        cost_factor: 0x37;\n"
    "        ai_engine_rank: 0x38;\n"
    "        engine_traction_type: 0x39;\n"
    "        sort_purchase_list: 0x1001;\n"
    "        power_from_each_wagon: 0x1003;\n"
    "        refit_cost: 0x41;\n"
    "        refit_cargo_types: 0x01020304;\n"
    "        callback_flags_mask: 0x67;\n"
    "        coeff_of_tractive_effort: 0x56;\n"
    "        coeff_of_air_drag: 0x57;\n"
    "        shorten_vehicle: 0x58;\n"
    "        visual_effect: effect(SteamPuffs, 0x07, Disable);\n"
    "        weight_from_wagons: 0x5A;\n"
    "        weight_high_byte: 0x5B;\n"
    "        mask_for_var_42: 0x5C;\n"
    "        retire_vehicle_early: 0x5D;\n"
    "        miscellaneous_flags: 0x5E;\n"
    "        refittable_cargo_classes: 0x2003;\n"
    "        non_refittable_cargo_classes: 0x2004;\n"
    "        long_introduction_date: date(3001/3/2);\n"
    "        custom_cargo_aging_period: 0x1234;\n"
    "        always_refittable_cargos: [ 0x02 0x04 0x16 ];\n"
    "        never_refittable_cargos: [ 0x03 0x05 0x11 0x14 ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "                 // Action00
    "00 "                 // Trains
    "22 "                 // 34 properties ...
    "01 "                 // ... for 1 train
    "FF 86 00 "           // First ID (extended byte)
    "05 12 "              // track_type
    "08 00 "              // ai_special_flag
    "09 02 10 "           // speed_kmh
    "0B 03 10 "           // power
    "0D 31 "              // running_cost_factor
    "0E 78 56 34 12 "     // running_cost_base
    "12 32 "              // sprite_id
    "13 00 "              // is_dual_headed
    "14 34 "              // cargo_capacity
    "15 35 "              // cargo_type
    "16 36 "              // weight_tons
    "17 37 "              // cost_factor
    "18 38 "              // ai_engine_rank
    "19 39 "              // engine_traction_type
    "1A FF 01 10 "        // sort_purchase_list
    "1B 03 10 "           // power_from_each_wagon
    "1C 41 "              // refit_cost
    "1D 04 03 02 01 "     // refit_cargo_types
    "1E 67 "              // callback_flags_mask
    "1F 56 "              // coeff_of_tractive_effort
    "20 57 "              // coeff_of_air_drag
    "21 58 "              // shorten_vehicle
    "22 97 "              // visual_effect  0x10 (steam puffs), 0x07 (position), x80 (disable)
    "23 5A "              // weight_from_wagons
    "24 5B "              // weight_high_byte
    "25 5C "              // mask_for_var_42
    "26 5D "              // retire_vehicle_early
    "27 5E "              // miscellaneous_flags
    "28 03 20 "           // refittable_cargo_classes
    "29 04 20 "           // non_refittable_cargo_classes
    "2A D9 B9 10 00 "     // long_introduction_date
    "2B 34 12 "           // custom_cargo_aging_period
    "2C 03 02 04 16 "     // always_refittable_cargos
    "2D 04 03 05 11 14 "; // never_refittable_cargos

static constexpr const char* str_YAGL2 =
    "properties<Trains, 0x0086> // Action00\n"
    "{\n"
    "    // instance_id: 0x0086\n"
    "    {\n"
    "        long_introduction_date: date(1883/1/1);\n"
    "        model_life_years: 110;\n"
    "        retire_vehicle_early: 0x3B;\n"
    "    }\n"
    "    // instance_id: 0x0087\n"
    "    {\n"
    "        long_introduction_date: date(1883/2/3);\n"
    "        model_life_years: 111;\n"
    "        retire_vehicle_early: 0x3C;\n"
    "    }\n"
    "    // instance_id: 0x0088\n"
    "    {\n"
    "        long_introduction_date: date(1883/4/5);\n"
    "        model_life_years: 112;\n"
    "        retire_vehicle_early: 0x3D;\n"
    "    }\n"
    "    // instance_id: 0x0089\n"
    "    {\n"
    "        long_introduction_date: date(1883/6/7);\n"
    "        model_life_years: 113;\n"
    "        retire_vehicle_early: 0x3E;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "              // Action00
    "00 "              // Trains
    "03 "              // 30 properties ...
    "04 "              // ... for 4 train
    "FF 86 00 "        // First ID (extended byte)
    "2A "
       "88 7E 0A 00 "
       "A9 7E 0A 00 "
       "E6 7E 0A 00 "
       "25 7F 0A 00 "
    "04 "
       "6E "
       "6F "
       "70 "
       "71 "
    "26 "
       "3B "
       "3C "
       "3D "
       "3E ";

} // namespace {


TEST_CASE("Action00Trains", "[actions]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
    test_yagl<Action00Record, 0x00>(str_YAGL2, str_NFO2);
}

