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


// Action 6 allows modifying the contents of the following sprite. It uses the values of
// the grf parameters and writes them into the data of the following sprite.
//
// This seems like a slightly mad thing to do in a mod file - very low level. Appears to be
// to set the feature instance IDs if these are reserved at runtime, and that sort of thing.
class Action06Record : public ActionRecord
{
public:
    Action06Record()
    : ActionRecord{RecordType::ACTION_06}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    struct Modification
    {
        uint8_t  param_num;  // One of the user settable parameters - can be set with Action0D, for example.
        uint8_t  param_size; // Can be bigger than 4 - uses the value of the next param(s).
        bool     add_bytes;  // Add the parameter value to the target rather than replace it.
        uint16_t offset;     // Offset into the next sprite - this is where the modification is made.
    };

    std::vector<Modification> m_modifications;
};
