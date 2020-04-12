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
#include "GRFStrings.h"
#include <array>
#include <vector>


class Action00GlobalSettings : public Action00Feature
{
public:
    Action00GlobalSettings() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

public:
    class SnowLine
    {
    public:
        // Binary serialisation
        void read(std::istream& is);
        void write(std::ostream& os) const;
        // Text serialisation
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);

    private:
        static constexpr uint32_t SNOW_LINE_SIZE = 32U * 12;
        std::array<uint8_t, SNOW_LINE_SIZE> m_snow_heights;
    };

public:
    class GenderCase
    {
    public:
        // Binary serialisation
        void read(std::istream& is);
        void write(std::ostream& os) const;
        // Text serialisation
        void print(std::ostream& os, uint16_t indent) const;
        void parse(TokenStream& is);

    private:
        // The ID used for these translation tables is the Action 4 (GRF version 7 or higher) 
        // language-id, i.e. this mapping only works with GRF version 7 or higher. Language-id 7F 
        // (any) is not allowed. You can can define an ID multiple times in which case the new 
        // mappings are simply appended to the already known mappings.     
        struct Item
        {
            uint8_t     id;
            std::string name;
        };

        std::vector<Item> m_items;
    };

private:
    // The global properties are organised a little oddly in the GRF file. The 
    // ID of the properties is not so much an instance as an index. So, for example,
    // The cargo translation table is an array of GRFLabel objects (uint32_t), but only
    // a single GRFLabel in this class. The file contains an Action00 for global
    // settings for one property but multiple instances. The instances represent the 
    // entries in the cargo table.

    // TTD has 49 base costs (66 in OpenTTD currently) which govern how much everything costs.
    UInt8 m_08_cost_base_multipliers{};

    // Translates a GRFLable, such as "MAIL" into an index so that different GRFs can work together.
    // Note that this property cannot be set incrementally, you must set all types in a single 
    // action 0 starting from ID 0. 
    GRFLabel m_09_cargo_translation_table{};

    // This and the following properties can be used to modify currencies. Each of them 
    // can have IDs 0-18 (decimal), the IDs being ordered the same as in the Currency 
    // drop-down list. This property allows changing currency names that are displayed in the 
    // Currency drop-down in the Game Options window. This property is a textID, and if you need 
    // to supply your own text, it must be a DCxx one. 
    UInt16 m_0A_currency_display_names{};
    // The equivalent of 1 British pound in this currency, multiplied by 1000. 
    // For example, 1 GBP=2 USD, so this should be 2000 for US dollars. 
    UInt32 m_0B_currency_multipliers{};
    // The low byte of this word specifies the thousands separator to be used for this currency 
    // (usually dot "." or comma ","). The high byte should be zero if the currency symbol 
    // should be in front of the number ($123,456) and should be 1 if the currency symbol should 
    // be shown after the number (123,456$).
    UInt16 m_0C_currency_options{}; 
    // These doublewords are interpreted as a string of up to 4 characters. If you need fewer 
    // characters, the remaining bytes should be zero. 
    GRFLabel m_0D_currency_symbols_prefix{};
    GRFLabel m_0E_currency_symbols_suffix{};
    // This value allows you to have Euro introduced instead the currency at a given time. If this 
    // value is zero, the currency is never substituted with the Euro (USD, for example). If it's 
    // nonzero, it gives the year when the currency is replaced by Euro (for example, 2002 for DM). 
    UInt16 m_0F_euro_introduction_dates{};

    // This property allows you to specify the snow line height for every day of the year. The 
    // only ID you can set is 0, and the value must be 12*32=384 bytes long. To simplify things 
    // for the patch, every month has 32 entries, and the impossible combinations (like 32th January 
    // or 31th April) will never be read. 
    SnowLine m_10_snow_line_table{};

    // Allows you to provide a list of 'source' and 'target' GRFIDs to let vehicles in the source 
    // GRF override those in the target GRF, when dynamic engines is enabled. 
    GRFLabelPair m_11_grf_overrides{};

    // Provides ability to specify rail types via a translation table, similar to using a cargo 
    // translation table. 
    GRFLabel m_12_railtype_translation_table{};

    // Provides ability to specify genders or cases via a translation table. These map NewGRF 
    // internal IDs for the genders or cases to the genders or cases as defined in OpenTTD's 
    // language files so NewGRF strings and OpenTTD strings can interact on eachother's gender 
    // or cases. 
    GenderCase m_13_gender_translation_table{};
    GenderCase m_14_case_translation_table{};

    // Defines the plural form for a language. The ID used is the Action 4 (GRF version 7 or higher) 
    // language-id, i.e. this only works with GRF version 7 or higher. Language-id 7F (any) is 
    // not allowed. 
    UInt8 m_15_plural_form{};
};


