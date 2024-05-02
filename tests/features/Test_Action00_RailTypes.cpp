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
    "properties<RailTypes, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        railtype_label: \"RAIL\";\n"
    "        toolbar_caption_id: 0xD001;\n"
    "        dropdown_text_id: 0xD002;\n"
    "        window_caption_id: 0xD003;\n"
    "        autoreplace_text_id: 0xD004;\n"
    "        new_engine_text_id: 0xD005;\n"
    "        compatible_railtypes: [ \"ABCD\" \"EFGH\" ];\n"
    "        powered_railtypes: [ \"1234\" \"2345\" ];\n"
    "        railtype_flags: 0x04;\n"
    "        curve_speed_multiplier: 0x05;\n"
    "        station_graphics: 0x07;\n"
    "        construction_costs: 0x1024;\n"
    "        speed_limit: 0x1025;\n"
    "        acceleration_model: 0x12;\n"
    "        minimap_colour: 0x13;\n"
    "        introduction_date: date(1066/12/31);\n"
    "        required_railtypes: [ \"ABCE\" \"EF12\" \"1267\" ];\n"
    "        introduced_railtypes: [ \"ABDE\" \"EF32\" \"G267\" ];\n"
    "        sort_order: 0x23;\n"
    "        rail_type_name_id: 0xD113;\n"
    "        maintenance_cost_factor: 0x1278;\n"
    "        alternate_railtypes: [ \"abcd\" ];\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "10 "             // RailTypes
    "16 "             // 22 properties...
    "01 "             // ... for 1 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 52 41 49 4C " // railtype_label
    "09 01 D0 "       // toolbar_caption_id
    "0A 02 D0 "       // dropdown_text_id
    "0B 03 D0 "       // window_caption_id
    "0C 04 D0 "       // autoreplace_text_id
    "0D 05 D0 "       // new_engine_text_id
    "0E 02 "          // compatible_railtypes
       "41 42 43 44 "
       "45 46 47 48 "
    "0F 02 "          // powered_railtypes
       "31 32 33 34 "
       "32 33 34 35 "
    "10 04 "          // railtype_flags
    "11 05 "          // curve_speed_multiplier
    "12 07 "          // station_graphics
    "13 24 10 "       // construction_costs
    "14 25 10 "       // speed_limit
    "15 12 "          // acceleration_model
    "16 13 "          // minimap_colour
    "17 51 F2 05 00 " // introduction_date
    "18 03 "          // required_railtypes
       "41 42 43 45 "
       "45 46 31 32 "
       "31 32 36 37 "
    "19 03 "          // introduced_railtypes
       "41 42 44 45 "
       "45 46 33 32 "
       "47 32 36 37 "
    "1A 23 "          // sort_order
    "1B 13 D1 "       // rail_type_name_id
    "1C 78 12 "       // maintenance_cost_factor
    "1D 01 "          // alternate_railtypes
       "61 62 63 64 ";

} // namespace {


TEST_CASE("Action00RailTypes", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
 