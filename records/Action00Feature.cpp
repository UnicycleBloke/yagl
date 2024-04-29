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
#include "Action00Property.h"


void Action00Feature::register_property(Action00Property* property)
{
    auto index = property->index();
    m_properties_by_index[index] = property;

    const auto& label = property->label();
    m_properties_by_label[label] = property;
}


bool Action00Feature::read_property(std::istream& is, uint8_t property)
{
    auto it = m_properties_by_index.find(property);
    if (it != m_properties_by_index.end())
    {
        Action00Property* prop = it->second;
        prop->read(is);
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
    auto it = m_properties_by_index.find(property);
    if (it != m_properties_by_index.end())
    {
        Action00Property* prop = it->second;
        prop->write(os);
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
    auto it = m_properties_by_index.find(property);
    if (it != m_properties_by_index.end())
    {
        Action00Property* prop = it->second;
        prop->print(os, indent);
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
    auto it = m_properties_by_label.find(label);
    if (it != m_properties_by_label.end())
    {
        Action00Property* prop = it->second;
        property = prop->index();
        prop->parse(is);
        return true;
    }
    else
    {
        throw PROPERTY_ERROR("Unknown property" + label, feature(), 0x00);
    }
    return false;
}
