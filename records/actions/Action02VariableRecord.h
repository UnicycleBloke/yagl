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
#include "IntegerDescriptor.h"


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
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

private:
    struct VarAction;
    std::string variable_name(const VarAction& va) const;
    std::string variable_expression(const VarAction& va) const;

    void print_ranges(std::ostream& os, uint16_t indent) const;
    void print_expression(std::ostream& os, uint16_t indent) const;

    void parse_ranges(TokenStream& is);
    void parse_expression(TokenStream& is);

public:
    // Must be 0x81/0x82(B), 0x85/0x86(W), 0x89/0x8A(D)
    enum class VarType : uint8_t
    {
        PrimaryByte  = 0x81,
        RelatedByte  = 0x82,
        PrimaryWord  = 0x85,
        RelatedWord  = 0x86,
        PrimaryDWord = 0x89,
        RelatedDWord = 0x8A,
    };

    enum class Operation : uint8_t
    {
        Addition           = 0x00, 
        Subtraction        = 0x01, 
        SignedMin          = 0x02, 
        SignedMax          = 0x03, 
        UnsignedMin        = 0x04, 
        UnsignedMax        = 0x05, 
        SignedDiv          = 0x06, 
        SignedMod          = 0x07, 
        UnsignedDiv        = 0x08, 
        UnsignedMod        = 0x09, 
        Multiply           = 0x0A, 
        BitwiseAnd         = 0x0B, 
        BitwiseOr          = 0x0C, 
        BitwiseXor         = 0x0D, 
        TempStore          = 0x0E, 
        Assign             = 0x0F, 
        PermStore          = 0x10, 
        RotateRight        = 0x11, 
        SignedCmp          = 0x12, 
        UnsignedCmp        = 0x13, 
        ShiftLeft          = 0x14, 
        UnsignedShiftRight = 0x15, 
        SignedShiftRight   = 0x16, 
    };

private:
    struct VarAction
    {
        bool has_parameter() const { return (variable >= 0x60) && (variable < 0x80); }

        Operation  operation;     // Ignored for first varaction. 
        uint8_t    variable;
        uint8_t    parameter;     // Additional byte following 60+ variables - but not 80+? 
        uint8_t    shift_num;     // Bit5 means advanced, bit6 and bit7 mutex 
        uint8_t    action;
        uint32_t   and_mask;
        uint32_t   add_value;     // If bit6 or bit7 of shift_num set 
        uint32_t   div_mod_value; // If bit6 or bit7  of shift_num set 
    };

    struct VarRange
    {
        uint16_t set_id;
        uint32_t low_range;
        uint32_t high_range;
    };

private:
    FeatureType m_feature;
    UInt8       m_set_id;
    VarType     m_var_type;

    std::vector<VarAction> m_actions;
    std::vector<VarRange>  m_ranges;

    UInt16 m_default;
};
