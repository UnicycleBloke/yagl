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


class Action02VariableRecord : public ActionRecord
{
public:
    Action02VariableRecord()
    : ActionRecord{RecordType::ACTION_02_VARIABLE}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;   
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

private:
    struct VarAction
    {
        bool has_parameter() const { return (variable >= 0x60) && (variable < 0x80); }

        uint8_t  operation;     /* Ignored for first varaction. */
        uint8_t  variable;
        uint8_t  parameter;     /* Additional byte following 60+ variables - but not 80+? */
        uint8_t  shift_num;     /* Bit5 means advanced, bit6 and bit7 mutex */
        uint8_t  action;
        uint32_t and_mask;
        uint32_t add_value;     /* If bit6 or bit7 of shift_num set */
        uint32_t div_mod_value; /* If bit6 or bit7  of shift_num set */
    };

    struct VarRange
    {
        uint16_t set_id;
        uint32_t low_range;
        uint32_t high_range;
    };

private:
    FeatureType m_feature;
    uint8_t     m_set_id;

    // Must be 0x81/0x82(B), 0x85/0x86(W), 0x89/0x8A(D)
    uint8_t     m_var_type;

    std::vector<VarAction> m_actions;
    std::vector<VarRange>  m_ranges;

    uint16_t m_default;
};
