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

    if (m_source2 == 0xFE)
    {
        int x = 0;
    }

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
        else if ((m_data & 0xFF) == 0xFF)
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
constexpr const char* str_signed         = "signed";
constexpr const char* str_unsigned       = "unsigned";


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
const EnumDescriptorT<Action0DRecord::GRMFeature> desc_feature 
{
    0x00, "", 
    {
        { 0x00, "Trains" },
        { 0x01, "Vehicles" },
        { 0x02, "Ships" },
        { 0x03, "Aircraft" },
        { 0x08, "GeneralSprites" },
        { 0x0B, "Cargos" },
    }    
};
constexpr BooleanDescriptor desc_defined = { 0x00, str_not_if_defined };


const char* short_op(Action0DRecord::Operation op)
{
    using Op = Action0DRecord::Operation;
    switch (op)
    {
        case Op::Assignment:       return "=";   // TokenType::Equals
        case Op::Addition:         return "+";   // TokenType::OpPlus      
        case Op::Subtraction:      return "-";   // TokenType::OpMinus      
        case Op::MultiplyUnsigned: return "*";   // TokenType::OpMultiply, unsigned       
        case Op::MultiplySigned:   return "*";   // TokenType::OpMultiply, signed       
        case Op::BitShiftUnsigned: return "<<";  // TokenType::ShiftLeft, unsigned        
        case Op::BitShiftSigned:   return "<<";  // TokenType::ShiftLeft, signed        
        case Op::BitwiseAND:       return "&";   // TokenType::Ampersand      
        case Op::BitwiseOR:        return "|";   // TokenType::Pipe      
        case Op::DivideUnsigned:   return "/";   // TokenType::OpDivide, unsigned       
        case Op::DivideSigned:     return "/";   // TokenType::OpDivide, signed       
        case Op::ModuloUnsigned:   return "%";   // TokenType::Percent, unsigned       
        case Op::ModuloSigned:     return "%";   // TokenType::Percent, signed      
    }

    // Should never get here. TODO Should throw here?
    return "?";
}


void print_signed(Action0DRecord::Operation op, std::ostream& os)
{
    using Op = Action0DRecord::Operation;
    switch (op)
    {
        case Op::MultiplyUnsigned:
        case Op::DivideUnsigned:
        case Op::ModuloUnsigned:
        case Op::BitShiftUnsigned:
            os << ", " << str_unsigned;
            break;

        case Op::MultiplySigned:
        case Op::DivideSigned:
        case Op::ModuloSigned:
        case Op::BitShiftSigned:
            os << ", " << str_signed;
            break;
    }
}


Action0DRecord::Operation parse_signed(Action0DRecord::Operation op, TokenStream& is)
{
    if (is.peek().type != TokenType::Comma)
    {
        return op;
    }

    is.match(TokenType::Comma);
    std::string ident = is.match(TokenType::Ident);
    if (ident == str_signed)
    {
        using Op = Action0DRecord::Operation;
        switch (op)
        {
            case Op::MultiplyUnsigned: return Op::MultiplySigned;
            case Op::DivideUnsigned:   return Op::DivideSigned;
            case Op::ModuloUnsigned:   return Op::ModuloSigned;
            case Op::BitShiftUnsigned: return Op::BitShiftSigned;
            //default: throw...    
        }
    }

    return op;
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


void Action0DRecord::parse_description(uint8_t& param, TokenStream& is)
{    
    if (is.peek().type == TokenType::Ident)
    {
        // One of param_str, str_global_var, ...
        is.match(TokenType::Ident); 
        is.match(TokenType::OpenBracket);
        param = is.match_uint8();
        is.match(TokenType::CloseBracket);
    }
    else
    {
        // 0xFF indicates that the parameter is a constant.
        param  = 0xFF;
        m_data = is.match_uint32();
    }
}


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
    // expression: param[0x00] = param[0x01] + param[0x02];
    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = " << param_description(m_source1);
    if (m_operation != Operation::Assignment)
    {  
        os << " " << short_op(m_operation) << " " << param_description(m_source2);
        print_signed(m_operation, os);
    }
    os << ";\n";

    desc_defined.print(m_not_if_defined, os, indent);
}


void Action0DRecord::parse_param(TokenStream& is)
{
    parse_description(m_target, is);
    is.match(TokenType::Equals);
    parse_description(m_source1, is);
    TokenValue token = is.peek();
    if (token.type != TokenType::SemiColon)
    {
        // Parse the operation
        using Op = Action0DRecord::Operation;
        switch (token.type)
        {
            case TokenType::OpPlus:     m_operation = Op::Addition; break;
            case TokenType::OpMinus:    m_operation = Op::Subtraction; break;
            case TokenType::OpMultiply: m_operation = Op::MultiplyUnsigned; break;
            case TokenType::ShiftLeft:  m_operation = Op::BitShiftUnsigned; break;
            case TokenType::Ampersand:  m_operation = Op::BitwiseAND; break;
            case TokenType::Pipe:       m_operation = Op::BitwiseOR; break;
            case TokenType::OpDivide:   m_operation = Op::DivideUnsigned; break;
            case TokenType::Percent:    m_operation = Op::ModuloUnsigned; break;
            default: 
                throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        } 

        is.match(token.type);  

        parse_description(m_source2, is);
        m_operation = parse_signed(m_operation, is);
    }
}


void Action0DRecord::print_other(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //os << pad(indent) << str_source1 << ":"  "<\"" << m_grf_id.to_string() << "\">" << param_description(m_source1) << ";\n"; 

    // expression: param[0x00] = <"">param[0x01] + param[0x02];
    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << "<\"" << m_grf_id.to_string() << "\">" << param_description(m_source1) << ";\n"; 
}


void Action0DRecord::parse_other(TokenStream& is)
{
    parse_description(m_target, is);
    is.match(TokenType::Equals);

    is.match(TokenType::OpenAngle);
    m_grf_id.parse(is);
    is.match(TokenType::CloseAngle);

    parse_description(m_source1, is);

    m_source2 = 0xFE;
}


void Action0DRecord::print_patch(std::ostream& os, uint16_t indent) const
{
    //os << pad(indent) << str_target  << ": " << param_description(m_target) << ";\n";
    //os << pad(indent) << str_source1 << ": " << str_patch_var << "[" << to_hex(m_source1) << ";\n";

    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << str_patch_var << "[" << to_hex(m_source1) << "];\n";

}


void Action0DRecord::parse_patch(TokenStream& is)
{
    parse_description(m_target, is);
    is.match(TokenType::Equals);

    // This just expects an Ident (for now), but doesn't care about the value.
    parse_description(m_source1, is);

    m_source2 = 0xFE;
    m_data    = 0xFFFF;
}


void Action0DRecord::print_resources(std::ostream& os, uint16_t indent) const
{
    os << pad(indent) << str_expression << ": " << param_description(m_target) << " = ";
    os << desc_grm_op.value(static_cast<GRMOperator>(m_source1)) << "(";
    os << desc_feature.value(m_feature) << ", " << to_hex(m_number) << ");\n";  
}


void Action0DRecord::parse_resources(TokenStream& is)
{
    parse_description(m_target, is);
    is.match(TokenType::Equals);

    GRMOperator grm_op;
    desc_grm_op.parse(grm_op, is);
    m_source1 = static_cast<uint8_t>(grm_op);

    is.match(TokenType::OpenParen);
    desc_feature.parse(m_feature, is);
    is.match(TokenType::Comma);
    m_number = is.match_uint16();
    is.match(TokenType::CloseParen);

    m_source2 = 0xFE;
    m_ff      = 0xFF;
}


void Action0DRecord::parse(TokenStream& is)
{
    Type operation_type;
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    desc_type.parse(operation_type, is);
    is.match(TokenType::CloseAngle);

    // Assignments don't seem to set this properly when parsed.
    m_source2 = 0x00;

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token  = is.peek();
        std::string ident = is.match(TokenType::Ident);
        is.match(TokenType::Colon);
        if (ident == str_expression)
        {
            switch (operation_type)
            {
                case Type::Param:     parse_param(is); break;
                case Type::OtherGRF:  parse_other(is); break;
                case Type::Patch:     parse_patch(is); break;
                case Type::Resources: parse_resources(is); break;
            }
        }
        else if (ident == str_not_if_defined)
        {
            desc_defined.parse(m_not_if_defined, is);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }

        is.match(TokenType::SemiColon);
    }

    is.match(TokenType::CloseBrace);
}
