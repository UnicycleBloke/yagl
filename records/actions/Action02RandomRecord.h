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
#include "Record.h"


class Action02RandomRecord : public ActionRecord
{
public:
    Action02RandomRecord()
    : ActionRecord{RecordType::ACTION_02_BASIC}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

private:
    // Use 80 to randomize the object (vehicle, station, building, industry, object) 
    //   based on its own triggers and bits.
    // Use 83 to randomize the object based on its "related" object (s.b.).
    // Use 84 to randomize the vehicle based on any vehicle in the consist. 
    enum class RandomType : uint8_t
    {
        Object  = 0x80,
        Related = 0x83,
        Consist = 0x84
    };

    enum class ConsistType : uint8_t
    {
        BackwardFromVehicle = 0x00,
        ForewardFromVehicle = 0x40,
        BackwardFromEngine  = 0x80,
        BackwardFromSameID  = 0xC0
    };

private:
    FeatureType           m_feature;
    uint8_t               m_set_id;
    RandomType            m_type;

    // Only present if type is RandomType::Consist 
    uint8_t               m_count;
    ConsistType           m_method;
    
    uint8_t               m_triggers;
    uint8_t               m_randbit;
    // Number of set IDs. Must be a power of 2 
    std::vector<uint16_t> m_set_ids;
};
