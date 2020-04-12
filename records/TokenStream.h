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
#include <fstream>


// Can we make this more stream based? Get the lexer to some work, and then a bit 
// more when the queue is empty. Could use a ring buffer since we don't care about
// items already matched.
class TokenStream
{
public:
    TokenStream(const std::string& yagl_file) //const std::vector<TokenValue> tokens)
    : m_yagl_file{yagl_file} 
    {
        Lexer lexer{yagl_file};
        m_tokens = lexer.lex();
    }

    const TokenValue& peek(uint16_t lookahead = 0); 
    const std::string& match(TokenType type);

    bool match_ident(const std::string& value);

    template <typename T>
    T match_uint()
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
            const TokenValue& token = peek();
            throw PARSER_ERROR("Unsupported type: " + token.value, token);
        }

        return {};
    }

    uint32_t match_uint32();
    uint16_t match_uint16();
    uint8_t  match_uint8();
    bool     match_bool();

    // Special case for integer matching.
    enum class DataType { U32, U16, U8 };
    uint32_t match_date(DataType type); 
    
    // Added to allow us to move to the beginning of the next record when an exception occurs
    // during parsing.
    void next_record();
    
    // Backtrack one step. This is a bodge really, and could easily be removed. For now the 
    // names of records are parsed to create the right type of object, but backtracked and 
    // parsed again by that object. This gives a nicer exception...
    void unmatch() { if (m_index > 0) --m_index; }

    const std::string& yagl_file() const { return m_yagl_file; }

private:
    uint64_t match_uint64(TokenValue& token, DataType type);

private:    
    // Cached name of the script file that we are parsing. Used for error reporting.
    std::string m_yagl_file;

    // List of all tokens found in the YAGL by the lexer, in order.
    std::vector<TokenValue> m_tokens;
    
    // Current position in the stream while parsing. 
    uint32_t m_index = 0;
    
    // Current block depth. Blocks are delineated by { and }. This can used 
    // after an exception to start parsing again with the next record.
    uint32_t m_blocks = 0; 
};



