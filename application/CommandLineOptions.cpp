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
#include "CommandLineOptions.h"
#include "Exceptions.h"
#include "FileSystem.h"
#include "yagl_version.h" // Generated in a pre-build step.
#include <iostream>


// Singleton implementation.
CommandLineOptions& CommandLineOptions::options()
{
    static CommandLineOptions opts;
    return opts;
}


void CommandLineOptions::parse(int argc, char* argv[])
{
    // There must a nicer way to handle an enumeration.
    bool     decode  = false;
    bool     encode  = false;
    bool     hexdump = false;

    uint16_t palette = 1;
    uint16_t format  = 2;

    try
    {
        cxxopts::Options options(argv[0], "yagl: a tool for decoding and encoding GRF files");
        options
            .positional_help("<grf_file> [<yagl_dir>]")
            .show_positional_help()
            //.allow_unrecognised_options()
            .add_options()
            // These are mutually exclusive
            ("d,decode",    "Decodes a GRF file to YAGL script and sprite sheets", cxxopts::value<bool>(decode))
            ("e,encode",    "Encodes a GRF file from YAGL script and sprite sheets", cxxopts::value<bool>(encode))
            ("x,hexdump",   "Reads a GRF file and dumps it to hex somewhat like NFO", cxxopts::value<bool>(hexdump))

            // Other options
            ("p,palette",   "Choose the initial palette for the GRF", cxxopts::value<uint16_t>(palette), "<idx>")
            ("w,width",     "Maximum width of sprite sheets", cxxopts::value<uint16_t>(m_width), "<num>")
            ("h,height",    "Maximum height of sprite sheets", cxxopts::value<uint16_t>(m_height), "<num>")
            ("v,version",   "Print version information")
            ("help",        "Print help")

            // A little debugging support
            ("g,debug",     "Debug trace output", cxxopts::value<bool>(m_debug), "<idx>")

            // This is required
            ("grf_file",    "Path to the GRF file to decode/encode", cxxopts::value<std::string>(m_grf_file))
            // This is optional - defaults to 'sprites'
            ("yagl_dir",    "Name of the sub-folder for YAGL script and sprite sheets", cxxopts::value<std::string>(m_yagl_dir), "sprites");
        options.parse_positional({"grf_file", "yagl_dir"});

        auto result = options.parse(argc, argv);

        // Print help about the command line options, and then exit.
        if (result.count("help"))
        {
            std::cout << options.help({""}) << '\n';
            exit(0);
        }

        // Print the version number of the yagl executable, and then exit.
        if (result.count("version"))
        {
            std::cout << str_yagl_version << "\n";
            exit(0);
        }

        // Make sure that one and only one operation is selected.
        uint16_t operation = decode + encode + hexdump;
        if (operation > 1)
        {
            std::cout << "ERROR: The --encode,-e and --decode,-d and --hexdump,-x options are mutually exclusive\n";
            exit(1);
        }
        if (operation == 0)
        {
            std::cout << "ERROR: One of the --encode,-e and --decode,-d or --hexdump,-x options is required\n";
            exit(1);
        }
        if (encode)  m_operation = Operation::Encode;
        if (decode)  m_operation = Operation::Decode;
        if (hexdump) m_operation = Operation::HexDump;

        // Make sure that we have GRF file.
        if (!result.count("grf_file"))
        {
            std::cout << "ERROR: The name of a GRF file is a required argument\n";
            exit(1);
        }
                
        // GRF is in '<some_path>/<some_file>.grf'. 
        // YAGL is in '<some_path>/<yagl_dir>/<some_file>.yagl'. 
        fs::path grf_file  = m_grf_file;
        fs::path yagl_file = grf_file.parent_path().append(m_yagl_dir);
        yagl_file.append(grf_file.filename().string()).replace_extension(".yagl");
        m_yagl_file = yagl_file.string();

        if (m_operation == Operation::Decode) 
        {
            if (!fs::is_regular_file(m_grf_file)) 
            {
                std::cout << "ERROR: File '" << m_grf_file << "' does not exist\n";
                exit(1);
            }
        }
        else if (m_operation == Operation::Encode) 
        {
            if (!fs::is_regular_file(m_yagl_file)) 
            {
                std::cout << "ERROR: File '" << m_yagl_file << "' does not exist\n";
                exit(1);
            }
        }

        switch (palette)
        {
            case 1: m_palette = PaletteType::Default;        break;
            case 2: m_palette = PaletteType::DOS;            break;
            case 3: m_palette = PaletteType::Windows;        break;
            case 4: m_palette = PaletteType::DOSToyland;     break;
            case 5: m_palette = PaletteType::WindowsToyland; break;
            default:
                std::cout << "ERROR: Invalid palette index. Permitted values are 1, 2, 3, 4 and 5.\n";
                exit(1);
        }
    } 
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "ERROR: Error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}

