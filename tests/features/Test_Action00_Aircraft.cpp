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
    "properties<Aircraft, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        sprite_id: 0xFD;\n"
    "        is_helicopter: false;\n"
    "        is_large: false;\n"
    "        cost_factor: 0x23;\n"
    "        speed_8_mph: 0x20;\n"
    "        acceleration: 0x33;\n"
    "        running_cost_factor: 0x67;\n"
    "        passenger_capacity: 0x1002;\n"
    "        mail_capacity: 0x27;\n"
    "        sound_effect_type: 0x01;\n"
    "        refit_cargo_types: 0x01020304;\n"
    "        callback_flags_mask: 0x12;\n"
    "        refit_cost: 0x13;\n"
    "        retire_vehicle_early: 0x34;\n"
    "        miscellaneous_flags: 0x11;\n"
    "        refittable_cargo_classes: 0x1234;\n"
    "        non_refittable_cargo_classes: 0x2345;\n"
    "        long_introduction_date: date(2020/1/2);\n"
    "        sort_purchase_list: 0x0012;\n"
    "        custom_cargo_aging_period: 0x1223;\n"
    "        always_refittable_cargos: [ 0x01 0x02 0x03 ];\n"
    "        never_refittable_cargos: [ 0x04 0x05 0x06 ];\n"
    "        aircraft_range: 0x0000;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "03 "             // Aircraft
    "17 "             // 0x17 properties... 
    "01 "             // ... for 0x01 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 FD "          // sprite_id
    "09 02 "          // is_helicopter
    "0A 00 "          // is_large
    "0B 23 "          // cost_factor
    "0C 20 "          // speed_8_mph
    "0D 33 "          // acceleration
    "0E 67 "          // running_cost_factor
    "0F 02 10 "       // passenger_capacity
    "11 27 "          // mail_capacity
    "12 01 "          // sound_effect_type
    "13 04 03 02 01 " // refit_cargo_types
    "14 12 "          // callback_flags_mask
    "15 13 "          // refit_cost
    "16 34 "          // retire_vehicle_early
    "17 11 "          // miscellaneous_flags
    "18 34 12 "       // refittable_cargo_classes
    "19 45 23 "       // non_refittable_cargo_classes
    "1A FF 41 0B 00 " // long_introduction_date
    "1B FF 12 00 "    // sort_purchase_list
    "1C 23 12 "       // custom_cargo_aging_period
    "1D 03 01 02 03 " // always_refittable_cargos - 3 bytes
    "1E 03 04 05 06 " // never_refittable_cargos - 3 bytes
    "1F 00 00 ";      // aircraft_range

} // namespace {


TEST_CASE("Action00Aircraft", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
