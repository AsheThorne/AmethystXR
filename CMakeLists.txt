# ---------------------------------------------
# Cmake Version
# ---------------------------------------------
cmake_minimum_required(VERSION 3.15)

project(AmethystXR VERSION 1.0.0)

# ---------------------------------------------
# Define AXR Compiler Flags
# ---------------------------------------------
add_library(axr_compiler_flags INTERFACE)
target_compile_features(axr_compiler_flags INTERFACE cxx_std_20)

# ---------------------------------------------
# Set Global Variables/Properties
# ---------------------------------------------

set(OUTPUT_DIRECTORY $<CONFIG>-$<PLATFORM_ID>)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(OPENXR_HPP_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/OpenXR-HPP)
# set(SPDLOG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog)
# set(EnTT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/entt)
# set(STB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)
# set(TINY_GLTF_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/tiny-gltf)
# set(JSON_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/json)

# ---- Add Predefined Cmake Projects To It's Own Folder ----
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(
    GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake predefined targets"
)

# ---------------------------------------------
# Add External Libraries
# ---------------------------------------------
find_package(OpenXR REQUIRED)
find_package(Vulkan)

# ---------------------------------------------
# Set Output Directories
# ---------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/bin)

# ---------------------------------------------
# Include AmethystXR Engine
# ---------------------------------------------
add_subdirectory(Engine)

# ---------------------------------------------
# Include AmethystXR Sandbox
# ---------------------------------------------
add_subdirectory(Sandbox)

# ---------------------------------------------
# Add Files To The IDE Without Building Them
#
# This is purely for visibility so they can be
# found and edited easily in an IDE.
# ---------------------------------------------
set(AXR_IDE_ONLY_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/TODO.md
)

add_custom_target(Common SOURCES ${AXR_IDE_ONLY_FILES})