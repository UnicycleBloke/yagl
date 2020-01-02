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
#include "GRFLabelListDescriptor.h"
#include "StreamHelpers.h"


void GRFLabelList::read(std::istream& is)
{
    uint8_t num_labels = read_uint8(is);
    for (uint8_t i = 0; i < num_labels; ++i)
    {
        GRFLabel label;
        label.read(is);
        m_labels.push_back(label);
    }
}


void GRFLabelList::write(std::ostream& os) const
{
    write_uint8(os, m_labels.size());
    for (auto label: m_labels)
    {
        label.write(os);
    }
}


void GRFLabelList::print(std::ostream& os) const
{
    os << "[";
    for (auto label: m_labels)
    {
        os << " \"" << label.to_string() << "\""; 
    }
    os << " ];\n";
}


void GRFLabelList::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        GRFLabel label;
        label.parse(is);
        m_labels.push_back(label);
    }
    is.match(TokenType::CloseBracket);
}


