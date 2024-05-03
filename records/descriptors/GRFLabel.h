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
#include <cstdint>
#include <iostream>
#include <string>
#include "TokenStream.h"
#include "DescriptorBase.h"


class GRFLabel
{
public:
    // This is uses non-default in only one place - Action07Record::print().
    explicit GRFLabel(uint32_t label = 0) : m_label{label} {}

    // Binary serialisation
    void read(std::istream& is);
    void write(std::ostream& os) const;
    // Text serialisation
    void print(std::ostream& os, uint16_t indent = 0) const; // Quoted version of to_string(). Added indent for consistent interface with properties.
    void parse(TokenStream& is);

    // Not used in many places - phase out.
    std::string to_string() const;        // Not quoted: ABCD or ABC\x01
    // Used in only one place - phase out.
    std::string to_string_or_hex() const; // If any character is not printable.

    uint32_t value() const { return m_label; }

private:
    uint32_t m_label{};
};


using GRFLabelPair           = ArrayOld<GRFLabel, 2>;
using GRFLabelList           = VectorOld<GRFLabel>;

using GRFLabelDescriptor     = GenericDescriptor<GRFLabel>;
using GRFLabelPairDescriptor = GenericDescriptor<GRFLabelPair>;
using GRFLabelListDescriptor = GenericDescriptor<GRFLabelList>;

