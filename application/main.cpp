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
#include <fstream>
#include <iostream>
#include "NewGRFData.h"
#include "Lexer.h"
#include "CommandLineOptions.h"
#include "yagl_version.h" // Generated in a pre-build step.
#include "FileSystem.h"


static void decode()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try 
    {
        // We first create the sub-directory for the output files.
        fs::path yagl_file = options.yagl_file();
        fs::path yagl_dir  = yagl_file.parent_path();
        fs::create_directory(yagl_dir);
        // The spritesheet generator appends various things to this base.
        fs::path image_base = yagl_file;
        image_base.replace_extension();

        std::cout << "Reading GRF:      " << options.grf_file() << "\n";
        std::cout << "Writing YAGL:     " << options.yagl_file() << "\n";
        std::cout << "Output directory: " << yagl_dir.string() << "\n";
        std::cout << "Image base:       " << image_base.string() << "\n" << std::endl;

        // Read in the GRF file ...
        // The GRF file already checked for existence.
        std::cout << "Reading GRF..." << std::endl;
        NewGRFData grf_data;
        std::ifstream is(options.grf_file(), std::ios::binary);
        grf_data.read(is);

        // Write out the YAGL file and associated sprite sheets ...
        std::cout << "Writing YAGL and other files..." << std::endl;
        std::ofstream os(yagl_file, std::ios::binary);
        grf_data.print(os, yagl_dir.string(), image_base.string());
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


static void encode()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try
    {
        // We first create the sub-directory for the output files.
        fs::path yagl_file = options.yagl_file();
        fs::path yagl_dir  = yagl_file.parent_path();
        fs::create_directory(yagl_dir);
        // The spritesheet generator appends various things to this base.
        fs::path image_base = yagl_file;
        image_base.replace_extension();

        std::cout << "Reading YAGL:     " << options.yagl_file() << "\n";
        std::cout << "Writing GRF:      " << options.grf_file() << "\n";
        std::cout << "Source directory: " << yagl_dir.string() << "\n";
        std::cout << "Image base:       " << image_base.string() << "\n" << std::endl;

        // Read in the YAGL file ...
        // This file already checked for existence.
        // Will need to check for the sprite sheets as we go along.
        std::cout << "Lexing YAGL..." << std::endl;
        Lexer lexer;
        std::ifstream is(yagl_file, std::ios::binary);       
        TokenStream token_stream(lexer.lex(is));

        // Parse the YAGL script ...
        std::cout << "Parsing YAGL..." << std::endl;
        NewGRFData grf_data;
        grf_data.parse(token_stream, yagl_dir.string(), image_base.string()); 

        // Back up the GRF before overwriting it ...
        fs::path grf_file = options.grf_file();
        if (fs::is_regular_file(grf_file))
        {
            fs::path bak_file = grf_file;
            bak_file.replace_extension("grf.bak");

            std::cout << "Creating back up GRF: " << grf_file.string() << " => " << bak_file.string() << std::endl;
            fs::rename(grf_file, bak_file);
        } 

        // Write out the GRF file ...
        std::cout << "Writing GRF..." << std::endl;
        std::ofstream os(options.grf_file(), std::ios::binary);
        grf_data.write(os);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


static void hexdump()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try 
    {
        // Read in the GRF file ...
        // The GRF file already checked for existence.
        NewGRFData grf_data;
        std::ifstream is(options.grf_file(), std::ios::binary);    
        grf_data.read(is);
        grf_data.hexdump();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


int main (int argc, char* argv[])
{
    std::cout << "\n";
    std::cout << "yagl (Yet Another GRF Language) " << str_yagl_version << "\n";
    std::cout << "Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)\n";
    std::cout << "Released under GNU General Public License version 3\n" << std::endl;

    CommandLineOptions& options = CommandLineOptions::options();
    options.parse(argc, argv);

    switch (options.operation())
    {
        case CommandLineOptions::Operation::Decode:
            decode();
            break;

        case CommandLineOptions::Operation::Encode:
            encode();
            break;

        case CommandLineOptions::Operation::HexDump:
            hexdump();
            break;
    }

    return 0;
}


