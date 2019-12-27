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


// Assign parameters and calculate results.

// Action D allows you to set the value of grf 
// parameter (i.e. those usually set as options in 
// the newgrf(w).cfg file), as well as do math on them. 

class Action0DRecord : public ActionRecord
{
public:
    Action0DRecord()
    : ActionRecord{RecordType::ACTION_0D}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override; 
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

    bool has_data() const;

private:
    enum class Operation : uint8_t
    {
        Assignment              = 0x00,	
        Addition 	            = 0x01,
        Subtraction 	        = 0x02,
        UnsignedMultiply        = 0x03,	
        SignedMultiply          = 0x04,
        UnsignedBitShift        = 0x05,
        SignedBitShift 	        = 0x06,
        BitwiseAND 	            = 0x07,
        BitwiseOR 	            = 0x08,
        UnsignedDivision        = 0x09,
        SignedDivision 	        = 0x0A,
        UnsignedModulo 	        = 0x0B,
        SignedModulo 	        = 0x0C
    }; 

private:
    // target, source1, source2
    // These bytes specify the target parameter and source parameters. 
    // They can either be simply the numbers of grf parameters, or they 
    // can be a global variable. 
    
    // If either source1 or source2 is FF, the value from <data> is used 
    // instead of a parameter value.
    
    // If <source2> is FE, then this action D will perform one of the 
    // following (see wiki) special variable accesses. In this case, <operation> 
    // must be assignment and <data> is an argument to the data access, not a 
    // source value. 
    // - Read parameters of another GRF
    // - Read game configuration settings
    // - Reserve GRF resources to prevent conflicts with other GRFs   

    // operation
    // You can add 0x80 to the operation number to make it apply only if the target 
    // is not defined yet. In this respect, a parameter is taken to be defined if 
    // any of the following applies:
    // - it has been set to any value in the newgrf(w).cfg parameter list
    // - it or a parameter with higher number has been set to any value by an earlier 
    //   action D
    // If, for example, parameters 0 and 1 are set in the newgrf(w).cfg file, and 
    // action D sets parameter 4, then parameters 2 and 3 automatically become defined 
    // and get a value of zero.  

    uint8_t   m_target = 0;       // Target parameter 
    Operation m_operation;        // Calculation to carry out - see wiki
    bool      m_if_undefined = 0; // Apply operation only if parameter undefined 
    uint8_t   m_source1 = 0;      // First argument 
    uint8_t   m_source2 = 0;      // Second argument 

    union
    {
        uint32_t m_data = 0;  // Value to use as source if not parameter
        struct 
        {
            uint8_t  m_ff;
            uint8_t  m_feature;
            uint16_t m_number;
        };
    };
};
