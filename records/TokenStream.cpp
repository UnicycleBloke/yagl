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


const TokenValue& TokenStream::peek(uint16_t lookahead) 
{
    static const TokenValue terminator{TokenType::Terminator, NumberType::None, ""}; 

    if ((m_index + lookahead) >= m_tokens.size())
        return terminator;

    //const TokenValue value = m_tokens[m_index + lookahead];    
    return m_tokens[m_index + lookahead]; 
}


std::string TokenStream::match(TokenType type) 
{
    const TokenValue& token = peek();
    if (type != token.type)
    {
        throw PARSER_ERROR("Unexpected token: " + token.value, token);
    }
    ++m_index;    
    return token.value;
}


bool TokenStream::match_ident(const std::string& value) 
{
    const TokenValue& token = peek();
    if ((TokenType::Ident != token.type) || (token.value != value))
    {
        throw PARSER_ERROR("Unexpected token: " + token.value, token);
    }
    match(TokenType::Ident);
    return true;
}


uint64_t TokenStream::match_uint64(TokenValue& token)
{
    token = peek();
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


uint32_t TokenStream::match_uint32()
{
    TokenValue token;
    uint64_t result = match_uint64(token);
    if (result > 0xFFFF'FFFF)
    {
        throw PARSER_ERROR("UNIT32 value out of range", token);
    }
    return static_cast<uint32_t>(result);
}


uint16_t TokenStream::match_uint16()
{
    TokenValue token;
    uint64_t result = match_uint64(token);
    if (result > 0xFFFF)
    {
        throw PARSER_ERROR("UNIT16 value out of range", token);
    }
    return static_cast<uint16_t>(result);
}


uint8_t TokenStream::match_uint8()
{
    TokenValue token;
    uint64_t result = match_uint64(token);
    if (result > 0xFF)
    {
        throw PARSER_ERROR("UNIT8 value out of range", token);
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

