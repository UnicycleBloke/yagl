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


// Action 5 is used to define graphics for several new features 
// of OpenTTD and TTDPatch. This is similar to action 1. 
class Action05Record : public ContainerRecord
{
public:
    Action05Record()
    : ContainerRecord{RecordType::ACTION_05}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;    
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

    // This is the number of real sprites records (or references) we expect to 
    // follow immediately after this record in the file.
    uint16_t num_sprites_to_read() const override { return m_num_sprites; }

private:
    NewFeatureType m_sprite_type{};
    uint16_t       m_num_sprites{};
    uint16_t       m_offset{};
};
