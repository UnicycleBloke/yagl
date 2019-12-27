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
#include "ActionFFRecord.h"
#include "StreamHelpers.h"
#include "FileSystem.h"
#include "CommandLineOptions.h"
#include <fstream>


void ActionFFRecord::read(std::istream& is, const GRFInfo& info)
{
    // The length is superfluous as the string is null terminated.
    uint8_t length = read_uint8(is);
    m_filename = read_string(is);
    if (m_filename.length() != length)
    {
        throw RUNTIME_ERROR("ActionFFRecord::read inconsistent length");
    }

    // Not sure how big this is likely to be. It contains a WAV encoded 
    // sound effect. It is a byte-for-byte copy of the original WAV file.
    while (is.peek() != EOF)
    {
        m_binary.push_back(read_uint8(is));
    }
}


void ActionFFRecord::write(std::ostream& os, const GRFInfo& info) const
{
    ActionRecord::write(os, info);

    write_uint8(os, m_filename.length());
    write_string(os, m_filename);

    for (const auto& byte: m_binary)
    os.write((char*)&m_binary[0], m_binary.size());
}  


void ActionFFRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    // Get the relative path for the spritesheet. This is used in the YAGL.
    fs::path file_path{CommandLineOptions::options().yagl_dir()};
    file_path.append(m_filename);

    // This need only distinguish itself from an ActionFE record.
    os << pad(indent) << "binary(\"" << file_path.string() << "\");\n";
}


void ActionFFRecord::parse(TokenStream& is)
{
    is.match_ident(RecordName(record_type()));
    throw RUNTIME_ERROR("ActionFFRecord::parse not implemented");
}


void ActionFFRecord::write_binary_file(const std::string& binary_dir)
{
    fs::path binary_path(binary_dir);
    binary_path.append(m_filename);

    std::ofstream os(binary_path, std::ios::binary);
    os.write((char*)&m_binary[0], m_binary.size());
}
