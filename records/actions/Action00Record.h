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
#include "Action00Feature.h"


class Action00Record : public ActionRecord
{
public:
    Action00Record()
    : ActionRecord{RecordType::ACTION_00}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    std::unique_ptr<Action00Feature> make_feature(FeatureType feature_type);

private:
    FeatureType m_feature{};
    // The record may contain properties for one or more feature instances
    // (e.g. such as two trains). This is the ID of the first one.
    uint16_t m_first_id{};
    // This vector holds objects representing the one or more feature instances.
    std::vector<std::unique_ptr<Action00Feature>> m_instances;
    // Used to keep track of the order in which properties are read from the source.
    // This will allow for duplicates, but only the last value is preserved. A map
    // would eliminate duplicates, but also lose the ordering.
    std::vector<uint8_t> m_properties;
};
