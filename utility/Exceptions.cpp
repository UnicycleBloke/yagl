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
#include "Exceptions.h"
#include "Lexer.h"
#include <sstream>


LexerError::LexerError(const std::string& what_arg, uint32_t line, uint32_t column)
: LexerError{what_arg.c_str(), line, column}
{
}


LexerError::LexerError(const char* what_arg, uint32_t line, uint32_t column)
: std::runtime_error{what_arg}
{
    std::ostringstream os;
    os << "YAGL lexer error: ";
    os << what_arg << " at line " << line << " column " << column << " in script file";
    m_what = os.str();
}


ParserError::ParserError(const std::string& what_arg, const TokenValue& token)
: ParserError{what_arg.c_str(), token}
{
}


ParserError::ParserError(const char* what_arg, const TokenValue& token)
: std::runtime_error{what_arg}
{
    std::ostringstream os;
    os << "YAGL parser error: ";
    os << what_arg << " at line " << token.line << " column " << token.column << " in script file";
    m_what = os.str();
}


RuntimeError::RuntimeError(const std::string& what_arg, const char* file, uint32_t line)
: RuntimeError(what_arg.c_str(), file, line)
{
}


RuntimeError::RuntimeError(const char* what_arg, const char* file, uint32_t line)
: std::runtime_error(what_arg)
{
    std::ostringstream os;
    os << "Runtime error: ";
    os << what_arg << " at line " << line << " in source file " << file;
    m_what = os.str();
}


