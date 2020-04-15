#///////////////////////////////////////////////////////////////////////////////
#// Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)
#//
#// This file is part of yagl.
#//
#// yagl is free software: you can redistribute it and/or modify
#// it under the terms of the GNU General Public License as published by
#// the Free Software Foundation, either version 3 of the License, or
#// (at your option) any later version.
#//
#// yagl is distributed in the hope that it will be useful,
#// but WITHOUT ANY WARRANTY; without even the implied warranty of
#// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#// GNU General Public License for more details.
#//
#// You should have received a copy of the GNU General Public License
#// along with yagl. If not, see <https://www.gnu.org/licenses/>.
#///////////////////////////////////////////////////////////////////////////////

cmake_minimum_required(VERSION 3.10) 

set(NEWGRF_PROGRAM_NAME "yagl")

project(${NEWGRF_PROGRAM_NAME} LANGUAGES CXX)

add_executable(${NEWGRF_PROGRAM_NAME}
    application/main.cpp
    application/CommandLineOptions.cpp

    # Top level data structure representing all the data in a GRF file. 
    records/NewGRFData.cpp
    # Base class for all types of record in a GRF file.
    records/Record.cpp
    # First stage of parsing a YAGL script - convert to a list of tokens with values.
    records/Lexer.cpp
    # Second stage of parsing a YAGL script - deserialise the data from a stream of tokens.
    records/TokenStream.cpp

    # Pseudo-sprites
    records/actions/Action00Record.cpp
    records/actions/Action01Record.cpp
    records/actions/Action02BasicRecord.cpp
    records/actions/Action02IndustryRecord.cpp
    records/actions/Action02SpriteLayoutRecord.cpp
    records/actions/Action02VariableRecord.cpp
    records/actions/Action02RandomRecord.cpp
    records/actions/Action03Record.cpp
    records/actions/Action04Record.cpp
    records/actions/Action05Record.cpp
    records/actions/Action06Record.cpp
    records/actions/Action07Record.cpp # Also accounts for Action09.
    records/actions/Action08Record.cpp
    records/actions/Action0ARecord.cpp
    records/actions/Action0BRecord.cpp
    records/actions/Action0CRecord.cpp
    records/actions/Action0DRecord.cpp
    records/actions/Action0ERecord.cpp
    records/actions/Action0FRecord.cpp
    records/actions/Action10Record.cpp
    records/actions/Action11Record.cpp
    records/actions/Action12Record.cpp
    records/actions/Action13Record.cpp
    records/actions/Action14Record.cpp

    records/actions/ActionFERecord.cpp # Imported sound effects
    records/actions/ActionFFRecord.cpp # Binary sound effects

    # Descriptors used to print and parse feature properties (Action00). 
    records/descriptors/DescriptorBase.cpp
    records/descriptors/BitfieldDescriptor.cpp
    records/descriptors/EnumDescriptor.cpp
    records/descriptors/AirportLayoutDescriptor.cpp
    records/descriptors/IndustryLayoutDescriptor.cpp
    records/descriptors/CargoAcceptanceDescriptor.cpp
    records/descriptors/DateDescriptor.cpp
    records/descriptors/GRFLabel.cpp

    # Handle properties for each different kind of feature (Action00).
    records/features/Action00Common.cpp
    records/features/Action00Trains.cpp
    records/features/Action00Vehicles.cpp
    records/features/Action00Ships.cpp
    records/features/Action00Aircraft.cpp
    records/features/Action00Stations.cpp
    records/features/Action00Canals.cpp
    records/features/Action00Bridges.cpp
    records/features/Action00Houses.cpp
    records/features/Action00GlobalSettings.cpp
    records/features/Action00IndustryTiles.cpp
    records/features/Action00Industries.cpp
    records/features/Action00Cargos.cpp
    records/features/Action00SoundEffects.cpp
    records/features/Action00Airports.cpp
    records/features/Action00Signals.cpp
    records/features/Action00Objects.cpp
    records/features/Action00RailTypes.cpp
    records/features/Action00AirportTiles.cpp
    records/features/Action00RoadTypes.cpp
    records/features/Action00TramTypes.cpp
    records/features/Action00OriginalStrings.cpp 

    # Graphics related records and helper classes.
    records/graphics/RecolourRecord.cpp
    records/graphics/RealSpriteRecord.cpp
    records/graphics/SpriteIndexRecord.cpp
    records/graphics/ChunkEncoder.cpp       # For sprites with a lot of transparent pixels.
    records/graphics/Palettes.cpp
    records/graphics/SpriteSheetGenerator.cpp
    records/graphics/SpriteIDLabel.cpp
    records/graphics/SpriteSheetReader.cpp

    # General utilities.
    utility/StreamHelpers.cpp
    utility/GRFStrings.cpp
    utility/Exceptions.cpp
    utility/Languages.cpp

    # Unit test.
    #tests/Test_StreamHelpers.cpp
    #tests/Test_IntegerDescriptor.cpp
    #tests/Test_YearDescriptor.cpp
    #tests/Test_DateDescriptor.cpp

    tests/Test_Action02BasicRecord.cpp  # Define graphics
    tests/Test_Action03Record.cpp  # Assign graphics
    tests/Test_Action04Record.cpp  # Strings
    #tests/Test_Action05Record.cpp # Sprites
    tests/Test_Action06Record.cpp  # Modify next
    tests/Test_Action07Record.cpp  # Skip records
    tests/Test_Action08Record.cpp  # GRF identification
)

# This appears to be the simplest way to create a pre-build step that
# actually works. Create a fake target and make the real target depend on it.
add_custom_target(
    prebuild_commands
    COMMAND echo Pre-build commands...
    # Python is required. I used Python 3.8.1, but the script doesn't do much...
    # Windows Subsystem for Linux: needed to manually create soft link to /usr/bin/python3.
    COMMAND python ${PROJECT_SOURCE_DIR}/yagl_version.py
)
add_dependencies(${NEWGRF_PROGRAM_NAME} prebuild_commands)

target_include_directories(${NEWGRF_PROGRAM_NAME} PUBLIC 
    .
    records
    records/actions 
    records/descriptors 
    records/graphics
    records/features
    utility
    application

    # Header only third party libraries used for convenience. 
    third_party/catch2  
    third_party/png++
    third_party/cxxopts/include
)

if (UNIX)
    # Builds on UNIX-like systems: Linux, MSYS2, Windows Subsystem for Linux, ...
    # We assume GCC is used for the build
    target_compile_options(${NEWGRF_PROGRAM_NAME} PUBLIC -g -std=c++17)
    target_link_libraries(${NEWGRF_PROGRAM_NAME} PUBLIC png stdc++fs) 
else()
    # Microsoft Visual Studio 2019 (2017 didn't work so well due to some of the C++17 features in the code).
    # Code be fixed with a bit off faff. Or just install VS2019. :)
    target_compile_options(${NEWGRF_PROGRAM_NAME} PUBLIC /std:c++17 /MT)
    target_link_options(${NEWGRF_PROGRAM_NAME} PUBLIC /VERBOSE:LIB)
    target_link_libraries(${NEWGRF_PROGRAM_NAME} PUBLIC libpng16 zlib)

    # Is their a nicer, more automatic, way to generalise the location of vcpkg? 
    # Here we expect -DVCPKG_DIR=<dir> to be given on the cmake command line.
    if (NOT EXISTS ${VCPKG_DIR})
        message(FATAL_ERROR "ERROR: The directory given by VCPKG_DIR does not exist - specify with -DVCPKG_DIR=<dir>")
    endif()    
    target_link_directories(${NEWGRF_PROGRAM_NAME} PUBLIC ${VCPKG_DIR}/installed/x64-windows-static/lib) 
    target_include_directories(${NEWGRF_PROGRAM_NAME} PUBLIC ${VCPKG_DIR}/installed/x64-windows-static/include)
    
    # Prevent the compiler whining about this deprecated feature used in the GRFStrings stuff.    
    add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
endif()
