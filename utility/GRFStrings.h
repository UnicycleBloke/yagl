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
#include "GRFLabel.h"
#include <string>


std::string    grf_string_to_readable_utf8(const std::string& str);
//std::u16string grf_string_to_utf16(const std::string& str);
//std::u16string grf_string_utf16_to_readable_utf16(const std::u16string& str);


// Replace std::string with this in all the records and other structures.
class GRFString
{
public:
    // Binary serialisation
    // Directly read or write m_value. 
    // TODO indicate whether the string is terminated - CTOR?.
    void read(std::istream& is, const GRFInfo& info);
    void write(std::ostream& os, const GRFInfo& info) const;

    // Text serialisation
    // Convert the internal representation to a human readable version.
    void print(std::ostream& os) const;
    // Convert the value that is read into the internal representation. 
    // Determine whether or not the internal version needs to be UTF8.
    void parse(TokenStream& is);

private:
    // This is the value as read from or written to a binary GRF file.
    // When reading from or writing to a YAGL file, some conversion is 
    // necessary.
    // TODO maybe prefer the intermediate std::u16string though this consumes more RAM 
    // Easier to test for particular control codes if we need to do so - more uniform.
    std::string m_value;
};


// TODO move to own file
std::string language_name(uint8_t language_id);
std::string language_iso(uint8_t language_id);
uint8_t     language_id(const std::string& iso);


