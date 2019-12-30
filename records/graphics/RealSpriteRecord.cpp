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
#include "RealSpriteRecord.h"
#include "StreamHelpers.h"
#include "ChunkEncoder.h"
#include "Descriptors.h"
#include <string>
#include <sstream>
#include <png.h>
#include <png.h>
#include <cstdio>
#include <algorithm>


void RealSpriteRecord::read(std::istream& is, const GRFInfo& info)
{
    // We already have the sprite ID, the size (whatever it actually means), and the compression.
    // Zoom level is only used in Format2 files. 
    m_zoom = (info.format == GRFFormat::Container2) ? static_cast<ZoomLevel>(read_uint8(is)) : ZoomLevel::Normal;        
    // Taller images are allowed Format2 files.
    m_ydim = (info.format == GRFFormat::Container2) ? read_uint16(is) : read_uint8(is);
    m_xdim = read_uint16(is);
    m_xrel = read_uint16(is);
    m_yrel = read_uint16(is);

    // The uncompressed size is only given in certain cases. The transparency bit tells us how to decode the 
    // data after reading if from the file. It is the size of the data before chunk-compression (tiles). I think.
    m_uncomp_size = ((info.format == GRFFormat::Container2) && (m_compression & CHUNKED_FORMAT)) ? read_uint32(is) : 0;

    // The compression byte is interpreted quite differently depending on the file format.
    // Format2 images may have more than one byte per pixel. Format1 images just have a palette byte.
    uint32_t img_size; 
    uint32_t pix_size;
    if (info.format == GRFFormat::Container1)
    {
        // (m_size - 8) here corresponds to the size of the record minus the compression and dimensions.
        pix_size = 1;
        img_size = (m_compression & COMPRESSED_IN_MEMORY) ? (m_xdim * m_ydim) : (m_size - 8);
        m_colour = HAS_PALETTE;
    }
    else
    {
        // We have potentially several bytes of data for each pixel.
        // Presumably at least one of these bits must be set.
        // Expected configurations are RGB, RGBA and P.
        pix_size  = (m_compression & HAS_RGB)     ? 3 : 0;
        pix_size += (m_compression & HAS_ALPHA)   ? 1 : 0;
        pix_size += (m_compression & HAS_PALETTE) ? 1 : 0;
        img_size  = (m_uncomp_size == 0) ? (m_xdim * m_ydim * pix_size) : m_uncomp_size;
        m_colour  = m_compression & (HAS_RGB | HAS_ALPHA | HAS_PALETTE);
    }   

    // Read the image data. This decompression is based on LZ77 in some way. I just followed 
    // the description in the GRF container documentation. Place the expanded data into a 
    // pre-sized buffer. Have subsequently compared the code to OpenTTD, and it looks fine.
    std::vector<uint8_t> pixdata(img_size);
    uint32_t index = 0;
    while (img_size > 0)
    {
        int8_t code = read_uint8(is);
        if (code < 0)
        {
            // The high bit is set, so we are going to copy data from earlier
            // in the sprite.
            uint16_t length = -(code >> 3);                      
            uint16_t offset = ((static_cast<uint16_t>(code) & 0x07) << 8) | read_uint8(is);
            if (offset > index)
            {
                throw RUNTIME_ERROR("");
            }
            if (img_size < length)
            {
                throw RUNTIME_ERROR("");
            }

            img_size -= length;
            for (; length > 0; length--)
            {
                pixdata[index] = pixdata[index - offset];
                index++;
            }
        }
        else
        {
            // The high bit is not set so we have to read the next number 
            // of bytes from the file.
            uint16_t length = (code == 0) ? 0x80 : code;
            if (img_size < length)
            {
                throw RUNTIME_ERROR("");
            }

            img_size -= length;
            for (; length > 0; length--)
            {
                uint8_t pix = read_uint8(is);
                pixdata[index++] = pix;
            }
        }
    }

    // This bit in the compression indicates that the image contains transparent sections.
    // In this case, it has been stored in a 'chunked' format. We now decode this information
    // to obtain the actual pixel data.
    if (m_compression & CHUNKED_FORMAT)
    {
        m_pixels = decode_tile(pixdata, m_xdim, m_ydim, m_compression, info.format);
    }
    else
    {
        m_pixels = pixdata;
    }    
}


RealSpriteRecord::Pixel RealSpriteRecord::pixel(uint32_t x, uint32_t y) const
{
    // This is probably slow. Need a better implementation/interaction with 
    // sprite sheet generator.
    uint8_t pix_size = 0;
    pix_size  = (m_colour & HAS_RGB)     ? 3 : 0;
    pix_size += (m_colour & HAS_ALPHA)   ? 1 : 0;
    pix_size += (m_colour & HAS_PALETTE) ? 1 : 0;

    uint32_t offset = (y * m_xdim + x) * pix_size;

    Pixel pixel = {};
    if (m_colour & HAS_RGB)
    {
        pixel.red   = m_pixels[offset++];
        pixel.green = m_pixels[offset++];
        pixel.blue  = m_pixels[offset++];
    }    
    if (m_colour & HAS_ALPHA)
    {
        pixel.alpha = m_pixels[offset++];
    }    
    if (m_colour & HAS_PALETTE)
    {
        pixel.index = m_pixels[offset++];
    }    

    return pixel;
}


void RealSpriteRecord::write(std::ostream& os, const GRFInfo& info) const
{
    if (info.format == GRFFormat::Container2)
    {
        write_format2(os);
    }
    else
    {
        write_format1(os);
    }   
}


void RealSpriteRecord::write_format1(std::ostream& os) const
{
    // This is a fake sprite used where there is no image at all, but a slot for one in the NFO.
    if (m_pixels.size() == 0)
    {
        write_uint8(os, 0x00);
        return;
    }

    std::vector<uint8_t> output_data;
    uint32_t uncomp_size = 0;
    //if (has_transparency(m_compression, GRFFormat::Container1))
    if (m_compression & CHUNKED_FORMAT)
    {
        std::vector<uint8_t> chunked_data = encode_tile(m_pixels, m_xdim, m_ydim, 
            m_compression, GRFFormat::Container1);
        output_data = encode_lz77(chunked_data);
    }
    else
    {
        output_data = encode_lz77(m_pixels);
    }

    // TODO this isn't quite right - a different size is written sometimes.
    // We need to know this value for reading chunked sprites.
    write_uint16(os, output_data.size() + 8);

    write_uint8(os,  m_compression);
    write_uint8(os,  m_ydim);
    write_uint16(os, m_xdim);
    write_uint16(os, m_xrel);
    write_uint16(os, m_yrel);

    for (const auto byte: output_data)
    {
        write_uint8(os, byte);
    }
}   


void RealSpriteRecord::write_format2(std::ostream& os) const
{
    // This is a fake sprite used where there is no image at all, but a slot for one in the NFO.
    if (m_pixels.size() == 0)
    {
        write_uint8(os, 0x00);
        return;
    }

    std::vector<uint8_t> output_data;
    uint32_t uncomp_size = 0;
    //if (has_transparency(m_compression, GRFFormat::Container2))
    if (m_compression & CHUNKED_FORMAT)
    {
        std::vector<uint8_t> chunked_data = encode_tile(m_pixels, m_xdim, m_ydim, 
            m_compression, GRFFormat::Container2);
        output_data = encode_lz77(chunked_data);
        uncomp_size = chunked_data.size();
    }
    else
    {
        output_data = encode_lz77(m_pixels);
    }

    uint32_t output_size = output_data.size() + ((m_compression & CHUNKED_FORMAT) ? 14 : 10);

    // TODO this isn't quite right - a different size is written sometimes.
    // We need to know this value for reading chunked sprites.
    write_uint32(os, m_sprite_id); // sprite id
    write_uint32(os, output_size); // TODO + 4 if trans
    write_uint8(os,  m_compression);

    write_uint8(os,  static_cast<uint8_t>(m_zoom));
    write_uint16(os, m_ydim);
    write_uint16(os, m_xdim);
    write_uint16(os, m_xrel);
    write_uint16(os, m_yrel);

    // The uncompressed size is only given in certain cases. The transparency bit tells us how to decode the 
    // data after reading if from the file. It is the size of the data before chunk-compression (tiles). I think.
    //if (has_transparency(m_compression, GRFFormat::Container2))
    if (m_compression & CHUNKED_FORMAT)
    {
        write_uint32(os, uncomp_size);
    }

    uint32_t size = output_data.size();
    for (uint32_t i = 0; i < size; ++i)
    //for (const auto byte: output_data)
    {
        //write_uint8(os, byte);
        write_uint8(os, output_data[i]);
    }
}  


static void append_byte(std::vector<uint8_t>& output, uint8_t byte)
{
    output.push_back(byte);
}


static void append_bytes(std::vector<uint8_t>& output, const uint8_t* bytes, uint8_t length)
{
    for (uint8_t b = 0; b < length; ++b)
    {
        output.push_back(bytes[b]);
    }
}


static inline int find(const uint8_t* pat_data, int32_t pat_size, const uint8_t* data, int32_t data_size)
{
    for (int32_t i = 0; i + pat_size <= data_size; ++i) 
    {
        int32_t j = 0;
        while (j < pat_size && pat_data[j] == data[i + j]) ++j;
        if (j == pat_size)
        {
            return i;
        } 
    }
    return -1;
}


// This implementation is directly copied from _lz77.c found in the NML source.
// Some types and whatnot have been changed, but the algorithm is the same.
std::vector<uint8_t> RealSpriteRecord::encode_lz77(const std::vector<uint8_t>& input_data) const
{
    std::vector<uint8_t> output;

    std::array<uint8_t, 0x80> literal;
    uint8_t literal_size = 0;
    uint32_t input_size  = input_data.size();
    
    int32_t position = 0;
    while (position < input_size) 
    {
        int32_t start_pos = position - (1 << 11) + 1;
        if (start_pos < 0) start_pos = 0;

        // Loop through the lookahead buffer. 
        int32_t max_look = input_size - position + 1;
        if (max_look > 16) 
        {
            max_look = 16;
        }

        int32_t overlap_pos = 0;
        int32_t overlap_len = 0;
        int32_t i;
        for (i = 3; i < max_look; ++i) 
        {
            // Find the pattern match in the window. 
            int result = find(&input_data[0] + position, i, &input_data[0] + start_pos, position - start_pos);
            // If match failed, we've found the longest. 
            if (result < 0) break;

            overlap_pos = position - start_pos - result;
            overlap_len = i;
            start_pos += result;
        }

        if (overlap_len > 0) 
        {
            if (literal_size > 0) 
            {
                append_byte(output, literal_size);
                append_bytes(output, &literal[0], literal_size);
                literal_size = 0;
            }
            int32_t val = 0x80 | (16 - overlap_len) << 3 | overlap_pos >> 8;
            append_byte(output, val);
            append_byte(output, overlap_pos & 0xFF);
            position += overlap_len;
        } 
        else 
        {
            literal[literal_size++] = input_data[position];
            if (literal_size == sizeof(literal)) 
            {
                append_byte(output, 0);
                append_bytes(output, &literal[0], literal_size);
                literal_size = 0;
            }
            position += 1;
        }
    }
    
    if (literal_size > 0) 
    {
        append_byte(output, literal_size);
        append_bytes(output, &literal[0], literal_size);
        literal_size = 0;
    }

    return output;
}


void RealSpriteRecord::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << "[" << m_xdim << ", " << m_ydim << ", " <<  m_xrel << ", " << m_yrel << "], ";
    switch (m_zoom)
    { 
        case ZoomLevel::Normal:    os << "normal, "; break;
        case ZoomLevel::ZoomInX2:  os << "zin2, ";   break;
        case ZoomLevel::ZoomInX4:  os << "zin4, ";   break;
        case ZoomLevel::ZoomOutX2: os << "zout2, ";  break;
        case ZoomLevel::ZoomOutX4: os << "zout4, ";  break;
        case ZoomLevel::ZoomOutX8: os << "zout8, ";  break;
    }

    // We expect one of the following colour depths. 
    switch (m_colour)
    {
        case HAS_PALETTE:                       os << "8bpp"; break;
        case HAS_RGB | HAS_ALPHA:               os << "32bpp"; break;
        case HAS_RGB | HAS_ALPHA | HAS_PALETTE: os << "32bpp|mask"; break;
        // Do these ever occur?
        //case HAS_RGB:                           os << "24bpp"; break;
        //case HAS_RGB | HAS_PALETTE:             os << "24bpp|mask"; break;
        default:  throw RUNTIME_ERROR("Invalid colour depth");   
    }
    
    // Tiles contain transparency and are stored in a chunked format to save space.
    if (m_compression & RealSpriteRecord::CHUNKED_FORMAT) os << "|chunked";
    
    // grfcodec likes to remove extraneous transparent borders.
    // Is the sense here the right way?
    //if (m_compression & RealSpriteRecord::CROP_TRANSARENT_BORDER) os << "|no_crop";

    // If we create a sprite_sheet containing this sprite, print the details.
    os << ", \"" << m_filename << "\", [" << m_xoff << ", " << m_yoff << "]";

    // This is a 32bpp image with a mask - display the name of the mask file.
    if ((m_colour & HAS_RGB) && (m_colour & HAS_PALETTE))
    {
        os << ", \"" << m_mask_filename << "\", [" << m_mask_xoff << ", " << m_mask_yoff << "]";
    }

    os << ";\n";
}

 
const EnumDescriptorT<RealSpriteRecord::ZoomLevel> zoom_desc = 
{ 
    0x00, "",                   
    {
        { 0x00, "normal" }, // ZoomLevel::Normal },    
        { 0x01, "zin4"   }, // ZoomLevel::ZoomInX4 }, 
        { 0x02, "zin2"   }, // ZoomLevel::ZoomInX2 }, 
        { 0x03, "zout2"  }, // ZoomLevel::ZoomOutX2 }, 
        { 0x04, "zout4"  }, // ZoomLevel::ZoomOutX4 }, 
        { 0x05, "zout8"  }, // ZoomLevel::ZoomOutX8 }, 
    }
};


const BitfieldDescriptorT<uint8_t> colour_desc = 
{ 
    0x00, "",                   
    {
        { 0x04, "8bpp"    }, 
        { 0x01, "24bpp"   }, 
        { 0x03, "32bpp"   }, 
        { 0x04, "mask"    }, 
        { 0x08, "chunked" },
        { 0x40, "no_crop" },
    }
};


void RealSpriteRecord::parse(TokenStream& is) 
{
    // [8, 21, -3, -9] normal M "base_name.normal.0.png" [168, 778];

    is.match(TokenType::OpenBracket);
    m_xdim = is.match_integer();
    is.match(TokenType::Comma);
    m_ydim = is.match_integer();
    is.match(TokenType::Comma);
    m_xrel = is.match_integer();
    is.match(TokenType::Comma);
    m_yrel = is.match_integer();
    is.match(TokenType::CloseBracket);

    zoom_desc.parse(m_zoom, is);
    colour_desc.parse(m_compression, is);

    std::string m_filename = is.match(TokenType::String);

    is.match(TokenType::OpenBracket);
    m_xoff = is.match_integer();
    is.match(TokenType::Comma);
    m_yoff = is.match_integer();
    is.match(TokenType::CloseBracket);

    is.match(TokenType::SemiColon);
}
