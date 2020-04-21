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
#include "Action13Record.h"


namespace {

static constexpr const char* str_YAGL =
    "grf_strings<\"GRFX\", en_US, 0xC504> // <grf_id, language, first_id> Action13, English (US)\n"
    "{\n"
    "    /*0xC504*/ \"Roofs\";\n"
    "    /*0xC505*/ \"Platform\";\n"
    "    /*0xC506*/ \"Benches\";\n"
    "    /*0xC507*/ \"Parking lot (front)\";\n"
    "    /*0xC508*/ \"Parking lot (back)\";\n"
    "    /*0xC509*/ \"Flat roofs\";\n"
    "    /*0xC50A*/ \"Glass roofs\";\n"
    "    /*0xC50B*/ \"Overpass\";\n"
    "    /*0xC50C*/ \"Station Hall (small)\";\n"
    "    /*0xC50D*/ \"Station Hall (large)\";\n"
    "    /*0xC50E*/ \"Underpass\";\n"
    "    /*0xC50F*/ \"empty\";\n"
    "    /*0xC510*/ \"void\";\n"
    "}\n";
    
static constexpr const char* str_NFO = 
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "13 "                         // Action13
    "47 52 46 58 "                // GRF ID
    "00 "                         // Language ID
    "0D "                         // Number of strings 
    "04 C5 "                      // First string ID
    "52 6F 6F 66 73 00 "          // Roofs
    "50 6C 61 74 66 6F 72 6D 00 " // Platform
    "42 65 6E 63 68 65 73 00 "    // Benches
    "50 61 72 6B 69 6E 67 20 6C 6F 74 20 28 66 72 6F 6E 74 29 00 "
    "50 61 72 6B 69 6E 67 20 6C 6F 74 20 28 62 61 63 6B 29 00 "
    "46 6C 61 74 20 72 6F 6F 66 73 00 "
    "47 6C 61 73 73 20 72 6F 6F 66 73 00 "
    "4F 76 65 72 70 61 73 73 00 "
    "53 74 61 74 69 6F 6E 20 48 61 6C 6C 20 28 73 6D 61 6C 6C 29 00 "
    "53 74 61 74 69 6F 6E 20 48 61 6C 6C 20 28 6C 61 72 67 65 29 00 "
    "55 6E 64 65 72 70 61 73 73 00 "
    "65 6D 70 74 79 00 "          // empty
    "76 6F 69 64 00 ";            // void

} // namespace {}


TEST_CASE("Action13Record", "[actions]")
{
    test_yagl<Action13Record, 0x13>(str_YAGL, str_NFO);
}

