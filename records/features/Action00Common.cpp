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
#include "Action00Common.h"
#include "Descriptors.h"


namespace {


constexpr const char* str_introduction_date       = "introduction_date";
constexpr const char* str_reliability_decay_speed = "reliability_decay_speed";
constexpr const char* str_vehicle_life_years      = "vehicle_life_years";
constexpr const char* str_model_life_years        = "model_life_years";
constexpr const char* str_climate_availability    = "climate_availability";
constexpr const char* str_loading_speed           = "loading_speed";


// Properties are only 8 bits. Pad to 16 bits to allow sub-properties to be 
// split out and not ambiguous for the parser. Not all features need this, but
// it's simpler to be consistent.
const std::map<std::string, uint16_t> g_indices =
{
    { str_introduction_date,       0x00'00 },
    { str_reliability_decay_speed, 0x02'00 },
    { str_vehicle_life_years,      0x03'00 },
    { str_model_life_years,        0x04'00 },
    { str_climate_availability,    0x06'00 },
    { str_loading_speed,           0x07'00 },
};


constexpr IntegerDescriptorT<uint16_t> desc_00 = { 0x00, str_introduction_date,       PropFormat::Hex };
constexpr IntegerDescriptorT<uint8_t>  desc_02 = { 0x02, str_reliability_decay_speed, PropFormat::Dec };
constexpr IntegerDescriptorT<uint8_t>  desc_03 = { 0x03, str_vehicle_life_years,      PropFormat::Dec };
constexpr IntegerDescriptorT<uint8_t>  desc_04 = { 0x04, str_model_life_years,        PropFormat::Dec };
const     BitfieldDescriptorT<uint8_t> desc_06 = { 0x06, str_climate_availability,    
    { {0, "Temperate"}, {1, "Arctic"}, {2, "Tropical"}, {3, "Toyland"} } };
constexpr IntegerDescriptorT<uint8_t>  desc_07 = { 0x07, str_loading_speed,           PropFormat::Hex };


} // namespace {


bool Action00Common::read_property(std::istream& is, uint8_t property)
{
    switch (property)
    {
        case 0x00: m_00_introduction_date       = read_uint16(is); break;
        case 0x02: m_02_reliability_decay_speed = read_uint8(is);  break;
        case 0x03: m_03_vehicle_life_years      = read_uint8(is);  break;
        case 0x04: m_04_model_life_years        = read_uint8(is);  break;
        case 0x06: m_06_climate_availability    = read_uint8(is);  break;
        case 0x07: m_07_loading_speed           = read_uint8(is);  break;
        default:   return false;
    }

    return true; 
}   

    
bool Action00Common::write_property(std::ostream& os, uint8_t property) const
{
    switch (property)
    {
        case 0x00: write_uint16(os, m_00_introduction_date); break;
        case 0x02: write_uint8(os, m_02_reliability_decay_speed); break;
        case 0x03: write_uint8(os, m_03_vehicle_life_years); break;
        case 0x04: write_uint8(os, m_04_model_life_years); break;
        case 0x06: write_uint8(os, m_06_climate_availability); break;
        case 0x07: write_uint8(os, m_07_loading_speed); break;
        default:   return false;
    }

    return true; 
}


bool Action00Common::print_property(std::ostream& os, uint8_t property, uint16_t indent) const
{
    switch (property)
    {
        case 0x00: desc_00.print(m_00_introduction_date, os, indent); break;
        case 0x02: desc_02.print(m_02_reliability_decay_speed, os, indent); break;
        case 0x03: desc_03.print(m_03_vehicle_life_years, os, indent); break;
        case 0x04: desc_04.print(m_04_model_life_years, os, indent); break;
        case 0x06: desc_06.print(m_06_climate_availability, os, indent); break;
        case 0x07: desc_07.print(m_07_loading_speed, os, indent); break;
        default:   return false;
    }    

    os << "\n";
    return true;
}


bool Action00Common::parse_property(TokenStream& is, const std::string& name, uint8_t& property)
{
    const auto& it = g_indices.find(name);
    if (it != g_indices.end())
    {
        uint16_t index = it->second;
        property = (index >> 8); // The property index is in the high byte.
        switch (index)
        {
            case 0x00'00: desc_00.parse(m_00_introduction_date, is); break;
            case 0x02'00: desc_02.parse(m_02_reliability_decay_speed, is); break;
            case 0x03'00: desc_03.parse(m_03_vehicle_life_years, is); break;
            case 0x04'00: desc_04.parse(m_04_model_life_years, is); break;
            case 0x06'00: desc_06.parse(m_06_climate_availability, is); break;
            case 0x07'00: desc_07.parse(m_07_loading_speed, is); break;
            default:      throw RUNTIME_ERROR("Unknown property");
        }
    
        return true;
    }

    // No exception here as the property might be parsed by the derived vehicle type. 
    return false;
}

