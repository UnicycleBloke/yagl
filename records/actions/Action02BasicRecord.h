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


// Defining graphics set IDs.
//
// Action2 is used to group sets of sprites from the previous Action1 together, and
// make them accessible by a variational or random action 2 (chain) or an Action3.
class Action02BasicRecord : public ActionRecord
{
public:
    Action02BasicRecord()
    : ActionRecord{RecordType::ACTION_02_BASIC}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    // The type of feature to which this record relates: trains or whatever.
    FeatureType m_feature = FeatureType::Trains;

    // The index of this Action02 set. Only a byte is allowed, which seems confusing.
    // This is referenced later by Action02Variable or Action03 to create chains of
    // selection, and to associate graphics with feature instances.
    uint8_t m_act02_set_id = 0;

    // There are two sets of entries for some features because they use different
    // graphics depending on their state. For vehicles, for example, the states are
    // 'loaded' and 'loading'. The number of entries corresponds to the number of
    // loading states, e.g. 0%, 50%, 100%.

    // Vehicles: loadtypes, loadingtypes
    // Stations: littlesets, lotssets - piles of goods waiting?
    std::vector<uint16_t> m_act01_set_ids_1;
    std::vector<uint16_t> m_act01_set_ids_2;
};
