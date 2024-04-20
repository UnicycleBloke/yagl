cmake_minimum_required(VERSION 3.10)

# Get the git version
execute_process(COMMAND git -C "${CMAKE_SOURCE_DIR}" describe --long --tags --dirty=-m
                OUTPUT_VARIABLE VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

message(STATUS "Repo version: ${VERSION}")

configure_file("${CMAKE_SOURCE_DIR}/version/yagl_version.cpp.in" "${OUTPUT_FILE}")
