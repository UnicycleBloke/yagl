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
#include <memory>


// CSTM C
// INFO C
//     NAME  T                // Translatable GRF name
//     DESC  T                // Translatable GRF description
//     URL_  T                // URL for the GRF website
//     PALS  B 1 'D' 'W' 'A'  // Palette
//     BLTR  B 1 '8' '3'      // Colour depth
//     VRSN  B 4              // Version of the GRF
//     MINV  B 4              // Minimum compatible version of GRFs with same GRFID
//     NPAR  B 1              // Number of parameters - up to 128 uint32_ts
//     PARA  C
//         <setting> C                               // Setting index < NPAR
//             NAME T                                // Name of the setting
//             DESC T                                // Description of the setting
//             MASK B len param [firstbit [numbits]] // Parameter to set with optional mask
//             TYPE B 1   int0|bool1                 // Type of the parameter in the GUI
//             LIMI B 8   min max words              // Range for valid values
//             VALU C
//                 <value> T                         // Names for enumerators
//             DFLT B 4                              // Default value

class Action14Record : public ActionRecord
{
public:
    Action14Record()
    : ActionRecord{RecordType::ACTION_14}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    struct Chunk
    {
        GRFLabel label;
        uint8_t  type;

        // Only for strings
        uint8_t   language;
        GRFString text;

        // Only for binary blobs
        std::vector<uint8_t> data;

        // Only for containers
        std::vector<Chunk> chunks;

        //void read(std::istream& is);
    };

private:
    void read_chunks(std::istream& is, std::vector<Chunk>& chunks);
    void write_chunks(std::ostream& os, const std::vector<Chunk>& chunks) const;
    void print_chunks(std::ostream& os, const std::vector<Chunk>& chunks, uint16_t indent) const;
    void parse_chunks(TokenStream& is, std::vector<Chunk>& chunks);

private:
    // Use a container because then all the reading logic recurses nicely.
    std::vector<Chunk> m_chunks;
};

