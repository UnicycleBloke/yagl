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
#include "Action0BRecord.h"


namespace {

// Add all properties, even with silly values, so that we can confirm the 
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL =
    "error_message<Fatal, default, 0x02> // Action0B <severity, language, message>\n"
    "{\n"
    "    // {grf_name} is designed to be used with {data}\n"
    "    data: \"OpenTTD\";\n"
    "}\n";
// NFO matching the YAGL.
static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0B "  // Action0B
    "03 "  // Fatal
    "7F "  // Language = default
    "02 "  // Message ID (from a table)
    "4F 70 65 6E 54 54 44 00 "; // Custom text: OpenTTD

// Add all properties, even with silly values, so that we can confirm the 
// formatting of each one if they are individually changed.
static constexpr const char* str_YAGL2 =
    "error_message<Warning, en_GB, 0xFF> // Action0B <severity, language, message>\n"
    "{\n"
    "    message: \"The cat sat on the {substring1}.\";\n"
    "    data: \"mat\";\n"
    "    param1: 0x12;\n"
    "    param2: 0x34;\n"
    "}\n";
// NFO matching the YAGL.
static constexpr const char* str_NFO2 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0B "  // Action0B
    "01 "  // Warning
    "01 "  // Language = en_GB
    "FF "  // Message ID (from a table) - custom
    "54 68 65 20 63 61 74 20 73 61 74 20 6F 6E 20 74 68 65 20 80 2E 00 "
    "6D 61 74 00 12 34 ";


} // namespace {


TEST_CASE("Action0BRecord", "[actions]")
{
    test_yagl<Action0BRecord, 0x0B>(str_YAGL, str_NFO);
    test_yagl<Action0BRecord, 0x0B>(str_YAGL2, str_NFO2);
}
