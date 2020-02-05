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
#include <iostream>
#include <memory>
#include <vector>
#include <map>


// This is used to append a sprite to the current NewGRFData::m_sprites during parsing.
void append_real_sprite(uint32_t sprite_id, std::shared_ptr<Record> sprite);


class NewGRFData
{
public:
    NewGRFData();

    // Binary serialisation
    void read(std::istream& is);
    void write(std::ostream& os) const;
    // Text serialisation
    void print(std::ostream& os, const std::string& output_dir, const std::string& image_file_base) const;
    void parse(TokenStream& is, const std::string& output_dir, const std::string& image_file_base);

    // Primarily for testing - comparing two GRFs at the binary level, record by record.
    // Dump the records as hex, but break lines between records so that diff tools can recover after diffs.
    void hexdump();

private:    
    // Helpers for reading a GRF binary file 
    GRFFormat               read_format(std::istream& is);
    std::shared_ptr<Record> read_record(std::istream& is, uint32_t size, bool top_level, const GRFInfo& info);
    void                    read_sprite(std::istream& is, uint32_t sprite_id, uint32_t size, uint8_t compression, const GRFInfo& info);
    std::shared_ptr<Record> make_record(RecordType record_type);

    friend void append_real_sprite(uint32_t sprite_id, std::shared_ptr<Record> sprite);
    void append_sprite(uint32_t sprite_id, std::shared_ptr<Record> sprite);
    void update_version_info(std::shared_ptr<Record> record);

    // Helpers for writing a GRF binary file
    void write_format(std::ostream& os, uint32_t sprite_offs = 0) const;
    void write_counter(std::ostream& os) const;
    void write_record(std::ostream& os, std::shared_ptr<Record> record) const;
    uint32_t total_records() const;

private:
    GRFInfo m_info;

    // Simple list of all records in the data section. 
    // Should be consistent between Format1 and Format2, so manufacture sprite references 
    // when reading Format1 (sprites are in the data section), and place the actual sprites 
    // into the map below. Could then be saved as Format2 if so desired.
    std::vector<std::shared_ptr<Record>> m_records;            
    
    // Map the sprite ID to images. There are possibly multiple images for each ID for
    // various zoom levels. The same zoom level may appear more than once (at least, maybe
    // an 8bpp and a 32bpp image for normal zoom). Perhaps these are conditionally selected.
    // Some images appear to have RGB + A + P.
    std::map<uint32_t, SpriteZoomVector> m_sprites;            
};

