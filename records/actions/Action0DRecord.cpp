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
#include "Action0DRecord.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"
#include <sstream>


void Action0DRecord::read(std::istream& is, const GRFInfo& info)
{
    m_target = read_uint8(is);
    
    uint8_t operation = read_uint8(is);
    // This action only applies to parameters not already defined?
    m_if_undefined = (operation & 0x80) != 0x00;
    m_operation    = static_cast<Operation>(operation & 0x7F);
    
    m_source1   = read_uint8(is);
    m_source2   = read_uint8(is);   

    m_data = 0x00000000;
    if (has_data())
    {
        m_data = read_uint32(is);
    }
}


void Action0DRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_target);
    write_uint8(os, static_cast<uint8_t>(m_operation) | (m_if_undefined ? 0x80 : 0x00));
    write_uint8(os, m_source1);
    write_uint8(os, m_source2);

    if (has_data())
    {
        write_uint32(os, m_data);
    }
}  


bool Action0DRecord::has_data() const
{
    return (m_source1 == 0xFF) || (m_source2 >= 0xFE);
}


// set_parameter // Action0D
// {
//     parameter[0x75]: global_var[0x9D];
//     only_undefined: false;
// }
// set_parameter // Action0D
// {
//     parameter[0x76]: parameter[0x75] - 0x00000001;
//     only_undefined: false;
// }
// set_parameter // Action0D
// {
//     parameter[0x76]: parameter[0x76] / parameter[0x76], signed;
//     only_undefined: false;
// }
// set_parameter // Action0D
// {
//     parameter[0x74]: global_var[0xA1];
//     only_undefined: false;
// }


void Action0DRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    // This is sooooo complicated. Action0D is a bit complicated, with the various values interpreted
    // in a multiple different ways. The code attempts to print something which is unambiguous so that 
    // it can be easily parsed to reconstruct the binary representation. 

    os << pad(indent) << RecordName(record_type()) << " // Action0D" << '\n';
    os << pad(indent) << "{" << '\n';

    std::ostringstream sst;
    sst << "parameter[" << to_hex(m_target) << "]";
    std::string target = sst.str();

    if (m_source2 != 0xFE)
    {
        std::ostringstream ss1;
        if (m_source1 == 0xFF)
            ss1 << to_hex(m_data);
        else if (m_source1 & 0x80)
            ss1 << "global_var[" << to_hex(m_source1) << "]";
        else
            ss1 << "parameter[" << to_hex(m_source1) << "]";
        std::string source1 = ss1.str();

        std::ostringstream ss2;
        if (m_source2 == 0xFF)
            ss2 << to_hex(m_data);
        else if (m_source2 & 0x80)
            ss2 << "global_var[" << to_hex(m_source2) << "]";
        else
            ss2 << "parameter[" << to_hex(m_source2) << "]";
        std::string source2 = ss2.str();

        os << pad(indent + 4) << target << ": " <<  source1;
        switch (m_operation)
        {
            case Operation::Assignment: break;
            case Operation::Addition:         os << " + "  << source2; break;
            case Operation::Subtraction:      os << " - "  << source2; break;
            case Operation::UnsignedMultiply: os << " * "  << source2 << ", unsigned"; break;
            case Operation::SignedMultiply:   os << " * "  << source2 << ", signed"; break;
            case Operation::UnsignedBitShift: os << " << " << source2 << ", unsigned"; break;
            case Operation::SignedBitShift:   os << " << " << source2 << ", signed"; break;
            case Operation::BitwiseAND:       os << " & "  << source2; break;
            case Operation::BitwiseOR:        os << " | "  << source2; break;
            case Operation::UnsignedDivision: os << " / "  << source2 << ", unsigned"; break;
            case Operation::SignedDivision:   os << " / "  << source2 << ", signed"; break;
            case Operation::UnsignedModulo:   os << " % "  << source2 << ", unsigned"; break;
            case Operation::SignedModulo:     os << " % "  << source2 << ", signed"; break;
        }
        os << ";\n";
    }
    else
    {
        if (m_data == 0xFFFF)
        {
            os << pad(indent + 4) << target << ": ";
            os << "patch_parameter[" << to_hex(m_source1) << "];\n";
        }
        else if ((m_data & 0xFF) == 0xFF)
        {
            os << pad(indent + 4) << target << ": ";
            switch (m_source1)
            {
                // Find available resource ID and mark as in use 
                case 00: os << "grfop_reserve(" << FeatureName(m_feature) << ", " << m_number << ")"; break;
                // Find available resource ID but do not mark
                case 01: os << "grfop_find(" << FeatureName(m_feature) << ", " << m_number << ")"; break;
                // Check whether given resources (ID stored in the <target> variable) are available 
                case 02: os << "grfop_check(" << FeatureName(m_feature) << ", " << m_number << ", " << target << ")"; break;
                // Check that the given resources are available, and if so mark as in use 
                case 03: os << "grfop_mark(" << FeatureName(m_feature) << ", " << m_number << ", " << target << ")"; break;
                // Like "Find", but does not deactive the grf if no IDs found 
                case 04: os << "grfop_find_no_fail(" << FeatureName(m_feature) << ", " << m_number << ")"; break;
                // Like "Check", but does not deactive the grf if there is a conflict 
                case 05: os << "grfop_check_no_fail(" << FeatureName(m_feature) << ", " << m_number << ", " << target << ")"; break;
                // Retrieve GRFID that has reserved the given ID (or 0 if not reserved)  
                case 06: os << "grfop_get_owner(" << FeatureName(m_feature) << ", " << m_number << ", " << target <<")"; break;
            }
            os << ";\n";
        }
        else
        {
            GRFLabel label{m_data}; 
            os << pad(indent + 4) << target << ": ";
            os << "parameter<";
            os << label.to_string();
            os << ">[" << to_hex(m_source1) << "];\n";
        }        
    }

    os << pad(indent + 4) << "only_undefined: " << (m_if_undefined ? "true" : "false") << ";\n";
    os << pad(indent) << "}" << '\n';
} 
    

void Action0DRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0DRecord::parse not implemented");
}
