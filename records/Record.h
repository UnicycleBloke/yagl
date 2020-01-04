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
#include "Lexer.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <array>


// Record types are distinct from action types because there are several
// different record structures which are called Action 02, and there are
// some additional records which are not called actions.
enum class RecordType
{
    RECORD_COUNT,
    ACTION_00,
    ACTION_01,
    ACTION_02_BASIC,
    ACTION_02_RANDOM,
    ACTION_02_VARIABLE,
    ACTION_02_INDUSTRY,
    ACTION_02_SPRITE_LAYOUT,
    ACTION_03,
    ACTION_04,
    ACTION_05,
    ACTION_06,
    ACTION_07,
    ACTION_08,
    ACTION_09,
    ACTION_0A,
    ACTION_0B,
    ACTION_0C,
    ACTION_0D,
    ACTION_0E,
    ACTION_0F,
    ACTION_10,
    ACTION_11,
    ACTION_12,
    ACTION_13,
    ACTION_14,
    ACTION_FE, // Used for sounds imported from other GRFs
    ACTION_FF, // Used for data blocks (sound effects after an Action 11)
    SPRITE_INDEX,
    REAL_SPRITE,
    FAKE_SPRITE,
    RECOLOUR,
    NONE
};
std::string RecordName(RecordType type);
RecordType RecordFromName(const std::string& name);


enum class FeatureType : uint8_t
{
    Trains          = 0x00,
    Vehicles        = 0x01,
    Ships           = 0x02,
    Aircraft        = 0x03,
    Stations        = 0x04,
    Canals          = 0x05,
    Bridges         = 0x06,
    Houses          = 0x07,
    GlobalSettings  = 0x08,
    IndustryTiles   = 0x09,
    Industries      = 0x0A,
    Cargos          = 0x0B,
    SoundEffects    = 0x0C,
    Airports        = 0x0D,
    Signals         = 0x0E,
    Objects         = 0x0F,
    Railtypes       = 0x10,
    AirportTiles    = 0x11,
    RoadTypes       = 0x12,
    TramTypes       = 0x13,
    OriginalStrings = 0x48,
};
std::string FeatureName(FeatureType type);
FeatureType FeatureFromName(const std::string& name);


enum class NewFeatureType : uint8_t
{
    PreSignal        = 0x04,
    Catenary         = 0x05,
    Foundations      = 0x06,
    TTDPatchGUI      = 0x07,
    Canals           = 0x08,
    OneWayArrows     = 0x09,
    TwoCompanyColour = 0x0A,
    TramTracks       = 0x0B,
    SnowyTrees       = 0x0C,
    CoastTiles       = 0x0D,
    NewSignals       = 0x0E,
    SlopeTrackMarks  = 0x0F,
    AirportExtra     = 0x10,
    RoadStops        = 0x11,
    Aqueducts        = 0x12, 
    AutoRail         = 0x13,
    Flags            = 0x14,
    OpenTTDGUI       = 0x15,
    AirportPreview   = 0x16,
    RailTypeTunnel   = 0x17,
    ExtraAllBlack    = 0x18,
};
std::string NewFeatureName(NewFeatureType type);
NewFeatureType NewFeatureFromName(const std::string& name);


enum class GRFFormat 
{ 
    Invalid,
    Container1, 
    Container2 
};


// Supported version of the NewGRF specs.
// Need to account for binary differences.
enum class GRFVersion 
{ 
    //GRF0 = 0,
    //GRF1 = 1,
    //GRF2 = 2,
    //GRF3 = 3,
    //GRF4 = 4,
    //GRF5 = 5,
    //GRF6 = 6,
    //GRF7 = 7,
    GRF8 = 8,
};


// Information useful globally when reading and writing data for records.
struct GRFInfo
{
    GRFFormat  format;  // Container format
    GRFVersion version; // Version of the NewGRF spe
};


class NewGRFSpriteAppender;
class Record;
using SpriteZoomVector = std::vector<std::shared_ptr<Record>>;
using SpriteZoomMap    = std::map<uint32_t, SpriteZoomVector>;


class Record
{
private:
    RecordType m_record_type;

public:
    Record(RecordType record_type = RecordType::NONE)
    : m_record_type{record_type}
    {
    }    

    virtual ~Record() {}

    // Binary serialisation
    virtual void read(std::istream& is, const GRFInfo& info) {};
    virtual void write(std::ostream& os, const GRFInfo& info) const {}; 
    // Text serialisation
    virtual void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const {};
    virtual void parse(TokenStream& is) {};

    // These methods are for adding sprites to a container action.
    virtual void append_sprite(std::shared_ptr<Record> record) { throw RUNTIME_ERROR("append_sprite"); }
    virtual uint16_t num_sprites_to_read() const { return 0; }
    virtual uint16_t num_sprites_to_write() const { return 0; }
    virtual std::shared_ptr<Record> get_sprite(uint16_t index) const { return nullptr; }

    RecordType record_type() const { return m_record_type; }
};


// Base for all Actions. 
class ActionRecord : public Record
{
public:
    using Record::Record;
    virtual ~ActionRecord() {}

    // Binary serialisation
    //void read(std::istream& is, const GRFInfo& info) override {};
    void write(std::ostream& os, const GRFInfo& info) const override; 
    // Text serialisation
    //void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override {};
    //void parse(TokenStream& is, NewGRFSpriteAppender& appender) override {};
};


// Only some Actions are containers: 01, 05, 0A, 11 and 12.
class ContainerRecord : public ActionRecord
{
public:
    using ActionRecord::ActionRecord;
    virtual ~ContainerRecord() {}

    // Binary serialisation
    //void read(std::istream& is, const GRFInfo& info) override {};
    //void write(std::ostream& os, const GRFInfo& info) const override; 
    // Text serialisation
    //void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override {};
    //void parse(TokenStream& is, NewGRFSpriteAppender& appender) override {};

    void append_sprite(std::shared_ptr<Record> record) override;
    std::shared_ptr<Record> get_sprite(uint16_t index) const override 
        { return (index < num_sprites_to_write()) ? m_sprites[index] : nullptr; }
    virtual uint16_t num_sprites_to_write() const { return m_sprites.size(); }

protected:    
    void print_sprite(uint16_t index, std::ostream& os, 
        const SpriteZoomMap& sprites, uint16_t indent) const;
    void parse_sprite(TokenStream& is);

private:
    std::vector<std::shared_ptr<Record>> m_sprites;            
};


