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
#include "BooleanDescriptor.h"
#include <array>


// Industries don't have graphics directly associated with them (they 
// have graphics through their tiles), so they can't have a traditional 
// action 2. However, they still can use action 2 for a special purpose: 
// production callbacks. A production callback can define exactly how an 
// industry is producing cargo, but this needs more than the 15 bits a 
// normal callback can return. 
class Action02IndustryRecord : public ActionRecord
{
public:
    Action02IndustryRecord()
    : ActionRecord{RecordType::ACTION_02_INDUSTRY}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;    
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

private:
    void print_version0(std::ostream& os, uint16_t indent) const;    
    void print_version1(std::ostream& os, uint16_t indent) const;    
    void print_version2(std::ostream& os, uint16_t indent) const;    

    void parse_version0(TokenStream& is);    
    void parse_version1(TokenStream& is);    
    void parse_version2(TokenStream& is);    

public:
    // Which version of the record structure are we using? At least 
    // we have a field to tell us this explicitly.
    enum class Format 
    {
        Version0 = 0, 
        Version1 = 1, 
        Version2 = 2
    };

private:
    struct Version0
    {
        // Amounts to be subtracted from the incoming cargoes waiting to 
        // be processed. (Signed) 
        UIntArray<UInt16, 3> sub_in_amounts{};
        // Amounts to be added to the produced cargoes. (Unsigned) 
        UIntArray<UInt16, 2> add_out_amounts{};
        //Repeat callback if nonzero, do not repeat if 00. 
        Bool                 repeat_flag{};
    };

private:
    // Using the version 1 format, available since TTDPatch 2.6 r1307, you can decide 
    // the instructions dynamically. Instead of constants, you give register numbers 
    // that contain the needed values; you can compute these values earlier in the 
    // action2 chain. Registers are 4 bytes long and are considered to be signed. 
    // Negative add-out values don't make sense and are ignored. The results are 
    // clamped to the allowed range (0..65535 for both incoming and outgoing cargo 
    // amounts).     
    struct Version1
    {
        // Numbers of registers that contain the amounts to be subtracted
        // from the incoming cargoes waiting to be processed. 
        UIntArray<UInt8, 3> sub_in_regs{};
        // Numbers of registers that hold the amounts to be added to the produced cargoes. 
        UIntArray<UInt8, 2> add_out_regs{};
        // Number of <again> register. Repeat callback if the value of the register  
        // isn't zero, do not repeat otherwise 
        UInt8               repeat_reg{};
    }; 

private:
    // Associates a cargo with a register containing an amount.
    struct Cargo 
    {         
        uint8_t cargo; 
        uint8_t reg;
    };

    struct CargoList
    {
        void read(std::istream& is);
        void write(std::ostream& os) const;    
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);
        std::vector<Cargo> cargos;
    };

    // Using the version 2 format, you can list more input and output cargos than 
    // in version 1. The cargo types are specified according to the cargo 
    // translation table; the amounts are specified via registers as in version 1. 
    // Listed cargo types which are not accepted/produced by the industry 
    // (properties 10..11, 25..26, callback 14B..14C) are ignored.     
    struct Version2
    {     
        CargoList sub_in_cargos;
        CargoList add_out_cargos;
        // Number of <again> register. Repeat callback if the value of the register  
        // isn't zero, do not repeat otherwise 
        UInt8     repeat_reg{};
    };

private:
    // The type of feature to which this record relates: trains or whatever.
    // This can only be Feature::Industries
    FeatureType m_feature = FeatureType::Trains;

    // The index of this Action02 set. Only a byte is allowed, which seems confusing.
    // This is referenced later by Action02Variable or Action03 to create chains of 
    // selection, and to associate graphics with feature instances.
    uint8_t m_act02_set_id = 0;

    // Version for the production callback. Value currently supported
    // are 00 (words), 01 (bytes) and 02.
    Format m_format = Format::Version0;

    // Cargo handling depends on the version.
    // Can't use a union here because of some constructor problem.
    Version0 m_version0;
    Version1 m_version1;
    Version2 m_version2;
};

