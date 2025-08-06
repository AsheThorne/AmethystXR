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
# Add External Libraries
# ---------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenXR REQUIRED)
find_package(Vulkan)

find_program(GLSLC glslc)

# ---------------------------------------------
# Define Functions
# ---------------------------------------------

# Generate vulkan shaders
function(generate_vulkan_shaders shaders_directory target)
    if (Vulkan_FOUND)
        file(GLOB SHADERS
            "${shaders_directory}/*.vert"
            "${shaders_directory}/*.frag"
            "${shaders_directory}/*.comp"
            "${shaders_directory}/*.geom"
            "${shaders_directory}/*.tesc"
            "${shaders_directory}/*.tese"
            "${shaders_directory}/*.mesh"
            "${shaders_directory}/*.task"
            "${shaders_directory}/*.rgen"
            "${shaders_directory}/*.rchit"
            "${shaders_directory}/*.rmiss"
        )

        foreach (shader ${SHADERS})
            get_filename_component(SHADER_NAME ${shader} NAME)
            add_custom_command(TARGET ${target} POST_BUILD COMMAND
                ${GLSLC} ${shader} -o ${shaders_directory}/${SHADER_NAME}.spv
            )
        endforeach ()
    endif ()
endfunction()

# ---------------------------------------------
# Set Global Variables/Properties
# ---------------------------------------------

set(OUTPUT_DIRECTORY $<CONFIG>-$<PLATFORM_ID>)

set(SPDLOG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog)
set(EnTT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/entt)
set(STB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)
set(TINY_GLTF_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf)
set(JSON_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/json)

if (Vulkan_FOUND)
    set(VULKAN_HPP_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/Vulkan-HPP)
endif ()

# ---- Add Predefined Cmake Projects To It's Own Folder ----
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(
    GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake predefined targets"
)

# ---------------------------------------------
# Set Output Directories
# ---------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/builds/${OUTPUT_DIRECTORY}/bin)

# ---------------------------------------------
# Compile Options
# ---------------------------------------------

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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