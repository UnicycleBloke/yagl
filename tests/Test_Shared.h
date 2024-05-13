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
#pragma once
#include "catch.hpp"
#include "Record.h"
#include "StreamHelpers.h"
// This just doesn't work. See remove_if usage below.
//#include <execution>


template <typename ActionRecord, uint8_t ACTION>
void test_yagl_matches(const char* YAGL_IN, const char* YAGL_OUT)
{
    SpriteZoomMap sprites;
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts, sprites);
    std::ostringstream os;
    action.print(os, sprites, 0);

    SpriteZoomMap sprites2;
    std::istringstream is2(YAGL_IN);
    TokenStream ts2{is2};
    ActionRecord action2;
    action2.parse(ts2, sprites2);
    std::ostringstream os2;
    action2.print(os2, sprites2, 0);

    CHECK(os.str() == os2.str());
}


// This parses some YAGL and ensures that printing it back out again matches. The
// second YAGL parameter can be used when we expect the printed output to be different
// to the input (due to ordering differences). The parsed information is also written
// out as binary and read back in, and then printed again, to confirm that the binary
// works as expected.
template <typename ActionRecord, uint8_t ACTION>
void test_yagl(const char* YAGL_IN, const char* NFO, const char* YAGL_OUT = nullptr)
{
    if (YAGL_OUT == nullptr)
    {
        YAGL_OUT = YAGL_IN;
    }
    if (YAGL_OUT != YAGL_IN)
    {
        test_yagl_matches<ActionRecord, ACTION>(YAGL_OUT, YAGL_IN);
    }

    SpriteZoomMap sprites;

    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts, sprites);

    std::ostringstream os;
    //action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL_OUT);
    if (os.str() != YAGL_OUT)
    {
        std::cout << hex_dump(os.str(), true) << "\n";
        std::cout << hex_dump(YAGL_OUT, true) << "\n";
    }

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
    ActionRecord action2;
    action2.read(is2, info);
    os.str("");
    action2.print(os, sprites, 0);
    CHECK(os.str() == YAGL_OUT);
}


template <typename ActionRecord, uint8_t ACTION>
void test_container(const char* YAGL_IN, const char* NFO, const char* YAGL_OUT = nullptr)
{
    if (YAGL_OUT == nullptr)
    {
        YAGL_OUT = YAGL_IN;
    }
    if (YAGL_OUT != YAGL_IN)
    {
        test_yagl_matches<ActionRecord, ACTION>(YAGL_OUT, YAGL_IN);
    }

    SpriteZoomMap sprites;

    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ActionRecord action;
    action.parse(ts, sprites);

    std::ostringstream os;
    //action.print(std::cout, sprites, 0);
    action.print(os, sprites, 0);
    CHECK(os.str() == YAGL_OUT);
    if (os.str() != YAGL_OUT)
    {
        std::cout << hex_dump(os.str(), true) << "\n";
        std::cout << hex_dump(YAGL_OUT, true) << "\n";
    }

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

    // Don't test that the read content matches. This is because we don't
    // read and write the child records which contain the actual sprites, nor
    // the real sprite records themselves in the case of Container2 files.
    //ActionRecord action2;
    //action2.read(is2, info);
    //os.str("");
    //action2.print(os, sprites, 0);
    //CHECK(os.str() == YAGL_OUT);
}


inline bool confirm_strings_equal(const char* p1, const char* p2)
{
    int row = 0;
    int col = 0;

    while (*p1 && *p2)
    {
        if (*p1 != *p2)
        {
            std::cout << "Strings differ at row=" << row << ", col=" << col << "\n";
            return false;
        }

        ++col;
        if (*p1 == '\n')
        {
            ++row;
            col = 0;
        }  

        ++p1;
        ++p2;  
    }

    if (*p1 || *p2)
    {
        std::cout << "Strings are of different lengths\n";
        return false;
    }

    return true;
}


template <typename ValueType>
void test_yagl_fragment(const char* YAGL_IN, const char* NFO, const char* YAGL_OUT = nullptr)
{
    if (YAGL_OUT == nullptr)
    {
        YAGL_OUT = YAGL_IN;
    }

    // Confirm that we print what we parse.
    // The sample is in the expected format.
    std::istringstream is(YAGL_IN);
    TokenStream ts{is};
    ValueType value{};
    value.parse(ts);

    std::ostringstream os;
    value.print(os);
    std::string yagl_new = os.str();
    std::string yagl_out = YAGL_OUT;
    auto confirm = confirm_strings_equal(yagl_new.c_str(), YAGL_OUT);
    // if (yagl_new != yagl_out)
    // {
    //     auto str1 = yagl_new;
    //     auto str2 = yagl_out;
    //     // For some reason this call causes error with g++9.4.0-Ubuntu-20.04 without the execution policy.
    //     // Nope. Stills fails. The code was fine with Visual Studio.
    //     str1.erase(std::remove_if(std::execution::seq, str1.begin(), str1.end(), std::isspace), str1.end());
    //     str2.erase(std::remove_if(std::execution::seq, str2.begin(), str2.end(), std::isspace), str2.end());
    //     if (str1 == str2)
    //     {
    //         std::cout << "Strings differ only in whitespace\n";
    //     }
    // }
    CHECK(confirm);
    CHECK(yagl_new == yagl_out);

    // Confirm that the written binary matches the sample.
    os.str("");
    value.write(os);
    auto str = os.str();
    CHECK(str.size() == (std::strlen(NFO) / 3));
    CHECK(hex_dump(str) == NFO);

    // Confirm that reading the binary and printing the
    // result gets us back to the example.
    std::istringstream is2(str);
    ValueType value2;
    value2.read(is2);
    os.str("");
    value2.print(os);
    CHECK(os.str() == YAGL_OUT);
}


template <typename ValueType>
void test_yagl_fragment(const std::string& YAGL_IN, const std::string& NFO, const std::string& YAGL_OUT)
{
    test_yagl_fragment<ValueType>(YAGL_IN.c_str(), NFO.c_str(), YAGL_OUT.c_str());
}


template <typename ValueType>
void test_yagl_fragment(const std::string& YAGL_IN, const std::string& NFO)
{
    test_yagl_fragment<ValueType>(YAGL_IN.c_str(), NFO.c_str(), nullptr);
}


