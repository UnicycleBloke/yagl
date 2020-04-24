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
#include "Action0FRecord.h"


namespace {

static constexpr const char* str_YAGL1 =
    "town_names<0x00> // Action0F\n"
    "{\n"
    "    part<0, 5>: // <first_bit, num_bits>\n"
    "    {\n"
    "        text(\"Roermond\", 15);\n"
    "        text(\"Bergen op Zoom\", 15);\n"
    "        text(\"Zwaagdijk-West\", 1);\n"
    "        text(\"Zuiderkolonie\", 1);\n"
    "    }\n"
    "}\n";
static constexpr const char* str_NFO1 = 
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0F "   // Action0F
    "00 "   // This ActionOF ID 
    "01 "   // 1 part ...
    "04 "   // ... containing four items
    "00 "   // ... first random bit
    "05 "   // ... number of random bits (2^N >= sum of probabilities)
            // Probabilities below sum to 32, so 5 bits required
    "0F "   // Roermond with probability 15
       "52 6F 65 72 6D 6F 6E 64 00 "
    "0F "   // Bergen op Zoom with probability 15
       "42 65 72 67 65 6E 20 6F 70 20 5A 6F 6F 6D 00 "
    "01 "   // Zwaagdijk-West with probability 1
       "5A 77 61 61 67 64 69 6A 6B 2D 57 65 73 74 00 "
    "01 "  // Zuiderkolonie with probability 1
       "5A 75 69 64 65 72 6B 6F 6C 6F 6E 69 65 00 ";

static constexpr const char* str_YAGL2 =
    "town_names<0x20> // Action0F\n"
    "{\n"
    "    styles:\n"
    "    {\n"
    "        // lang_id: style_name;\n"
    "        de_DE: \"Niederlaendisch\";\n"
    "        nl_NL: \"Nederlandse\";\n"
    "        default: \"Dutch\";\n"
    "    }\n"
    "    part<5, 2>: // <first_bit, num_bits>\n"
    "    {\n"
    "        town_names(0x00, 1);\n"
    "        town_names(0x01, 1);\n"
    "        town_names(0x02, 1);\n"
    "        town_names(0x03, 1);\n"
    "    }\n"
    "}\n";
static constexpr const char* str_NFO2 = 
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "0F "     // Action0F
    "A0 "     // This Action0F ID is 0x20 - the high bit means there are style names
    "02 "     // de_DE style name
       "4E 69 65 64 65 72 6C 61 65 6E 64 69 73 63 68 00 "
    "1F "     // nl_NL style name
       "4E 65 64 65 72 6C 61 6E 64 73 65 00 "
    "7F "     // Default style name
       "44 75 74 63 68 00 "
    "00 "     // Style names terminator    
    "01 "     // 1 part ...
    "04 "     // ... containing 4 items
    "05 "     // ... first random bit
    "02 "     // ... number of random bits
    "81 00 "  // Reference to Action0F with ID 0x00, with probability 1
    "81 01 "  // Reference to Action0F with ID 0x01, with probability 1
    "81 02 "  // Reference to Action0F with ID 0x02, with probability 1
    "81 03 "; // Reference to Action0F with ID 0x03, with probability 1

} // namespace {}


TEST_CASE("Action0FRecord", "[actions]")
{
    test_yagl<Action0FRecord, 0x0F>(str_YAGL1, str_NFO1);
    test_yagl<Action0FRecord, 0x0F>(str_YAGL2, str_NFO2);
}

