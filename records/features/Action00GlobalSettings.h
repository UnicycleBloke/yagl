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
#include "properties2/IntegerValue.h"
#include "GRFStrings.h"
#include "properties2/GenderCase.h"
#include "properties2/SnowLine.h"


using GRFLabelProperty     = Property<GRFLabel>;
using SnowLineProperty     = Property<SnowLine>;
using GenderCaseProperty   = Property<GenderCase>;
using GRFLabelPairProperty = Property<Array<GRFLabel, 2>>;


class Action00GlobalSettings : public Action00Feature
{
public:
    Action00GlobalSettings() : Action00Feature(FeatureType::GlobalSettings) {}

private:
    // The global properties are organised a little oddly in the GRF file. The
    // ID of the properties is not so much an instance as an index. So, for example,
    // The cargo translation table is an array of GRFLabel objects (uint32_t), but only
    // a single GRFLabel in this class. The file contains an Action00 for global
    // settings for one property but multiple instances. The instances represent the
    // entries in the cargo table.

    // TTD has 49 base costs (66 in OpenTTD currently) which govern how much everything costs.
    UInt8Property m_prop_08{m_properties, 0x08, "cost_base_multipliers"};

    // Translates a GRFLable, such as "MAIL" into an index so that different GRFs can work together.
    // Note that this property cannot be set incrementally, you must set all types in a single
    // action 0 starting from ID 0.
    GRFLabelProperty m_prop_09{m_properties, 0x09, "cargo_translation_table"};

    // This and the following properties can be used to modify currencies. Each of them
    // can have IDs 0-18 (decimal), the IDs being ordered the same as in the Currency
    // drop-down list. This property allows changing currency names that are displayed in the
    // Currency drop-down in the Game Options window. This property is a textID, and if you need
    // to supply your own text, it must be a DCxx one.
    UInt16Property m_prop_0A{m_properties, 0x0A, "currency_display_names"};
    // The equivalent of 1 British pound in this currency, multiplied by 1000.
    // For example, 1 GBP=2 USD, so this should be 2000 for US dollars.
    UInt32Property m_prop_0B{m_properties, 0x0B, "currency_multipliers"};
    // The low byte of this word specifies the thousands separator to be used for this currency
    // (usually dot "." or comma ","). The high byte should be zero if the currency symbol
    // should be in front of the number ($123,456) and should be 1 if the currency symbol should
    // be shown after the number (123,456$).
    UInt16Property m_prop_0C{m_properties, 0x0C, "currency_options"};
    // These doublewords are interpreted as a string of up to 4 characters. If you need fewer
    // characters, the remaining bytes should be zero.
    GRFLabelProperty m_prop_0D{m_properties, 0x0D, "currency_symbols_prefix"};
    GRFLabelProperty m_prop_0E{m_properties, 0x0E, "currency_symbols_suffix"};
    // This value allows you to have Euro introduced instead the currency at a given time. If this
    // value is zero, the currency is never substituted with the Euro (USD, for example). If it's
    // nonzero, it gives the year when the currency is replaced by Euro (for example, 2002 for DM).
    UInt16Property m_prop_0F{m_properties, 0x0F, "euro_introduction_dates"};

    // This property allows you to specify the snow line height for every day of the year. The
    // only ID you can set is 0, and the value must be 12*32=384 bytes long. To simplify things
    // for the patch, every month has 32 entries, and the impossible combinations (like 32th January
    // or 31th April) will never be read.
    SnowLineProperty m_prop_10{m_properties, 0x10, "snow_line_table"};

    // Allows you to provide a list of 'source' and 'target' GRFIDs to let vehicles in the source
    // GRF override those in the target GRF, when dynamic engines is enabled.
    GRFLabelPairProperty m_prop_11{m_properties, 0x11, "grf_overrides"};

    // Provides ability to specify rail types via a translation table, similar to using a cargo
    // translation table.
    GRFLabelProperty m_prop_12{m_properties, 0x12, "railtype_translation_table"};

    // Provides ability to specify genders or cases via a translation table. These map NewGRF
    // internal IDs for the genders or cases to the genders or cases as defined in OpenTTD's
    // language files so NewGRF strings and OpenTTD strings can interact on eachother's gender
    // or cases.
    GenderCaseProperty m_prop_13{m_properties, 0x13, "gender_translation_table"};
    GenderCaseProperty m_prop_14{m_properties, 0x14, "case_translation_table"};

    // Defines the plural form for a language. The ID used is the Action 4 (GRF version 7 or higher)
    // language-id, i.e. this only works with GRF version 7 or higher. Language-id 7F (any) is
    // not allowed.
    UInt8Property m_prop_15{m_properties, 0x15, "plural_form"};

    // These work in much the same way as the cargo translation table and the railtype translation 
    // table. Added in OTTD v1.10.
    GRFLabelProperty m_prop_16{m_properties, 0x16, "roadtype_translation_table"};
    GRFLabelProperty m_prop_17{m_properties, 0x17, "tramtype_translation_table"};
};


