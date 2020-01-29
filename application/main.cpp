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
        if (options.debug()) 
        {
            std::cout << "Reading GRF: " << options.grf_file() << '\n';
            std::cout << "Writing YAGL: " << options.yagl_file() << '\n';
        }

        NewGRFData grf_data;
        // This file already checked for existence.
        std::ifstream is(options.grf_file(), std::ios::binary);    
        grf_data.read(is);

        // We first create the sub-directory for the output files.
        fs::path yagl_file = options.yagl_file();
        fs::path yagl_dir  = yagl_file.parent_path();
        fs::create_directory(yagl_dir);

        // The spritesheet generator appends various things to this base.
        fs::path image_base = yagl_file;
        image_base.replace_extension();
        
        if (options.debug()) 
        {
            std::cout << "Output directory: " << yagl_dir.string() << '\n';
            std::cout << "Image base: " << image_base.string() << '\n';
        }

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

        // This file already checked for existence.
        // Will need to check for the sprite sheets as we go along.
        std::ifstream is(yagl_file, std::ios::binary);
        
        Lexer lexer;
        TokenStream token_stream(lexer.lex(is));

        NewGRFData grf_data;
        grf_data.parse(token_stream, yagl_dir.string(), image_base.string()); 

        //std::ofstream os(options.grf_file(), std::ios::binary);
        //grf_data.write(os);

        fs::path yagl_file2  = options.yagl_file();
        fs::path image_base2 = yagl_file2;
        yagl_file2.replace_extension("yagl2");
        image_base2.replace_extension("images2");

        std::ofstream os(yagl_file2, std::ios::binary);
        grf_data.print(os, yagl_dir.string(), image_base2.string());
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


