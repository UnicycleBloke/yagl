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
#include <sstream>
#include <iostream>
#include <array>
#include "TokenStream.h"
#include "Action03Record.h"
#include "StreamHelpers.h"


static constexpr const char* YAGL =
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

static const std::string BIN = "03 00 01 87 02 12 FE 02 13 FD 03 F8 01 ";

std::string hex_dump(const std::string& data)
{
    std::ostringstream os;
    for (auto byte: data)
    {
        os << to_hex(byte, false) << " ";
    }
    return os.str();
}


TEST_CASE("Action03", "[actions]")
{
    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL);
    TokenStream ts{is};
    Action03Record action;
    action.parse(ts);

    std::ostringstream os;
    SpriteZoomMap sprites; // Empty set is fine for this record.
    action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL);

    // Confirm that the written binary matches the sample.
    os.str("");
    GRFInfo info; // Defaults to Container2 and GRF8.
    action.write(os, info);
    auto str = os.str();
    CHECK(str.size() == (BIN.size() / 3));
    CHECK(hex_dump(str) == BIN);

    // Confirm that reading the binary and printing the 
    // result gets us back to the example.
    std::istringstream is2(str);
    CHECK(uint8_t(is2.get()) == 0x03);
    Action03Record action2;
    action2.read(is2, info);
    os.str("");
    action2.print(os, sprites, 0);
    CHECK(os.str() == YAGL);
}

