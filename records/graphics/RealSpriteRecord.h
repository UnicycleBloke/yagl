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
#pragma once 
#include "Record.h"
#include <vector>


class RealSpriteRecord : public Record
{
public:
    // Compression bits for container format 1 - see grf.txt for details
    static constexpr uint8_t INDEX_0_IS_TRANSPARENT = 0x01; // Refers to palette index - should always be 
                                                            // set - ignored if CHUNKED_FORMAT is set
    static constexpr uint8_t COMPRESSED_IN_MEMORY   = 0x02; // Not really sure what this does

    // Colour depth bits for Container2 files - see grf.txt for details
    // Note that a sprite can have both RGA[A] and P bits set. Not sure that this is a valid PNG format, but 
    // two images (of the same size) are combined to create the sprite data. The palette is used for company
    // colours, glass effect, and other things. 
    static constexpr uint8_t HAS_RGB                = 0x01; // Three byte of colour information for each pixel 
    static constexpr uint8_t HAS_ALPHA              = 0x02; // One byte of alpha information for each pixel
                                                            // 0x00 means the pixel is transparent
    static constexpr uint8_t HAS_PALETTE            = 0x04; // One byte palette index for each pixel

    // Compression bits common to both Container1 and Container2 formats
    static constexpr uint8_t CHUNKED_FORMAT         = 0x08; // Indicates a tile sprite - use chunked format to
                                                            // compress transparent bits before (or in place of?) 
                                                            // LZ77 compression
    static constexpr uint8_t CROP_TRANSARENT_BORDER = 0x40; // Ignore for now - grfcodec trims extraneous transparent
                                                            // borders from sprites

    // Zoom levels supported by OpenTTD.
    enum class ZoomLevel : uint8_t 
    {
        Normal      = 0,
        ZoomInX4    = 1,
        ZoomInX2    = 2,
        ZoomOutX2   = 3,
        ZoomOutX4   = 4,
        ZoomOutX8   = 5
    };

    //enum class ColourDepth : uint8_t
    //{ 
    //    Palette = HAS_PALETTE, 
    //    RGB     = HAS_RGB, 
    //    RGBA    = RGB | HAS_ALPHA 
    //};

public:
    RealSpriteRecord(uint32_t sprite_id, uint32_t size, uint8_t compression)
    : Record{RecordType::REAL_SPRITE}
    , m_sprite_id{sprite_id}
    , m_size{size}
    , m_compression{compression}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;   
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is) override;

    uint32_t    sprite_id() const   { return m_sprite_id; }
    ZoomLevel   zoom() const        { return m_zoom; }
    uint8_t     colour() const      { return m_colour; } 
    uint8_t     compression() const { return m_compression; }

    uint16_t  xdim() const { return m_xdim; }
    uint16_t  ydim() const { return m_ydim; }

    uint16_t  xoff() const { return m_xoff; }
    uint16_t  yoff() const { return m_yoff; }

    uint16_t  mask_xoff() const { return m_mask_xoff; }
    uint16_t  mask_yoff() const { return m_mask_yoff; }

    // Only some of the members are filled, depending on the image colour depth, but this
    // provides a simple common API for the spritesheet generator.
    struct Pixel
    {
        uint8_t red; 
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        uint8_t index; 
    };
    Pixel pixel(uint32_t x, uint32_t y) const;
    void  set_pixel(uint32_t x, uint32_t y, const Pixel& pix);
    
    void set_xoff(uint16_t offset) { m_xoff = offset; }
    void set_yoff(uint16_t offset) { m_yoff = offset; }
    void set_filename(const std::string& filename) { m_filename = filename; }

    // Only necessary for RGB[A]P sprites which contain both sprite and mask.
    void set_mask_xoff(uint16_t offset) { m_mask_xoff = offset; }
    void set_mask_yoff(uint16_t offset) { m_mask_yoff = offset; }
    void set_mask_filename(const std::string& filename) { m_mask_filename = filename; }

private:
    void write_format1(std::ostream& os) const;
    void write_format2(std::ostream& os) const;     
    
    std::vector<uint8_t> encode_lz77(const std::vector<uint8_t>& input) const;

    // Check whether a pixel is pure white - we warn about this, and perhaps fix.
    void check_pixel(Pixel& pixel);

private:
    // This information is read before working out what type of record we have,
    // as is passed to the constructor.
    uint32_t  m_sprite_id   = 0;
    uint32_t  m_size        = 0; // Needed only for reading the GRF, I think.
    uint8_t   m_compression = 0; // Contains colour depth/type information - depends on container format. 
    uint8_t   m_colour      = 0; // Derived from m_compression but does not depend on container format.

    ZoomLevel m_zoom        = ZoomLevel::Normal;

    uint16_t  m_ydim        = 0;
    uint16_t  m_xdim        = 0;
    int16_t   m_xrel        = 0;
    int16_t   m_yrel        = 0;
    uint32_t  m_uncomp_size = 0;

    uint16_t  m_xoff        = 0;
    uint16_t  m_yoff        = 0;
    std::string m_filename;

    uint16_t  m_mask_xoff        = 0;
    uint16_t  m_mask_yoff        = 0;
    std::string m_mask_filename;

    std::vector<uint8_t> m_pixels = {};
    bool m_has_pure_white         = false;
};
