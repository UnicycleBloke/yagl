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


// The sizes of a number of the fields in the record depend on the type 
// read near the beginning. 
static uint32_t read_action(std::istream& is, uint8_t var_type)
{
     switch (var_type)
     {
         case 0x81:
         case 0x82:
             return read_uint8(is);

         case 0x85:
         case 0x86:
             return read_uint16(is);

         case 0x89:
         case 0x8A:
             return read_uint32(is);
     }

     return 0;
}


static void write_action(std::ostream& os, uint8_t type, uint32_t value)
{
     switch (type)
     {
         case 0x81:
         case 0x82:
             write_uint8(os, value); return;

         case 0x85:
         case 0x86:
             write_uint16(os, value); return;

         case 0x89:
         case 0x8A:
             write_uint32(os, value); return;
     }
}


void Action02VariableRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature  = static_cast<FeatureType>(read_uint8(is));
    m_set_id   = read_uint8(is);
    m_var_type = read_uint8(is);

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
            var_action.operation = read_uint8(is);
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
    write_uint8(os, m_var_type);

    uint16_t num_actions = m_actions.size();
    for (uint16_t i = 0; i < num_actions; ++i)
    {
        const auto& varaction = m_actions[i];

        if (i > 0)
        {
            write_uint8(os, varaction.operation);
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


// TODO convert to a map
static std::string operation_name(uint8_t operation)
{
    switch (operation)
    {
        case 0x00: return "add"; 
        case 0x01: return "sub"; 
        case 0x02: return "signed_min"; 
        case 0x03: return "signed_max"; 
        case 0x04: return "unsigned_min"; 
        case 0x05: return "unsigned_max"; 
        case 0x06: return "signed_div"; 
        case 0x07: return "signed_mod"; 
        case 0x08: return "unsigned_div"; 
        case 0x09: return "unsigned_mod"; 
        case 0x0A: return "mul"; 
        case 0x0B: return "bitwise_and"; 
        case 0x0C: return "bitwise_or"; 
        case 0x0D: return "bitwise_xor"; 
        case 0x0E: return "temp_store"; 
        case 0x0F: return "assign"; 
        case 0x10: return "perm_store"; 
        case 0x11: return "ror"; 
        case 0x12: return "signed_cmp"; 
        case 0x13: return "unsigned_cmp"; 
        case 0x14: return "lshift"; 
        case 0x15: return "unsigned_rshift"; 
        case 0x16: return "signed_rshift"; 
    }
    return "<unknown>"; 
}




static const std::map<uint8_t, std::string> g_train_vars =
{
    // Global vars
    // TODO this list is incomplete.
    //{ 0x02, "current_month" },        
    //{ 0x02, "current_day_of_month" }, 
    //{ 0x02, "is_leapyear" },          
    //{ 0x02, "current_day_of_year" },  
    { 0x03, "current_climate" },         
    { 0x06, "traffic_side" },         
    { 0x09, "date_fraction" },         
    { 0x0A, "animation_counter" },    
    { 0x0B, "ttd_patch_version" },    
    { 0x0C, "current_callback" },     
    { 0x0D, "ttd_os_version" },    
    { 0x10, "extra_callback_info1" }, 
    { 0x12, "game_mode" },            
    { 0x18, "extra_callback_info2" }, 
    { 0x1B, "display_options" },      
    { 0x1C, "last_computed_result" }, 
    { 0x20, "snowline_height" },      
    { 0x22, "difficulty_level" },     
    { 0x23, "current_date" },         
    { 0x24, "current_year" },         

    // TODO this list is incomplete
    { 0x40,	"position_length_in_consist" },
    { 0x41,	"position_in_consecutive_chain" },
    { 0x42,	"cargo_types" },
    { 0x43,	"player_info" },
    { 0x44,	"aircraft_info" },
    { 0x45,	"curvature_info" },
    { 0x46,	"motion_counter" },
    { 0x47,	"vehicle_cargo_info" },
    { 0x48,	"vehicle_type_info" },
    { 0x49,	"year_of_construction" },
    { 0x4A,	"current_rail_type_info" },
    { 0x4B,	"long_date_of_last_service" },
    { 0x4C,	"current_max_speed" },
    { 0x4D,	"position_in_articulated_vehicle" },
    { 0xF2,	"refit_cycle" },
};


void Action02VariableRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature) << "> // Action02 variable" << '\n';
    os << pad(indent) << "{" << '\n';
    os << pad(indent + 4) << "this_set_id: " << to_hex(m_set_id, true) << ";\n";
    //os << pad(indent + 4) << "var_type: " << to_hex(m_var_type, true) << '\n';

    // Access general variable or variable of the primary object 81 85 89 (all odd)
    // Access variable of "related" object 82 86 8A (all even)
    bool primary_object = (m_var_type & 0x01) == 0x01;
    uint8_t var_size    = 4;
    switch (m_var_type)
    {
         case 0x81: case 0x82: var_size = 1; break;
         case 0x85: case 0x86: var_size = 2; break;
         case 0x89: case 0x8A: var_size = 4; break;
    }

    for (const auto& va: m_actions)
    {
        // For the second and later items - this is an advanced variational action 02.
        bool advanced = &va != &m_actions[0];
                 
        os << pad(indent + 4) << "test: "; 
        if (advanced)
        {
            os << operation_name(va.operation) << "(test, ";
        }  
        
        // TODO replace the variable with the proper name for the feature.
        // Need a few maps basically. Give some thought to how we can simplify the output
        // to make it clearer. Or maybe there is no need...
        os << "(";
        const auto& it = g_train_vars.find(va.variable);
        if (it != g_train_vars.end())
        {
            os << it->second;
        }
        else
        {
            os << "variable[" << to_hex(va.variable, true) << "]";
        }

        // Some variables take an additional argument
        if (va.variable >= 0x60 && va.variable < 0x80)
        {
            os << "(" << to_hex(va.parameter, true) << ")";
        }

        // No point showing the shift if it does nothing.
        if (va.shift_num > 0)
        {
            os << " >> " << uint16_t(va.shift_num); 
        }

        os << " & " << to_hex(va.and_mask, true) << ")";
        if ((va.action & 0xC0) != 0x00)
        {
            if (va.add_value > 0)
            {
                os << " + " << to_hex(va.add_value, true);  
            }
            
            if ((va.action & 0x80) == 0x00)
            {
                os << " / " << to_hex(va.div_mod_value, true);  
            }
            else
            {
                os << " % " << to_hex(va.div_mod_value, true);  
            }         
        }

        if (advanced)
        {
            os << ")";
        }  

        os << ";\n";
    }

    os << pad(indent + 4) << "switch (test)\n";
    os << pad(indent + 4) << "{\n" ;
    for (const auto& r: m_ranges)
    {
        os << pad(indent + 8);
        if (r.low_range == r.high_range)
        {
            os << to_hex(r.low_range, true) << ": ";
        }
        else    
        {
            os << to_hex(r.low_range, true) << ".." << to_hex(r.high_range, true) << ": ";
        }
        os << to_hex(r.set_id, true) << "; ";
        if (r.set_id & 0x8000)
        {
            os << "// Callback result";
        }
        os << "\n";
    }

    os << pad(indent + 8) << "default: " << to_hex(m_default, true) << ";" ;
    if (m_default & 0x8000)
    {
        os << "// Callback result";
    }
    os << "\n";

    os << pad(indent + 4) << "}\n" ;

    os << pad(indent) << "}\n" ;
}


void Action02VariableRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action02VariableRecord::parse not implemented");
}
