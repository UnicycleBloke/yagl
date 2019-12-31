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


void write_uint8_ext(std::ostream& os, uint16_t value)
{
    write_uint8(os, 0xFF);
    write_uint16(os, value);

    // if (value >= 0xFF)
    // {
    //     write_uint8(os, 0xFF);
    //     write_uint16(os, value);
    // }
    // else
    // {
    //     write_uint8(os, uint8_t(value));
    // }
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


std::string read_string(std::istream& is)
{
    std::string result;
    std::getline(is, result, char(0));

    if (is.fail())
    {
        throw RUNTIME_ERROR("read_string failed");
    }

    return result;
}


void write_string(std::ostream& os, const std::string& value)
{
    write_string(os, value, StringTerm::Null);
}


void write_string(std::ostream& os, const std::string& value, StringTerm term)
{
    // Optionally write without a 0 terminator - one or two strings in the GRF are terminated by end of record or whatever.
    os.write(value.c_str(), value.length() + ((term == StringTerm::None) ? 0 : 1));
}


// Used for debugging to see what bytes are in the stream as sort of NFO. 
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
    catch(const std::exception& e)
    {
        std::cout << '\n';
        std::cout << e.what() << '\n';
    }    

    is.seekg(done, std::istream::cur);
}