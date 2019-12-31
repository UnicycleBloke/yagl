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


// This is a container for binaries - WAV files. It can also contain non-binaries which refer to 
// binaries in this or other GRFs. Binaries are captured in ActionFF. Binary references in ActionFE.
class Action11Record : public ContainerRecord
{
public:
    Action11Record()
    : ContainerRecord{RecordType::ACTION_11}
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
    uint16_t num_sprites_to_read() const override { return m_num_binaries; }

    // This is called to write out binary files for the sound effects. Equivalent to sprite
    // sheets for images, but a lot simpler.
    void write_binary_files(const std::map<uint32_t, SpriteZoomVector>& sprites, const std::string& binary_dir) const;

private:
    uint16_t m_num_binaries;
};
