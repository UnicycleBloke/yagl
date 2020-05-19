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
    "properties<Ships, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        sprite_id: 0x34;\n"
    "        is_refittable: true;\n"
    "        cost_factor: 0x24;\n"
    "        speed_2_kmh: 0x25;\n"
    "        cargo_type: 0x36;\n"
    "        cargo_capacity: 0x1002;\n"
    "        running_cost_factor: 0x45;\n"
    "        sound_effect_type: 0x32;\n"
    "        refit_cargo_types: 0x01020304;\n"
    "        callback_flags_mask: 0x13;\n"
    "        refit_cost: 0x14;\n"
    "        ocean_speed_fraction: 0x15;\n"
    "        canal_speed_fraction: 0x16;\n"
    "        retire_vehicle_early: 0x17;\n"
    "        miscellaneous_flags: 0x18;\n"
    "        refittable_cargo_classes: 0x1030;\n"
    "        non_refittable_cargo_classes: 0x1040;\n"
    "        long_introduction_date: date(1342/10/23);\n"
    "        sort_purchase_list: 0x0103;\n"
    "        visual_effect: effect(SteamPuffs, 0x07, Disable);\n"
    "        custom_cargo_aging_period: 0x1234;\n"
    "        always_refittable_cargos: [ 0x01 0x03 0x12 ];\n"
    "        never_refittable_cargos: [ 0x02 0x04 0x13 ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "              // Action00
    "02 "              // Ships
    "17 "              // 23 properties...
    "01 "              // ... for 1 item
    "FF 23 00 "        // First ID 0x23 (extended byte)
    "08 34 "           // sprite_id
    "09 01 "           // is_refittable
    "0A 24 "           // cost_factor
    "0B 25 "           // speed_2_kmh
    "0C 36 "           // cargo_type
    "0D 02 10 "        // cargo_capacity
    "0F 45 "           // running_cost_factor
    "10 32 "           // sound_effect_type
    "11 04 03 02 01 "  // refit_cargo_types
    "12 13 "           // callback_flags_mask
    "13 14 "           // refit_cost
    "14 15 "           // ocean_speed_fraction
    "15 16 "           // canal_speed_fraction
    "16 17 "           // retire_vehicle_early
    "17 18 "           // miscellaneous_flags
    "18 30 10 "        // refittable_cargo_classes
    "19 40 10 "        // non_refittable_cargo_classes
    "1A D3 7B 07 00 "  // long_introduction_date
    "1B FF 03 01 "     // sort_purchase_list
    "1C 97 "           // visual_effect
    "1D 34 12 "        // custom_cargo_aging_period
    "1E 03 01 03 12 "  // always_refittable_cargos
    "1F 03 02 04 13 "; // never_refittable_cargos

} // namespace {


TEST_CASE("Action00Ships", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
