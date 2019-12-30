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
    void print(GRFLabel label, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        os << " \"" << label.to_string() << "\";"; 
    }

    void parse(GRFLabel& label, TokenStream& is) const
    {
        label.parse(is);
    }
};


struct GRFLabelListDescriptor : PropertyDescriptor
{
    void print(const GRFLabelList& labels, std::ostream& os, uint8_t indent) const
    {
        prefix(os, indent);
        labels.print(os);
    }

    void parse(GRFLabelList& labels, TokenStream& is) const
    {
        labels.parse(is);
    }
};


