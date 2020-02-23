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
#include "StreamHelpers.h"
#include "CommandLineOptions.h"
#include <sstream>


LexerError::LexerError(const std::string& what_arg, uint32_t line, uint32_t column, const char* file, uint32_t line2)
: LexerError{what_arg.c_str(), line, column, file, line2}
{
}


LexerError::LexerError(const char* what_arg, uint32_t line, uint32_t column, const char* file, uint32_t line2)
: std::runtime_error{what_arg}
{
    CommandLineOptions& options = CommandLineOptions::options(); 

    std::ostringstream os;
    os << "YAGL lexer error: ";
    os << what_arg << " at line " << line << " column " << column << " in " << options.yagl_file();
    if (options.debug())
    {
        os << "\n  [at line " << line << " in source file " << file << "]";
    }
    m_what = os.str();
}


ParserError::ParserError(const std::string& what_arg, const TokenValue& token, const char* file, uint32_t line)
: ParserError{what_arg.c_str(), token, file, line}
{
}


ParserError::ParserError(const char* what_arg, const TokenValue& token, const char* file, uint32_t line)
: std::runtime_error{what_arg}
{
    CommandLineOptions& options = CommandLineOptions::options(); 

    std::ostringstream os;
    os << "YAGL parser error: ";
    os << what_arg << " at line " << token.line << " column " << token.column << " in " << options.yagl_file();
    if (options.debug())
    {
        os << "\n  [at line " << line << " in source file " << file << "]";
    }
    m_what = os.str();
}


RuntimeError::RuntimeError(const std::string& what_arg, const char* file, uint32_t line)
: RuntimeError(what_arg.c_str(), file, line)
{
}


RuntimeError::RuntimeError(const char* what_arg, const char* file, uint32_t line)
: std::runtime_error(what_arg)
{
    CommandLineOptions& options = CommandLineOptions::options(); 

    std::ostringstream os;
    os << "Runtime error: ";
    os << what_arg;
    if (options.debug())
    {
        os << "\n  [at line " << line << " in source file " << file << "]";
    }
    m_what = os.str();
}


PropertyError::PropertyError(const std::string& what_arg, uint8_t property, const char* file, uint32_t line)
: PropertyError(what_arg.c_str(), property, file, line)
{
}


PropertyError::PropertyError(const char* what_arg, uint8_t property, const char* file, uint32_t line)
: std::runtime_error(what_arg)
{
    CommandLineOptions& options = CommandLineOptions::options(); 

    std::ostringstream os;
    os << "Property error: ";
    os << what_arg << ": property=" << to_hex(property);
    if (options.debug())
    {
        os << "\n  [at line " << line << " in source file " << file << "]";
    }
    m_what = os.str();
}


