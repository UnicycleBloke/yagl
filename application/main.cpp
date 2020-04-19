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

// Unit testing framework
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"


static void decode()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try 
    {
        // We first create the sub-directory for the output files.
        fs::create_directory(options.yagl_dir());

        std::cout << "Reading GRF:      " << options.grf_file() << "\n";
        std::cout << "Writing YAGL:     " << options.yagl_file() << "\n";
        std::cout << "Output directory: " << options.yagl_dir() << "\n";
        std::cout << "Image base:       " << options.image_base() << "\n" << std::endl;

        // Read in the GRF file ...
        // The GRF file already checked for existence.
        std::cout << "Reading GRF..." << std::endl;
        NewGRFData grf_data;
        std::ifstream is(options.grf_file(), std::ios::binary);
        grf_data.read(is);

        // Write out the YAGL file and associated sprite sheets ...
        std::cout << "Writing YAGL and other files..." << std::endl;
        std::ofstream os(options.yagl_file(), std::ios::binary);
        grf_data.print(os, options.yagl_dir(), options.image_base());
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


static void encode()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try
    {
        std::cout << "Reading YAGL:     " << options.yagl_file() << "\n";
        std::cout << "Writing GRF:      " << options.grf_file() << "\n";
        std::cout << "Source directory: " << options.yagl_dir() << "\n";
        std::cout << "Image base:       " << options.image_base() << "\n" << std::endl;

        // Read in the YAGL file ...
        // This file already checked for existence.
        // Will need to check for the sprite sheets as we go along.
        std::cout << "Lexing YAGL..." << std::endl;
        std::ifstream is(options.yagl_file(), std::ios::binary);
        TokenStream token_stream{is};

        // Parse the YAGL script ...
        std::cout << "Parsing YAGL..." << std::endl;
        NewGRFData grf_data;
        grf_data.parse(token_stream, options.yagl_dir(), options.image_base()); 

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
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


static void hex_dump()
{
    CommandLineOptions& options = CommandLineOptions::options();

    try 
    {
        // We first create the sub-directory for the output files.
        fs::create_directory(options.yagl_dir());

        std::cout << "Reading GRF:      " << options.grf_file() << "\n";
        std::cout << "Writing HEX:      " << options.yagl_file() << "\n";
        std::cout << "Output directory: " << options.yagl_dir() << "\n";

        // Read in the GRF file ...
        // The GRF file already checked for existence.
        std::cout << "Reading GRF..." << std::endl;
        NewGRFData grf_data;
        std::ifstream is(options.grf_file(), std::ios::binary);
        grf_data.read(is);

        // Write out the HEX file...
        std::cout << "Writing HEX..." << std::endl;
        std::ofstream os(options.hex_file(), std::ios::binary);
        grf_data.hex_dump(os);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}


std::vector<std::string> split(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream is(str);
    std::string token;
    while (std::getline(is, token, ' ')) 
    {
        result.push_back(token);
    }
    return result;
}


int test(const char* arg0, const std::string str_args)
{
    // Recreate the command line for the benefit of the Catch2 tests.
    auto split_args = split(str_args);
    std::vector<const char*> ptr_args;
    ptr_args.push_back(arg0);
    for (const auto& arg: split_args)
    {
        ptr_args.push_back(&arg[0]);
    } 
 
    std::cout << "Performing unit tests...\n";
    Catch::Session session; // There must be exactly one instance
 
    // Writing to session.configData() here sets defaults
    // this is the preferred way to set them.   
    int result = session.applyCommandLine(int(ptr_args.size()), &ptr_args[0]);
    if( result != 0 ) // Indicates a command line error
        return result;
 
    // Writing to session.configData() or session.Config() here 
    // overrides command line args. Only do this if you know you need to.

    // num_failed is clamped to 255 as some unices only use the lower 8 bits.
    // This clamping has already been applied, so just return it here.
    // You can also do any post run clean-up here.
    int num_failed = session.run();
    return num_failed;
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
            hex_dump();
            break;

        case CommandLineOptions::Operation::Test:
            test(argv[0], options.test_args());
            break;
    }

    return 0;
}
