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
#include "Action02RandomRecord.h"


namespace {

static constexpr const char* str_YAGL =
"random_switch<Trains, 0xFC, Object> // Action02 random\n"
"{\n"
"    triggers: 0x00;\n"
"    rand_bit: 0x00;\n"
"    set_ids: // set_id: probability;\n"
"    {\n"
"        0x00F5: 103;\n"
"        0x00FA: 8;\n"
"        0x00FD: 17;\n"
"    };\n"
"}\n";
// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "02 " // Action02
    "00 " // Trains
    "FC " // This set's ID
    "80 " // Randomise the object itself (not related item)
    "00 " // triggers
    "00 " // rand_bit
    "80 " // 128 items - must be a power of 2
    // F5 probability is 103 / 128
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 F5 00 "
    "F5 00 F5 00 F5 00 "
    // FA probability is 8 / 128
    "FA 00 FA 00 FA 00 FA 00 FA 00 FA 00 FA 00 FA 00 "
    // FD probability is 17 / 128
    "FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 "
    "FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 FD 00 ";

static constexpr const char* str_YAGL2 =
R"(random_switch<Trains, 0x52, Consist, BackwardFromSameID[0x00]> // Action02 random
{
    triggers: 0x00;
    rand_bit: 0x00;
    set_ids: // set_id: probability;
    {
        0x0001: 1;
        0x0002: 1;
        0x0003: 1;
        0x0004: 5;
    };
}
)";

static constexpr const char* str_NFO2 = "02 00 52 84 C0 00 00 08 01 00 02 00 03 00 04 00 04 00 04 00 04 00 04 00 ";

} // namespace {


TEST_CASE("Action02RandomRecord", "[actions]")
{
    test_yagl<Action02RandomRecord, 0x02>(str_YAGL, str_NFO);
    test_yagl<Action02RandomRecord, 0x02>(str_YAGL2, str_NFO2);
}

