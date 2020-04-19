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
#include "StreamHelpers.h"
#include "Exceptions.h"


uint8_t read_uint8(std::istream& is)
{
    uint8_t result;
    is.read((char*)&result, sizeof(result));

    if (is.fail())
    {
        throw RUNTIME_ERROR("read_uint8 failed");
    }

    return result;
}


void write_uint8(std::ostream& os, uint8_t value)
{
    os.write((char*)&value, sizeof(value));
}


uint16_t read_uint8_ext(std::istream& is)
{
    uint8_t result = read_uint8(is);
    return result == 0xFF ? read_uint16(is) : result;
}

void write_uint8_ext(std::ostream& os, uint16_t value, ExtByteFormat format)
{
    // Use extended format only when it is necessary for the value.
    // OR Always used the extended format - this is what NMLC appears to do - default. 
    if ((value >= 0xFF) || (format == ExtByteFormat::Long))
    {
        write_uint8(os, 0xFF);
        write_uint16(os, value);
    }
    else
    {
        write_uint8(os, uint8_t(value));
    }
}


uint16_t read_uint16(std::istream& is)
{
    uint16_t result;
    is.read((char*)&result, sizeof(result));

    if (is.fail())
    {
        throw RUNTIME_ERROR("read_uint16 failed");
    }

    return result;
}


void write_uint16(std::ostream& os, uint16_t value)
{
    os.write((char*)&value, sizeof(value));
}


uint32_t read_uint32(std::istream& is)
{
    uint32_t result;
    is.read((char*)&result, sizeof(result));

    if (is.fail())
    {
        throw RUNTIME_ERROR("read_uint32 failed");
    }

    return result;
}


void write_uint32(std::ostream& os, uint32_t value)
{
    os.write((char*)&value, sizeof(value));
}


// Used for debugging to see what bytes are in the stream as sort of NFO. 
/*
void dump_hex(std::istream& is, uint16_t length)
{ 
    std::streamoff done = 0;
    try
    {
        for (uint16_t i = 0; i < length; ++i)
        {
            uint8_t byte = read_uint8(is);
            std::cout << to_hex(byte, false) << " ";
            --done;
        }
        std::cout << '\n';        
    }
    catch (const std::exception& e)
    {
        std::cout << '\n';
        std::cout << e.what() << '\n';
    }    

    is.seekg(done, std::istream::cur);
}
*/

// Used for the unit tests.
std::string hex_dump(const std::string& data, bool split_lines)
{
    uint32_t index = 0;
    std::ostringstream os;
    for (auto byte: data)
    {
        os << to_hex(byte, false) << " ";
        ++index;
        if (split_lines)
        {
            if ((index % 16) == 0)
                os << "\n";
        }
    }
    return os.str();
}


