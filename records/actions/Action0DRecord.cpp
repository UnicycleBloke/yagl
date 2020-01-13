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
#include "Descriptors.h"
#include <sstream>


void Action0DRecord::read(std::istream& is, const GRFInfo& info)
{
    m_target = read_uint8(is);
    
    uint8_t operation = read_uint8(is);
    // This action only applies to parameters not already defined?
    m_not_if_defined = (operation & 0x80) != 0x00;
    m_operation      = static_cast<Operation>(operation & 0x7F);
    
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
    write_uint8(os, static_cast<uint8_t>(m_operation) | (m_not_if_defined ? 0x80 : 0x00));
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


Action0DRecord::Type Action0DRecord::type() const
{
    // Check for special operations
    if (m_source2 == 0xFE)
    {
        if (m_data == 0xFFFF)
        {
            return Type::Patch;
        } 
        else if ((m_data & 0x00FF) == 0x00FF)
        {
            return Type::Resources;
        }
        else 
        {
            return Type::OtherGRF;
        }
    }

    return Type::Param;
}


namespace {


constexpr const char* str_target         = "target";
constexpr const char* str_source1        = "source1";
constexpr const char* str_source2        = "source2";
constexpr const char* str_param          = "parameter";
constexpr const char* str_global_var     = "global_var";
constexpr const char* str_patch_var      = "patch_var";
constexpr const char* str_operator       = "operator";
constexpr const char* str_not_if_defined = "not_if_defined";
constexpr const char* str_grf_id         = "grf_id";
constexpr const char* str_feature        = "feature";
constexpr const char* str_number         = "number";
constexpr const char* str_expression     = "expression";


const EnumDescriptorT<Action0DRecord::Type> desc_type 
{ 
    0x00, "type", 
    {
        { 0, "Param" },      
        { 1, "Patch" },  
        { 2, "Resources" },
        { 3, "OtherGRF" }, 
    }
};
const EnumDescriptorT<Action0DRecord::Operation> desc_operator 
{ 
    0x00, str_operator, 
    {
        { 0x00, "Assignment" },
        { 0x01, "Addition" },
        { 0x02, "Subtraction" },
        { 0x03, "MultiplyUnsigned" },
        { 0x04, "MultiplySigned" },
        { 0x05, "BitShiftUnsigned" },
        { 0x06, "BitShiftSigned" },
        { 0x07, "BitwiseAND" },
        { 0x08, "BitwiseOR" },
        { 0x09, "DivideUnsigned" },
        { 0x0A, "DivideSigned" },
        { 0x0B, "ModuloUnsigned" },
        { 0x0C, "ModuloSigned" },
    }
};        
const EnumDescriptorT<Action0DRecord::GRMOperator> desc_grm_op 
{
    0x00, str_operator, 
    {
        { 0x00, "GRM_Reserve" },
        { 0x01, "GRM_Find" },
        { 0x02, "GRM_Check" },
        { 0x03, "GRM_Mark" },
        { 0x04, "GRM_FindNoFail" },
        { 0x05, "GRM_CheckNoFail" },
        { 0x06, "GRM_GetOwner" },
    }    
};
constexpr BooleanDescriptor            desc_defined = { 0x00, str_not_if_defined };
constexpr GRFLabelDescriptor           desc_grf_id  = { 0x00, str_grf_id };
constexpr IntegerDescriptorT<uint16_t> desc_number  = { 0x00, str_number, PropFormat::Dec };


const char* short_op(Action0DRecord::Operation op)
{
    using Op = Action0DRecord::Operation;
    switch (op)
    {
        // u and s to disambiguate signed and unsigned is an experiment - ugly.
        case Op::Assignment:       return "=";
        case Op::Addition:         return "+";
        case Op::Subtraction:      return "-";
        case Op::MultiplyUnsigned: return "*u";
        case Op::MultiplySigned:   return "*s";
        case Op::BitShiftUnsigned: return "<<u";
        case Op::BitShiftSigned:   return "<<s";
        case Op::BitwiseAND:       return "&";
        case Op::BitwiseOR:        return "|";
        case Op::DivideUnsigned:   return "/u";
        case Op::DivideSigned:     return "/s";
        case Op::ModuloUnsigned:   return "%u";
        case Op::ModuloSigned:     return "%s";
    }

    // Should never get here.
    return "?";
}

} // namespace {


void Action0DRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    Type operation_type = type();

    os << pad(indent) << RecordName(record_type());
    os << "<" << desc_type.value(operation_type) << ">";
    os << " // Action0D\n";
    os << pad(indent) << "{\n";

    switch (operation_type)
    {
        case Type::Param:     print_param(os, indent + 4); break;
        case Type::OtherGRF:  print_other(os, indent + 4); break;
        case Type::Patch:     print_patch(os, indent + 4); break;
        case Type::Resources: print_resources(os, indent + 4); break;
    }

    os << pad(indent) << "}\n";
}


std::string Action0DRecord::param_description(uint8_t param) const
{
    std::ostringstream ss;
    if (param == 0xFF)
    {
        ss << to_hex(m_data);
    }
    else if (param & 0x80)
    {
        ss << str_global_var << "[" << to_hex(param) << "]";
    }   
    else
    {
        ss << str_param << "[" << to_hex(param) << "]";
    }    
    return ss.str();
}


// set_parameter<Param>
// {
//     target: param[0x00];
//     source1: global_var[0x00]; //0x80 - names?
//     source2: 0x00000000; // FF
//     operation: Add;
//     not_if_defined: true; // 0x80
// }


void Action0DRecord::print_param(std::ostream& os, uint16_t indent) const
{
    // Create a descriptor to decode these strings?
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //os << pad(indent) << str_source1 << ": " << param_description(m_source1) << ";\n";
    //if (m_operation != Operation::Assignment)
    //{
    //    os << pad(indent) << str_source2 << ": " << param_description(m_source2) << ";\n";
    //}
    //desc_operator.print(m_operation, os, indent);

    // This expression is better than four properties. Might be harder to parse.
    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = " << param_description(m_source1);
    if (m_operation != Operation::Assignment)
    {  
        os << " " << short_op(m_operation) << " " << param_description(m_source2);
    }
    os << ";\n";

    desc_defined.print(m_not_if_defined, os, indent);
}


// set_parameter<GRF>
// {
//     target: param[0x00];
//     source: param<"GRFx">[0x00]; 
// }


void Action0DRecord::print_other(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //os << pad(indent) << str_source1 << ":"  "<\"" << m_grf_id.to_string() << "\">" << param_description(m_source1) << ";\n"; 

    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << "<\"" << m_grf_id.to_string() << "\">" << param_description(m_source1) << ";\n"; 
}


// set_parameter<Patch>
// {
//     target: param[0x00];
//     source: patch_var[0x00]; // names? 
// }


void Action0DRecord::print_patch(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //os << pad(indent) << str_source1 << ": " << str_patch_var << "[" << to_hex(m_source1) << ";\n";

    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << str_patch_var << "[" << to_hex(m_source1) << "];\n";

}


// set_parameter<GRM>
// {
//     target: param[0x00];
//     operation: Check;
//     feature: Trains;
//     count: 10;
// }


void Action0DRecord::print_resources(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //desc_grm_op.print(static_cast<GRMOperator>(m_source1), os, indent);
    //os << pad(indent) << str_feature << ": " << FeatureName(m_feature) << ";\n";
    //desc_number.print(m_number, os, indent);

    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << desc_grm_op.value(static_cast<GRMOperator>(m_source1)) << "(";
    os << FeatureName(m_feature) << ", " << to_hex(m_number) << ");\n";  
}
    

void Action0DRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action0DRecord::parse not implemented");
}
