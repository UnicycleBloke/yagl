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


// Action 7 & Action 9

// Conditionally skip sprites or jump to label

// These two actions allow you to skip a specified number or all 
// following sprites in this .grf file. This can be used to have, 
// for example, climate-specific graphics, patch-version checks and 
// error messages, and deactivating in the presence of other active 
// .grf files. 

class Action07Record : public ActionRecord
{
public:
    Action07Record(RecordType type)
    : ActionRecord{type}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;   
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

public:
    enum class Condition : uint8_t
    {
        // e.g. BitSet(parameter[m_variable] && 0xFF, 1 << m_value) - 1-byte values
        BitSet           = 0x00,
        BitClear         = 0x01,

        // e.g. Equal(parameter[m_variable] && m_mask, m_value)
        Equal            = 0x02,
        NotEqual         = 0x03,
        LessThan         = 0x04,
        GreaterThan      = 0x05,

        // e.g. GRFActivated(m_value, m_mask) - 4-byte values
        GRFActivated     = 0x06, // Is GRFID active?
        GRFNotActivated  = 0x07, // Is GRFID non-active?
        GRFInitialised   = 0x08, // GRFID is not but will be active?
        GRFInitOrActive  = 0x09, // GRFID is or will be active?
        GRFDisabled      = 0x0A, // GRFID is not nor will be active

        // e.g. CargoTypeInvalid(m_value) - 4-byte value
        CargoTypeInvalid = 0x0B,
        CargoTypeValid   = 0x0C,
        RailTypeInvalid  = 0x0D,
        RailTypeValid    = 0x0E,
        RoadTypeInvalid  = 0x0F,
        RoadTypeValid    = 0x10,
        TramTypeInvalid  = 0x11,
        TramTypeValid    = 0x12,
    };        

private:
    uint8_t   m_variable{};    // This sets the variable to base the decision on. 
    uint8_t   m_varsize{};     // For GRF parameters, this is the same as <param-size> 
                               // in action 6. For built-in variables, it depends on 
                               // the variable, see the above table. 
    Condition m_condition{};   // There are several conditions you can choose from to test against. 
    uint32_t  m_value{};       // This term is what the variable is compared to. Its size is given by <varsize>. 
    uint32_t  m_mask{};        // This is a function of the variable size.  
    uint8_t   m_num_sprites{}; // This element sets how many sprites will be skipped if the condition is true.
                               // Doubles as the index of a label: Action10 to jump to if one exists.
};
