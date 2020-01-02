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
#include "Action02RandomRecord.h"
#include "StreamHelpers.h"


void Action02RandomRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature = static_cast<FeatureType>(read_uint8(is));
    m_set_id  = read_uint8(is);
    
    m_type = static_cast<RandomType>(read_uint8(is));
    if (m_type == RandomType::Consist)
    {
        // For type 84, the count byte specifies which vehicle's random bits this vehicle 
        // will be using and/or modifying.
        // - The low nibble (bits 0-3) specifies how far to count from the starting vehicle. 
        //   If it is zero, the count will be read from GRF register 100h instead.
        // - The high nibble (bits 6-7, actually) specifies which vehicle is the starting 
        //   vehicle, and which direction to count.     
        m_count   = read_uint8(is);
        m_method  = static_cast<ConsistType>(m_count & 0xC0);
        m_count  &= 0x0F;
    }
    
    m_triggers = read_uint8(is);
    m_randbit  = read_uint8(is);

    // This must be a power of two.
    uint8_t nrand = read_uint8(is);
    m_set_ids.resize(nrand);
    for (uint8_t i = 0; i < nrand; ++i)
    {
        m_set_ids[i] = read_uint16(is);
    }
}


void Action02RandomRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_set_id);
    
    write_uint8(os, static_cast<uint8_t>(m_type));
    if (m_type == RandomType::Consist)
    {
        write_uint8(os, m_count | static_cast<uint8_t>(m_method));
    }

    write_uint8(os, m_triggers);
    write_uint8(os, m_randbit);

    uint8_t nrand = m_set_ids.size();
    write_uint8(os, nrand);

    for (uint8_t i = 0; i < nrand; ++i)
    {
        write_uint16(os, m_set_ids[i]);
    }
}  


// random_switch<Trains, 0xFD> // Action02 random
// {
//     random_type: Object;
//     triggers: 0x00;
//     rand_bit: 0x00;
//     set_ids:
//     [
//         0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 
//         0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00F5 0x00FA 0x00FA 
//     ];
// }


void Action02RandomRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_set_id) << "> // Action02 random\n";
    os << pad(indent) << "{\n";

    os << pad(indent + 4) << "random_type: ";
    switch (m_type)
    {
        case RandomType::Object:  os << "Object";  break;
        case RandomType::Related: os << "Related"; break;
        case RandomType::Consist: 
            os << "Consist[" << to_hex(m_count) << ", ";
            switch (m_method)
            {
                case ConsistType::BackwardFromVehicle: os << "BackwardFromVehicle"; break;
                case ConsistType::ForewardFromVehicle: os << "ForewardFromVehicle"; break;
                case ConsistType::BackwardFromEngine:  os << "BackwardFromEngine";  break;
                case ConsistType::BackwardFromSameID:  os << "BackwardFromSameID";  break;
            }
            os << "]";
            break;
    }
    os << ";\n";

    os << pad(indent + 4) << "triggers: " << to_hex(m_triggers) << ";\n";
    os << pad(indent + 4) << "rand_bit: " << to_hex(m_randbit) << ";\n";

    std::map<uint16_t, uint16_t> values;
    // Assert size of m_set_ids is a power of two.
    for (auto id: m_set_ids)
    {
        if (values.find(id) == values.end())
        {
            values[id] = 1;
        }
        else
        {
            values[id] = values[id] + 1;
        }        
    }

    os << pad(indent + 4) << "set_ids: // set_id: probability;\n";
    os << pad(indent + 4) << "{\n";
    for (const auto& it: values)
    {
        os << pad(indent + 8) << to_hex(it.first) << ": " << it.second << ";\n";
    }
    os << pad(indent + 4) << "};\n";

    os << pad(indent) << "}" << '\n';
}


void Action02RandomRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action02RandomRecord::parse not implemented");
}
