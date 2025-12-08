# ---------------------------------------------
# Project Setup
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(AmethystXR VERSION 0.1.0)

# ---------------------------------------------
# Set Global Variables/Properties
# ---------------------------------------------
set(OUTPUT_DIRECTORY $<CONFIG>)

set(SPDLOG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)

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
# TODO:...
#include(CTest)
#enable_testing()

# ---------------------------------------------
# Include AmethystXR Engine
# ---------------------------------------------
add_subdirectory(Engine)

# ---------------------------------------------
# Include AmethystXR Sandbox
# ---------------------------------------------
add_subdirectory(Sandbox)
