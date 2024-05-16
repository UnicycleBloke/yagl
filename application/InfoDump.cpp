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


class TypeRegistry
{
public:
    TypeRegistry();
    void print_info(const std::string& item);

private:
    std::map<std::string, void(*)()> m_types;
};


TypeRegistry::TypeRegistry()
{
    // Features
    m_types["Trains"]         = print_type_info<Action00Trains>;
    m_types["RoadVehicles"]   = print_type_info<Action00RoadVehicles>;
    m_types["Ships"]          = print_type_info<Action00Ships>;
    m_types["Aircraft"]       = print_type_info<Action00Aircraft>;
    m_types["Stations"]       = print_type_info<Action00Stations>;
    m_types["Canals"]         = print_type_info<Action00Canals>;
    m_types["Bridges"]        = print_type_info<Action00Bridges>;
    m_types["Houses"]         = print_type_info<Action00Houses>;
    m_types["GlobalSettings"] = print_type_info<Action00GlobalSettings>;
    m_types["IndustryTiles"]  = print_type_info<Action00IndustryTiles>;
    m_types["Industries"]     = print_type_info<Action00Industries>;
    m_types["Cargos"]         = print_type_info<Action00Cargos>;
    m_types["SoundEffects"]   = print_type_info<Action00SoundEffects>;
    m_types["Airports"]       = print_type_info<Action00Airports>;
    m_types["Objects"]        = print_type_info<Action00Objects>;
    m_types["RailTypes"]      = print_type_info<Action00RailTypes>;
    m_types["AirportTiles"]   = print_type_info<Action00AirportTiles>;
    m_types["RoadTypes"]      = print_type_info<Action00RoadTypes>;
    m_types["TramTypes"]      = print_type_info<Action00TramTypes>;
    m_types["RoadStops"]      = print_type_info<Action00RoadStops>;
}


void TypeRegistry::print_info(const std::string& item)
{
    if (item == "list")
    {
        for (const auto& [key, value]: m_types)
        {
            std::cout << key << "\n";
        }            
    }
    else
    {
        auto it = m_types.find(item);
        if (it != m_types.end())
        {
            it->second();
        }
        else
        {
            std::cout << "Unknown item: '" << item << "'\n";
        }
    }
}


void info_dump()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try
    {  
        auto item = options.info_item();
        //std::transform(item.begin(), item.end(), item.begin(), [](unsigned char c){ return std::tolower(c); });

        TypeRegistry registry;
        registry.print_info(item);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


