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
#include "ChunkEncoder.h"
#include "RealSpriteRecord.h"
#include "CommandLineOptions.h"
#include <exception>


// Each row is represented by a series of chunks which skip transparent sections.
// The format is short (bytes) or long (words), depending on the width of the image.
// The following values used to mark the last chunk for each row.
static constexpr uint16_t SHORT_LAST_CHUNK = 0x0080;
static constexpr uint16_t LONG_LAST_CHUNK  = 0x8000;


class ChunkEncoder
{
public:
    ChunkEncoder(const std::vector<uint8_t>& pixels, uint16_t xdim, uint16_t ydim, 
        uint8_t compression, GRFFormat format);
    std::vector<uint8_t> encode(); 

private:
    struct Chunk 
    { 
        uint16_t length; // in pixels, high bit set if last chunk
        uint16_t offset; // in pixels
    };

private:
    std::vector<uint16_t> find_row_edges(uint16_t y); // y indicates the current row.
    std::vector<Chunk> find_row_chunks(const std::vector<uint16_t>& edges);
    std::vector<uint8_t> make_row_data(const std::vector<Chunk>& chunks, uint16_t y);

private:
    const std::vector<uint8_t>& m_pixels;

    uint16_t  m_xdim;
    uint16_t  m_ydim;
    GRFFormat m_format;
    
    uint16_t  m_pixel_size   = 0; 
    uint16_t  m_trans_offset = 0;
    uint16_t  m_last_chunk   = 0;
};


std::vector<uint8_t> encode_tile(const std::vector<uint8_t>& pixels, uint16_t xdim, uint16_t ydim, 
    uint8_t compression, GRFFormat format)
{
    ChunkEncoder encoder(pixels, xdim, ydim, compression, format);
    return encoder.encode(); 
}


std::vector<uint8_t> decode_tile(const std::vector<uint8_t>& chunks, uint16_t xdim, uint16_t ydim, 
    uint8_t compression, GRFFormat format)
{
    const uint16_t LAST_CHUNK = (xdim > 0x100) ? LONG_LAST_CHUNK : SHORT_LAST_CHUNK;

    bool     long_offset  = chunks.size() > 0x10000;
    uint16_t pixel_size   = 0; 
    uint16_t trans_offset = 0;

    // Interpret the compression information
    if (format == GRFFormat::Container2)
    {
        // Is this format really supported? Have seen examples in 
        // zbase-v5588/zbase_extra.grf. Makes no sense to me.
        if ((compression & RealSpriteRecord::HAS_RGB) && 
            (compression & RealSpriteRecord::HAS_ALPHA) && 
            (compression & RealSpriteRecord::HAS_PALETTE))
        {
            pixel_size   = 5;
            trans_offset = 3;
        }
        // Is it possible to have only RGB? Or are such images converted to 
        // RGBA with all alpha fully opaque?
        else if ((compression & RealSpriteRecord::HAS_RGB) && 
                 (compression & RealSpriteRecord::HAS_ALPHA))
        {
            pixel_size   = 4;
            trans_offset = 3;
        }
        else if (compression & RealSpriteRecord::HAS_PALETTE)
        {
            pixel_size   = 1;
            trans_offset = 0;
        }
    }
    else
    {
        pixel_size   = 1;
        trans_offset = 0;
    }
    
    // Create a array of the lengths for the chunk data in each row. We care about this to more 
    // clearly work out which rows are empty and which are not. It seems that the length of the 
    // first chunk cannot be entirely trusted in the case of zero or full width chunks.
    std::vector<uint32_t> offsets;
    for (uint16_t y = 0; y < ydim; ++y)
    {
        // Index of the row offset in the chunked data's index array. 
        uint32_t index = y * (long_offset ? sizeof(uint32_t) : sizeof(uint16_t));
        // Offset of the first chunk for the current row.
        uint32_t offset = chunks[index++];
        offset |= (chunks[index++] << 8);
        if (long_offset)
        {
            offset |= (chunks[index++] << 16);
            offset |= (chunks[index++] << 24);
        }

        offsets.push_back(offset);
    }
    offsets.push_back(chunks.size());

    // This will contain the decompressed image.
    // Make sure it is all initialised to zeroes.
    std::vector<uint8_t>  output(xdim * ydim * pixel_size, 0);

    for (uint16_t y = 0; y < ydim; ++y)
    {
        // Skips empty rows. These are indicated by rows whose length are the size of one null chunk.
        // An empty row contains a single chunk header (byte/word cinfo and byte/word cofs), but no 
        // further bytes. cinfo should have length zero in this case, but this doesn't seem entirely 
        // reliable.
        uint32_t offset = offsets[y];
        if ((offsets[y+1] - offset) == (long_offset ? sizeof(uint32_t) : sizeof(uint16_t)))
        {
            continue;
        }

        // Now read out the data for each chunk.
        uint32_t chunk_len;
        uint32_t chunk_off;
        bool     is_last_chunk;
        do
        {
            // Access the chunks for the current row.
            // Length of the current chunk, and the flag for last chunk.
            chunk_len = chunks[offset++];
            if (LAST_CHUNK == LONG_LAST_CHUNK)
            {
                chunk_len |= (chunks[offset++] << 8);
            }
            is_last_chunk  = (chunk_len >= LAST_CHUNK);
            chunk_len     &= ~LAST_CHUNK;

            // Row offset for the current chunk.
            chunk_off = chunks[offset++];
            if (LAST_CHUNK == LONG_LAST_CHUNK)
            {
                chunk_off |= (chunks[offset++] << 8);
            }

            // Empty rows still in chunked data. This supposed to be with a length of zero and 
            // the last chunk bit set. dutchtrains.grf seems to have a length of xdim instead. I
            // have used % to get rid of this. Linux builds were fine, but this caused an exception
            // on Windows (correctly). 
            //if (is_last_chunk && (chunk_len == 0)) 
            //{
            //    break;
            //}

            uint32_t imax  = (chunk_len & ~LAST_CHUNK) * pixel_size;
            uint32_t pixel = (y * xdim + chunk_off) * pixel_size;
            for (uint16_t i = 0; i < imax ; ++i) 
            { 
                uint8_t pix = chunks[offset];
                output[pixel] = pix;
                ++pixel;
                ++offset;
            }
        }
        // High bit means this is the last chunk for the current row.
        while (!is_last_chunk);
    }

    return output;
}


ChunkEncoder::ChunkEncoder(const std::vector<uint8_t>& pixels, uint16_t xdim, uint16_t ydim, uint8_t compression, GRFFormat format)
: m_pixels{pixels}
, m_xdim{xdim}
, m_ydim{ydim}
, m_format{format}
{
    // Interpret the compression information
    if (format == GRFFormat::Container2)
    {
        // Is this format really supported? Have seen examples in 
        // zbase-v5588/zbase_extra.grf. Makes no sense to me.
        if ((compression & RealSpriteRecord::HAS_RGB) && 
            (compression & RealSpriteRecord::HAS_ALPHA) && 
            (compression & RealSpriteRecord::HAS_PALETTE))
        {
            m_pixel_size   = 5;
            m_trans_offset = 3;
        }
        // Is it possible to have only RGB? Or are such images converted to 
        // RGBA with all alpha fully opaque?
        else if ((compression & RealSpriteRecord::HAS_RGB) && 
                 (compression & RealSpriteRecord::HAS_ALPHA))
        {
            m_pixel_size   = 4;
            m_trans_offset = 3;
        }
        else if (compression & RealSpriteRecord::HAS_PALETTE)
        {
            m_pixel_size   = 1;
            m_trans_offset = 0;
        }
    }
    else
    {
        m_pixel_size   = 1;
        m_trans_offset = 0;
    }

    m_last_chunk = (m_xdim > 0x100) ? LONG_LAST_CHUNK : SHORT_LAST_CHUNK;
}


std::vector<uint16_t> ChunkEncoder::find_row_edges(uint16_t y)
{
    // Scan a single row of the image data to find edges where pixels transition from 
    // transparent to visible, and vice versa.
    std::vector<uint16_t> edges;

    // Index of the first pixel in the current row.
    uint32_t byte_index  = y * m_xdim * m_pixel_size + m_trans_offset;

    // Scan through the row to find all the edges.
    uint16_t x = 0;
    while (x < m_xdim)
    {
        // Scan through the line to find a visible pixel.
        for (; x < m_xdim; ++x)
        {
            if (m_pixels[byte_index] == 0x00)
            {
                byte_index += m_pixel_size;
                continue;
            }
            edges.push_back(x);
            break;
        }

        // Scan through the line to find a transparent pixel.
        for (; x < m_xdim; ++x)
        {
            if (m_pixels[byte_index] != 0x00)
            {
                byte_index += m_pixel_size;
                continue;
            }
            edges.push_back(x);
            break;
        }
    }
    // Ensure that we have pairs of edges. Each pair represents a 
    // chunk in the row. We will combine chunks with only small gaps.
    if ((edges.size() % 2) == 1)
    {
        edges.push_back(m_xdim);
    }

    return edges;
}


std::vector<ChunkEncoder::Chunk> ChunkEncoder::find_row_chunks(const std::vector<uint16_t>& edges)
{
    // Create a collection of chunks for the current row. 
    std::vector<Chunk> chunks; 
    uint16_t e = 0;
    while (e < edges.size())
    {
        // Set the flag for last chunk pre-emptively.
        uint16_t edge1 = edges[e++];
        uint16_t edge2 = edges[e++];
        while (e < edges.size())
        {
            // Combine chunks which are separated by small gaps
            // of transparent pixels.
            if ((edges[e] - edge2) >= CommandLineOptions::options().chunk_gap())
            {
                break;
            }
            ++e;
            edge2 = edges[e++];
        }

        Chunk chunk;
        chunk.offset = edge1;
        chunk.length = static_cast<uint16_t>(edge2 - edge1); 
        chunks.push_back(chunk);
    }

    if (chunks.size() > 0)
    {
        chunks.back().length |= m_last_chunk;
    }

    return chunks;
}


std::vector<uint8_t> ChunkEncoder::make_row_data(const std::vector<ChunkEncoder::Chunk>& chunks, uint16_t y)
{
    // Created the chunked data for a single row. 
    std::vector<uint8_t> data;
    if (chunks.size() > 0)
    {
        for (const auto& chunk: chunks)
        {
            // Store the length in pixels of the chunk. This may be in short or long format.
            data.push_back(chunk.length & 0xFF);
            if (m_last_chunk == LONG_LAST_CHUNK)
            {
                data.push_back(chunk.length >> 8);
            }

            // Store the offset in pixels from the start of the row. This may be in short or long format.
            data.push_back(chunk.offset & 0xFF);
            if (m_last_chunk == LONG_LAST_CHUNK)
            {
                data.push_back(chunk.offset >> 8);
            }

            // Store the data for the pixels in the chunk.
            uint32_t offset = (y * m_xdim + chunk.offset) * m_pixel_size;  
            uint16_t imax   = chunk.length & ~m_last_chunk;  
            for (uint16_t i = 0; i < imax; ++i)
            {
                for (uint8_t p = 0; p < m_pixel_size; ++p)
                {
                    data.push_back(m_pixels[offset++]);
                }
            }
        }
    }
    else
    {
        // There are no chunks in this line of the image.
        data.push_back(0x80);
        data.push_back(0x00);
        if (m_last_chunk == LONG_LAST_CHUNK)
        {
            data.push_back(0x00);
            data.push_back(0x00);
        }
    }
    

    return data;
}


std::vector<uint8_t> ChunkEncoder::encode()
{
    // Preliminaries: create chunked data for each row in the image.
    uint32_t chunked_size = 0;
    std::vector<std::vector<uint8_t>> chunked_rows;
    for (uint32_t y = 0; y < m_ydim; ++y)
    {
        // Find edges in the row is a preliminary to finding visible chunks.
        std::vector<uint16_t> edges       = find_row_edges(y);
        std::vector<Chunk>    chunks      = find_row_chunks(edges);
        std::vector<uint8_t>  chunked_row = make_row_data(chunks, y);       
        chunked_size += uint32_t(chunked_row.size());
        chunked_rows.push_back(chunked_row);
    }

    bool long_offset = (chunked_size + (m_ydim * 2)) > 0x10000;
    uint32_t offset  = m_ydim * (long_offset ? 4 : 2);

    // This is what we are going to return.
    std::vector<uint8_t> output;

    // First create an array of offsets for the chunked row data.
    // This may use a long or short format depending on the final 
    // length of the output.
    for (const auto& chunked_row: chunked_rows)
    {
        output.push_back(offset & 0xFF);
        output.push_back((offset >> 8) & 0xFF);
        if (long_offset)
        {
            output.push_back((offset >> 16) & 0xFF);
            output.push_back((offset >> 24) & 0xFF);
        }

        offset += uint32_t(chunked_row.size());
    }

    // Then store the chunked data for each row. 
    for (const auto& chunked_row: chunked_rows)
    {
        for (const auto byte: chunked_row)
        {
            output.push_back(byte);
        }
    }

    return output;
}


        
