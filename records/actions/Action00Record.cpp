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
#include "Action00Record.h"
#include "StreamHelpers.h"
#include "Action00Trains.h"
#include "Action00Vehicles.h"
#include "Action00Ships.h"
#include "Action00Aircraft.h"
#include "Action00Stations.h"
#include "Action00Canals.h"
#include "Action00Bridges.h"
#include "Action00Houses.h"
#include "Action00GlobalSettings.h"
#include "Action00IndustryTiles.h"
#include "Action00Industries.h"
#include "Action00Cargos.h"
#include "Action00SoundEffects.h"
#include "Action00Airports.h"
#include "Action00Signals.h"
#include "Action00Objects.h"
#include "Action00RailTypes.h"
#include "Action00AirportTiles.h"
#include "Action00RoadTypes.h"
#include "Action00TramTypes.h"
#include "Action00OriginalStrings.h" 


void Action00Record::read(std::istream& is, const GRFInfo& info)
{
    // We probably already know this from peeking - get from traits?
    m_feature = static_cast<FeatureType>(read_uint8(is));

    // The record define N properties for M feature instances. The ID
    // of the first instance is given here.
    uint8_t num_props = read_uint8(is);
    uint8_t num_info  = read_uint8(is);
    m_first_id        = read_uint8_ext(is);

    // Create the feature instances first, due to the interesting read order.
    // The data is stored by property, with the values for all feature instances together.
    // Seems a littel odd, but not a problem.
    for (uint8_t i = 0; i < num_info; ++i)
    {
        m_instances.push_back(make_feature(m_feature));
    }

    // For each property, get each feature instances to read its copy of the property.
    for (uint8_t p = 0; p < num_props; ++p)
    {
        // The property index is always a uint8_t. It is interpreted differently 
        // depending on the particular feature we are dealing with.
        uint8_t property = read_uint8(is);
        m_properties_used[property] = true;

        // TODO remove - Test code 
        m_properties.push_back(property);

        for (uint8_t i = 0; i < num_info; ++i)
        {
            m_instances[i]->read_property(is, property);
        }
    }
}


void Action00Record::write(std::ostream& os, const GRFInfo& info) const
{
    // Make a list of the properties which have been set.
    std::vector<uint8_t> properties;
    //std::map<uint8_t, bool>::const_iterator it;
    for (const auto& it: m_properties_used)
    {
        if (it.second)
        {
            properties.push_back(it.first);
        }
    }
    //uint8_t num_props = properties.size();
    uint8_t num_props = m_properties.size();
    uint8_t num_info  = m_instances.size();    

    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, num_props);
    write_uint8(os, num_info);
    write_uint8_ext(os, m_first_id);

    // For each property, get each item instances to read its copy of the property.
    for (const auto& property: m_properties)
    {
        write_uint8(os, property);

        for (const auto& instance: m_instances)
        {
            instance->write_property(os, property);
        }
    }
}  


void Action00Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action00" << '\n';
    os << pad(indent) << "{\n";

    uint16_t id = m_first_id;
    for (auto instance: m_instances)
    {
        os << pad(indent + 4) << "instance_id: " << to_hex(id++, true) << "\n";  
        os << pad(indent + 4) << "{\n";  

        for (auto [property, used]: m_properties_used) 
        {
            if (used)
            {
                bool result = instance->print_property(os, property, indent + 8);
                if (!result)
                {
                    os << pad(indent + 8) << "TODO <unknown> property #" << to_hex(property, true) << '\n';
                }
            }
        }

        os << pad(indent + 4) << "}\n";  
    }

    os << pad(indent) << "}\n";
}


std::shared_ptr<Action00Feature> Action00Record::make_feature(FeatureType feature)
{
    switch (feature)
    {
        case FeatureType::Trains:          return std::make_shared<Action00Trains>();
        case FeatureType::Vehicles:        return std::make_shared<Action00Vehicles>();
        case FeatureType::Ships:           return std::make_shared<Action00Ships>();
        case FeatureType::Aircraft:        return std::make_shared<Action00Aircraft>();
        case FeatureType::Stations:        return std::make_shared<Action00Stations>();
        case FeatureType::Canals:          return std::make_shared<Action00Canals>();
        case FeatureType::Bridges:         return std::make_shared<Action00Bridges>();
        case FeatureType::Houses:          return std::make_shared<Action00Houses>();
        case FeatureType::GlobalSettings:  return std::make_shared<Action00GlobalSettings>();
        case FeatureType::IndustryTiles:   return std::make_shared<Action00IndustryTiles>();
        case FeatureType::Industries:      return std::make_shared<Action00Industries>();
        case FeatureType::Cargos:          return std::make_shared<Action00Cargos>();
        case FeatureType::SoundEffects:    return std::make_shared<Action00SoundEffects>();
        case FeatureType::Airports:        return std::make_shared<Action00Airports>();
        case FeatureType::Signals:         return std::make_shared<Action00Signals>();
        case FeatureType::Objects:         return std::make_shared<Action00Objects>();
        case FeatureType::Railtypes:       return std::make_shared<Action00RailTypes>();
        case FeatureType::AirportTiles:    return std::make_shared<Action00AirportTiles>();
        case FeatureType::RoadTypes:       return std::make_shared<Action00RoadTypes>();
        case FeatureType::TramTypes:       return std::make_shared<Action00TramTypes>();
        case FeatureType::OriginalStrings: return std::make_shared<Action00OriginalStrings>();
    }

    throw RUNTIME_ERROR("Action00Record::make_feature");
}


// properties<Trains> // Action00
// {
//     instance_id: 0x009C
//     {
//         reliability_decay_speed: 20;
//         vehicle_life_years: 75;
//         model_life_years: 158;
//         track_type: Rail;
//         climate_availability: Temperate | Arctic | Tropical | Toyland;
//         loading_speed: 0x00;


void Action00Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);

    while (is.peek().value == "instance_id")
    {
        is.match(TokenType::Ident);
        is.match(TokenType::Colon);
        m_first_id = is.match_integer();
        // TODO ignore second and subsequent items.

        is.match(TokenType::OpenBrace);

        // Parse the set of properties that have been given for this item.
        std::shared_ptr<Action00Feature> instance = make_feature(m_feature);
        m_instances.push_back(instance);
 
        while (is.peek().type != TokenType::CloseBrace)
        {
            std::string name = is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            // The following token(s) represent the value of the property.
            // The numeric index of the property is determined during the call
            // to parse_property().
            uint8_t property;
            instance->parse_property(is, name, property);
            m_properties_used[property] = true;

            is.match(TokenType::SemiColon);
        }

        is.match(TokenType::CloseBrace);
    }

    is.match(TokenType::CloseBrace);
}

