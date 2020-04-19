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
    "properties<SoundEffects, 0x0023> // Action00\n"
    "{\n"
    "    // instance_id: 0x0023\n"
    "    {\n"
    "        relative_volume: 0x12;\n"
    "        priority: 0x34;\n"
    "        override_old_sound: 0x56;\n"
    "    }\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "00 "             // Action00
    "0C "             // SoundEffects
    "03 "             // 3 properties...
    "01 "             // ... for 0x01 item
    "FF 23 00 "       // First ID 0x23 (extended byte)
    "08 12 "          // relative_volume
    "09 34 "          // priority
    "0A 56 ";         // override_old_sound

} // namespace {


TEST_CASE("Action00SoundEffects", "[features]")
{
    test_yagl<Action00Record, 0x00>(str_YAGL, str_NFO);
}
