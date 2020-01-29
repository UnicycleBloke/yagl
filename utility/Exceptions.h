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
#include <string>
#include <cstdint>
#include <stdexcept>


// Exception used during lexing to indicate where in the input stream the error occurred.
class LexerError : public std::runtime_error 
{
public:
    explicit LexerError(const std::string& what_arg, uint32_t line, uint32_t column, const char* file, uint32_t line2);
    explicit LexerError(const char* what_arg, uint32_t line, uint32_t column, const char* file, uint32_t line2);

    const char* what() const noexcept { return m_what.c_str(); }

private:    
    std::string m_what;
};
#define LEXER_ERROR(what, line, column) LexerError(what, line, column, __FILE__, __LINE__)


// Exception used during parsing to indicates where in the original input the error occured.
struct TokenValue;
class ParserError : public std::runtime_error 
{
public:
    explicit ParserError(const std::string& what_arg, const TokenValue& token, const char* file, uint32_t line);
    explicit ParserError(const char* what_arg, const TokenValue& token, const char* file, uint32_t line);

    const char* what() const noexcept { return m_what.c_str(); }

private:    
    std::string m_what;
};
#define PARSER_ERROR(what, token) ParserError(what, token, __FILE__, __LINE__)


class RuntimeError : public std::runtime_error 
{
public:
    explicit RuntimeError(const std::string& what_arg, const char* file, uint32_t line);
    explicit RuntimeError(const char* what_arg, const char* file, uint32_t line);

    const char* what() const noexcept { return m_what.c_str(); }

private:    
    std::string m_what;
};
#define RUNTIME_ERROR(what) RuntimeError(what, __FILE__, __LINE__)


class PropertyError : public std::runtime_error 
{
public:
    explicit PropertyError(const std::string& what_arg, uint8_t property, const char* file, uint32_t line);
    explicit PropertyError(const char* what_arg, uint8_t property, const char* file, uint32_t line);

    const char* what() const noexcept { return m_what.c_str(); }

private:    
    std::string m_what;
};
#define PROPERTY_ERROR(what, property) PropertyError(what, property, __FILE__, __LINE__)


