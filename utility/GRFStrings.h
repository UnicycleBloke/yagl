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
#include <string>
#include "Languages.h"
#include "Record.h"
#include "DescriptorBase.h"


//std::string grf_string_to_readable_utf8(const std::string& str);

enum class StringTerm { None, Null };


// Replace std::string with this in all the records and other structures.
class GRFString
{
public:
    // Binary serialisation
    // Directly read or write m_value.
    void read(std::istream& is, StringTerm term = StringTerm::Null);
    void write(std::ostream& os, StringTerm term = StringTerm::Null) const;

    // Text serialisation
    // Convert the internal representation to a human readable version.
    void print(std::ostream& os) const;
    std::string readable() const;
    // Convert the value that is read into the internal representation.
    // Determine whether or not the internal version needs to be UTF8.
    void parse(TokenStream& is);

    uint32_t length() const { return uint32_t(m_value.length()); }

private:
    // This is the value as read from or written to a binary GRF file.
    // When reading from or writing to a YAGL file, some conversion is
    // necessary.
    std::string m_value;
};


std::string read_string(std::istream& is);
void write_string(std::ostream& os, const std::string& value, StringTerm term);
void write_string(std::ostream& os, const std::string& value);
std::string grf_string_to_readable_utf8(const std::string& value);


using GRFStringDescriptor = GenericDescriptor<GRFString>;


