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


static void decode(const std::string& grf_file, const std::string& yagl_dir)
{
    //try 
    {
        NewGRFData grf_data;
        std::ifstream is(grf_file, std::ios::binary);    
        grf_data.read(is);

        // GRF is in '<some_path>/<some_file>.grf'. 
        // We first create the directory '<some_path>/<yagl_dir>'
        fs::path grf_path  = grf_file;
        fs::path yagl_path = grf_path.parent_path().append(yagl_dir);
        fs::create_directory(yagl_path);

        // The YAGL file path is '<some_path>/<yagl_dir>/<some_file>.yagl'
        fs::path yagl_file = yagl_path;
        yagl_file.append(grf_path.filename().string()).replace_extension(".yagl");

        // The base file path for spritesheets is '<some_path>/<yagl_dir>/<some_file>'.
        // The spritesheet generator appends various things to this.
        fs::path image_base = yagl_file;
        image_base.replace_extension();
        
        std::ofstream os(yagl_file, std::ios::binary);
        grf_data.print(os, yagl_path, image_base);
    }
    //catch (std::exception& e)
    {
        //std::cout << "ERROR: " << e.what() << '\n';
    }
}


static void encode(const std::string& grf_file, const std::string& yagl_dir)
{
    // std::ifstream is(text_file, std::ios::binary);

    // Lexer lexer;
    // TokenStream token_stream(lexer.lex(is));

    // NewGRFData grf_data;
    // grf_data.parse(token_stream); 

    // if (grf_file.size() > 0)
    // {
    //     std::ofstream os(grf_file, std::ios::binary);
    //     grf_data.write(os);
    // }
    // else
    // {
    //     grf_data.write(std::cout);
    // }        
}


int main (int argc, char* argv[])
{
    CommandLineOptions& options = CommandLineOptions::options();
    options.parse(argc, argv);
    
    switch (options.operation())
    {
        case CommandLineOptions::Operation::Decode:
            decode(options.grf_file(), options.yagl_dir());
            break;

        case CommandLineOptions::Operation::Encode:
            encode(options.grf_file(), options.yagl_dir());
            break;
    }
}


