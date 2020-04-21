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


// Action 3 assigns graphics sets (referenced to by (chains of) action 2(s) 
// to feature IDs (vehicles, stations, houses, industries, ...). 
class Action03Record : public ActionRecord
{
public:
    Action03Record()
    : ActionRecord{RecordType::ACTION_03}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;  
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    void parse_cargo_types(TokenStream& is);
    
private:
    struct CargoType
    {
        // If defined, cargo-type FF is used for graphics shown in the purchase 
        // or construction window. For stations, you can additionally define a 
        // special cargo-type of FE which prevents the default from being used 
        // (which would show the sum of all cargo).  Instead, the given set-ID is 
        // displayed with no cargo at all. 
        uint8_t  cargo_type   = 0;
        uint16_t act02_set_id = 0;
    };

private:
    // The type of feature for which we are making an association 
    // of graphics with properties.
    FeatureType m_feature = FeatureType::Trains;

    // This is the ID of the graphics set to use for the feature (Action02).
    uint16_t m_default_act02_set_id = 0;

    // These are the IDs of the feature instances (Action00) for which to 
    // use these graphics. 
    std::vector<uint16_t> m_feature_ids;

    // These are cargo-specific alternatives for the graphics - only vehicels
    // and stations. The cargo_type is an index into the global cargo translation
    // table, or something like that.
    std::vector<CargoType> m_cargo_types;

    // This set for wagons whose graphics can change when used with 
    //particular engines.
    bool m_livery_override = false;
};
