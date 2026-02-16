# ---------------------------------------------
# Project Setup
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(AXR_Engine VERSION 0.1.0)

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
# Set Public Header Files
# ---------------------------------------------
set(AXR_PUBLIC_HEADER_FILES
    # ---- Root ----
    ${AXR_INCLUDE_DIR}/axr/axr.h
    ${AXR_INCLUDE_DIR}/axr/common.h
    ${AXR_INCLUDE_DIR}/axr/logging.h
    ${AXR_INCLUDE_DIR}/axr/lifecycle.h
    ${AXR_INCLUDE_DIR}/axr/application.h
    ${AXR_INCLUDE_DIR}/axr/vulkanApi.h
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
    ${AXR_INCLUDE_DIR}/axr/common/enums.h
    ${AXR_INCLUDE_DIR}/axr/common/callback.h
    ${AXR_INCLUDE_DIR}/axr/common/utils.h
)

# ---------------------------------------------
# Set Source Files
# ---------------------------------------------
set(AXR_SOURCE_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/lifecycle.cpp
    ${AXR_SRC_DIR}/utils.h
    # ---- Memory ----
    ${AXR_SRC_DIR}/memory/types.h
    ${AXR_SRC_DIR}/memory/allocator.h
    ${AXR_SRC_DIR}/memory/allocator.cpp
    ${AXR_SRC_DIR}/memory/subAllocatorBase.h
    ${AXR_SRC_DIR}/memory/subAllocatorBase.cpp
    ${AXR_SRC_DIR}/memory/stackAllocator.h
    ${AXR_SRC_DIR}/memory/stackAllocator.cpp
    ${AXR_SRC_DIR}/memory/doubleStackAllocator.h
    ${AXR_SRC_DIR}/memory/doubleStackAllocator.cpp
    ${AXR_SRC_DIR}/memory/poolAllocator.h
    ${AXR_SRC_DIR}/memory/utils.h
    # ---- Platform ----
    ${AXR_SRC_DIR}/platform/platform.h
    ${AXR_SRC_DIR}/platform/platform.cpp
    # ---- Application ----
    ${AXR_SRC_DIR}/application/application.h
    ${AXR_SRC_DIR}/application/application.cpp
    # ---- Server ----
    ${AXR_SRC_DIR}/server/server.h
    ${AXR_SRC_DIR}/server/server.cpp
    # ---- Renderer ----
    ${AXR_SRC_DIR}/renderer/renderer.h
    ${AXR_SRC_DIR}/renderer/renderer.cpp
    ${AXR_SRC_DIR}/renderer/utils.h
    # ---- Renderer - Vulkan ----
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanApi.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanRenderer.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanRenderer.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanUtils.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensions.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensions.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensionFunctions.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanQueueFamilies.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanQueueFamilies.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanEnvironment.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanEnvironment.cpp
    # ---- Common ----
    ${AXR_SRC_DIR}/common/enums.cpp
    ${AXR_SRC_DIR}/common/array.h
    ${AXR_SRC_DIR}/common/extensionArray.h
    ${AXR_SRC_DIR}/common/vector_stack.h
)

# ---------------------------------------------
# Create AXR_Engine library
# ---------------------------------------------
list(APPEND AXR_FILES ${AXR_SOURCE_FILES} ${AXR_PUBLIC_HEADER_FILES})
add_library(AXR_Engine SHARED ${AXR_FILES})

set_target_properties(AXR_Engine PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

target_link_libraries(AXR_Engine PRIVATE
    SDL3::SDL3
)

if(Vulkan_FOUND)
    target_link_libraries(AXR_Engine PRIVATE
        Vulkan::Vulkan
    )
endif()

# ---------------------------------------------
# AXR_Engine Include Directories
# ---------------------------------------------
target_include_directories(AXR_Engine
    PUBLIC ${AXR_INCLUDE_DIR}
    ${SPDLOG_DIRECTORY}/include
)

# ---------------------------------------------
# Set AXR_Engine compile options
# ---------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(AXR_Engine PRIVATE
        $<$<CONFIG:Debug>:-Wall>
        $<$<CONFIG:Debug>:-Werror>
    )
endif()

# ---------------------------------------------
# Set AXR_Engine compile definitions
# ---------------------------------------------
target_compile_definitions(AXR_Engine PRIVATE
    AXR_BUILD_DLL
    $<$<CONFIG:Debug>:AXR_LOG_SOURCE_LOCATION>
)

if(WIN32)
    target_compile_definitions(AXR_Engine PRIVATE
        AXR_PLATFORM_WIN32
        NOMINMAX
        UNICODE
        _UNICODE
    )
elseif(SDL_WAYLAND)
    target_compile_definitions(AXR_Engine PRIVATE
        AXR_PLATFORM_WAYLAND
    )
endif()

if(Vulkan_FOUND)
    target_compile_definitions(AXR_Engine PRIVATE
        AXR_VULKAN_SUPPORTED
        # This definition is for OpenXR
        XR_USE_GRAPHICS_API_VULKAN
    )

    if(WIN32)
        target_compile_definitions(AXR_Engine PRIVATE
            VK_USE_PLATFORM_WIN32_KHR
        )
    elseif(SDL_WAYLAND)
        target_compile_definitions(AXR_Engine PRIVATE
            VK_USE_PLATFORM_WAYLAND_KHR
        )
    endif()
endif()

# ---------------------------------------------
# Testing
# ---------------------------------------------
include(CTest)
enable_testing()

set(AXR_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/tests)

# ---------------------------------------------
# Set Testing Files
# ---------------------------------------------
set(AXR_TEST_FILES
    # ---- Root ----
    ${AXR_TEST_DIR}/main.cpp
    # ---- Memory ----
    ${AXR_TEST_DIR}/memory/stackAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/doubleStackAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/poolAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/utilsTests.cpp
    # ---- Common ----
    ${AXR_TEST_DIR}/common/arrayTests.cpp
    ${AXR_TEST_DIR}/common/extensionArrayTests.cpp
    ${AXR_TEST_DIR}/common/vector_stackTests.cpp
)

# ---------------------------------------------
# Create Test Executables
# ---------------------------------------------

if(WIN32)
    add_executable(AXR_Test WIN32 ${AXR_TEST_FILES} ${AXR_FILES})
else()
    add_executable(AXR_Test ${AXR_TEST_FILES} ${AXR_FILES})
endif()

target_link_libraries(AXR_Test PRIVATE
    GTest::gtest_main
    AXR_Engine
    SDL3::SDL3
)

if(Vulkan_FOUND)
    target_link_libraries(AXR_Engine PRIVATE
        Vulkan::Vulkan
    )
endif()

set_target_properties(AXR_Test PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

# ---------------------------------------------
# AXR_Test Include Directories
# ---------------------------------------------

target_include_directories(AXR_Test PUBLIC
    ${AXR_SRC_DIR}
)

# ---------------------------------------------
# Set AXR_Test compile definitions
# ---------------------------------------------

if(Vulkan_FOUND)
    target_compile_definitions(AXR_Engine PRIVATE
        AXR_VULKAN_SUPPORTED
        # This definition is for OpenXR
        XR_USE_GRAPHICS_API_VULKAN
    )
endif()

# ---------------------------------------------
# Add Tests
# ---------------------------------------------
add_test(AXR_Engine_Tests AXR_Test)
