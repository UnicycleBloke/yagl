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
#include "Action12Record.h"
#include "StreamHelpers.h"
#include "Descriptors.h"


void Action12Record::read(std::istream& is, const GRFInfo& info)
{
    uint8_t num_ranges = read_uint8(is);

    for (uint8_t i = 0; i < num_ranges; ++i)
    {
        Range range;
        range.font      = static_cast<Font>(read_uint8(is));
        range.num_chars = read_uint8(is);
        range.base_char = read_uint16(is);
        m_ranges.push_back(range);
    }
}


void Action12Record::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_ranges.size());

    for (const auto& range: m_ranges)
    {
        write_uint8(os, static_cast<uint8_t>(range.font));
        write_uint8(os, range.num_chars);
        write_uint16(os, range.base_char);
    }
}  


uint16_t Action12Record::num_sprites_to_read() const
{
    uint16_t total = 0;
    for (const auto& range: m_ranges)
    {
        total += range.num_chars;
    }
    return total;
}


    enum class Font
    {
        NormalProp  = 0,
        SmallProp   = 1,
        LargeProp   = 2,
        NormalFixed = 3
    };


const EnumDescriptorT<Action12Record::Font> font_desc = 
{ 
    0x00, "",                   
    {
        { 0x00, "NormalProp"  }, 
        { 0x01, "SmallProp"   }, 
        { 0x02, "LargeProp"   }, 
        { 0x03, "NormalFixed" },
    }        
};


void Action12Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << " // Action12" << '\n';
    os << pad(indent) << "{" << '\n';

    uint16_t index = 0;    
    for (const auto& range: m_ranges)
    {
        os << pad(indent + 4) << "range<";
        font_desc.print_value(range.font, os);
        os << ", " << to_hex(range.base_char, true) << ">:\n";
        os << pad(indent + 4) << "{" << '\n';
        
        for (uint16_t i = 0; i < range.num_chars; ++i)
        {
            // This is a SpriteIndexRecord or a RecolourRecord
            print_sprite(index, os, sprites, indent + 8);
            ++index;
        }

        os << pad(indent + 4) << "}" << '\n';
    }

    os << pad(indent) << "}" << '\n';
}


void Action12Record::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("Action12Record::parse not implemented");
}
