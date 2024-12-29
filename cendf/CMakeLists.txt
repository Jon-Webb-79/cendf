# ================================================================================
# ================================================================================
# - File:    CMakeLists.txt
# - Purpose: Primary CMake file for C build system
#
# Source Metadata
# - Author:  Jonathan A. Webb
# - Date:    December 17, 2024
# - Version: 1.0
# - Copyright: Copyright 2024, Jonathan A. Webb Inc.
# ================================================================================
# ================================================================================

cmake_minimum_required(VERSION 3.31.2)
project(cendf)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

# Include FetchContent for downloading dependencies
include(FetchContent)

# Declare Jansson dependency
FetchContent_Declare(
    jansson
    GIT_REPOSITORY https://github.com/akheron/jansson.git
    GIT_TAG v2.14  # You can specify a specific version/tag here
)

# Set compiler flags
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")
endif()

# Add the `endf` library
add_library(cendf
            read_file.c
            dstructures.c
)

# Link against jansson
target_link_libraries(cendf PUBLIC jansson)  # Changed from PRIVATE to PUBLIC

target_include_directories(cendf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cendf/include)

# Add the test directory
add_subdirectory(test)
# ================================================================================
# ================================================================================
# eof
