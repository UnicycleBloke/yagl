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
#include "Record.h"
#include "StreamHelpers.h"


void ActionRecord::write(std::ostream& os, const GRFInfo& info) const
{
    // The read and write methods for this class are unbalanced because 
    // the file reader needs to read ahead to work out what kind of 
    // record to create.

    // Convert the record type enum into an index for the record in the file.
    // This is complicated by the multiple types of Action02 records.
    uint8_t type = 0x00;
    switch (record_type())
    {
        case RecordType::ACTION_00:               type = 0x00; break;
        case RecordType::ACTION_01:               type = 0x01; break;
        case RecordType::ACTION_02_BASIC:         type = 0x02; break;
        case RecordType::ACTION_02_RANDOM:        type = 0x02; break;
        case RecordType::ACTION_02_VARIABLE:      type = 0x02; break;
        case RecordType::ACTION_02_INDUSTRY:      type = 0x02; break;
        case RecordType::ACTION_02_SPRITE_LAYOUT: type = 0x02; break;
        case RecordType::ACTION_03:               type = 0x03; break;
        case RecordType::ACTION_04:               type = 0x04; break;
        case RecordType::ACTION_05:               type = 0x05; break;
        case RecordType::ACTION_06:               type = 0x06; break;
        case RecordType::ACTION_07:               type = 0x07; break;
        case RecordType::ACTION_08:               type = 0x08; break;
        case RecordType::ACTION_09:               type = 0x09; break;
        case RecordType::ACTION_0A:               type = 0x0A; break;
        case RecordType::ACTION_0B:               type = 0x0B; break;
        case RecordType::ACTION_0C:               type = 0x0C; break;
        case RecordType::ACTION_0D:               type = 0x0D; break;
        case RecordType::ACTION_0E:               type = 0x0E; break;
        case RecordType::ACTION_0F:               type = 0x0F; break;
        case RecordType::ACTION_10:               type = 0x10; break;
        case RecordType::ACTION_11:               type = 0x11; break;
        case RecordType::ACTION_12:               type = 0x12; break;
        case RecordType::ACTION_13:               type = 0x13; break;
        case RecordType::ACTION_14:               type = 0x14; break;
        case RecordType::ACTION_FE:               type = 0xFE; break; 
        case RecordType::ACTION_FF:               type = 0xFF; break;
    }

    write_uint8(os, type);
}


void ContainerRecord::append_sprite(std::shared_ptr<Record> record)
{
    m_sprites.push_back(record);
}


void ContainerRecord::print_sprite(uint16_t index, std::ostream& os, 
    const SpriteZoomMap& sprites, uint16_t indent) const
{
    auto record = get_sprite(index);
    if (record != nullptr)
    {
        record->print(os, sprites, indent);
    }
    else
    {
        os << pad(indent) << "Missing record\n"; 
    }
}


// TODO Does it make sense to use NML-ish names - or just action01 etc?
const std::map<RecordType, std::string> g_record_names =
{
    { RecordType::RECORD_COUNT,            "record_count" },
    { RecordType::ACTION_00,               "properties" },
    { RecordType::ACTION_01,               "sprite_sets" },
    { RecordType::ACTION_02_BASIC,         "sprite_groups" },
    { RecordType::ACTION_02_RANDOM,        "random_switch" },
    { RecordType::ACTION_02_VARIABLE,      "switch" },
    { RecordType::ACTION_02_INDUSTRY,      "industry" },
    { RecordType::ACTION_02_SPRITE_LAYOUT, "sprite_layout" },
    { RecordType::ACTION_03,               "feature_graphics" },
    { RecordType::ACTION_04,               "strings" },
    { RecordType::ACTION_05,               "replace_sprites" },
    { RecordType::ACTION_06,               "modify_next" },
    { RecordType::ACTION_07,               "if" },
    { RecordType::ACTION_08,               "grf" },
    { RecordType::ACTION_09,               "if2" },
    { RecordType::ACTION_0A,               "replacement_sprite_sets" },
    { RecordType::ACTION_0B,               "error_message" },
    { RecordType::ACTION_0C,               "ignore" },
    { RecordType::ACTION_0D,               "set_parameter" },
    { RecordType::ACTION_0E,               "disable_grfs" },
    { RecordType::ACTION_0F,               "town_names" },
    { RecordType::ACTION_10,               "label" }, // Target for Action07/09
    { RecordType::ACTION_11,               "sound_effects" },
    { RecordType::ACTION_12,               "unicode_glyphs" },
    { RecordType::ACTION_13,               "grf_strings" },
    { RecordType::ACTION_14,               "optional_info" },
    { RecordType::ACTION_FE,               "binary_sound" }, // ??? Binary sound effect
    { RecordType::ACTION_FF,               "imported_sound" }, // ??? Imported sound effect
    { RecordType::SPRITE_INDEX,            "" }, // Always found inside a container
    { RecordType::REAL_SPRITE,             "" }, // Always found inside a sprite index
    { RecordType::FAKE_SPRITE,             "" }, // Is this a thing?
    { RecordType::RECOLOUR,                "" }, // Always found inside a container
};


std::string RecordName(RecordType type)
{
    const auto& it = g_record_names.find(type);
    if (it != g_record_names.end())
    {
        return it->second;
    }

    throw RUNTIME_ERROR("RecordName");
}


RecordType RecordFromName(const std::string& name)
{
    for (const auto& it: g_record_names)
    {
        if (it.second == name)
        {
            return it.first;
        }
    }

    throw RUNTIME_ERROR("RecordFromName");
}


const std::map<FeatureType, std::string> g_feature_names =
{
    { FeatureType::Trains,          "Trains" },
    { FeatureType::Vehicles,        "Vehicles" },
    { FeatureType::Ships,           "Ships" },
    { FeatureType::Aircraft,        "Aircraft" },
    { FeatureType::Stations,        "Stations" },
    { FeatureType::Canals,          "Canals" },
    { FeatureType::Bridges,         "Bridges" },
    { FeatureType::Houses,          "Houses" },
    { FeatureType::GlobalSettings,  "GlobalSettings" },
    { FeatureType::IndustryTiles,   "IndustryTiles" },
    { FeatureType::Industries,      "Industries" },
    { FeatureType::Cargos,          "Cargos" },
    { FeatureType::SoundEffects,    "SoundEffects" },
    { FeatureType::Airports,        "Airports" },
    { FeatureType::Signals,         "Signals" },
    { FeatureType::Objects,         "Objects" },
    { FeatureType::Railtypes,       "Railtypes" },
    { FeatureType::AirportTiles,    "AirportTiles" },
    { FeatureType::RoadTypes,       "RoadTypes" },
    { FeatureType::TramTypes,       "TramTypes" },
    { FeatureType::OriginalStrings, "OriginalStrings" },
};


std::string FeatureName(FeatureType type)
{
    const auto& it = g_feature_names.find(type);
    if (it != g_feature_names.end())
    {
        return it->second;
    }

    throw RUNTIME_ERROR("FeatureName");
}


FeatureType FeatureFromName(const std::string& name)
{
    for (const auto& it: g_feature_names)
    {
        if (it.second == name)
        {
            return it.first;
        }
    }

    throw RUNTIME_ERROR("FeatureFromName");
}


std::string NewFeatureName(NewFeatureType type)
{
    switch (type)
    {
        case NewFeatureType::PreSignal:        return "PreSignal";  
        case NewFeatureType::Catenary:         return "Catenary";  
        case NewFeatureType::Foundations:      return "Foundations";  
        case NewFeatureType::TTDPatchGUI:      return "TTDPatchGUI";  
        case NewFeatureType::Canals:           return "Canals";  
        case NewFeatureType::OneWayArrows:     return "OneWayArrows";  
        case NewFeatureType::TwoCompanyColour: return "TwoCompanyColour";  
        case NewFeatureType::TramTracks:       return "TramTracks";  
        case NewFeatureType::SnowyTrees:       return "SnowyTrees";  
        case NewFeatureType::CoastTiles:       return "CoastTiles";  
        case NewFeatureType::NewSignals:       return "NewSignals";  
        case NewFeatureType::SlopeTrackMarks:  return "SlopeTrackMarks";  
        case NewFeatureType::AirportExtra:     return "AirportExtra";  
        case NewFeatureType::RoadStops:        return "RoadStops";  
        case NewFeatureType::Aqueducts:        return "Aqueducts";   
        case NewFeatureType::AutoRail:         return "AutoRail";  
        case NewFeatureType::Flags:            return "Flags";  
        case NewFeatureType::OpenTTDGUI:       return "OpenTTDGUI";  
        case NewFeatureType::AirportPreview:   return "AirportPreview";  
        case NewFeatureType::RailTypeTunnel:   return "RailTypeTunnel";  
        case NewFeatureType::ExtraAllBlack:    return "ExtraAllBlack";  
    }

    throw RUNTIME_ERROR("NewFeatureName");
}
