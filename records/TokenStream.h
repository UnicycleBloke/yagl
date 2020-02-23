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
#include "Lexer.h"


// Can we make this more stream based? Get the lexer to some work, and then a bit 
// more when the queue is empty. Could use a ring buffer since we don't care about
// items already matched.
class TokenStream
{
public:
    TokenStream(const std::vector<TokenValue> tokens)
    : m_tokens(tokens)
    {
    }

    const TokenValue& peek(uint16_t lookahead = 0); 
    std::string match(TokenType type); 

    bool match_ident(const std::string& value);

    template <typename T>
    T match_integer_t()
    {
        if constexpr (std::is_same_v<T, uint8_t>)
        {
            return match_uint8();
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            return match_uint16();
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            return match_uint32();
        }
        else
        {
            const Token& token = peek();
            throw PARSER_ERROR("Unsupported type: " + token.value, token);
        }

        return {};
    }

    uint32_t match_uint32();
    uint16_t match_uint16();
    uint8_t  match_uint8();
    bool     match_bool();

private:
    uint64_t match_uint64(TokenValue& token);

private:    
    uint32_t                      m_index = 0;
    const std::vector<TokenValue> m_tokens;
};



