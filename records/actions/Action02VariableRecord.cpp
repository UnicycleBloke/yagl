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
#include "Action02VariableRecord.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


using VarType = Action02VariableRecord::VarType;


// The sizes of a number of the fields in the record depend on the type 
// read near the beginning. 
static uint32_t read_action(std::istream& is, VarType var_type)
{
     switch (var_type)
     {
         case VarType::PrimaryByte:
         case VarType::RelatedByte:
             return read_uint8(is);

         case VarType::PrimaryWord:
         case VarType::RelatedWord:
             return read_uint16(is);

         case VarType::PrimaryDWord:
         case VarType::RelatedDWord:
             return read_uint32(is);
     }

     return 0;
}


static void write_action(std::ostream& os, VarType type, uint32_t value)
{
     switch (type)
     {
         case VarType::PrimaryByte:
         case VarType::RelatedByte:
             write_uint8(os, value); return;

         case VarType::PrimaryWord:
         case VarType::RelatedWord:
             write_uint16(os, value); return;

         case VarType::PrimaryDWord:
         case VarType::RelatedDWord:
             write_uint32(os, value); return;
     }
}


void Action02VariableRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature  = static_cast<FeatureType>(read_uint8(is));
    m_set_id   = read_uint8(is);
    m_var_type = static_cast<VarType>(read_uint8(is));

    // It's a little involved, but basically a chain of variable operations 
    // which are performed and combined in sequence.
    bool advanced = false;
    do
    {
        VarAction var_action;

        // For the second and subsequent iterations, we need to read 
        // an operation which tells us how to combine the previous result
        // with the next variable calculation. 
        if (m_actions.size() > 0)
        {
            var_action.operation = static_cast<Operation>(read_uint8(is));
        }

        // What variable are we going to perform a calculation with?
        var_action.variable  = read_uint8(is);
        // 0x60+x variables require an additional byte - an index? 
        if (var_action.variable >= 0x60 && var_action.variable < 0x80)
        {
            var_action.parameter = read_uint8(is);
        }

        // This section is the <varadjust> that appears in the online documentation.
        var_action.shift_num = read_uint8(is);
        // Get the bits which say whether it is DIV, MOD or neither
        var_action.action    = var_action.shift_num & 0xC0;
        // Whether there is another following calculation.
        advanced             = var_action.shift_num & 0x20;
        // The actual number of bits to shift is in the low 5 bits.
        var_action.shift_num = var_action.shift_num & 0x1F;

        // We AND the shifted result with a mask and then have three options:
        // 1. Return the result (bit 6 and bit 7 are not set)                
        // 2. Add a value and divide the result (bit 6 is set)
        // 3. Add a value and modulo the result (bit 7 is set)
        var_action.and_mask = read_action(is, m_var_type);
        switch (var_action.action)
        {
            case 0x00:
                var_action.add_value     = 0x00000000;
                var_action.div_mod_value = 0x00000000;
                break;
            case 0x40:
            case 0x80:
            case 0xC0: // What does this mean?
                var_action.add_value     = read_action(is, m_var_type);
                var_action.div_mod_value = read_action(is, m_var_type);
                break;
            default:
                throw RUNTIME_ERROR("Action02VariableRecord::read");
        }

        m_actions.push_back(var_action);
    }
    while (advanced);

    // Since TTDPatch 2.0.1 alpha 57, nvar=0 is a special case. Instead 
    // of using ranges, nvar=0 means that the result of an advanced calculation 
    // (or, if no calculation is performed, the adjusted variable value itself) 
    // is returned as callback result, with bit 15 set.  This is useful for those 
    // callbacks where many different return values are possible and it is easier 
    // to calculate them than list them in ranges.  The default value must still 
    // be specified, and will be used in case the variable(s) used are not available. 
    uint8_t num_ranges = read_uint8(is);
    for (uint8_t i = 0; i < num_ranges; ++i)
    {
        VarRange range;
        range.set_id     = read_uint16(is);
        range.low_range  = read_action(is, m_var_type);
        range.high_range = read_action(is, m_var_type);
        m_ranges.push_back(range);
    }

    m_default = read_uint16(is);
}


void Action02VariableRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_set_id);
    write_uint8(os, static_cast<uint8_t>(m_var_type));

    uint16_t num_actions = m_actions.size();
    for (uint16_t i = 0; i < num_actions; ++i)
    {
        const auto& varaction = m_actions[i];

        if (i > 0)
        {
            write_uint8(os, static_cast<uint8_t>(varaction.operation));
        }

        write_uint8(os, varaction.variable);
        if (varaction.variable >= 0x60 && varaction.variable < 0x80)
        {
            write_uint8(os, varaction.parameter);
        }

        // Whether there is a following calculation.
        bool advanced = ((i + 1) < num_actions);
        write_uint8(os, varaction.shift_num | varaction.action | (advanced ? 0x20 : 0x00));

        write_action(os, m_var_type, varaction.and_mask);
        switch (varaction.action)
        {
            case 0x00:
                break;
            case 0x40:
            case 0x80:
                write_action(os, m_var_type, varaction.add_value);
                write_action(os, m_var_type, varaction.div_mod_value);
                break;
            default:
                throw RUNTIME_ERROR("Action02VariableRecord::write");
        }
    }

    uint8_t num_ranges = m_ranges.size();
    write_uint8(os, num_ranges);
    for (uint8_t i = 0; i < num_ranges; ++i)
    {
        const VarRange& range = m_ranges[i];
        write_uint16(os, range.set_id);
        write_action(os, m_var_type, range.low_range);
        write_action(os, m_var_type, range.high_range);
    }

    write_uint16(os, m_default);
}  


namespace {


const EnumDescriptorT<Action02VariableRecord::Operation> desc_operation = 
{ 
    0x00, "operation",                   
    {
        { 0x00, "Addition" },
        { 0x01, "Subtraction" },
        { 0x02, "SignedMin" },
        { 0x03, "SignedMax" },
        { 0x04, "UnsignedMin" },
        { 0x05, "UnsignedMax" },
        { 0x06, "SignedDiv" },
        { 0x07, "SignedMod" },
        { 0x08, "UnsignedDiv" },
        { 0x09, "UnsignedMod" },
        { 0x0A, "Multiply" },
        { 0x0B, "BitwiseAnd" },
        { 0x0C, "BitwiseOr" },
        { 0x0D, "BitwiseXor" },
        { 0x0E, "TempStore" },
        { 0x0F, "Assign" },
        { 0x10, "PermStore" },
        { 0x11, "RotateRight" },
        { 0x12, "SignedCmp" },
        { 0x13, "UnsignedCmp" },
        { 0x14, "ShiftLeft" },
        { 0x15, "UnsignedShiftRight" },
        { 0x16, "SignedShiftRight" },
    }    
};


constexpr const char* str_default    = "default";
constexpr const char* str_ranges     = "ranges";
constexpr const char* str_value1     = "value1";
constexpr const char* str_value2     = "value2";
constexpr const char* str_variable   = "variable";
constexpr const char* str_expression = "expression";


const std::map<std::string, uint8_t> g_indices =
{
    { str_expression,  0x01 },
    { str_ranges,      0x02 },
    { str_default,     0x03 },
};


const IntegerDescriptorT<uint16_t> desc_default { 0x03, str_default, PropFormat::Hex };


} // namespace {


// Could re-implement this to name the variable.
std::string Action02VariableRecord::variable_name(const VarAction& va) const
{
    std::ostringstream os;
    os << str_variable << "[" << to_hex(va.variable);
    // Some variables take an additional argument
    if (va.variable >= 0x60 && va.variable < 0x80)
    {
        os << ", " << to_hex(va.parameter);
    }
    os  << "]";

    return os.str();
}


std::string Action02VariableRecord::variable_expression(const VarAction& va) const
{
    std::ostringstream os;
    os << variable_name(va);

    // No point showing the shift if it does nothing.
    if (va.shift_num > 0)
    {
        os << " >> " << uint16_t(va.shift_num); 
    }

    os << " & " << to_hex(va.and_mask);

    if ((va.action & 0xC0) != 0x00)
    {
        // No point showing the add value if it is zero.
        if (va.add_value > 0)
        {
            os << " + " << to_hex(va.add_value);  
        }
        
        os << (((va.action & 0x80) == 0x00) ? " / " : " % ");
        os << to_hex(va.div_mod_value);  
    }

    return os.str();
}


void Action02VariableRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_set_id);
    os << "> // Action02 variable" << '\n';
    os << pad(indent) << "{" << '\n';

    // Access general variable or variable of the primary object 81 85 89 (all odd)
    // Access variable of "related" object 82 86 8A (all even)
    // bool primary_object = (static_cast<uint8_t>(m_var_type) & 0x01) == 0x01;
    // uint8_t var_size    = 4;
    // switch (m_var_type)
    // {
    //      case VarType::PrimaryByte: 
    //      case VarType::RelatedByte:  var_size = 1; break;
    //      case VarType::PrimaryWord: 
    //      case VarType::RelatedWord:  var_size = 2; break;
    //      case VarType::PrimaryDWord: 
    //      case VarType::RelatedDWord: var_size = 4; break;
    // }


    print_expression(os, indent + 4);
    print_ranges(os, indent + 4);
    desc_default.print(m_default, os, indent + 4);

    os << pad(indent) << "}\n" ;
}


void Action02VariableRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_set_id = is.match_integer();
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        TokenValue token = is.peek();
        const auto& it = g_indices.find(token.value);
        if (it != g_indices.end())
        {
            is.match(TokenType::Ident);
            is.match(TokenType::Colon);

            switch (it->second)
            {
                case 0x01: parse_expression(is); break;
                case 0x02: parse_ranges(is); break;
                case 0x03: desc_default.parse(m_default, is); break;
            }

            is.match(TokenType::SemiColon);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: " + token.value, token);
        }
    }

    is.match(TokenType::CloseBrace);
}


void Action02VariableRecord::print_expression(std::ostream& os, uint16_t indent) const
{
    // Series of one or more VarAction calculations.
    os << pad(indent) << str_expression << ":\n";
    os << pad(indent) << "{\n" ;
    for (const auto& va: m_actions)
    {
        // For the second and later items - this is an advanced variational action 02.
        if (&va == &m_actions[0])
        {
            os << pad(indent + 4) << str_value1 << " = "; 
            os << variable_expression(va);
        }
        else
        {
            os << "\n";
            os << pad(indent + 4) << str_value2 << " = "; 
            os  << variable_expression(va) << ";\n";
            
            os << pad(indent + 4) << str_value1 << " = "; 
            os << desc_operation.value(va.operation);
            os << "(" << str_value1 << ", " << str_value2 << ")";
        }          
 
        os << ";\n";
    }
    os << pad(indent) << "};\n" ;
}


    // struct VarAction
    // {
    //     bool has_parameter() const { return (variable >= 0x60) && (variable < 0x80); }

    //     Operation  operation;     // Ignored for first varaction. 
    //     uint8_t    variable;
    //     uint8_t    parameter;     // Additional byte following 60+ variables - but not 80+? 
    //     uint8_t    shift_num;     // Bit5 means advanced, bit6 and bit7 mutex 
    //     uint8_t    action;
    //     uint32_t   and_mask;
    //     uint32_t   add_value;     // If bit6 or bit7 of shift_num set 
    //     uint32_t   div_mod_value; // If bit6 or bit7  of shift_num set 
    // };


void Action02VariableRecord::parse_expression(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        VarAction action = {};

        // value2 = 
        is.match(TokenType::Ident);  // value1 or value2
        is.match(TokenType::Equals);

        // variable[variable, parameter]
        is.match_ident(str_variable);
        is.match(TokenType::OpenBracket);
        action.variable = is.match_integer();
        if (is.peek().type == TokenType::Comma)
        {
            is.match(TokenType::Comma);
            action.parameter = is.match_integer();
        }
        is.match(TokenType::CloseBracket);

        // >> shift_num
        if (is.peek().type == TokenType::ShiftRight)
        {
            is.match(TokenType::ShiftRight);
            action.shift_num = is.match_integer();
        }

        // & and_mask
        if (is.peek().type == TokenType::Ampersand)
        {
            is.match(TokenType::Ampersand);
            action.and_mask = is.match_integer();
        }

        // + add_value
        if (is.peek().type == TokenType::OpPlus)
        {
            is.match(TokenType::OpPlus);
            action.add_value = is.match_integer();
        }

        // / div_mod_value or % div_mod_value
        if (is.peek().type == TokenType::OpDivide)
        {
            is.match(TokenType::OpDivide);
            action.div_mod_value = is.match_integer();
        }
        if (is.peek().type == TokenType::Percent)
        {
            is.match(TokenType::Percent);
            action.div_mod_value = is.match_integer();
        }
        is.match(TokenType::SemiColon);

        // Advanced operations.
        // value1 = Subtraction(value1, value2); - mostly ignored - need a more efficient representation.
        if (m_actions.size() > 0)
        {
            // Horribly inefficient.
            is.match_ident(str_value1);
            is.match(TokenType::Equals);
            desc_operation.parse(action.operation, is);
            is.match(TokenType::OpenParen);
            is.match_ident(str_value1);
            is.match(TokenType::Comma);
            is.match_ident(str_value2);
            is.match(TokenType::CloseParen);
            is.match(TokenType::SemiColon);
        }

        m_actions.push_back(action);
    }

    is.match(TokenType::CloseBrace);
}


void Action02VariableRecord::print_ranges(std::ostream& os, uint16_t indent) const
{
    // Selection ranges for the switch.
    os << pad(indent) << str_ranges << ":\n";
    os << pad(indent) << "{\n" ;
    for (const auto& r: m_ranges)
    {
        os << pad(indent + 4);
        if (r.low_range == r.high_range)
        {
            os << to_hex(r.low_range) << ": ";
        }
        else    
        {
            os << to_hex(r.low_range) << ".." << to_hex(r.high_range) << ": ";
        }
        os << to_hex(r.set_id) << ";\n";
    }
    os << pad(indent) << "};\n" ;
}


void Action02VariableRecord::parse_ranges(TokenStream& is)
{
    is.match(TokenType::OpenBrace);
    while (is.peek().type != TokenType::CloseBrace)
    {
        VarRange range = {};
        range.low_range = is.match_integer();
        if (is.peek().type == TokenType::DoubleDot)
        {
            is.match(TokenType::DoubleDot);
            range.high_range = is.match_integer();
        }
        else
        {
            range.high_range = range.low_range;
        }

        is.match(TokenType::Colon);
        range.set_id = is.match_integer();
        is.match(TokenType::SemiColon);

        m_ranges.push_back(range);
    }

    is.match(TokenType::CloseBrace);
}
