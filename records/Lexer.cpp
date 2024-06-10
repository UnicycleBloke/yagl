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
#include "Lexer.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <regex>


std::vector<TokenValue> Lexer::lex(std::istream& is)
{
    m_tokens.clear();

    while (!is.eof())
    {
        uint8_t c;
        is.read(reinterpret_cast<char*>(&c), 1);

        ++m_column;
        while (handle_byte(c, is.peek()) == false);
    }

    // Terminate C++ comment, numbers and ident, but not C comments or strings.
    handle_byte('\n', '\n');

    if (m_state != LexerState::None)
    {
        throw LEXER_ERROR("End of input in invalid state", m_line, m_column);
    }

    return m_tokens;
}


bool Lexer::handle_byte(uint8_t c, uint8_t p)
{
    switch (m_state)
    {
        // This the base state from which we kick off the states for non-trivial items in the stream
        // possibly comprising two or more bytes.
        case LexerState::None:
            // First character of an identifier.
            if ( ((c >= 'a') && (c <= 'z')) ||
                 ((c >= 'A') && (c <= 'Z')) ||
                 (c == '_')                 )
            {
                m_value = c;
                m_state = LexerState::Ident;
                return true;
            }

            // First character of a number - could be decimal, hexadecimal, binary, octal or float.
            // Need more characters to determine this later.
            else if ((c >= '0') && (c <= '9'))
            {
                m_value = c;
                // Starting with '0' means we could be octal, binary or hex. The next character
                // will determine this. Otherwise we must be a decimal or maybe a float.
                m_state = (c == '0') ? LexerState::Oct : LexerState::Dec;
                return true;
            }
            // Unary minus for decimal numbers only.
            else if (c == '-')
            {
                m_value = c;
                m_state = LexerState::Dec;
                return true;
            }

            // First character of a string literal.
            else if (c == '"')
            {
                m_value.clear();
                m_state = LexerState::String;
                return true;
            }

            // Possibly the first character of a comment.
            else if (c == '/')
            {
                switch (p)
                {
                    case '/':
                    case '*':
                        // This is the start of a comment
                        m_state = LexerState::Slash;
                        return true;
                    default:
                        // This is a division operator
                        handle_symbol(c, p);
                        return true;
                }
            }

            // Possibly the first character of a bash-style comment.
            else if (c == '#')
            {
                m_state = LexerState::Bash;
                m_value = c;
                return true;
            }

            // All the other tokens are single character symbols.
            return handle_symbol(c, p);

        case LexerState::Peeked:
            // We already consume the current byte to create a digraph.
            m_state = LexerState::None;
            return true;

        // Multi-character token continuation.
        case LexerState::Ident:  return handle_ident(c);
        case LexerState::String: return handle_string(c);
        case LexerState::Oct:    return handle_octal(c);
        case LexerState::Dec:    return handle_decimal(c);
        case LexerState::Hex:    return handle_hexadecimal(c);
        case LexerState::Bin:    return handle_binary(c);
        case LexerState::Float:  return handle_float(c);

        case LexerState::C:     return handle_comment_c(c);
        case LexerState::CPP:   return handle_comment_cpp(c);
        case LexerState::Slash: return handle_comment_slash(c);
        case LexerState::Star:  return handle_comment_star(c);

        case LexerState::Bash:  return handle_bash(c);
    }

    throw LEXER_ERROR("Unhandled character", m_line, m_column);
}


bool Lexer::handle_comment_c(uint8_t c)
{
    // We are in a C-style comment: '*' heralds the maybe end of the comment.
    if (c == '*') m_state = LexerState::Star;
    return true;
}


bool Lexer::handle_comment_cpp(uint8_t c)
{
    // We are in a C++-style comment: '\n' terminates it.
    if (c == '\n')
    {
        m_state  = LexerState::None;
        m_column = 0;
        ++m_line;
    }
    return true;
}


bool Lexer::handle_comment_slash(uint8_t c)
{
    // We are starting a comment: which type is it?
    switch (c)
    {
        case '/': m_state = LexerState::CPP; break;
        case '*': m_state = LexerState::C; break;
        default:  throw LEXER_ERROR("Comment is ill-formed", m_line, m_column);
    }
    return true;
}


bool Lexer::handle_comment_star(uint8_t c)
{
    // We might be about to terminate a C-style comment: if we get '/'.
    switch (c)
    {
        // The comment is finished.
        case '/': m_state = LexerState::None; break;
        // The comment might terminate on the next character.
        case '*': m_state = LexerState::Star; break;
        // Revert to C-style.
        default:  m_state = LexerState::C;
    }
    return true;
}


void Lexer::parse_bash_message()
{
    // This whole function is experimental...

    // we attempt to parse m_value to see it is meaningful as a command.
    // This could have three outcomes:
    // - we find a valid command and execute it,
    // - we find an invalid command and throw an exception, or
    // - we don't find anything and silently ignore this line as a comment.
    std::cout << "Found bash-style comment '" << m_value << "'\n";

    // We can use gcc to combine multiple YAGL files into one with the following command (or similar):
    // 
    //     gcc -C -E -nostdinc -x c-header whatever.pyagl > whatever.yagl
    //
    // Where 'whatever.pyagl' is a regular YAGL file which contains C-style '#include "something.pyagl"' statements.
    // gcc will recursively expand the included files, resulting in a single combined YAGL file. gcc also adds 
    // some bash-style comments which indicate the original PYAGL file from which each block of lines was included.
    // We can parse these to provide a little context for errors while reading the YAGL. It appears that files in
    // sub-directories work just fine, and result in comments with the relative paths preserved.
    //
    // While using gcc in this way is clever, it seems to me that it would be better for yagl to directly implement 
    // '#include "whatever.yagl"' statements. This could be achieved by making Lexer recursive. The lex() method 
    // returns a std::vector<TokenValue> whose items could be move-appended to m_tokens in the current instance.
    // This would remove the dependency on gcc, and on the assumptions about the format of the bash-style comments. 
    // But it's more work...

    // These formats seem to be what gcc inserts when you use gcc -E to combine multiple files into one.
    //     '# 1 "file-name.yagl"'
    //     '# 1 "file-name.yagl" 1'
    const std::regex file_regex(R"(\s*#\s+(\d+)\s+\"(.+)\"\s*)");
    const std::regex file_regex2(R"(\s*#\s+(\d+)\s+\"(.+)\"\s+(\d+)\s*)");

    std::smatch file_match;
    if (std::regex_match(m_value, file_match, file_regex))
    {
        // This should be assigned to m_file or some such, and used when reporting Lexer errors. 
        // Would like a cheap way to make this available to the parser... Perhaps emit a new type of 
        // token which changes the parser's equivalient of m_file...
        std::cout << "Original source file: " << file_match[2] << '\n';
    }
    else if (std::regex_match(m_value, file_match, file_regex2))
    {
        std::cout << "Original source file: " << file_match[2] << '\n';
    }
    else
    {
        std::cout << "No match found\n";
    }    
}


bool Lexer::handle_bash(uint8_t c)
{
    // We are in a bash-style comment: '\n' terminates it.
    if (c == '\n')
    {
        parse_bash_message();
        m_state  = LexerState::None;
        m_column = 0;
        ++m_line;
    }
    else
    {
        m_value += c;
    }
    return true;
}


bool Lexer::handle_symbol(uint8_t c, uint8_t p)
{
    switch (c)
    {
        case '|' : emit(TokenType::Pipe,         "|"); return true;
        case '(' : emit(TokenType::OpenParen,    "("); return true;
        case ')' : emit(TokenType::CloseParen,   ")"); return true;
        case '[' : emit(TokenType::OpenBracket,  "["); return true;
        case ']' : emit(TokenType::CloseBracket, "]"); return true;
        case '{' : emit(TokenType::OpenBrace,    "{"); return true;
        case '}' : emit(TokenType::CloseBrace,   "}"); return true;
        case ':' : emit(TokenType::Colon,        ":"); return true;
        case ';' : emit(TokenType::SemiColon,    ";"); return true;
        case ',' : emit(TokenType::Comma,        ","); return true;
        case '=' : emit(TokenType::Equals,       "="); return true;
        case '&' : emit(TokenType::Ampersand,    "&"); return true;
        case '%' : emit(TokenType::Percent,      "%"); return true;
        case '+' : emit(TokenType::OpPlus,       "+"); return true;
        case '-' : emit(TokenType::OpMinus,      "-"); return true;
        case '*' : emit(TokenType::OpMultiply,   "*"); return true;
        case '/' : emit(TokenType::OpDivide,     "/"); return true;

        case '<' :
            if (p == '<')
            {
                emit(TokenType::ShiftLeft, "<<");
                m_state = LexerState::Peeked;
                return true;
            }
            else
            {
                emit(TokenType::OpenAngle, "<");
                return true;
            }
            break;

        case '>' :
            if (p == '>')
            {
                emit(TokenType::ShiftRight, ">>");
                m_state = LexerState::Peeked;
                return true;
            }
            else
            {
                emit(TokenType::CloseAngle, ">");
                return true;
            }
            break;

        case '.' :
            if (p == '.')
            {
                emit(TokenType::DoubleDot, "..");
                m_state = LexerState::Peeked;
                return true;
            }
            else
            {
                emit(TokenType::SingleDot, ".");
                return true;
            }
            break;

        case '!' :
            if (p == '=')
            {
                emit(TokenType::NotEqual, "!=");
                m_state = LexerState::Peeked;
                return true;
            }
            break;

        // Ignore whitespace. Technically not a symbol but
        case ' '  : return true;
        case '\r' : return true;
        case '\t' : return true;

        case '\n' :
            m_column = 0;
            ++m_line;
            return true;
    }

    throw LEXER_ERROR("Invalid symbol character", m_line, m_column);
}


bool Lexer::handle_ident(uint8_t c)
{
    if ( ((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')) ||
         ((c >= '0') && (c <= '9')) ||
         (c == '_')                 )
    {
        m_value += c;
        return true;
    }

    emit(TokenType::Ident, m_value);
    return false;
}


bool Lexer::handle_string(uint8_t c)
{
    // All strings in YAGL are expected to be well-formed UTF8.
    if (c != '"')
    {
        m_value += c;
        return true;
    }

    emit(TokenType::String, m_value);
    return true;
}


bool Lexer::handle_octal(uint8_t c)
{
    // Only if this is the second character in the number.
    if (m_value.size() == 1)
    {
        // Maybe switch to binary
        if (c == 'b')
        {
            m_value += c;
            m_state = LexerState::Bin;
            return true;
        }
        // Maybe switch to hexadecimal
        else if (c == 'x')
        {
            m_value += c;
            m_state = LexerState::Hex;
            return true;
        }
        // Switch to dealing with a floating point number.
        if (c == '.')
        {
            m_value += c;
            m_state = LexerState::Float;
            return true;
        }
    }

    // It would be confusing to have an octal number followed by another number with
    // no space or anything. So don't allow it.
    if ((c >= '8') && (c <= '9'))
    {
        throw LEXER_ERROR("Invalid octal character", m_line, m_column);
    }

    // Octal digits...
    if ((c >= '0') && (c <= '7'))
    {
        m_value += c;
        return true;
    }

    emit(TokenType::Number, NumberType::Oct, m_value);
    return false;
}


bool Lexer::handle_decimal(uint8_t c)
{
    // Decimal digits...
    if ((c >= '0') && (c <= '9'))
    {
        m_value += c;
        return true;
    }

    // Ignore thousand separators.
    if (c == '\'')
    {
        return true;
    }

    // Switch to dealing with a floating point number.
    if (c == '.')
    {
        m_value += c;
        m_state = LexerState::Float;
        return true;
    }

    emit(TokenType::Number, NumberType::Dec, m_value);
    return false;
}


bool Lexer::handle_float(uint8_t c)
{
    // Decimal digits...
    if ((c >= '0') && (c <= '9'))
    {
        m_value += c;
        return true;
    }

    // Ignore thousand separators - probably don't have these after the point...
    if (c == '\'')
    {
        return true;
    }

    emit(TokenType::Number, NumberType::Float, m_value);
    return false;
}


bool Lexer::handle_hexadecimal(uint8_t c)
{
    if ( ((c >= 'a') && (c <= 'f')) ||
         ((c >= 'A') && (c <= 'F')) ||
         ((c >= '0') && (c <= '9')) )
    {
        m_value += c;
        return true;
    }

    // It is a fault if the hexadecimal number is terminated by
    // an m_value character.
    if ( ((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')) ||
         ((c >= '0') && (c <= '9')) ||
         (c == '_')                 )
    {
        throw LEXER_ERROR("Invalid hexadecimal character", m_line, m_column);
    }

    emit(TokenType::Number, NumberType::Hex, m_value);
    return false;
}


bool Lexer::handle_binary(uint8_t c)
{
    // Binary digits...
    if ((c >= '0') && (c <= '1'))
    {
        m_value += c;
        return true;
    }

    emit(TokenType::Number, NumberType::Bin, m_value);
    return false;
}


void Lexer::emit(TokenType type, std::string value)
{
    emit(type, NumberType::None, value);
}


void Lexer::emit(TokenType type, NumberType num_type, std::string value)
{
    // A binary minus is detected a decimal number: add a correction here.
    if ((type == TokenType::Number) && (value == "-"))
    {
        type = TokenType::OpMinus;
    }

    // Concatenate adjacent strings.
    // if ((type == TokenType::String) && (m_tokens.size() > 0))
    // {
    //     TokenValue& prev_token = *m_tokens.rbegin();
    //     if (prev_token.type == TokenType::String)
    //     {
    //         prev_token.value = prev_token.value + value;
    //     }
    //     else
    //     {
    //         m_tokens.push_back({ type, num_type, value, m_line, m_column });
    //     }
    // }
    // else
    {
        m_tokens.push_back({ type, num_type, value, m_line, m_column });
    }

    m_state = LexerState::None;
}

