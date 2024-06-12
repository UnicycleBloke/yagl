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
#include "TokenStream.h"
#include "application/CommandLineOptions.h"
#include <sstream>


void TokenStream::match_filename()
{
    // Deal with multiple consecutive file name tokens.
    while (true)
    {
        const TokenValue& token = peek();
        if (token.type == TokenType::FileName)
        {
            //std::cout << "Changing YAGL file name: " << token.value << '\n';
            CommandLineOptions::options().set_curr_file(token.value);
            match(TokenType::FileName);
        }
        else
        {
            return;
        }
    }
}


const TokenValue& TokenStream::peek(uint16_t lookahead)
{
    static const TokenValue terminator{TokenType::Terminator, NumberType::None, ""};

    uint32_t index = m_index + lookahead;
    if (index >= static_cast<uint32_t>(m_tokens.size()))
        return terminator;

    return m_tokens[index];
}


const std::string& TokenStream::match(TokenType type)
{
    const TokenValue& token = peek();
    if (type != token.type)
    {
        throw PARSER_ERROR("Unexpected match token: '" + token.value + "'", token);
    }
    ++m_index;
    if (token.type == TokenType::OpenBrace)
    {
        //std::cout << m_blocks << " -> " << (m_blocks + 1) << "\n";
        ++m_blocks;
    }
    if (token.type == TokenType::CloseBrace)
    {
        //std::cout << m_blocks << " -> " << (m_blocks - 1) << "\n";
        --m_blocks;
    }

    match_filename();

    return token.value;
}


void TokenStream::next_record()
{
    // We are at the top level already. Skip tokens until we enter a block. 
    while (m_blocks == 0)
    {
        const TokenValue& token = peek();
        if (token.type == TokenType::Terminator)
        {
            throw PARSER_ERROR("Unexpected end of token stream", token);
        }

        match(token.type);
    }

    // Skip tokens until we leave the current record, by checking block indent.
    while (m_blocks > 0)
    {
        const TokenValue& token = peek();
        if (token.type == TokenType::Terminator)
        {
            throw PARSER_ERROR("Unexpected end of token stream", token);
        }

        match(token.type);
    }
}


bool TokenStream::match_ident(const std::string& value)
{
    const TokenValue& token = peek();
    if ((TokenType::Ident != token.type) || (token.value != value))
    {
        throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
    }
    match(TokenType::Ident);
    return true;
}


uint64_t TokenStream::match_uint64(TokenValue& token, DataType type)
{
    token = peek();
    if (token.type == TokenType::Number)
    {
        match(TokenType::Number);

        // The number format has already been checked during lexing,
        // so we can be confident that it has one of the following formats:
        // - bin: 0bBBBBBBBBB, B is a binary digit
        // - oct: 0DDDDDDDDDD, D is an octal digit
        // - dec: DDDDDDDDDDD, D is a decimal digit - the first is not 0
        // - hex: 0xXXXXXXXXX, X is a hex digit
        switch (token.num_type)
        {
            case NumberType::Bin: return strtoull(token.value.c_str() + 2, nullptr, 2);
            case NumberType::Oct: return strtoull(token.value.c_str(),     nullptr, 8);
            case NumberType::Dec: return strtoull(token.value.c_str(),     nullptr, 10);
            case NumberType::Hex: return strtoull(token.value.c_str() + 2, nullptr, 16);
            default:              throw PARSER_ERROR("Unexpected number format", token);
        }
    }
    else
    {
        return match_date(type);
    }
}


// Based on the rules for the Gregorian calendar.
static constexpr bool is_leap_year(uint32_t year)
{
    bool result = (year % 4) == 0;
    if ((year % 100) == 0)
        result = false;
    if ((year % 400) == 0)
        result = true;
    return result;
}


// Calculates the number of days from year zero 0000/1/1.
static constexpr uint32_t days(uint32_t year, uint8_t month, uint8_t day)
{
    // Based on the rules for the Gregorian calendar.
    uint32_t result = 365 * year;
    result += year / 4;
    result -= year / 100;
    result += year / 400;

    // This is to match the calculation in grfcodec. I think.
    result -= 1;

    // Add days for the whole months already elapsed.
    switch (month)
    {
        case 12: result += 30; // Nov
        case 11: result += 31; // Oct
        case 10: result += 30; // Sep
        case  9: result += 31; // Aug
        case  8: result += 31; // Jul
        case  7: result += 30; // Jun
        case  6: result += 31; // May
        case  5: result += 30; // Apr
        case  4: result += 31; // Mar
        case  3: result += (is_leap_year(year) ? 29 : 28); // Feb
        case  2: result += 31; // Jan
        //case  1: result +=  0;
        //default:
    }

    result += day;
    return result;
}


// Return the number of days since 1920/1/1.
uint32_t TokenStream::match_date(DataType type)
{
    // This number was obtained from the grfcodec source. Checking here to
    // make sure our calculation matches.
    static_assert(days(1920, 1, 1) == 701265, "");
    static constexpr uint32_t DAYS_TO_1920 = days(1920, 1, 1);
    static constexpr uint32_t BASE_YEAR    = 1920;

    if (type == DataType::U8)
    {
        match_ident("year");
        match(TokenType::OpenParen);
        uint32_t year  = match_uint32();
        match(TokenType::CloseParen);
        return year - BASE_YEAR;
    }
    else
    {
        match_ident("date");
        match(TokenType::OpenParen);
        uint32_t year  = match_uint32();
        match(TokenType::OpDivide);
        uint8_t  month = match_uint8();
        match(TokenType::OpDivide);
        uint8_t  day   = match_uint8();
        match(TokenType::CloseParen);

        if (month < 1 || month > 12)
        {
            throw PARSER_ERROR("Invalid month value", peek());
        }

        uint32_t result = days(year, month, day);
        if (type == DataType::U16)
            result -= DAYS_TO_1920;
        return result;
    }
}


uint32_t TokenStream::match_uint32()
{
    TokenValue token;
    uint64_t result = match_uint64(token, DataType::U32);
    // Check for negative value by inverting all the bits. High bits are all 0 or all 1 for
    // numbers which are in range.
    if ((result > 0xFFFF'FFFF) && (~result > 0xFFFF'FFFF))
    {
        throw PARSER_ERROR("UNIT32 value out of range: '" + token.value + "'", token);
    }
    return static_cast<uint32_t>(result);
}


uint16_t TokenStream::match_uint16()
{
    TokenValue token;
    uint64_t result = match_uint64(token, DataType::U16);
    if ((result > 0xFFFF) && (~result > 0xFFFF))
    {
        throw PARSER_ERROR("UNIT16 value out of range: '" + token.value + "'", token);
    }
    return static_cast<uint16_t>(result);
}


uint8_t TokenStream::match_uint8()
{
    TokenValue token;
    uint64_t result = match_uint64(token, DataType::U8);
    if ((result > 0xFF) && (~result > 0xFF))
    {
        throw PARSER_ERROR("UNIT8 value out of range: '" + token.value + "'", token);
    }
    return static_cast<uint8_t>(result);
}


bool TokenStream::match_bool()
{
    const TokenValue& token = peek();
    std::string name = match(TokenType::Ident);
    if (name == "true")
    {
        return true;
    }
    else if (name == "false")
    {
        return false;
    }
    throw PARSER_ERROR("Unexpected number format", token);
}

