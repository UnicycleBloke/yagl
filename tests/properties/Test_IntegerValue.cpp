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
#include "properties/Array.h"
#include "properties/IntegerValue.h"


TEST_CASE("UInt8", "[properties]")
{
    // Just confirm the aliases used in the main code are what I think they are.
    static_assert(std::is_same_v<UInt8,  UInt<uint8_t>>);
    static_assert(std::is_same_v<UInt8X, UInt<uint16_t, true>>);
    static_assert(std::is_same_v<UInt16, UInt<uint16_t>>);
    static_assert(std::is_same_v<UInt32, UInt<uint32_t>>);

    // YAGL_IN is parsed and written out to NFO. The NFO is read in and printed as YAGL_OUT. 
    //test_yagl_fragment<ValueType>(YAGL_IN, NFO, YAGL_OUT);

    // UInt value types are generally all written out as hex as this is closer to 
    // NFO and the specifications. But we can create type aliases from the same 
    // template which formats output as decimal.
    using UInt8Dec  = UInt<uint8_t,  false, UIntFormat::Dec>;
    using UInt8XDec = UInt<uint16_t,  true, UIntFormat::Dec>;
    using UInt16Dec = UInt<uint16_t, false, UIntFormat::Dec>;
    using UInt32Dec = UInt<uint32_t, false, UIntFormat::Dec>;

    test_yagl_fragment<UInt8>("80", "50 ", "0x50");
    test_yagl_fragment<UInt8Dec>("80", "50 ", "80");

    // By default, all extended bytes are written out in long form. All value types 
    // have the same API for print(), but we could specify the format as a template 
    // argument and then have an alias UInt8XShort or some such. 
    test_yagl_fragment<UInt8X>("80", "FF 50 00 ", "0x0050");
    test_yagl_fragment<UInt8X>("512", "FF 00 02 ", "0x0200");
    test_yagl_fragment<UInt8XDec> ("81", "FF 51 00 ", "81");
    test_yagl_fragment<UInt8XDec>("513", "FF 01 02 ", "513");
    test_yagl_fragment<UInt8XDec>("01001", "FF 01 02 ", "513"); // Octal
    test_yagl_fragment<UInt8XDec>("0b1000000001", "FF 01 02 ", "513"); // Binary

    test_yagl_fragment<UInt16>("80", "50 00 ", "0x0050");
    test_yagl_fragment<UInt16>("10001", "11 27 ", "0x2711");
    test_yagl_fragment<UInt16Dec>("80", "50 00 ", "80");
    test_yagl_fragment<UInt16Dec>("10001", "11 27 ", "10001");
    test_yagl_fragment<UInt16Dec>("023421", "11 27 ", "10001"); // Octal
    test_yagl_fragment<UInt16Dec>("0b0010011100010001", "11 27 ", "10001"); // Binary

    test_yagl_fragment<UInt32>("80", "50 00 00 00 ", "0x00000050");
    test_yagl_fragment<UInt32>("10001", "11 27 00 00 ", "0x00002711");
    test_yagl_fragment<UInt32>("27182818", "E2 C6 9E 01 ", "0x019EC6E2");
    test_yagl_fragment<UInt32Dec>("80", "50 00 00 00 ", "80");
    test_yagl_fragment<UInt32Dec>("10001", "11 27 00 00 ", "10001");
    test_yagl_fragment<UInt32Dec>("27182818", "E2 C6 9E 01 ", "27182818");
    test_yagl_fragment<UInt32Dec>("0147543342", "E2 C6 9E 01 ", "27182818"); // Octal
    test_yagl_fragment<UInt32Dec>("0b0001100111101100011011100010", "E2 C6 9E 01 ", "27182818"); // Binary

    // TODO How much support do we need for signed integral value types? Add tests.
}
