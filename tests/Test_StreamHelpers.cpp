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
#include "StreamHelpers.h"
#include <sstream>


TEST_CASE("Basic integer read/write tests", "[integers]") 
{
    SECTION("Byte values")
    {
        for (uint16_t index = 0; index <=255; ++index)
        {
            uint8_t value = index;
            std::ostringstream os;
            write_uint8(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 1);
            REQUIRE(uint8_t(str[0]) == index);

            std::istringstream is(str);
            value = read_uint8(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Extended byte values - always long format")
    {    
        for (uint32_t index = 0; index <=65535; index += 5)
        {
            uint16_t value = index;
            std::ostringstream os;
            write_uint8_ext(os, value, ExtByteFormat::Long);
            std::string str = os.str();
            REQUIRE(str.size() == 3);
            REQUIRE(uint8_t(str[0]) == 0xFF);
            REQUIRE(uint8_t(str[1]) == (index & 0xFF));
            REQUIRE(uint8_t(str[2]) == (index >> 8));

            std::istringstream is(str);
            value = read_uint8_ext(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Extended byte values - prefer short format")
    {
        for (uint32_t index = 0; index <=65535; index += 5)
        {
            uint16_t value = index;
            std::ostringstream os;
            write_uint8_ext(os, value, ExtByteFormat::Short);
            std::string str = os.str();
            if (value < 0xFF)
            {
                REQUIRE(str.size() == 1);
                REQUIRE(uint8_t(str[0]) == index);
            }
            else
            {
                REQUIRE(str.size() == 3);
                REQUIRE(uint8_t(str[0]) == 0xFF);
                REQUIRE(uint8_t(str[1]) == (index & 0xFF));
                REQUIRE(uint8_t(str[2]) == (index >> 8));
            }
            
            std::istringstream is(str);
            value = read_uint8_ext(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Word values")
    {
        for (uint32_t index = 0; index <=65535; index += 5)
        {
            uint16_t value = index;
            std::ostringstream os;
            write_uint16(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 2);
            REQUIRE(uint8_t(str[0]) == (index & 0xFF));
            REQUIRE(uint8_t(str[1]) == (index >> 8));

            std::istringstream is(str);
            value = read_uint16(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Double word values")
    {
        for (uint64_t index = 0; index <= 0xFFFF'FFFF; index += 0x10101)
        {
            uint32_t value = index;
            std::ostringstream os;
            write_uint32(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 4);
            REQUIRE(uint8_t(str[0]) == (index & 0xFF));
            REQUIRE(uint8_t(str[1]) == ((index >> 8) & 0xFF));
            REQUIRE(uint8_t(str[2]) == ((index >> 16) & 0xFF));
            REQUIRE(uint8_t(str[3]) == (index >> 24));

            std::istringstream is(str);
            value = read_uint32(is);
            REQUIRE(value == index);
        }
    }
}


TEST_CASE("Template integer read/write tests", "[integers]") 
{
    SECTION("Byte values")
    {
        for (uint16_t index = 0; index <=255; ++index)
        {
            uint8_t value = index;
            std::ostringstream os;
            write_uint<uint8_t>(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 1);
            REQUIRE(uint8_t(str[0]) == index);

            std::istringstream is(str);
            value = read_uint<uint8_t>(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Extended byte values - always long format when using the template")
    {    
        for (uint32_t index = 0; index <=65535; index += 5)
        {
            uint16_t value = index;
            std::ostringstream os;
            write_uint<uint16_t, true>(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 3);
            REQUIRE(uint8_t(str[0]) == 0xFF);
            REQUIRE(uint8_t(str[1]) == (index & 0xFF));
            REQUIRE(uint8_t(str[2]) == (index >> 8));

            std::istringstream is(str);
            value = read_uint<uint16_t, true>(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Word values")
    {
        for (uint32_t index = 0; index <=65535; index += 5)
        {
            uint16_t value = index;
            std::ostringstream os;
            write_uint<uint16_t>(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 2);
            REQUIRE(uint8_t(str[0]) == (index & 0xFF));
            REQUIRE(uint8_t(str[1]) == (index >> 8));

            std::istringstream is(str);
            value = read_uint<uint16_t>(is);
            REQUIRE(value == index);
        }
    }

    SECTION("Double word values")
    {
        for (uint64_t index = 0; index <= 0xFFFF'FFFF; index += 0x10101)
        {
            uint32_t value = index;
            std::ostringstream os;
            write_uint<uint32_t>(os, value);
            std::string str = os.str();
            REQUIRE(str.size() == 4);
            REQUIRE(uint8_t(str[0]) == (index & 0xFF));
            REQUIRE(uint8_t(str[1]) == ((index >> 8) & 0xFF));
            REQUIRE(uint8_t(str[2]) == ((index >> 16) & 0xFF));
            REQUIRE(uint8_t(str[3]) == (index >> 24));

            std::istringstream is(str);
            value = read_uint<uint32_t>(is);
            REQUIRE(value == index);
        }
    }
}


TEST_CASE("to_hex() template", "[integers]") 
{
    SECTION("to_hex<uint8_t>")
    {
        uint8_t value = 0x13;
        REQUIRE(to_hex(value) == "0x13"); 
        REQUIRE(to_hex(value, false) == "13"); // No prefix 
    }

    SECTION("to_hex<uint16_t>")
    {
        uint16_t value = 0x1357;
        REQUIRE(to_hex(value) == "0x1357"); 
        REQUIRE(to_hex(value, false) == "1357"); // No prefix 
    }

    SECTION("to_hex<uint32_t>")
    {
        uint32_t value = 0x1323'4567;
        REQUIRE(to_hex(value) == "0x13234567"); 
        REQUIRE(to_hex(value, false) == "13234567"); // No prefix 
    }
}