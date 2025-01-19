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
    ${AXR_INCLUDE_DIR}/axr/application.h
    ${AXR_INCLUDE_DIR}/axr/logger.h
    ${AXR_INCLUDE_DIR}/axr/setup.h
    ${AXR_INCLUDE_DIR}/axr/windowSystem.h
    ${AXR_INCLUDE_DIR}/axr/graphicsSystem.h
    ${AXR_INCLUDE_DIR}/axr/vulkanApi.h
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
    ${AXR_INCLUDE_DIR}/axr-hpp/application.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/logger.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/setup.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/windowSystem.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/graphicsSystem.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/vulkanApi.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/flags.hpp
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr-hpp/common/defines.hpp
    ${AXR_INCLUDE_DIR}/axr-hpp/common/result.hpp
)

set(AXR_HEADER_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/utils.hpp
    # ---- AXR Application ----
    ${AXR_SRC_DIR}/application/application.hpp
    # ---- Window System ----
    ${AXR_SRC_DIR}/windowSystem/windowSystem.hpp
    # ---- Window System - Win32 ----
    ${AXR_SRC_DIR}/windowSystem/win32/win32WindowSystem.hpp
    # ---- Graphics System ----
    ${AXR_SRC_DIR}/graphicsSystem/graphicsSystem.hpp
    # ---- Graphics System - Vulkan ----
    ${AXR_SRC_DIR}/graphicsSystem/vulkan/vulkanGraphicsSystem.hpp
    ${AXR_SRC_DIR}/graphicsSystem/vulkan/vulkanUtils.hpp
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------

set(AXR_CPP_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/setup.cpp
    ${AXR_SRC_DIR}/utils.cpp
    # ---- AXR Application ----
    ${AXR_SRC_DIR}/application/application.cpp
    # ---- Window System ----
    ${AXR_SRC_DIR}/windowSystem/windowSystem.cpp
    # ---- Window System - Win32 ----
    ${AXR_SRC_DIR}/windowSystem/win32/win32WindowSystem.cpp
    # ---- Graphics System ----
    ${AXR_SRC_DIR}/graphicsSystem/graphicsSystem.cpp
    # ---- Graphics System - Vulkan ----
    ${AXR_SRC_DIR}/graphicsSystem/vulkan/vulkanGraphicsSystem.cpp
    ${AXR_SRC_DIR}/graphicsSystem/vulkan/vulkanUtils.cpp
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
    add_compile_definitions(AXR_PLATFORM_WIN32)
    add_compile_definitions(NOMINMAX)
endif()

if(Vulkan_FOUND)
    add_compile_definitions(AXR_SUPPORTED_GRAPHICS_VULKAN)
    # This definition is for OpenXR
    add_compile_definitions(XR_USE_GRAPHICS_API_VULKAN)
    add_compile_definitions(VULKAN_HPP_NO_EXCEPTIONS)
    if(WIN32)
        add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
    endif()
endif()
