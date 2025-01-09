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
    # ---- AXR ----
    ${AXR_INCLUDE_DIR}/axr/common.h
    ${AXR_INCLUDE_DIR}/axr/axrApplication.h
    ${AXR_INCLUDE_DIR}/axr/logger.h
    ${AXR_INCLUDE_DIR}/axr/setup.h
    ${AXR_INCLUDE_DIR}/axr/windowSystem.h
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h.in
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
    ${AXR_INCLUDE_DIR}/axr/common/result.h
)

set(AXR_PUBLIC_HEADER_HPP_FILES
    # ---- Root ----
    ${AXR_INCLUDE_DIR}/axr.hpp
    # ---- AXR HPP ----
    ${AXR_INCLUDE_DIR}/axr-hpp/common.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/axrApplication.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/logger.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/setup.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/windowSystem.hpp
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr-hpp/common/defines.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/common/result.hpp
)

set(AXR_HEADER_FILES
    # ---- AXR Application ----
    ${AXR_SRC_DIR}/axrApplication/axrApplication.hpp
    # ---- Utils ----
    ${AXR_SRC_DIR}/utils/stringUtils.hpp
    # ---- Window System ----
    ${AXR_SRC_DIR}/windowSystem/windowSystem.hpp
    # ---- Window System - Win32 ----
    ${AXR_SRC_DIR}/windowSystem/win32/win32WindowSystem.hpp
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------

set(AXR_CPP_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/setup.cpp
    # ---- AXR Application ----
    ${AXR_SRC_DIR}/axrApplication/axrApplication.cpp
    # ---- Utils ----
    ${AXR_SRC_DIR}/utils/stringUtils.cpp
    # ---- Window System ----
    ${AXR_SRC_DIR}/windowSystem/windowSystem.cpp
    # ---- Window System - Win32 ----
    ${AXR_SRC_DIR}/windowSystem/win32/win32WindowSystem.cpp
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
    ${SPDLOG_DIRECTORY}/include
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
