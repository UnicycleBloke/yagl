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
#include "Action03Record.h"


namespace {

static constexpr const char* str_YAGL =
    "feature_graphics<Trains> // Action03\n"
    "{\n"
    "    livery_override: false;\n"
    "    default_set_id: 0x01F8;\n"
    "    feature_ids: [ 0x0087 ];\n"
    "    cargo_types:\n"
    "    {\n"
    "        // <cargo_type>: <cargo_id>;\n"
    "        0x12: 0x02FE;\n"
    "        0x13: 0x03FD;\n"
    "    };\n"
    "}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "03 "           // Action03
    "00 "           // Trains
    "01 87 "        // 1 train (87)
    "02 "           // 2 cargo types
    "12 FE 02 "     // Cargo 0
    "13 FD 03 "     // Cargo 1
    "F8 01 ";       // Default

} // namespace {


TEST_CASE("Action03Record", "[actions]")
{
    test_yagl<Action03Record, 0x03>(str_YAGL, str_NFO);
}

