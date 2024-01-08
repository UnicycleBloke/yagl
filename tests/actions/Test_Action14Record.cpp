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
#include "Action14Record.h"


namespace {

static constexpr const char* str_YAGL =
    "optional_info // Action14\n"
    "{\n"
    "    INFO: \n"
    "    {\n"
    "        DESC: fr_FR, \"{lt-gray}Trains Hollandais.\"; // French\n"
    "        URL_: default, \"http://www.tt-forums.net/viewtopic.php?t=59353\"; // Default\n"
    "        VRSN: [ 0xE1 0x1C 0x00 0x00 ];\n"
    "        MINV: [ 0x7F 0x1C 0x00 0x00 ];\n"
    "        NPAR: [ 0x06 ];\n"
    "        PARA: \n"
    "        {\n"
    "            0x00000000: \n"
    "            {\n"
    "                NAME: default, \"Use English vehicle nicknames\"; // Default\n"
    "                NAME: fr_FR, \"Traduire les surnoms en français\"; // French\n"
    "                DESC: default, \"Enable this to have the vehicle nicknames translated to English.\"; // Default\n"
    "                DESC: fr_FR, \"Certaines locomotives ont des surnoms en néerlandais.\"; // French\n"
    "                TYPE: [ 0x01 ];\n"
    "                MASK: [ 0x00 0x00 0x01 ];\n"
    "                DFLT: [ 0x00 0x00 0x00 0x00 ];\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "}\n";

static constexpr const char* str_NFO =
//    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    "14 "
    "43 49 4E 46 4F " // INFO
       "54 44 45 53 43 " // DESC
          "03 "
          "89 54 72 61 69 6E 73 20 48 6F 6C 6C 61 6E 64 61 69 73 2E 00 "
       "54 55 52 4C 5F " // URL_
          "7F "
          "68 74 74 70 3A 2F 2F 77 77 77 2E 74 74 2D 66 6F 72 75 6D 73 "
          "2E 6E 65 74 2F 76 69 65 77 74 6F 70 69 63 2E 70 68 70 3F 74 "
          "3D 35 39 33 35 33 00 "
       "42 56 52 53 4E " // VRSN
          "04 00 "
          "E1 1C 00 00 "
       "42 4D 49 4E 56 " // MINV
          "04 00 "
          "7F 1C 00 00 "
       "42 4E 50 41 52 " // NPAR
          "01 00 "
          "06 "
       "43 50 41 52 41 " // PARA
          "43 00 00 00 00 " // 0x00000000
             "54 4E 41 4D 45 " // NAME
                "7F "
                "55 73 65 20 45 6E 67 6C 69 73 68 20 76 65 68 69 63 6C "
                "65 20 6E 69 63 6B 6E 61 6D 65 73 00 "
             "54 4E 41 4D 45 " // NAME
                "03 "
                "C3 9E 54 72 61 64 75 69 72 65 20 6C 65 73 20 73 75 72 6E "
                "6F 6D 73 20 65 6E 20 66 72 61 6E C3 A7 61 69 73 00 "
             "54 44 45 53 43 " // DESC
                "7F "
                "45 6E 61 62 6C 65 20 74 68 69 73 20 74 6F 20 68 61 76 65 "
                "20 74 68 65 20 76 65 68 69 63 6C 65 20 6E 69 63 6B 6E 61 "
                "6D 65 73 20 74 72 61 6E 73 6C 61 74 65 64 20 74 6F 20 45 "
                "6E 67 6C 69 73 68 2E 00 "
             "54 44 45 53 43 " // DESC
                "03 "
                "C3 9E 43 65 72 74 61 69 6E 65 73 20 6C 6F 63 6F 6D 6F 74 "
                "69 76 65 73 20 6F 6E 74 20 64 65 73 20 73 75 72 6E 6F 6D "
                "73 20 65 6E 20 6E C3 A9 65 72 6C 61 6E 64 61 69 73 2E 00 "
             "42 54 59 50 45 " // TYPE
                "01 00 "
                "01 "
             "42 4D 41 53 4B " // MASK
                "03 00 "
                "00 00 01 "
             "42 44 46 4C 54 " // DFLT
                "04 00 "
                "00 00 00 00 "
             "00 "
          "00 "
       "00 "
    "00 ";

} // namespace {}


TEST_CASE("Action14Record", "[actions]")
{
    test_yagl<Action14Record, 0x14>(str_YAGL, str_NFO);
}

