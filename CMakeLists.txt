# ---------------------------------------------
# Project Setup
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(AmethystXR VERSION 0.1.0)

# ---------------------------------------------
# Add External Libraries
# ---------------------------------------------
find_package(Vulkan COMPONENTS
    glslc
)

# ---------------------------------------------
# Define Functions
# ---------------------------------------------

# Generate vulkan shaders
function(generate_vulkan_shaders shaders_directory target)
    if(Vulkan_FOUND)
        file(GLOB_RECURSE SHADERS
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

        foreach(shader ${SHADERS})
            get_filename_component(SHADER_NAME ${shader} NAME)
            add_custom_command(TARGET ${target} POST_BUILD COMMAND
                Vulkan::glslc ${shader} -o ${shader}.spv
            )
        endforeach()
    endif()
endfunction()

# ---------------------------------------------
# Set Global Variables/Properties
# ---------------------------------------------
set(OUTPUT_DIRECTORY $<CONFIG>)

set(DEPS_SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake-deps-src")
set(DEPS_BUILD_DIRECTORY "${CMAKE_BINARY_DIR}/cmake-deps-build")

set(SPDLOG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog)
set(SDL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL)

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
# Set platform
# ---------------------------------------------

if(WIN32)
    set(AXR_PLATFORM win32)
elseif(SDL_WAYLAND)
    set(AXR_PLATFORM wayland)
endif()

# ---------------------------------------------
# Include AmethystXR Engine
# ---------------------------------------------
add_subdirectory(Engine)

# ---------------------------------------------
# Include AmethystXR Sandbox
# ---------------------------------------------
add_subdirectory(Sandbox)
