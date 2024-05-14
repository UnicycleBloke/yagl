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
#include "Action00Feature.h"


bool Action00Feature::read_property(std::istream& is, uint8_t property)
{
    if (m_properties.read_property(is, property))
    {
        return true;
    }
    else
    {
        throw PROPERTY_ERROR("Unknown property", feature(), property);
    }
    return false;
}


bool Action00Feature::write_property(std::ostream& os, uint8_t property) const
{
    if (m_properties.write_property(os, property))
    {
        return true;
    }
    else
    {
        throw PROPERTY_ERROR("Unknown property", feature(), property);
    }
    return false;
}


bool Action00Feature::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    if (m_properties.print_property(os, property, indent))
    {
        return true;
    }
    else
    {
        throw PROPERTY_ERROR("Unknown property", feature(), property);
    }
    return false;
}


bool Action00Feature::parse_property(TokenStream& is, const std::string& label, uint8_t& property)
{
    if (m_properties.parse_property(is, label, property))
    {
        return true;
    }
    else
    {
        throw PROPERTY_ERROR("Unknown property" + label, feature(), 0x00);
    }
    return false;
}


void Action00Feature::print_info()
{
    m_properties.print_info();
}

