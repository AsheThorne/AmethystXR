# ---------------------------------------------
# Create AXR_Engine Project
# ---------------------------------------------
project(AXR_Engine VERSION 1.0.0)

set(AXR_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(AXR_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# ---------------------------------------------
# Generate defines.h from defines.h.in
# ---------------------------------------------
configure_file(
    ${AXR_INCLUDE_DIR}/axr/common/defines.h.in 
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
)

# ---------------------------------------------
# Set Header Files
# ---------------------------------------------

set(AXR_PUBLIC_HEADER_FILES
    # ---- Root ----
    ${AXR_INCLUDE_DIR}/axr.h
    # ---- Axr ----
    ${AXR_INCLUDE_DIR}/axr/common_includes.h
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h.in
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
    # ---- Test ----
    ${AXR_INCLUDE_DIR}/axr/test.h
)

set(AXR_PUBLIC_HEADER_HPP_FILES
    # ---- Root ----
    ${AXR_INCLUDE_DIR}/axr.hpp
    # ---- Test ----
    ${AXR_INCLUDE_DIR}/axr-hpp/test.hpp
)

set(AXR_HEADER_FILES
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------

set(AXR_CPP_FILES
    # ---- Test ----
    ${AXR_SRC_DIR}/test.cpp
)

# ---------------------------------------------
# Create AXR_Engine library
# ---------------------------------------------

# ---- Combine cpp and header files into one list called AXR_SOURCE_FILES ----
list(APPEND AXR_SOURCE_FILES ${AXR_CPP_FILES} ${AXR_HEADER_FILES} ${AXR_PUBLIC_HEADER_FILES} ${AXR_PUBLIC_HEADER_HPP_FILES})

add_library(AXR_Engine SHARED ${AXR_SOURCE_FILES})

set_target_properties(AXR_Engine PROPERTIES LINKER_LANGUAGE CXX)

# ---------------------------------------------
# Link libraries to AXR_Engine
# ---------------------------------------------
target_link_libraries(AXR_Engine
    PRIVATE axr_compiler_flags
    Vulkan::Vulkan
    OpenXR::openxr_loader
)

# ---------------------------------------------
# AXR_Engine Include Directories
# ---------------------------------------------
target_include_directories(AXR_Engine 
    PUBLIC ${AXR_INCLUDE_DIR} 
    ${OPENXR_HPP_DIRECTORY}/include
    # ${SPDLOG_DIRECTORY}/include
    # ${EnTT_DIRECTORY}/single_include
    # ${Vulkan_INCLUDE_DIR}
    # ${STB_DIRECTORY}/include
    # ${TINY_GLTF_DIRECTORY}/include
    # ${JSON_DIRECTORY}/include
)

if(VULKAN_HPP_DIRECTORY)
    target_include_directories(AXR_Engine
        PUBLIC ${VULKAN_HPP_DIRECTORY}
    )
endif()

# ---------------------------------------------
# Set AXR_Engine compile definitions
# ---------------------------------------------
add_compile_definitions(UNICODE)
add_compile_definitions(AXR_BUILD_DLL)
add_compile_definitions(OPENXR_HPP_NO_EXCEPTIONS)

if(WIN32)
    add_compile_definitions(AXR_PLATFORM_WINDOWS)
    add_compile_definitions(NOMINMAX)
endif()

if(Vulkan_FOUND)
    add_compile_definitions(AXR_SUPPORTED_GRAPHICS_VULKAN)
    # This definition is for OpenXR
    add_compile_definitions(XR_USE_GRAPHICS_API_VULKAN)
    add_compile_definitions(VULKAN_HPP_NO_EXCEPTIONS)
endif()
