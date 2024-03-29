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
#include "GRFStrings.h"


// Generate error messages.
// With this action, you can alert the user to problems in the way
// the grf file is loaded, e.g. not in the right order, not the right
// patch version, or wrong parameters.
class Action0BRecord : public ActionRecord
{
public:
    Action0BRecord()
    : ActionRecord{RecordType::ACTION_0B}
    {
    }

    // Binary serialisation
    void read(std::istream& is, const GRFInfo& info) override;
    void write(std::ostream& os, const GRFInfo& info) const override;
    // Text serialisation
    void print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const override;
    void parse(TokenStream& is, SpriteZoomMap& sprites) override;

public:
    enum class Severity
    {
        Notice  = 0,
        Warning = 1,
        Error   = 2,
        Fatal   = 3
    };

private:
    Severity    m_severity          = Severity::Warning;
    bool        m_apply_during_init = false; // High bit in severity byte
    uint8_t     m_language_id       = 0x7F;  // Same as Action04
    uint8_t     m_message_id        = 0;     // Some built-in, 0xFF for custom
    GRFString   m_message;                   // Only present if 0xFF ID.
    GRFString   m_data;                      // Inserted at second 0x80 in the string.
    uint8_t     m_num_params        = 0;     // Optional up to two.
    uint8_t     m_param1            = 0;
    uint8_t     m_param2            = 0;
};
