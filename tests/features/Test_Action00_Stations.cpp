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
    "properties<Stations, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        class_id: \"STNS\";\n"
    //"        sprite_layout: ;\n"
    "        copy_sprite_layout_id: 0x12;\n"
    "        callback_flags: 0x13;\n"
    "        disabled_platform_numbers: 0x14;\n"
    "        disabled_platform_lengths: 0x15;\n"
    "        custom_layouts: \n"
    "        {\n"
    "            layout\n"
    "            {\n"
    "                // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight\n"
    "                B B B B B B ;\n" // Plaform 0
    "                B B B B B B ;\n" // Plaform 1
    "                B B B B B B ;\n" // Plaform 2
    "            }\n"
    "            layout\n"
    "            {\n"
    "                // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight\n"
    "                B B P P P B B R B B ;\n" // Plaform 0
    "                L R B P P P P B B B ;\n" // Plaform 1
    "            }\n"
    "        };\n"
    "        copy_custom_layout_id: 0x23;\n"
    "        little_lots_threshold: 0x1234;\n"
    "        pylon_placement: 0x33;\n"
    "        cargo_type_triggers: 0x12345678;\n"
    "        general_flags: 0x56;\n"
    "        overhead_wire_placement: 0x67;\n"
    "        can_train_enter_tile: 0x45;\n"
    "        animation_info: 0x4321;\n"
    "        animation_speed: 0x43;\n"
    "        animation_triggers: 0x1122;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "              // Action00
    "04 "              // Stations
    "10 "              // 16 properties...
    "01 "              // ... for 1 item
    "FF 23 00 "        // First ID 0x23 (extended byte)
    "08 53 54 4E 53 "  // class_id
                       // sprite_layout
    "0A 12 "           // copy_sprite_layout_id
    "0B 13 "           // callback_flags
    "0C 14 "           // disabled_platform_numbers
    "0D 15 "           // disabled_platform_lengths
    "0E "              // custom_layout
       "06 03 "
          "02 02 02 02 02 02 "
          "02 02 02 02 02 02 "
          "02 02 02 02 02 02 "
       "0A 02 "
          "02 02 00 00 00 02 02 06 02 02 "
          "04 06 02 00 00 00 00 02 02 02 "
       "00 00 "
    "0F 23 "           // copy_custom_layout_id
    "10 34 12 "        // little_lots_threshold
    "11 33 "           // pylon_placement
    "12 78 56 34 12 "  // cargo_type_triggers
    "13 56 "           // general_flags
    "14 67 "           // overhead_wire_placement
    "15 45 "           // can_train_enter_tile
    "16 21 43 "        // animation_info
    "17 43 "           // animation_speed
    "18 22 11 ";       // animation_triggers

} // namespace {


TEST_CASE("Action00Stations", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
