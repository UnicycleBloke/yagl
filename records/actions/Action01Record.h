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
#include <memory>


// Defining a set of real sprites. Action 01 is used to define sets of 
// real sprites (as opposed to pseudo-sprites). This record is a container
// for real sprites records. In Format2 files it is followed by references
// to real sprites - this allows the sprite IDs to be associated with 
// multiple images, which enables zoom alternatives.
class Action01Record : public ContainerRecord
{
public:
    Action01Record()
    : ContainerRecord{RecordType::ACTION_01}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

    // This is the number of real sprites records (or references) we expect to 
    // follow immediately after this record in the file.
    uint16_t num_sprites_to_read() const override { return m_num_sets * m_num_sprites; }

private:
    // The type of feature for which we are defining sprites sets.
    FeatureType m_feature = FeatureType::Trains;

    // Extended format lets us specify the ID of the first sprite set defined
    // in this record. This could permit overwriting only some sprite sets in later 
    // records. TODO not entirely sure how this numbering works or how this is 
    // then used in Action02 records. Do the Action01 records have to come immediately
    // before Action02? Are the sprite set IDs numbered from zero for each Action02?
    bool m_has_first_id = 0;

    // The number given to the first sprite set defined by this Action00. The others
    // are just incrementing values.
    uint16_t m_first_set = 0;
    
    // The number of sprites sets defined by the Action00.
    uint16_t m_num_sets = 0;

    // The number of sprites contained in each sprite set. These are all the same.
    uint16_t m_num_sprites = 0;

    //std::vector<std::vector<std::shared_ptr<Record>>> m_sprite_sets;
};
