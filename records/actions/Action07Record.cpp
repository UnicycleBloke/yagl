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
#include "Action07Record.h"
#include "StreamHelpers.h"
#include "EnumDescriptor.h"


namespace{


const EnumDescriptorT<Action07Record::Condition> desc_condition
{
    0x00, "",
    {
        { 0x00, "is_bit_set" },
        { 0x01, "is_bit_clear" },
        { 0x02, "is_equal" },
        { 0x03, "is_not_equal" },
        { 0x04, "is_less_than" },
        { 0x05, "is_greater_than" },
        { 0x06, "is_grf_activated" },
        { 0x07, "is_grf_not_activated" },
        { 0x08, "is_grf_initialised" },
        { 0x09, "is_grf_init_or_active" },
        { 0x0A, "is_grf_disabled" },
        { 0x0B, "is_cargo_type_invalid" },
        { 0x0C, "is_cargo_type_valid" },
        { 0x0D, "is_rail_type_invalid" },
        { 0x0E, "is_rail_type_valid" },
        { 0x0F, "is_road_type_invalid" },
        { 0x10, "is_road_type_valid" },
        { 0x11, "is_tram_type_invalid" },
        { 0x12, "is_tram_type_valid" },
    }
};


constexpr const char* str_param        = "param";
constexpr const char* str_global_var   = "global_var";
constexpr const char* str_skip_sprites = "skip_sprites";


std::string param_description(uint8_t param)
{
    std::ostringstream ss;
    if (param & 0x80)
    {
        ss << str_global_var << "[" << to_hex(param) << "]";
    }
    else
    {
        ss << str_param << "[" << to_hex(param) << "]";
    }
    return ss.str();
}


void parse_description(uint8_t& param, TokenStream& is)
{
    // One of param_str, str_global_var, ...
    is.match(TokenType::Ident);
    is.match(TokenType::OpenBracket);
    param = is.match_uint8();
    is.match(TokenType::CloseBracket);
}


} // namespace {


void Action07Record::read(std::istream& is, const GRFInfo& info)
{
    m_variable  = read_uint8(is);
    m_varsize   = read_uint8(is);
    m_condition = static_cast<Condition>(read_uint8(is));

    if ( (m_condition == Condition::BitClear) ||
         (m_condition == Condition::BitSet)   )
    {
        // Always 1 for bit tests, the given value should be ignored.
        m_varsize = 1;
    }

    switch (m_varsize)
    {
        case 8: m_value = read_uint32(is); m_mask = read_uint32(is); break;
        case 4: m_value = read_uint32(is); m_mask = 0xFFFFFFFF;      break;
        case 2: m_value = read_uint16(is); m_mask = 0x0000FFFF;      break;
        case 1: m_value = read_uint8(is);  m_mask = 0x000000FF;      break;
        default: break;
    }

    m_num_sprites = read_uint8(is);
}


void Action07Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_variable);
    write_uint8(os, m_varsize);
    write_uint8(os, static_cast<uint8_t>(m_condition));

    switch (m_varsize)
    {
        case 8: write_uint32(os, m_value);
                write_uint32(os, m_mask); break;
        case 4: write_uint32(os, m_value); break;
        case 2: write_uint16(os, m_value); break;
        case 1: write_uint8(os, m_value);  break;
        default: break;
    }

    write_uint8(os, m_num_sprites);
}


void Action07Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " (";

    GRFLabel label{m_value};
    switch (m_condition)
    {
        // e.g. BitSet(parameter[m_variable] & 0xFF, 1 << m_value) - 1-byte values
        case Condition::BitSet:
        case Condition::BitClear:
            os << desc_condition.value(m_condition) << "(";
            os << param_description(m_variable) << " & 0xFF, 1 << " << m_value << ")";
            break;

        // e.g. Equal(parameter[m_variable] & m_mask, m_value)
        case Condition::Equal:
        case Condition::NotEqual:
        case Condition::LessThan:
        case Condition::GreaterThan:
            os << desc_condition.value(m_condition) << "(";
            os << param_description(m_variable) << " & ";
            switch (m_varsize)
            {
                case 1: os << to_hex<uint8_t>(m_mask) << ", " << to_hex<uint8_t>(m_value) << ")"; break;
                case 2: os << to_hex<uint16_t>(m_mask) << ", " << to_hex<uint16_t>(m_value) << ")"; break;
                case 4:
                case 8: os << to_hex<uint32_t>(m_mask) << ", " << to_hex<uint32_t>(m_value) << ")"; break;
            }
            break;

        // e.g. GRFActivated(m_value, m_mask) - 4-byte values
        case Condition::GRFActivated:
        case Condition::GRFNotActivated:
        case Condition::GRFInitialised:
        case Condition::GRFInitOrActive:
        case Condition::GRFDisabled:
            os << desc_condition.value(m_condition) << "(";
            os << "\"" << label.to_string() << "\", " << to_hex(m_mask) << ")";
            break;

        // e.g. CargoTypeInvalid(m_value) - 4-byte value
        case Condition::CargoTypeInvalid:
        case Condition::CargoTypeValid:
        case Condition::RailTypeInvalid:
        case Condition::RailTypeValid:
        case Condition::RoadTypeInvalid:
        case Condition::RoadTypeValid:
        case Condition::TramTypeInvalid:
        case Condition::TramTypeValid:
            os << desc_condition.value(m_condition) << "(";
            os << "\"" << label.to_string() << "\")";
            break;
    }

    os << ") // Action0";
    os << (record_type() == RecordType::ACTION_07 ? "7" : "9");
    os << pad(indent) << "\n{\n";

    os << pad(indent + 4) << str_skip_sprites << ": " << to_hex(m_num_sprites) << ";\n";
    os << pad(indent + 4) << "// Or skip to the next label (Action10) with this value - search wraps at end of GRF.\n";
    os << pad(indent + 4) << "// 0x00 means skip to end of GRF file - may disable the GRF.\n";

    os << pad(indent) << "}\n";
}


// if_act9 (is_equal(param[0x6B] & 0xFFFFFFFF, 0x00000001)) // Action09
// {
//     skip_sprites: 0x02;
//     // Or skip to the next label (Action10) with this value - search wraps at end of GRF.
//     // 0x00 means skip to end of GRF file - may disable the GRF.
// }
// if_act9 (is_grf_disabled("DJT\x01", 0xFFFFFFFF)) // Action09
// {
//     skip_sprites: 0x01;
//     // Or skip to the next label (Action10) with this value - search wraps at end of GRF.
//     // 0x00 means skip to end of GRF file - may disable the GRF.
// }
// if_act7 (is_bit_set(param[0x6C] & 0xFF, 1 << 0)) // Action07
// {
//     skip_sprites: 0x01;
//     // Or skip to the next label (Action10) with this value - search wraps at end of GRF.
//     // 0x00 means skip to end of GRF file - may disable the GRF.
// }
// if_act9 (is_rail_type_invalid("SAAN")) // Action09
// {
//     skip_sprites: 0x01;
//     // Or skip to the next label (Action10) with this value - search wraps at end of GRF.
//     // 0x00 means skip to end of GRF file - may disable the GRF.
// }


void Action07Record::parse(TokenStream& is, SpriteZoomMap& sprites)
{
    is.match_ident(RecordName(record_type()));
    is.match(TokenType::OpenParen);
    desc_condition.parse(m_condition, is);
    is.match(TokenType::OpenParen);

    GRFLabel label;
    switch (m_condition)
    {
        // e.g. BitSet(parameter[m_variable] & 0xFF, 1 << m_value) - 1-byte values
        case Condition::BitSet:
        case Condition::BitClear:
            parse_description(m_variable, is);
            is.match(TokenType::Ampersand);
            is.match_uint8(); // 0xFF
            m_mask = 0xFF;
            is.match(TokenType::Comma);
            is.match_uint8(); // 1
            is.match(TokenType::ShiftLeft);
            m_value = is.match_uint32();
            break;

        // e.g. Equal(parameter[m_variable] & m_mask, m_value)
        case Condition::Equal:
        case Condition::NotEqual:
        case Condition::LessThan:
        case Condition::GreaterThan:
            parse_description(m_variable, is);
            is.match(TokenType::Ampersand);
            m_mask = is.match_uint32();
            is.match(TokenType::Comma);
            m_value = is.match_uint32();
            break;

        // e.g. GRFActivated(m_value, m_mask) - 4-byte values
        case Condition::GRFActivated:
        case Condition::GRFNotActivated:
        case Condition::GRFInitialised:
        case Condition::GRFInitOrActive:
        case Condition::GRFDisabled:
            label.parse(is);
            m_variable = 0x88; // Must be this value
            m_value = label.value();
            is.match(TokenType::Comma);
            m_mask = is.match_uint32();
            break;

        // e.g. CargoTypeInvalid(m_value) - 4-byte value
        case Condition::CargoTypeInvalid:
        case Condition::CargoTypeValid:
        case Condition::RailTypeInvalid:
        case Condition::RailTypeValid:
        case Condition::RoadTypeInvalid:
        case Condition::RoadTypeValid:
        case Condition::TramTypeInvalid:
        case Condition::TramTypeValid:
            label.parse(is);
            m_variable = 0x00; // Ignored
            m_value = label.value();
            m_mask = 0xFFFFFFFF;
            break;
    }

    switch (m_mask)
    {
        case 0x000000FF: m_varsize = 1; break;
        case 0x0000FFFF: m_varsize = 2; break;
        case 0xFFFFFFFF: m_varsize = 4; break;
        default:         m_varsize = 8; break;
    }

    is.match(TokenType::CloseParen);
    is.match(TokenType::CloseParen);
    is.match(TokenType::OpenBrace);

    is.match_ident(str_skip_sprites);
    is.match(TokenType::Colon);
    m_num_sprites = is.match_uint8();
    is.match(TokenType::SemiColon);

    is.match(TokenType::CloseBrace);

}
