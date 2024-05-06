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
#include "IntegerDescriptor.h"
#include "TokenStream.h"
#include <sstream>


TEST_CASE("to_string()", "[integers]")
{
    SECTION("to_string<uint8_t>()")
    {
        uint8_t value = 0x23;
        CHECK(to_string(value, UIntFormat::Hex).compare("0x23") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("35") == 0);

        value = 0xF3;
        CHECK(to_string(value, UIntFormat::Hex).compare("0xF3") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("243") == 0);

        value = 0x35;
        CHECK(to_string(value, UIntFormat::Bool).compare("true") == 0);
        value = 0x00;
        CHECK(to_string(value, UIntFormat::Bool).compare("false") == 0);
    }

    SECTION("to_string<uint16_t>()")
    {
        uint16_t value = 0x1234;
        CHECK(to_string(value, UIntFormat::Hex).compare("0x1234") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("4660") == 0);

        value = 0xffF3;
        CHECK(to_string(value, UIntFormat::Hex).compare("0xFFF3") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("65523") == 0);

        value = 0x1235;
        CHECK(to_string(value, UIntFormat::Bool).compare("true") == 0);
        value = 0x00;
        CHECK(to_string(value, UIntFormat::Bool).compare("false") == 0);
    }

    SECTION("to_string<uint32_t>()")
    {
        uint32_t value = 0x12345678;
        CHECK(to_string(value, UIntFormat::Hex).compare("0x12345678") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("305419896") == 0);

        value = 0xFFF3'FFF3;
        CHECK(to_string(value, UIntFormat::Hex).compare("0xFFF3FFF3") == 0);
        CHECK(to_string(value, UIntFormat::Dec).compare("4294180851") == 0);

        value = 0x1235;
        CHECK(to_string(value, UIntFormat::Bool).compare("true") == 0);
        value = 0x00;
        CHECK(to_string(value, UIntFormat::Bool).compare("false") == 0);
    }

    // Don't use any uint64_t types anywhere.
}


TEST_CASE("UInt types", "[integers]")
{
    SECTION("UInt8")
    {
        for (uint16_t i = 0; i < 256; ++i)
        {
            UInt8 value;
            value.set(static_cast<uint8_t>(i));

            std::ostringstream os;
            value.print(os, UIntFormat::Dec);
            CHECK(os.str() == to_string(static_cast<uint8_t>(i), UIntFormat::Dec));

            os.str("");
            value.print(os, UIntFormat::Hex);
            auto str = os.str();
            CHECK(str == to_string(static_cast<uint8_t>(i), UIntFormat::Hex));

            std::istringstream is(str);
            TokenStream ts{is};
            UInt8 value2;
            value2.parse(ts);
            CHECK(value2.get() == i);
        }

        for (uint16_t i = 0; i < 256; ++i)
        {
            UInt8 value;
            value.set(static_cast<uint8_t>(i));
            std::ostringstream os;
            value.write(os);

            auto str = os.str();
            CHECK(str.size() == 1);
            CHECK(uint8_t(str[0]) == i);

            std::istringstream is(str);
            UInt8 value2;
            value2.read(is);
            CHECK(value2.get() == i);
        }
    }

    SECTION("UInt8Ext")
    {
        for (uint32_t i = 0; i <= 0xFFFF; i += 101)
        {
            UInt8Ext value;
            value.set(static_cast<uint16_t>(i));

            std::ostringstream os;
            value.print(os, UIntFormat::Dec);
            CHECK(os.str() == to_string(static_cast<uint16_t>(i), UIntFormat::Dec));

            os.str("");
            value.print(os, UIntFormat::Hex);
            auto str = os.str();
            CHECK(str == to_string(static_cast<uint16_t>(i), UIntFormat::Hex));

            std::istringstream is(str);
            TokenStream ts{is};
            UInt8Ext value2;
            value2.parse(ts);
            CHECK(value2.get() == i);
        }

        for (uint32_t i = 0; i <= 0xFFFF; i += 101)
        {
            UInt8Ext value;
            value.set(static_cast<uint16_t>(i));
            std::ostringstream os;
            value.write(os);

            // Always used the long format for extended bytes.
            // May need to change this later.
            auto str = os.str();
            CHECK(str.size() == 3);
            CHECK(uint8_t(str[0]) == 0xFF);
            CHECK(uint8_t(str[1]) == (i & 0xFF));
            CHECK(uint8_t(str[2]) == (i >> 8));

            std::istringstream is(str);
            UInt8Ext value2;
            value2.read(is);
            CHECK(value2.get() == i);
        }
    }

    SECTION("UInt16")
    {
        for (uint32_t i = 0; i <= 0xFFFF; i += 101)
        {
            UInt16 value;
            value.set(static_cast<uint16_t>(i));

            std::ostringstream os;
            value.print(os, UIntFormat::Dec);
            CHECK(os.str() == to_string(static_cast<uint16_t>(i), UIntFormat::Dec));

            os.str("");
            value.print(os, UIntFormat::Hex);
            auto str = os.str();
            CHECK(str == to_string(static_cast<uint16_t>(i), UIntFormat::Hex));

            std::istringstream is(str);
            TokenStream ts{is};
            UInt16 value2;
            value2.parse(ts);
            CHECK(value2.get() == i);
        }

        for (uint32_t i = 0; i <= 0xFFFF; i += 101)
        {
            UInt16 value;
            value.set(static_cast<uint16_t>(i));
            std::ostringstream os;
            value.write(os);

            auto str = os.str();
            CHECK(str.size() == 2);
            CHECK(uint8_t(str[0]) == (i & 0xFF));
            CHECK(uint8_t(str[1]) == ((i >> 8) & 0xFF));

            std::istringstream is(str);
            UInt16 value2;
            value2.read(is);
            CHECK(value2.get() == i);
        }
    }

    SECTION("UInt32")
    {
        for (uint64_t i = 0; i <= 0xFFFF'FFFF; i += 1010101)
        {
            UInt32 value;
            value.set(static_cast<uint32_t>(i));

            std::ostringstream os;
            value.print(os, UIntFormat::Dec);
            CHECK(os.str() == to_string(static_cast<uint32_t>(i), UIntFormat::Dec));

            os.str("");
            value.print(os, UIntFormat::Hex);
            auto str = os.str();
            CHECK(str == to_string(static_cast<uint32_t>(i), UIntFormat::Hex));

            std::istringstream is(str);
            TokenStream ts{is};
            UInt32 value2;
            value2.parse(ts);
            CHECK(value2.get() == i);
        }

        for (uint64_t i = 0; i <= 0xFFFF'FFFF; i += 1010101)
        {
            UInt32 value;
            value.set(static_cast<uint32_t>(i));
            std::ostringstream os;
            value.write(os);

            auto str = os.str();
            CHECK(str.size() == 4);
            CHECK(uint8_t(str[0]) == (i & 0xFF));
            CHECK(uint8_t(str[1]) == ((i >> 8) & 0xFF));
            CHECK(uint8_t(str[2]) == ((i >> 16) & 0xFF));
            CHECK(uint8_t(str[3]) == ((i >> 24) & 0xFF));

            std::istringstream is(str);
            UInt32 value2;
            value2.read(is);
            CHECK(value2.get() == i);
        }
    }
}


TEST_CASE("UIntDescriptor", "[integers]")
{
    SECTION("UInt8")
    {
        UIntDescriptor<UInt8> desc{ 0, "name", UIntFormat::Hex };
        UInt8 value{0x23};

        std::ostringstream os;
        desc.print(value, os, 4); // Indent
        CHECK(os.str() == "    name: 0x23;\n");
    }
}
