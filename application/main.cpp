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

        if (options.debug()) 
        {
            std::cout << "Reading GRF:      " << options.grf_file() << '\n';
            std::cout << "Writing YAGL:     " << options.yagl_file() << '\n';
            std::cout << "Output directory: " << yagl_dir.string() << '\n';
            std::cout << "Image base:       " << image_base.string() << '\n';
        }

        // Read in the GRF file ...
        // The GRF file already checked for existence.
        NewGRFData grf_data;
        std::ifstream is(options.grf_file(), std::ios::binary);    
        grf_data.read(is);

        // Write out the YAGL file and associated sprite sheets ...
        std::ofstream os(yagl_file, std::ios::binary);
        grf_data.print(os, yagl_dir.string(), image_base.string());

        // Sometimes useful to check that GRF --read--> MEMORY --write--> GRF results in identical 
        // files - there are actually some harmless sources of discrepancies, which is a bit of a pain.
        //std::ofstream os2(options.grf_file() + "2", std::ios::binary);
        //grf_data.write(os2);

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

        if (options.debug()) 
        {
            std::cout << "Reading YAGL:     " << options.yagl_file() << '\n';
            std::cout << "Writing GRF:      " << options.grf_file() << '\n';
            std::cout << "Source directory: " << yagl_dir.string() << '\n';
            std::cout << "Image base:       " << image_base.string() << '\n';
        }

        // Read in the YAGL file ...
        // This file already checked for existence.
        // Will need to check for the sprite sheets as we go along.
        Lexer lexer;
        std::ifstream is(yagl_file, std::ios::binary);       
        TokenStream token_stream(lexer.lex(is));

        // Parse the YAGL script ...
        NewGRFData grf_data;
        grf_data.parse(token_stream, yagl_dir.string(), image_base.string()); 

        // Back up the GRF before overwriting it ...
        fs::path grf_file = options.grf_file();
        if (fs::is_regular_file(grf_file))
        {
            fs::path bak_file = grf_file;
            bak_file.replace_extension("grf.bak");
            fs::rename(grf_file, bak_file);
        } 

        // Write out the GRF file ...
        std::ofstream os(options.grf_file(), std::ios::binary);
        grf_data.write(os);

        // Debug test which reproduces the source YAGL and images.
        // fs::path yagl_file2  = options.yagl_file();
        // fs::path image_base2 = yagl_file2;
        // yagl_file2.replace_extension("yagl2");
        // image_base2.replace_extension("images2");

        // std::ofstream os(yagl_file2, std::ios::binary);
        // grf_data.print(os, yagl_dir.string(), image_base2.string());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


int main (int argc, char* argv[])
{
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
    }
}


