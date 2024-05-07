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
#include "Action0DRecordSimple.h"
#include "StreamHelpers.h"
#include "GRFStrings.h"
#include "EnumDescriptor.h"
#include <sstream>
#include <set>


namespace {


constexpr uint8_t idTarget     = 0x01;
constexpr uint8_t idOperation  = 0x02;
constexpr uint8_t idSource1    = 0x03;
constexpr uint8_t idSource2    = 0x04;
constexpr uint8_t idDataValue  = 0x05;

constexpr const char* str_target     = "target";
constexpr const char* str_source1    = "source1";
constexpr const char* str_source2    = "source2";
constexpr const char* str_operation  = "operation";
constexpr const char* str_data_value = "data_value";

constexpr UInt8Descriptor  desc_target     = { idTarget,    str_target };
constexpr UInt8Descriptor  desc_operation  = { idOperation, str_operation };
constexpr UInt8Descriptor  desc_source1    = { idSource1,   str_source1 };
constexpr UInt8Descriptor  desc_source2    = { idSource2,   str_source2 };
constexpr UInt32Descriptor desc_data_value = { idDataValue, str_data_value };

const std::map<std::string, uint8_t> g_indices =
{
    { str_target,     idTarget },
    { str_operation,  idOperation },
    { str_source1,    idSource1 },
    { str_source2,    idSource2 },
    { str_data_value, idDataValue },
};


} // namespace {


void Action0DRecordSimple::read(std::istream& is, const GRFInfo& info)
{
    m_target.read(is);
    m_operation.read(is);
    m_source1.read(is);
    m_source2.read(is);
    if (has_data())
    {
        m_data_value.read(is);
    }
}


void Action0DRecordSimple::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    m_target.write(os);
    m_operation.write(os);
    m_source1.write(os);
    m_source2.write(os);
    if (has_data())
    {
        m_data_value.write(os);
    }
}


bool Action0DRecordSimple::has_data() const
{
    // From NewGRF specs:
    // If either <source1> or <source2> is FF, the value from is used instead of a parameter value.
    // If <source2> is FE, then this action D will perform one of the following special variable accesses. 
    // In this case, <operation> must be assignment and is an argument to the data access, not a source value.     
    return (m_source1.get() == 0xFF) || (m_source2.get() >= 0xFE);
}


void Action0DRecordSimple::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type());
    os << " // Action0D\n";
    os << pad(indent) << "{\n";

    desc_target.print(m_target, os, indent + 4);
    desc_operation.print(m_operation, os, indent + 4);
    desc_source1.print(m_source1, os, indent + 4);
    desc_source2.print(m_source2, os, indent + 4);
    if (has_data())
    {
        desc_data_value.print(m_data_value, os, indent + 4);
    }

    os << pad(indent) << "}\n";
}


void Action0DRecordSimple::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenBrace);

    std::set<uint8_t> items;

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
                case idTarget:    desc_target.parse(m_target, is);         items.insert(it->second); break;
                case idOperation: desc_operation.parse(m_operation, is);   items.insert(it->second); break;
                case idSource1:   desc_source1.parse(m_source1, is);       items.insert(it->second); break;
                case idSource2:   desc_source2.parse(m_source2, is);       items.insert(it->second); break;
                case idDataValue: desc_data_value.parse(m_data_value, is); items.insert(it->second); break;
            }

            is.match(TokenType::SemiColon);
        }
        else
        {
            throw PARSER_ERROR("Unexpected identifier: '" + token.value + "'", token);
        }
    }

    is.match(TokenType::CloseBrace);

    // Confirm that all the expected fields of the data structure were found in the YAGL record.
    // Could also check for duplicates above. 
    if (items.find(idTarget) == items.end())
        throw RUNTIME_ERROR("Missing identifier: '" + std::string{str_target} + "'");
    if (items.find(idOperation) == items.end())
        throw RUNTIME_ERROR("Missing identifier: '" + std::string{str_operation} + "'");
    if (items.find(idSource1) == items.end())
        throw RUNTIME_ERROR("Missing identifier: '" + std::string{str_source1} + "'");
    if (items.find(idSource2) == items.end())
        throw RUNTIME_ERROR("Missing identifier: '" + std::string{str_source2} + "'");
    if (has_data())
    {
        if (items.find(idDataValue) == items.end())
            throw RUNTIME_ERROR("Missing identifier: '" + std::string{str_data_value} + "'");
    }
}
