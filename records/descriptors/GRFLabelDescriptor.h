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
#include "DescriptorBase.h"
#include "GRFLabel.h"
#include <array>


class GRFLabelPair
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os) const;
    void parse(TokenStream& is);

private:
    GRFLabel m_source;
    GRFLabel m_target;
};


class GRFLabelList
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os) const;
    void parse(TokenStream& is);

private:
    // This is a list of indices into the (presumably global) cargo table.
    std::vector<GRFLabel> m_labels;
};


using GRFLabelDescriptor     = GenericDescriptor<GRFLabel>;
using GRFLabelListDescriptor = GenericDescriptor<GRFLabelList>;
using GRFLabelPairDescriptor = GenericDescriptor<GRFLabelPair>;


