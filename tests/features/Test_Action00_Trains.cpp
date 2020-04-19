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
    "        long_introduction_date: date(1883/1/1);\n"
    "        model_life_years: 110;\n"
    "        retire_vehicle_early: 0x3B;\n"
    "        vehicle_life_years: 51;\n"
    "        loading_speed: 0x05;\n"
    "        cost_factor: 0x15;\n"
    "        running_cost_factor: 0x14;\n"
    "        speed_kmh: 90;\n"
    "        power: 554;\n"
    "        cargo_capacity: 1;\n"
    "        weight_tons: 71;\n"
    "        weight_high_byte: 0x00;\n"
    "        coeff_of_tractive_effort: 0x11;\n"
    "        coeff_of_air_drag: 0x00;\n"
    "        reliability_decay_speed: 20;\n"
    "        always_refittable_cargos: [ 0x00 ];\n"
    "        refit_cargo_types: 0x00000000;\n"
    "        miscellaneous_flags: 0x00;\n"
    "        refit_cost: 0x00;\n"
    "        track_type: 1;\n"
    "        ai_special_flag: false;\n"
    "        running_cost_base: 0x00004C30;\n"
    "        engine_traction_type: 0x00;\n"
    "        visual_effect: 0x98;\n"
    "        climate_availability: Temperate | Arctic | Tropical | Toyland;\n"
    "        sprite_id: 0xFD;\n"
    "        is_dual_headed: false;\n"
    "        shorten_vehicle: 0x00;\n"
    "        power_from_each_wagon: 0x0000;\n"
    "        mask_for_var_42: 0x00;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "              // Action00
    "00 "              // Trains
    "1E "              // 30 properties ...
    "01 "              // ... for 1 train
    "FF 86 00 "        // First ID (extended byte)
    "2A 88 7E 0A 00 "  // long_introduction_date
    "04 6E "           // model_life_years
    "26 3B "           // retire_vehicle_early
    "03 33 "           // vehicle_life_years
    "07 05 "           // loading_speed
    "17 15 "           // cost_factor
    "0D 14 "           // running_cost_factor
    "09 5A 00 "        // speed_kmh
    "0B 2A 02 "        // power
    "14 01 "           // cargo_capacity
    "16 47 "           // weight_tons
    "24 00 "           // weight_high_byte
    "1F 11 "           // coeff_of_tractive_effort
    "20 00 "           // coeff_of_air_drag
    "02 14 "           // reliability_decay_speed
    "2C 01 00 "        // always_refittable_cargos
    "1D 00 00 00 00 "  // refit_cargo_types
    "27 00 "           // miscellaneous_flags
    "1C 00 "           // refit_cost
    "05 01 "           // track_type
    "08 00 "           // ai_special_flag
    "0E 30 4C 00 00 "  // running_cost_base
    "19 00 "           // engine_traction_type
    "22 98 "           // visual_effect
    "06 0F "           // climate_availability
    "12 FD "           // sprite_id
    "13 00 "           // is_dual_headed
    "21 00 "           // shorten_vehicle
    "1B 00 00 "        // power_from_each_wagon
    "25 00 ";          // mask_for_var_42

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

