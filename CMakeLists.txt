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
    #records/descriptors/Descriptors.cpp
    records/descriptors/DescriptorBase.cpp
    records/descriptors/GRFLabelListDescriptor.cpp
    records/descriptors/BitfieldDescriptor.cpp
    records/descriptors/EnumDescriptor.cpp
    records/descriptors/AirportLayoutDescriptor.cpp
    records/descriptors/IndustryLayoutDescriptor.cpp
    records/descriptors/CargoAcceptanceDescriptor.cpp

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

    # General utilities.
    utility/StreamHelpers.cpp
    utility/GRFStrings.cpp
    utility/GRFLabel.cpp
    utility/Exceptions.cpp
    utility/Languages.cpp
)

# This appears to be the simplest way to create a pre-build step that
# actually works. Create a fake target and make the real target depend on it.
add_custom_target(
    prebuild_commands
    COMMAND echo Pre-build commands...
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
    third_party/png++
    third_party/cxxopts/include
)

target_link_libraries(${NEWGRF_PROGRAM_NAME} PUBLIC png stdc++fs) 

target_compile_options(${NEWGRF_PROGRAM_NAME} PUBLIC -g -std=c++17)
