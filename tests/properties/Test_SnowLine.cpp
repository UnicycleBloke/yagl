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
#include "properties/SnowLine.h"


static constexpr const char* str_YAGL =
R"(
[
    0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x10 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x1E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x20 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x2E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x30 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x3E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x40 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x4E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x50 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x5E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x60 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x6E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x70 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x7E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x80 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x8E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0x90 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x9E 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0xA0 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0xAE 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
    0xB0 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0xBE 0x0F 0x00 0x01 0x02 0x02 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 
])";


// NFO matching the YAGL. Same as Array except preceded by the length.
static constexpr const char* str_NFO = 
"00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"10 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 1E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"20 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 2E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"30 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 3E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"40 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 4E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"50 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 5E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"60 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 6E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"70 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 7E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"80 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 8E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"90 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 9E 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"A0 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D AE 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "
"B0 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D BE 0F 00 01 02 02 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ";


TEST_CASE("Snowline", "[properties]")
{
    test_yagl_fragment<SnowLine>(str_YAGL, str_NFO);
}