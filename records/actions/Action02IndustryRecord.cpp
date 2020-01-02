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
#include "Action02IndustryRecord.h"
#include "StreamHelpers.h"


void Action02IndustryRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature      = static_cast<FeatureType>(read_uint8(is));
    m_act02_set_id = read_uint8(is);
    m_format       = static_cast<Format>(read_uint8(is));

    if (m_format == Format::Version0)
    {
        // Subtract amounts
        m_version0.sub_in_amounts[0] = read_uint16(is);
        m_version0.sub_in_amounts[1] = read_uint16(is);
        m_version0.sub_in_amounts[2] = read_uint16(is);

        // Add amounts
        m_version0.add_out_amounts[0] = read_uint16(is);
        m_version0.add_out_amounts[1] = read_uint16(is);

        // Repeat flag
        m_version0.repeat_flag = (read_uint8(is) != 0);
    }
    else if (m_format == Format::Version1)
    {
        // Subtract amounts
        m_version1.sub_in_regs[0] = read_uint8(is);
        m_version1.sub_in_regs[1] = read_uint8(is);
        m_version1.sub_in_regs[2] = read_uint8(is);

        // Add amounts
        m_version1.add_out_regs[0] = read_uint8(is);
        m_version1.add_out_regs[1] = read_uint8(is);

        // Repeat flag
        m_version1.repeat_reg = read_uint8(is);
    }
    else if (m_format == Format::Version2)
    {
        // Subtract amounts
        uint8_t num_input = read_uint8(is);
        m_version2.sub_in_cargos.resize(num_input);
        for (uint8_t i = 0; i < num_input; ++i)
        {
            Cargo c;
            c.cargo = read_uint8(is);
            c.reg   = read_uint8(is);
            m_version2.sub_in_cargos[i] = c;
        }
        
        // Add amounts
        uint8_t num_output = read_uint8(is);
        m_version2.add_out_cargos.resize(num_output);
        for (uint8_t i = 0; i < num_output; ++i)
        {
            Cargo c;
            c.cargo = read_uint8(is);
            c.reg   = read_uint8(is);
            m_version2.add_out_cargos[i] = c;
        }

        // Repeat flag
        m_version2.repeat_reg = read_uint8(is);
    }
    else
    {
        throw RUNTIME_ERROR("Action02IndustryRecord::read unknown format");
    }
}


void Action02IndustryRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, static_cast<uint8_t>(m_feature));
    write_uint8(os, m_act02_set_id);
    write_uint8(os, static_cast<uint8_t>(m_format));

    if (m_format == Format::Version0)
    {
        // Subtract amounts
        write_uint16(os, m_version0.sub_in_amounts[0]);
        write_uint16(os, m_version0.sub_in_amounts[1]);
        write_uint16(os, m_version0.sub_in_amounts[2]);

        // Add amounts
        write_uint16(os, m_version0.add_out_amounts[0]);
        write_uint16(os, m_version0.add_out_amounts[1]);

        // Repeat flag
        write_uint8(os, m_version0.repeat_flag ? 0x01 : 0x00);
    }
    else if (m_format == Format::Version1)
    {
        // Subtract amounts
        write_uint8(os, m_version1.sub_in_regs[0]);
        write_uint8(os, m_version1.sub_in_regs[1]);
        write_uint8(os, m_version1.sub_in_regs[2]);

        // Add amounts
        write_uint8(os, m_version1.add_out_regs[0]);
        write_uint8(os, m_version1.add_out_regs[1]);

        // Repeat flag
        write_uint8(os, m_version1.repeat_reg);
    }
    else if (m_format == Format::Version2)
    {
        // Subtract amounts
        write_uint8(os, m_version2.sub_in_cargos.size());
        for (const auto& c: m_version2.sub_in_cargos)
        {
            write_uint8(os, c.cargo);
            write_uint8(os, c.reg);
        }
        
        // Add amounts
        write_uint8(os, m_version2.add_out_cargos.size());
        for (const auto& c: m_version2.add_out_cargos)
        {
            write_uint8(os, c.cargo);
            write_uint8(os, c.reg);
        }

        // Repeat flag
        write_uint8(os, m_version2.repeat_reg);
    }
    else
    {
        throw RUNTIME_ERROR("Action02IndustryRecord::write not implemented");
    }
}  


using Format = Action02IndustryRecord::Format;
const EnumNames<Format> g_format_names = 
{{
    { Format::Version0, "Version0" },
    { Format::Version1, "Version1" },
    { Format::Version2, "Version2" },
}};


// Three different versions of this.
// industry<Industries, 0xEF, Version1> // Action02 industry
// {
//     sub_in_registers: [ 0x80 0x81 0x82 ];
//     add_out_registers: [ 0x8B 0x94 ];
//     repeat_register: 0x9B;
// }


void Action02IndustryRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature); 
    os << ", " << to_hex(m_act02_set_id);
    os << ", " << g_format_names.name_of(m_format);
    os << "> // Action02 industry" << '\n';
    os << pad(indent) << "{" << '\n';

    if (m_format == Format::Version0)
    {
        print_version0(os, indent + 4);
    }
    else if (m_format == Format::Version1)
    {
        print_version1(os, indent + 4);
    }
    else if (m_format == Format::Version2)
    {
        print_version2(os, indent + 4);
    }

    os << pad(indent) << "}" << '\n';
}


void Action02IndustryRecord::print_version0(std::ostream& os, uint16_t indent) const
{
    // Subtract amounts
    os << pad(indent) << "sub_in_amounts: [ ";
    os << to_hex(m_version0.sub_in_amounts[0]) << " ";
    os << to_hex(m_version0.sub_in_amounts[1]) << " ";
    os << to_hex(m_version0.sub_in_amounts[2]) << " ];\n";

    // Add amounts
    os << pad(indent) << "add_out_amounts: [ ";
    os << to_hex(m_version0.add_out_amounts[0]) << " ";
    os << to_hex(m_version0.add_out_amounts[1]) << " ];\n";

    // Repeat flag
    os << pad(indent) << "repeat_flag: " << to_bool(m_version0.repeat_flag) << ";\n";
}   


void Action02IndustryRecord::print_version1(std::ostream& os, uint16_t indent) const  
{
    // Subtract amounts
    os << pad(indent) << "sub_in_registers: [ ";
    os << to_hex(m_version1.sub_in_regs[0]) << " ";
    os << to_hex(m_version1.sub_in_regs[1]) << " ";
    os << to_hex(m_version1.sub_in_regs[2]) << " ];\n";

    // Add amounts
    os << pad(indent) << "add_out_registers: [ ";
    os << to_hex(m_version1.add_out_regs[0]) << " ";
    os << to_hex(m_version1.add_out_regs[1]) << " ];\n";

    // Repeat flag
    os << pad(indent) << "repeat_register: " << to_hex(m_version1.repeat_reg) << ";\n";
}   


void Action02IndustryRecord::print_version2(std::ostream& os, uint16_t indent) const    
{
    // Subtract amounts
    os << pad(indent) << "sub_in_cargos: [ ";
    for (const auto& c: m_version2.sub_in_cargos)
    {
        os << "(" << to_hex(c.cargo) << ", " << to_hex(c.reg) << ") ";
    }
    os << " ];\n";
    
    // Add amounts
    os << pad(indent) << "add_out_cargos: [ ";
    for (const auto& c: m_version2.add_out_cargos)
    {
        os << "(" << to_hex(c.cargo) << ", " << to_hex(c.reg) << ") ";
    }
    os << " ];\n";

    // Repeat flag
    os << pad(indent) << "repeat_register: " << to_hex(m_version1.repeat_reg) << ";\n";
}   


void Action02IndustryRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action02IndustryRecord::parse not implemented");
}
