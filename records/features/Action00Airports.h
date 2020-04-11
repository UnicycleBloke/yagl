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
#include "Descriptors.h"
#include <vector>


class Action00Airports : public Action00Feature
{
public:
    Action00Airports() : Action00Feature() {}

    // Binary serialisation
    bool read_property(std::istream& is, uint8_t property) override;
    bool write_property(std::ostream& os, uint8_t property) const override;
    // Text serialisation
    bool print_property(std::ostream& os, uint8_t property, uint16_t indent) const override;
    bool parse_property(TokenStream& is, const std::string& name, uint8_t& index) override;

private:
    UInt8               m_08_airport_override_id{};
    AirportLayouts      m_0A_airport_layouts{};
    YearPair<uint16_t>  m_0C_years_available{};
    UInt8               m_0D_compatible_ttd_airport{};
    UInt8               m_0E_catchment_area{};
    UInt8               m_0F_noise_level{};
    UInt16              m_10_airport_name_id{};
    UInt16              m_11_maintenance_cost_factor{};
};


