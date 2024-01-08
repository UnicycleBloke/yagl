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


set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


project(yagl LANGUAGES CXX)


add_library(yagl_lib STATIC
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
    records/descriptors/VisualEffect.cpp

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
    records/features/Action00Objects.cpp
    records/features/Action00RailTypes.cpp
    records/features/Action00AirportTiles.cpp
    records/features/Action00RoadTypes.cpp
    records/features/Action00TramTypes.cpp

    # Graphics related records and helper classes.
    records/graphics/RecolourRecord.cpp
    records/graphics/RealSpriteRecord.cpp
    records/graphics/FakeSpriteRecord.cpp
    records/graphics/SpriteWrapperRecord.cpp
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

    # Version
    "${CMAKE_BINARY_DIR}/generated/yagl_version.cpp"
)


add_executable(yagl
    application/main.cpp
)


add_executable(yagl_tests
    third_party/catch2/catch_amalgamated.cpp

    # Unit test.
    tests/sundries/Test_StreamHelpers.cpp
    tests/sundries/Test_IntegerDescriptor.cpp
    tests/sundries/Test_YearDescriptor.cpp
    tests/sundries/Test_DateDescriptor.cpp

    # Properties for various features.
    tests/features/Test_Action00_Aircraft.cpp
    tests/features/Test_Action00_Airports.cpp
    tests/features/Test_Action00_AirportTiles.cpp
    tests/features/Test_Action00_Bridges.cpp
    tests/features/Test_Action00_Canals.cpp
    tests/features/Test_Action00_Cargos.cpp
    tests/features/Test_Action00_Common.cpp
    tests/features/Test_Action00_GlobalSettings.cpp
    tests/features/Test_Action00_Houses.cpp
    tests/features/Test_Action00_Industries.cpp
    tests/features/Test_Action00_IndustryTiles.cpp
    tests/features/Test_Action00_Objects.cpp
    tests/features/Test_Action00_RailTypes.cpp
    tests/features/Test_Action00_RoadTypes.cpp
    tests/features/Test_Action00_Ships.cpp
    tests/features/Test_Action00_SoundEffects.cpp
    tests/features/Test_Action00_Stations.cpp
    tests/features/Test_Action00_Trains.cpp
    tests/features/Test_Action00_TramTypes.cpp
    tests/features/Test_Action00_Vehicles.cpp

    # Other ActionXX records.
    tests/actions/Test_Action01Record.cpp             # Define sprites
    tests/actions/Test_Action02BasicRecord.cpp        # Define graphics
    tests/actions/Test_Action02RandomRecord.cpp
    tests/actions/Test_Action02VariableRecord.cpp
    tests/actions/Test_Action02IndustryRecord.cpp
    tests/actions/Test_Action02SpriteLayoutRecord.cpp
    tests/actions/Test_Action03Record.cpp             # Assign graphics
    tests/actions/Test_Action04Record.cpp             # Strings
    tests/actions/Test_Action05Record.cpp             # Sprites
    tests/actions/Test_Action06Record.cpp             # Modify next
    tests/actions/Test_Action07Record.cpp             # Skip records
    tests/actions/Test_Action08Record.cpp             # GRF identification
    tests/actions/Test_Action0ARecord.cpp
    tests/actions/Test_Action0BRecord.cpp
    tests/actions/Test_Action0CRecord.cpp
    tests/actions/Test_Action0DRecord.cpp             # Set parameters
    tests/actions/Test_Action0ERecord.cpp
    tests/actions/Test_Action0FRecord.cpp
    tests/actions/Test_Action10Record.cpp
    tests/actions/Test_Action11Record.cpp
    tests/actions/Test_Action12Record.cpp
    tests/actions/Test_Action13Record.cpp
    tests/actions/Test_Action14Record.cpp
    tests/actions/Test_ActionFERecord.cpp
    tests/actions/Test_ActionFFRecord.cpp
)


# Generate a target to determine the version, output to yagl_version.cpp
add_custom_target(
    prebuild_commands
    ${CMAKE_COMMAND} "-DOUTPUT_FILE=${CMAKE_BINARY_DIR}/generated/yagl_version.cpp" -P "${CMAKE_SOURCE_DIR}/yagl_version.cmake"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    BYPRODUCTS "${CMAKE_BINARY_DIR}/generated/yagl_version.cpp"
)
add_dependencies(yagl prebuild_commands)


target_include_directories(yagl_lib PUBLIC
    .
    records
    records/actions
    records/descriptors
    records/graphics
    records/features
    utility
    application
    tests
    version

    # Header only third party libraries used for convenience.
    third_party/catch2
    third_party/png++
    third_party/cxxopts/include
)


if (UNIX)
    # Builds on UNIX-like systems: Linux, MSYS2, Windows Subsystem for Linux, ...
    # We assume GCC is used for the build
    target_link_libraries(yagl PUBLIC yagl_lib png stdc++fs)
    target_link_libraries(yagl_tests PUBLIC yagl_lib png stdc++fs)
    target_link_libraries(yagl_lib PUBLIC png stdc++fs)
else()
    # Microsoft Visual Studio 2019 (2017 didn't work so well due to some of the C++17 features in the code).
    # Code be fixed with a bit off faff. Or just install VS2019. :)
    target_link_libraries(yagl PUBLIC yagl_lib libpng16 zlib)
    target_link_libraries(yagl_tests PUBLIC yagl_lib libpng16 zlib)

    # Is there a nicer, more automatic, way to generalise the location of vcpkg?
    # Here we expect -DVCPKG_DIR=<dir> to be given on the cmake command line.
    if (NOT EXISTS ${VCPKG_DIR})
        message(FATAL_ERROR "ERROR: The directory given by VCPKG_DIR does not exist - specify with -DVCPKG_DIR=<dir>")
    endif()
    target_link_directories(yagl_lib PUBLIC ${VCPKG_DIR}/installed/x64-windows-static/lib)
    target_include_directories(yagl_lib PUBLIC ${VCPKG_DIR}/installed/x64-windows-static/include)

    # Prevent the compiler whining about this deprecated feature used in the GRFStrings stuff.
    add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
endif()
