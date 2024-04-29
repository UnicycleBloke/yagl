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
#include "Action00Feature.h"
#include "TokenStream.h"
#include <string>
#include <cstdint>
#include <iostream>


// This is a common base class for all Action00 properties. The Action00 for each
// FeatureType is basically a map from the property index to the property object.
// The property object can read, write, print and parse itself.
class Action00Property
{
public:
    Action00Property(Action00Feature& container, uint8_t index, const std::string& label)
    : m_index{index}, m_label{label} 
    {
        container.register_property(this);
    }

    uint8_t index() const { return m_index; } 
    const std::string& label() const { return m_label; }

    // Each type of property has the same API, from a simple integer to a complex sprite layout.
    virtual void read(std::istream& is) = 0;
    virtual void write(std::ostream& os) const = 0;
    virtual void print(std::ostream& os, uint16_t indent) const = 0;
    virtual void parse(TokenStream& is) = 0;

private:   
    uint8_t m_index;
    std::string m_label;
};