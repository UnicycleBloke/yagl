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
#include <vector>
#include <string>


class Action0FRecord : public ActionRecord
{
public:
    Action0FRecord()
    : ActionRecord{RecordType::ACTION_0F}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

private:
    struct Name
    {
        uint8_t     lang_id;
        std::string name;
    };

private:
    struct Text
    {
        uint8_t     probability  = 1;
        bool        is_string    = true;
        std::string text         = "unknown";
        uint8_t     action_0F_id = 0;
    };

private:
    struct Part
    {
        uint8_t first_bit = 0;
        uint8_t num_bits  = 1;
        std::vector<Text> texts;
    };

private:
    // This ID is used as a reference by other chains.
    uint8_t           m_id;
    // These only exist for the final definition in a chain.
    std::vector<Name> m_style_names;
    std::vector<Part> m_style_parts;
};
