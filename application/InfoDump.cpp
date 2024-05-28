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
#include "InfoDump.h"
#include "CommandLineOptions.h"
// Features
#include "FeatureType.h"
#include "Action00Trains.h"
#include "Action00RoadVehicles.h"
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
#include "Action00Objects.h"
#include "Action00RailTypes.h"
#include "Action00AirportTiles.h"
#include "Action00RoadTypes.h"
#include "Action00TramTypes.h"
#include "Action00RoadStops.h"


template <typename ItemType>
void print_type_info()
{
    ItemType item;
    // TODO need a caption/title. Arrange index, label, type and example in a table?
    item.print_info();
}


class FeatureRegistry
{
public:
    FeatureRegistry();
    void print_info(const std::string& item);

private:
    std::map<FeatureType, void(*)()> m_features;
};


FeatureRegistry::FeatureRegistry()
{
    // Features
    m_features[FeatureType::Trains]         = print_type_info<Action00Trains>;
    m_features[FeatureType::RoadVehicles]   = print_type_info<Action00RoadVehicles>;
    m_features[FeatureType::Ships]          = print_type_info<Action00Ships>;
    m_features[FeatureType::Aircraft]       = print_type_info<Action00Aircraft>;
    m_features[FeatureType::Stations]       = print_type_info<Action00Stations>;
    m_features[FeatureType::Canals]         = print_type_info<Action00Canals>;
    m_features[FeatureType::Bridges]        = print_type_info<Action00Bridges>;
    m_features[FeatureType::Houses]         = print_type_info<Action00Houses>;
    m_features[FeatureType::GlobalSettings] = print_type_info<Action00GlobalSettings>;
    m_features[FeatureType::IndustryTiles]  = print_type_info<Action00IndustryTiles>;
    m_features[FeatureType::Industries]     = print_type_info<Action00Industries>;
    m_features[FeatureType::Cargos]         = print_type_info<Action00Cargos>;
    m_features[FeatureType::SoundEffects]   = print_type_info<Action00SoundEffects>;
    m_features[FeatureType::Airports]       = print_type_info<Action00Airports>;
    m_features[FeatureType::Objects]        = print_type_info<Action00Objects>;
    m_features[FeatureType::RailTypes]      = print_type_info<Action00RailTypes>;
    m_features[FeatureType::AirportTiles]   = print_type_info<Action00AirportTiles>;
    m_features[FeatureType::RoadTypes]      = print_type_info<Action00RoadTypes>;
    m_features[FeatureType::TramTypes]      = print_type_info<Action00TramTypes>;
    m_features[FeatureType::RoadStops]      = print_type_info<Action00RoadStops>;
}


void FeatureRegistry::print_info(const std::string& item)
{
    if (item == "...")
    {
        std::cout << "Features for which properties can be listed:\n";
        for (const auto& [feature, value]: m_features)
        {
            std::cout << "    " << to_hex(feature) << "  " << FeatureName(feature) << "\n";
        }            
        std::cout << "For example, try 'yagl -i Feature:Trains'\n";
    }
    else
    {
        try 
        {
            auto feature = FeatureFromName(item);
            std::cout << "Properties for feature '" << item << "':\n";
            auto it = m_features.find(feature);
            if (it != m_features.end())
            {
                it->second();
            }
            else
            {
                std::cout << "No properties for feature: '" << item << "'\n";
            }
        }
        catch (...)
        {
            std::cout << "Unknown feature: '" << item << "'\n";
        }
    }
    std::cout << "\n";
}


std::vector<std::string> split(std::string source, std::string delim)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = source.find(delim);
    while (end != std::string::npos)
    {
        auto sub = source.substr(start, end - start);
        if (sub.size() > 0) result.push_back(sub);

        start = end + delim.length();
        end   = source.find(delim, start);
    }

    auto sub = source.substr(start, end - start);
    if (sub.size() > 0) result.push_back(sub);

    return result;
}


void info_dump()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try
    {  
        auto items = split(options.info_item(), ":");
        items.push_back("...");

        if (items[0] == "Feature")
        {
            FeatureRegistry registry;
            registry.print_info(items[1]);
        }
        // else if (items[0] == "Action")
        // {
        //     TypeRegistry registry;
        //     registry.print_info(items[1]);
        // }
        // else if (items[0] == "Enum")
        // {
        //     TypeRegistry registry;
        //     registry.print_info(items[1]);
        // }
        else
        {
            //std::cout << "Top-level items are: Feature, Action, Enum.\n";
            std::cout << "Top-level items are: Feature (for now). Try 'yagl -i Feature'\n";
        } 
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


