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


// This is used for GRF overrides in the global settings.
using GRFLabelPair = std::array<GRFLabel, 2>;


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


struct GRFLabelDescriptor : PropertyDescriptor
{
    void print(GRFLabel label, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        os << "\"" << label.to_string() << "\";\n"; 
    }

    void parse(GRFLabel& label, TokenStream& is) const
    {
        label.parse(is);
    }
};


struct GRFLabelPairDescriptor : PropertyDescriptor
{
    void print(const GRFLabelPair& labels, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        os << "[ ";
        os << "\"" << labels[0].to_string() << "\" "; 
        os << "\"" << labels[1].to_string() << "\" "; 
        os << "]; // [ <source> <target> ]\n";
    }

    void parse(GRFLabelPair& labels, TokenStream& is) const
    {
        is.match(TokenType::OpenBracket);
        labels[0].parse(is);
        labels[1].parse(is);
    }

    void read(GRFLabelPair& labels, std::istream& is) const
    {
        labels[0].read(is);
        labels[1].read(is);
    } 

    void write(const GRFLabelPair& labels, std::ostream& os) const
    {
        labels[0].write(os);
        labels[1].write(os);
    }
};


struct GRFLabelListDescriptor : PropertyDescriptor
{
    void print(const GRFLabelList& labels, std::ostream& os, uint16_t indent) const
    {
        prefix(os, indent);
        labels.print(os);
    }

    void parse(GRFLabelList& labels, TokenStream& is) const
    {
        labels.parse(is);
    }
};


