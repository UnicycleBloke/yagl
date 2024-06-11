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
#include <string>
#include "cxxopts.hpp"
#include "Palettes.h"
#include "Record.h"


// A simple singleton so that we can more easily access the command line options
// from everywhere in the software, if we need to.
class CommandLineOptions
{
    public:
        enum class Operation { Decode, Encode, HexDump, Info };

    public:
        void parse(int argc, char* argv[]);
        static CommandLineOptions& options();

        Operation          operation()  const { return m_operation; }

        const std::string& grf_file()   const { return m_grf_file; }
        const std::string& yagl_dir()   const { return m_yagl_dir; }
        const std::string& yagl_file()  const { return (m_curr_file.size() > 0) ? m_curr_file : m_yagl_file; }
        const std::string& hex_file()   const { return m_hex_file; }
        const std::string& image_base() const { return m_image_base; }
        const std::string& info_item()  const { return m_info_item; }

        uint32_t           width()      const { return m_width; }
        uint32_t           height()     const { return m_height; }
        PaletteType        palette()    const { return m_palette; }
        uint8_t            chunk_gap()  const { return m_chunk_gap; }

        bool               debug()      const { return m_debug; }
        const std::string& test_args()  const { return m_test_args; }

        // A bit of a bodge - see below.
        void set_curr_file(const std::string file) { m_curr_file = file; }

    private:
        CommandLineOptions() {}

    private:
        // Required arguments.
        Operation   m_operation{};
        std::string m_grf_file;
        std::string m_test_args;

        // Optional arguments.
        uint16_t    m_width     = 800;                    // Max width of spritesheets
        uint16_t    m_height    = 16'000;                 // Max height of spritesheets
        PaletteType m_palette   = PaletteType::Default;
        uint8_t     m_chunk_gap = 3;                      // Join chunks in tiles gaps smaller than is.
        std::string m_info_item;

        // Calculated from m_grf_file and m_yagl_dir.
        std::string m_yagl_dir  = "sprites";
        std::string m_yagl_file;
        std::string m_hex_file;
        std::string m_image_base;

        // Current source file as found while parsing the YAGL.
        // Putting it into the options is a bit of a bodge but makes
        // access simple for parser exceptions.
        std::string m_curr_file;

        // Used for debugging
        bool        m_debug    = false;
};

