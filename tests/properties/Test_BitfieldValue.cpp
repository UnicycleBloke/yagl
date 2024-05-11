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
#include "properties/BitfieldValue.h"


// Create a self-contained value types for each specific enumeration. This is not 
// strictly necessary, but is needed here because the tests expect a default constructor 
// for the value type.
class TestBitfield8 : public BitfieldValue<uint8_t>
{
public:
    TestBitfield8()
    : BitfieldValue<uint8_t>{{ {1, "One"}, {0, "Zero"}, {5, "Five"}, {2, "Two"}, {3, "Three"}, {7, "Seven"} }} 
    {}
};


class TestBitfield16 : public BitfieldValue<uint16_t>
{
public:
    TestBitfield16()
    : BitfieldValue<uint16_t>{{ {1, "One"}, {0, "Zero"}, {5, "Five"}, {2, "Two"}, {13, "Three"}, {14, "Seven"} }} 
    {}
};


class TestBitfield32 : public BitfieldValue<uint32_t>
{
public:
    TestBitfield32()
    : BitfieldValue<uint32_t>{{ {1, "One"}, {0, "Zero"}, {5, "Five"}, {2, "Two"}, {23, "Three"}, {27, "Seven"} }} 
    {}
};


TEST_CASE("BitfieldValue", "[properties]")
{
    test_yagl_fragment<TestBitfield8>("Five | Seven", "A0 ");
    test_yagl_fragment<TestBitfield8>("One | Zero | Three", "0B ");

    test_yagl_fragment<TestBitfield16>("Five | Three", "20 20 ");
    test_yagl_fragment<TestBitfield16>("One | Zero | Three | Seven", "03 60 ");

    test_yagl_fragment<TestBitfield32>("Five | Three", "20 00 80 00 ");
    test_yagl_fragment<TestBitfield32>("One | Zero | Three | Seven", "03 00 80 08 ");
}
