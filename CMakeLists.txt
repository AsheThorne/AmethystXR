# ---------------------------------------------
# Project Setup
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(AmethystXR VERSION 0.1.0)

# ---------------------------------------------
# Set Global Variables/Properties
# ---------------------------------------------
set(OUTPUT_DIRECTORY $<CONFIG>)

set(DEPS_SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake-deps-src")
set(DEPS_BUILD_DIRECTORY "${CMAKE_BINARY_DIR}/cmake-deps-build")

set(SPDLOG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
set(SDL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL)

if(WIN32)
    set(AXR_PLATFORM win32)
elseif(LINUX)
    set(AXR_PLATFORM linux)
endif()

# ---------------------------------------------
# Setup Build Configurations
# ---------------------------------------------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# ---------------------------------------------
# Set Output Directories
# ---------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/bin)

# ---------------------------------------------
# Testing
# ---------------------------------------------
include(FetchContent)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        SOURCE_DIR "${DEPS_SOURCE_DIRECTORY}/googletest"
        BINARY_DIR "${DEPS_BUILD_DIRECTORY}/googletest"
    )
endif()

add_subdirectory(
    "${googletest_SOURCE_DIR}"
    "${googletest_BINARY_DIR}"
)

# ---------------------------------------------
# Include thirdparty libraries
# ---------------------------------------------
add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)

# ---------------------------------------------
# Include AmethystXR Engine
# ---------------------------------------------
add_subdirectory(Engine)

# ---------------------------------------------
# Include AmethystXR Sandbox
# ---------------------------------------------
add_subdirectory(Sandbox)
