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
#include "GRFStrings.h"


// Deactivate other graphics files or force activation of current file.
//
// Normally, graphics files should use Action7 to deactivate themselves,
// if they find that other incompatible graphics have been loaded.
//
// However, when new sets come out, it is often not feasible to change
// ll existing sets so that they can detect the new set. This action
// therefore allows the new graphics set to deactivate older sets with
// which it is incompatible.
class Action0ERecord : public ActionRecord
{
public:
    Action0ERecord()
    : ActionRecord{RecordType::ACTION_0E}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    GRFLabelList m_grf_ids;
};
