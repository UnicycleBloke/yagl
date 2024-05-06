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
#include "EnumDescriptor.h"
#include "BooleanDescriptor.h"


void Action02IndustryRecord::read(std::istream& is, const GRFInfo& info)
{
    m_feature      = static_cast<FeatureType>(read_uint8(is));
    m_act02_set_id = read_uint8(is);
    m_format       = static_cast<Format>(read_uint8(is));

    if (m_format == Format::Version0)
    {
        // Subtract amounts
        m_version0.sub_in_amounts.read(is);
        // Add amounts
        m_version0.add_out_amounts.read(is);
        // Repeat flag
        m_version0.repeat_flag.read(is);
    }
    else if (m_format == Format::Version1)
    {
        // Subtract amounts
        m_version1.sub_in_regs.read(is);
        // Add amounts
        m_version1.add_out_regs.read(is);
        // Repeat flag
        m_version1.repeat_reg.read(is);
    }
    else if (m_format == Format::Version2)
    {
        // Subtract amounts
        m_version2.sub_in_cargos.read(is);
        // Add amounts
        m_version2.add_out_cargos.read(is);
        // Repeat flag
        m_version2.repeat_reg.read(is);
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
        m_version0.sub_in_amounts.write(os);
        // Add amounts
        m_version0.add_out_amounts.write(os);
        // Repeat flag
        m_version0.repeat_flag.write(os);
    }
    else if (m_format == Format::Version1)
    {
        // Subtract amounts
        m_version1.sub_in_regs.write(os);
        // Add amounts
        m_version1.add_out_regs.write(os);
        // Repeat flag
        m_version1.repeat_reg.write(os);
    }
    else if (m_format == Format::Version2)
    {
        // Subtract amounts
        m_version2.sub_in_cargos.write(os);
        // Add amounts
        m_version2.add_out_cargos.write(os);
        // Repeat flag
        m_version2.repeat_reg.write(os);
    }
    else
    {
        throw RUNTIME_ERROR("Action02IndustryRecord::write not implemented");
    }
}


namespace {


constexpr const char* str_format = "format";


const EnumDescriptorT<Action02IndustryRecord::Format> desc_format =
{
    0x00, str_format,
    {
        { 0x00, "Version0" },
        { 0x01, "Version1" },
        { 0x02, "Version2" },
    }
};


} // namespace {


void Action02IndustryRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<" << FeatureName(m_feature);
    os << ", " << to_hex(m_act02_set_id);
    os << ", " << desc_format.value(m_format);
    os << "> // Action02 industry\n";
    os << pad(indent) << "{\n";

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

    os << pad(indent) << "}\n";
}


void Action02IndustryRecord::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenAngle);
    m_feature = FeatureFromName(is.match(TokenType::Ident));
    is.match(TokenType::Comma);
    m_act02_set_id = is.match_uint8();
    is.match(TokenType::Comma);
    desc_format.parse(m_format, is);
    is.match(TokenType::CloseAngle);

    is.match(TokenType::OpenBrace);

    while (is.peek().type != TokenType::CloseBrace)
    {
        switch (m_format)
        {
            case Format::Version0: parse_version0(is); break;
            case Format::Version1: parse_version1(is); break;
            case Format::Version2: parse_version2(is); break;
            default:               throw PARSER_ERROR("Unexpected industry format", is.peek());
        }
    }

    is.match(TokenType::CloseBrace);
}


namespace {


constexpr const char* str_sub_in_amounts  = "sub_in_amounts";
constexpr const char* str_add_out_amounts = "add_out_amounts";
constexpr const char* str_repeat_flag     = "repeat_flag";

constexpr const char* str_sub_in_regs     = "sub_in_registers";
constexpr const char* str_add_out_regs    = "add_out_registers";
constexpr const char* str_repeat_reg      = "repeat_register";

constexpr const char* str_sub_in_cargos   = "sub_in_cargos";
constexpr const char* str_add_out_cargos  = "add_out_cargos";


const std::map<std::string, uint8_t> g_indices0 =
{
    { str_sub_in_amounts,  0x01 },
    { str_add_out_amounts, 0x02 },
    { str_repeat_flag,     0x03 },
};

const std::map<std::string, uint8_t> g_indices1 =
{
    { str_sub_in_regs,  0x01 },
    { str_add_out_regs, 0x02 },
    { str_repeat_reg,   0x03 },
};

const std::map<std::string, uint8_t> g_indices2 =
{
    { str_sub_in_cargos,  0x01 },
    { str_add_out_cargos, 0x02 },
    { str_repeat_reg,     0x03 },
};


GenericDescriptor<Array<UInt16New, 3>> desc_sub_in_amounts { 0x01, str_sub_in_amounts };
GenericDescriptor<Array<UInt16New, 2>> desc_add_out_amounts{ 0x02, str_add_out_amounts };
BoolDescriptor                         desc_repeat_flag    { 0x03, str_repeat_flag };

GenericDescriptor<Array<UInt8New, 3>> desc_sub_in_regs { 0x01, str_sub_in_regs };
GenericDescriptor<Array<UInt8New, 2>> desc_add_out_regs{ 0x02, str_add_out_regs };
UInt8Descriptor                       desc_repeat_reg  { 0x03, str_repeat_reg };


} // namespace {


void Action02IndustryRecord::print_version0(std::ostream& os, uint16_t indent) const
{
    desc_sub_in_amounts.print(m_version0.sub_in_amounts, os, indent);
    desc_add_out_amounts.print(m_version0.add_out_amounts, os, indent);
    desc_repeat_flag.print(m_version0.repeat_flag, os, indent);
}


void Action02IndustryRecord::parse_version0(TokenStream& is)
{
    TokenValue token = is.peek();
    const auto& it = g_indices0.find(token.value);
    if (it != g_indices0.end())
    {
        is.match(TokenType::Ident);
        is.match(TokenType::Colon);

        switch (it->second)
        {
            case 0x01: desc_sub_in_amounts.parse(m_version0.sub_in_amounts, is); break;
            case 0x02: desc_add_out_amounts.parse(m_version0.add_out_amounts, is); break;
            case 0x03: desc_repeat_flag.parse(m_version0.repeat_flag, is); break;
        }

        is.match(TokenType::SemiColon);
    }
    else
    {
        throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
    }
}


void Action02IndustryRecord::print_version1(std::ostream& os, uint16_t indent) const
{
    desc_sub_in_regs.print(m_version1.sub_in_regs, os, indent);
    desc_add_out_regs.print(m_version1.add_out_regs, os, indent);
    desc_repeat_reg.print(m_version1.repeat_reg, os, indent);
}


void Action02IndustryRecord::parse_version1(TokenStream& is)
{
    TokenValue token = is.peek();
    const auto& it = g_indices1.find(token.value);
    if (it != g_indices1.end())
    {
        is.match(TokenType::Ident);
        is.match(TokenType::Colon);

        switch (it->second)
        {
            case 0x01: desc_sub_in_regs.parse(m_version1.sub_in_regs, is); break;
            case 0x02: desc_add_out_regs.parse(m_version1.add_out_regs, is); break;
            case 0x03: desc_repeat_reg.parse(m_version1.repeat_reg, is); break;
        }

        is.match(TokenType::SemiColon);
    }
    else
    {
        throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
    }
}


void Action02IndustryRecord::CargoList::read(std::istream& is)
{
    uint8_t num_input = read_uint8(is);
    cargos.resize(num_input);
    for (uint8_t i = 0; i < num_input; ++i)
    {
        Cargo c;
        c.cargo = read_uint8(is);
        c.reg   = read_uint8(is);
        cargos[i] = c;
    }
}


void Action02IndustryRecord::CargoList::write(std::ostream& os) const
{
    write_uint8(os, uint8_t(cargos.size()));
    for (const auto& c: cargos)
    {
        write_uint8(os, c.cargo);
        write_uint8(os, c.reg);
    }
}


void Action02IndustryRecord::CargoList::print(std::ostream& os, uint16_t indent) const
{
    os << "[";
    for (const auto& c: cargos)
    {
        os << " (" << to_hex(c.cargo) << ", " << to_hex(c.reg) << ")";
    }
    os << " ];\n";
}


void Action02IndustryRecord::CargoList::parse(TokenStream& is)
{
    is.match(TokenType::OpenBracket);
    while (is.peek().type != TokenType::CloseBracket)
    {
        Cargo c = {};

        is.match(TokenType::OpenParen);
        c.cargo = is.match_uint8();
        is.match(TokenType::Comma);
        c.reg   = is.match_uint8();
        is.match(TokenType::CloseParen);

        cargos.push_back(c);
    }

    is.match(TokenType::CloseBracket);
}


void Action02IndustryRecord::print_version2(std::ostream& os, uint16_t indent) const
{
    // Subtract amounts
    os << pad(indent) << str_sub_in_cargos << ": ";
    m_version2.sub_in_cargos.print(os, indent);

    // Add amounts
    os << pad(indent) << str_add_out_cargos << ": ";
    m_version2.add_out_cargos.print(os, indent);

    // Repeat flag
    desc_repeat_reg.print(m_version2.repeat_reg, os, indent);
}


void Action02IndustryRecord::parse_version2(TokenStream& is)
{
    TokenValue token = is.peek();
    const auto& it = g_indices2.find(token.value);
    if (it != g_indices2.end())
    {
        is.match(TokenType::Ident);
        is.match(TokenType::Colon);

        switch (it->second)
        {
            case 0x01: m_version2.sub_in_cargos.parse(is); break;
            case 0x02: m_version2.add_out_cargos.parse(is); break;
            case 0x03: desc_repeat_reg.parse(m_version2.repeat_reg, is); break;
        }

        is.match(TokenType::SemiColon);
    }
    else
    {
        throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
    }
}

