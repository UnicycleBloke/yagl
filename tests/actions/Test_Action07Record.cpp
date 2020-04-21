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
#include "Action07Record.h"


namespace {

static constexpr const char* str_YAGL7 =
// param[0x6C] & <mask> 0xFF, 0xFFFF, 0xFFFF'FFFF or custom
"if_act7 (is_bit_set(param[0x6C] & 0xFF, 1 << 0)) // Action07\n" 
"{\n"
"    skip_sprites: 0x01;\n"
"    // Or skip to the next label (Action10) with this value - search wraps at end of GRF.\n"
"    // 0x00 means skip to end of GRF file - may disable the GRF.\n"
"}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO7 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
      "07 "  // Action07
      "6C "  // Parameter 6C
      "01 "  // Variable size (1, 2, 4 or 8 - determined by value of mask)
      "00 "  // Condition type (test for a bit being set)
      "00 "  // Value - size matches variable size above (index of the bit to test)
      "01 "; // Number of records to skip - 00 means to end of file.


static constexpr const char* str_YAGL9 =
// param[0x6C] & <mask> 0xFF, 0xFFFF, 0xFFFF'FFFF or custom
"if_act9 (is_bit_set(param[0x6C] & 0xFF, 1 << 0)) // Action09\n" 
"{\n"
"    skip_sprites: 0x01;\n"
"    // Or skip to the next label (Action10) with this value - search wraps at end of GRF.\n"
"    // 0x00 means skip to end of GRF file - may disable the GRF.\n"
"}\n";

// NFO matching the YAGL.
static constexpr const char* str_NFO9 =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
      "09 "  // Action07
      "6C "  // Parameter 6C
      "01 "  // Variable size (1, 2, 4 or 8 - determined by value of mask)
      "00 "  // Condition type (test for a bit being set)
      "00 "  // Value - size matches variable size above (index of the bit to test)
      "01 "; // Number of records to skip - 00 means to end of file.
} // namespace {


// Specific version due the constructor args.
template <RecordType TYPE, uint8_t ACTION>
void test_yagl_action07(const char* YAGL, const char* NFO)
{
    SpriteZoomMap sprites; 

    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL);
    TokenStream ts{is};
    Action07Record action{TYPE};
    action.parse(ts, sprites);

    std::ostringstream os;
    //action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL);

    // Confirm that the written binary matches the sample.
    os.str("");
    GRFInfo info; // Defaults to Container2 and GRF8.
    action.write(os, info);
    auto str = os.str();
    CHECK(str.size() == (std::strlen(NFO) / 3));
    CHECK(hex_dump(str) == NFO);

    // Confirm that reading the binary and printing the 
    // result gets us back to the example.
    std::istringstream is2(str);
    CHECK(uint8_t(is2.get()) == ACTION);
    Action07Record action2{TYPE};
    action2.read(is2, info);
    os.str("");
    action2.print(os, sprites, 0);
    CHECK(os.str() == YAGL);
}


TEST_CASE("Action07Record", "[actions]")
{
    test_yagl_action07<RecordType::ACTION_07, 0x07>(str_YAGL7, str_NFO7);
    test_yagl_action07<RecordType::ACTION_09, 0x09>(str_YAGL9, str_NFO9);
}
