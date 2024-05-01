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
#include "ValueType.h"
#include "TokenStream.h"
#include "StreamHelpers.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <map>


// Defined below.
class PropertyBase;


// This maps a set of properties indices/labels to corresponding Property
// objects which know how to read/write/print/parse the corresponding property
// value. This is mainly useful for implementing Action00 for the various 
// FeatureTypes, but could be useful in other places.
class PropertyMap
{
public:
    void register_property(PropertyBase* property);

    bool read_property(std::istream& is, uint8_t property);
    bool write_property(std::ostream& os, uint8_t property) const;
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index);

private:
    std::map<uint8_t, PropertyBase*>     m_properties_by_index; 
    std::map<std::string, PropertyBase*> m_properties_by_label; 
};


// This is a common base class for all Action00 properties. The Action00 for each
// FeatureType is basically a map from the property index to the property object.
// The property object can read, write, print and parse itself.
class PropertyBase
{
public:
    PropertyBase(PropertyMap& container, uint8_t index, const std::string& label)
    : m_index{index}, m_label{label} 
    {
        container.register_property(this);
    }

    uint8_t index() const            { return m_index; } 
    const std::string& label() const { return m_label; }

    // Each type of property has the same API, from a simple integer 
    // to a complex sprite layout.
    virtual void read(std::istream& is) = 0;
    virtual void write(std::ostream& os) const = 0;
    virtual void print(std::ostream& os, uint16_t indent) const = 0;
    virtual void parse(TokenStream& is) = 0;

private:   
    uint8_t     m_index;
    std::string m_label;
};


// Properties are the main feature of Action00 records. In the NewGRF file, each 
// property has an index (a uint8_t) and an associated data type to hold a value of 
// that property. In the YAGL, each property has a corresponding label and an 
// associated text format to hold a value of that property. The data type for the 
// property could be as simple as a byte (many are), but could be a complicated 
// variable length structure (e.g. station sprite layouts).
//
// This template associates an index, a label and a data type, which together define 
// a property. The common base class deals with the index and label, and allows for 
// a polymorphic approach to dealing with a set of properties.
template <typename T>
//requires ValueType<T>
class Property : public PropertyBase
{
public:
    Property(PropertyMap& container, uint8_t index, const std::string& label)
    : PropertyBase(container, index, label) 
    {
    }

    void read(std::istream& is) override
    {        
        m_value.read(is);
    }

    void write(std::ostream& os) const override
    {        
        m_value.write(os);
    }

    void print(std::ostream& os, uint16_t indent) const override
    { 
        os << pad(indent) << label() << ": ";  
        m_value.print(os, indent);
    }

    void parse(TokenStream& is) override
    {   
        m_value.parse(is);     
    }

private:
    T m_value{};
};
