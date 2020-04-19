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
#include "Action00Feature.h"
#include "IntegerDescriptor.h"
#include "GRFLabel.h"
#include <vector>


class StationTileData
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:    
    bool     m_new_bb; ///< Indicates that this sprite has its own bounding box
    int8_t   m_x_off;
    int8_t   m_y_off;
    uint8_t  m_z_off;
    uint8_t  m_x_ext;
    uint8_t  m_y_ext;
    uint8_t  m_z_ext;
    uint32_t m_sprite;
};


class StationTile
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:    
    uint32_t m_ground_sprite;
    std::vector<StationTileData> m_data;
};


class StationLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    std::vector<StationTile> m_tiles;
};


class CustomLayout
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

    bool terminator() const { return (m_platform_count == 0) && (m_platform_length == 0); }

public:
    enum class Platform 
    {
        Plain     = 0x00, 
        Building  = 0x02, 
        RoofLeft  = 0x04, 
        RoofRight = 0x06
    }; 

private:    
    uint8_t m_platform_length;
    uint8_t m_platform_count;
    // length * count, All bytes 00, 02, 04 or 06
    std::vector<Platform> m_platform_tiles; 
};


class CustomStation
{
public:
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void print(std::ostream& os, uint16_t indent) const;
    void parse(TokenStream& is);

private:
    std::vector<CustomLayout> m_layouts;
};


class Action00Stations : public Action00Feature
{
public:
    Action00Stations() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    GRFLabel       m_08_class_id{};
    StationLayout  m_09_sprite_layout{};
    UInt8          m_0A_copy_sprite_layout_id{};
    UInt8          m_0B_callback_flags{};
    UInt8          m_0C_disabled_platform_numbers{};
    UInt8          m_0D_disabled_platform_lengths{};
    CustomStation  m_0E_custom_layout{};
    UInt8          m_0F_copy_custom_layout_id{};
    UInt16         m_10_little_lots_threshold{};
    UInt8          m_11_pylon_placement{};
    UInt32         m_12_cargo_type_triggers{};
    UInt8          m_13_general_flags{};
    UInt8          m_14_overhead_wire_placement{};
    UInt8          m_15_can_train_enter_tile{};
    UInt16         m_16_animation_info{};
    UInt8          m_17_animation_speed{};
    UInt16         m_18_animation_triggers{};

    // TODO 
    //Action00StationRouting  m_19_station_routing;
    //Action00StationAdvanced m_1A_station_advanced; - like Action02SpriteLayout?

};


