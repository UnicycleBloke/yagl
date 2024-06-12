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
#include "Exceptions.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>


// All the different
enum class TokenType
{
    // Tokens with no value
    Pipe,                      // Bitmask items?
    Colon,                     // Property assignment/setting
    SemiColon,                 // Statement termination
    Comma,                     // Function arguments
    OpenParen,   CloseParen,   // Function arguments
    OpenBracket, CloseBracket, // Lists
    OpenBrace,   CloseBrace,   // Blocks
    OpenAngle,   CloseAngle,   // Arguments?

    Equals,     // =
    NotEqual,   // !=
    Ampersand,  // &
    Percent,    // %
    SingleDot,  // . Remove - only for file name in current output.
    DoubleDot,  // ..
    ShiftLeft,  // <<
    ShiftRight, // >>

    OpPlus,     // +
    OpMinus,    // -
    OpMultiply, // *
    OpDivide,   // / - single slash

    // Tokens with values attached
    Number,     // Any decimal, binary (0bXXX), octal (0XXX), hexadecimal (0xXXX), or (decimal) floating point number
    Ident,      // Any name or keyword
    String,     // Any string literal - this is a unicode string endoded as UTF-8.
    FileName,   // A pseudo-token which indicates the source file from which the subsequent tokens come.
                // This is more efficient than duplicating the same string in a huge number of Token objects.

    // Indicates the end of input for parsing.
    Terminator
};


// Used to rememeber the type of number that a TokenType::Number is. Not
// really required as we can work it out from the string, but convenient.
enum class NumberType
{
    Dec, Bin, Oct, Hex, Float, None
};


// Lexeme extracted from the input stream with its associated value and position in the file.
struct TokenValue
{
    TokenType      type{};
    NumberType     num_type{};
    std::string    value{};
    uint32_t       line{};
    uint32_t       column{};
    //std::string    filename{}; // See TokenType::FileName instead.
};


// Very simple lexer to create a stream of tokens for parsing.
class Lexer
{
public:
    std::vector<TokenValue> lex(std::istream& yagl_stream);

private:
    // These return true if the byte is consumed. In some cases, the state
    // changes but the byte is not consumed - a peek without a peek.
    bool handle_byte(uint8_t c, uint8_t p);
    bool handle_symbol(uint8_t c, uint8_t p);
    bool handle_ident(uint8_t c);
    bool handle_octal(uint8_t c);
    bool handle_decimal(uint8_t c);
    bool handle_hexadecimal(uint8_t c);
    bool handle_binary(uint8_t c);
    bool handle_float(uint8_t c);
    bool handle_string(uint8_t c);

    // There is quite a lot of palaver involved in stripping C and CPP style comments.
    bool handle_comment_c(uint8_t c);
    bool handle_comment_cpp(uint8_t c);
    bool handle_comment_slash(uint8_t c);
    bool handle_comment_star(uint8_t c);

    // Lines beginning with '#' are essentially comments, but we attempt to parse them
    // to find one of a set of preprocessor/pragma-like directives.
    bool handle_bash(uint8_t c);
    void parse_bash_message();
    bool parse_bash_filename();

    // Append a new token to the output list.
    void emit(TokenType type, std::string value = "");
    void emit(TokenType type, NumberType num_type, std::string value = "");

private:
    // The lexer is simple state machine for the purposes of combining
    // multiple characters into a single lexeme.
    enum class LexerState
    {
        // We are not in the middle of a multicharacter lexeme.
        None,
        // We used the peek value to create a digraph.
        Peeked,
        // We are constructing and alphanumeric identifier.
        Ident,
        // We are constructing a literal string - can contain anything. Delimited by double quotes.
        String,
        // States for constructing numbers in the supported formats.
        Dec, Bin, Oct, Hex, Float,
        // States for eliminating comments from the input stream.
        // '/' starts a comment: C-style if followed by '*'; C++-style if followed by '/'; or an error.
        // '*' heralds the end of C-style, if followed by '/', or is just a continuation.
        // '\n' marks the end of C++-style
        Slash, Star, CPP, C,
        // State for bash-style "comments" which begin with '#' and run to the end of the line. These may be 
        // interpreted as commands or pragmas in some case. 
        Bash
    };

    // The lexer is a simple state machine.
    LexerState m_state = LexerState::None;

    // Keep track of the position in the file to allow more targeted errors.
    uint32_t                m_line   = 1;
    uint32_t                m_column = 0;
    std::string             m_value;
    std::vector<TokenValue> m_tokens;
};
