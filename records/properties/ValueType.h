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
#include "TokenStream.h"
#include <iostream>
#include <cstdint>


// This concept is used to enforce the required API for data types which 
// can be used as properties. They must be capable of:
// - reading themselves from a stream of binary bytes (the NewGRF file)
// - writing themselves to a stream of binary bytes
// - printing themselves to a stream of text (the YAGL file)
// - parsing themselves from a stream of text
// Reading and writing are the inverse of each other.
// Printing and parsing are the inverse of each other. 
// The binary form and the textual form are semantically equivalent.
// template <typename T>
// concept ValueType = requires (T& d, std::istream& is, std::ostream& os,
//     uint16_t indent, TokenStream& ts)
// {
//     d.read(is);
//     d.write(os);
//     d.print(os, indent);
//     d.parse(ts);
// };
