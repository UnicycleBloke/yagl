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
#include <map>
#include <iostream>


// Base class for all the different types of features.
// Each one has its own distinct set of properties.
class Action00Feature
{
public:
    Action00Feature() {}
    virtual ~Action00Feature() {}

    // These methods only return bool so we can know if the base class for vehicles, ships, etc.
    // handled the property or not. We don't care about the result - an exception will be thrown
    // in cases where this was not so.
    // Binary serialisation
    virtual bool read_property(std::istream& is, uint8_t property) = 0;
    virtual bool write_property(std::ostream& os, uint8_t property) const = 0;
    // Text serialisation
    virtual bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const { return false; }
    virtual bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) = 0;
};
