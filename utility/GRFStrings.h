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
std::u16string grf_string_to_utf16(const std::string& str);
std::u16string grf_string_utf16_to_readable_utf16(const std::u16string& str);

std::string language_name(uint8_t language_id);
std::string language_iso(uint8_t language_id);
uint8_t     language_id(const std::string& iso);

std::string grf_id_to_string(uint32_t grf_id);


//uint32_t decode_number(const std::string value);


