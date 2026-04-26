# ---------------------------------------------
# Project Setup
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(AXR_Engine VERSION 0.1.0)

set(AXR_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(AXR_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(AXR_ASSETS_DIR ${CMAKE_CURRENT_LIST_DIR}/assets)
set(AXR_SHADERS_DIR ${AXR_ASSETS_DIR}/shaders)

# ---------------------------------------------
# Generate defines.h from defines.h.in
# ---------------------------------------------
configure_file(
    ${AXR_INCLUDE_DIR}/axr/common/defines.h.in
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
)

# ---------------------------------------------
# Set Third Party Source Files
# ---------------------------------------------
set(AXR_THIRDPARTY_SOURCE_FILES
    ${TOML_DIRECTORY}/src/tomlc17.c
    ${TOML_DIRECTORY}/src/tomlc17.h
    ${TOML_DIRECTORY}/src/tomlcpp.hpp
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
    ${AXR_INCLUDE_DIR}/axr/assets.h
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
    ${AXR_INCLUDE_DIR}/axr/common/enums.h
    ${AXR_INCLUDE_DIR}/axr/common/callback.h
    ${AXR_INCLUDE_DIR}/axr/common/utils.h
    ${AXR_INCLUDE_DIR}/axr/common/types.h
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
    ${AXR_SRC_DIR}/memory/memoryUtils.h
    ${AXR_SRC_DIR}/memory/dynamicAllocator.h
    ${AXR_SRC_DIR}/memory/dynamicAllocator.cpp
    # ---- Platform ----
    ${AXR_SRC_DIR}/platform/platform.h
    ${AXR_SRC_DIR}/platform/platform.cpp
    ${AXR_SRC_DIR}/platform/platform_${AXR_PLATFORM}.cpp
    # ---- Application ----
    ${AXR_SRC_DIR}/application/application.h
    ${AXR_SRC_DIR}/application/application.cpp
    # ---- Debug Info ----
    ${AXR_SRC_DIR}/debugInfo/debugInfo.h
    ${AXR_SRC_DIR}/debugInfo/debugInfo.cpp
    # ---- Server ----
    ${AXR_SRC_DIR}/server/server.h
    ${AXR_SRC_DIR}/server/server.cpp
    # ---- Renderer ----
    ${AXR_SRC_DIR}/renderer/renderer.h
    ${AXR_SRC_DIR}/renderer/renderer.cpp
    ${AXR_SRC_DIR}/renderer/types.h
    ${AXR_SRC_DIR}/renderer/rendererUtils.h
    # ---- Renderer - Vulkan ----
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanApi.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanRenderer.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanRenderer.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanUtils.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanUtils.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensions.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensions.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanExtensionFunctions.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanQueueFamilies.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanQueueFamilies.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanEnvironment.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanEnvironment.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanImage.h
    ${AXR_SRC_DIR}/renderer/vulkan/vulkanImage.cpp
    ${AXR_SRC_DIR}/renderer/vulkan/types.h
    # ---- Common ----
    ${AXR_SRC_DIR}/common/enums.cpp
    ${AXR_SRC_DIR}/common/handle.h
    ${AXR_SRC_DIR}/common/id.h
    ${AXR_SRC_DIR}/common/id.cpp
    # ---- Common - String ----
    ${AXR_SRC_DIR}/common/string/stringIterator.h
    ${AXR_SRC_DIR}/common/string/stringIterator.cpp
    ${AXR_SRC_DIR}/common/string/string.h
    ${AXR_SRC_DIR}/common/string/string.cpp
    ${AXR_SRC_DIR}/common/string/stringView.h
    ${AXR_SRC_DIR}/common/string/stringView.cpp
    ${AXR_SRC_DIR}/common/string/path.h
    ${AXR_SRC_DIR}/common/string/path.cpp
    # ---- Common - Containers ----
    ${AXR_SRC_DIR}/common/containers/array.h
    ${AXR_SRC_DIR}/common/containers/extensionArray.h
    ${AXR_SRC_DIR}/common/containers/vectorBase.h
    ${AXR_SRC_DIR}/common/containers/vector_stack.h
    ${AXR_SRC_DIR}/common/containers/vector_dynamic.h
    ${AXR_SRC_DIR}/common/containers/redBlackTree_pool.h
    ${AXR_SRC_DIR}/common/containers/unorderedMap_dynamic.h
    # ---- Assets ----
    ${AXR_SRC_DIR}/assets/assets.cpp
    ${AXR_SRC_DIR}/assets/assets.h
    ${AXR_SRC_DIR}/assets/shaderAsset.cpp
    ${AXR_SRC_DIR}/assets/shaderAsset.h
    ${AXR_SRC_DIR}/assets/sceneAsset.cpp
    ${AXR_SRC_DIR}/assets/sceneAsset.h
)

# ---------------------------------------------
# Create AXR_Engine library
# ---------------------------------------------
list(APPEND AXR_FILES ${AXR_SOURCE_FILES} ${AXR_PUBLIC_HEADER_FILES} ${AXR_THIRDPARTY_SOURCE_FILES})
add_library(AXR_Engine SHARED ${AXR_FILES})

set_target_properties(AXR_Engine PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

target_link_libraries(AXR_Engine
    PUBLIC glm::glm
    PRIVATE SDL3::SDL3
    PRIVATE xxHash::xxhash
)

if(Vulkan_FOUND)
    target_link_libraries(AXR_Engine
        PUBLIC Vulkan::Vulkan
    )
endif()

# ---------------------------------------------
# AXR_Engine Include Directories
# ---------------------------------------------
target_include_directories(AXR_Engine
    PUBLIC ${AXR_INCLUDE_DIR}
    PUBLIC ${SPDLOG_DIRECTORY}/include
    PUBLIC ${TOML_DIRECTORY}/src
)

# ---------------------------------------------
# Set AXR_Engine compile options
# ---------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(AXR_Engine
        PRIVATE $<$<CONFIG:Debug>:-Wall>
        PRIVATE $<$<CONFIG:Debug>:-Werror>
    )
endif()

# ---------------------------------------------
# Set AXR_Engine compile definitions
# ---------------------------------------------
target_compile_definitions(AXR_Engine
    PUBLIC $<$<CONFIG:Debug>:AXR_LOG_SOURCE_LOCATION>
    PUBLIC $<$<CONFIG:Debug>:AXR_TRACK_ALLOCATOR_PEAK_USAGE>
    PUBLIC $<$<CONFIG:Debug>:AXR_DEBUG_INFO_ENABLED>
    PRIVATE AXR_BUILD_DLL
)

if(WIN32)
    target_compile_definitions(AXR_Engine
        PUBLIC AXR_PLATFORM_WIN32
        PRIVATE NOMINMAX
        PRIVATE UNICODE
        PRIVATE _UNICODE
    )
elseif(SDL_WAYLAND)
    target_compile_definitions(AXR_Engine
        PUBLIC AXR_PLATFORM_WAYLAND
    )
endif()

if(Vulkan_FOUND)
    target_compile_definitions(AXR_Engine
        PUBLIC AXR_VULKAN_SUPPORTED
        PUBLIC XR_USE_GRAPHICS_API_VULKAN # This definition is for OpenXR
    )

    if(WIN32)
        target_compile_definitions(AXR_Engine
            PUBLIC VK_USE_PLATFORM_WIN32_KHR # This definition is for Vulkan
        )
    elseif(SDL_WAYLAND)
        target_compile_definitions(AXR_Engine
            PUBLIC VK_USE_PLATFORM_WAYLAND_KHR # This definition is for Vulkan
        )
    endif()
endif()

# ---------------------------------------------
# Generate vulkan shaders
# ---------------------------------------------

generate_vulkan_shaders(${AXR_SHADERS_DIR} AXR_Engine)

# ---------------------------------------------
# Copy Axr engine assets to output folder
# ---------------------------------------------

set(AXR_OUTPUT_ASSETS_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/axr-assets)

# Delete old axr assets folder
add_custom_command(TARGET AXR_Engine POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E remove_directory ${AXR_OUTPUT_ASSETS_DIR}
)

# Copy axr assets folder to output directory
add_custom_command(TARGET AXR_Engine POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${AXR_ASSETS_DIR} ${AXR_OUTPUT_ASSETS_DIR}
)

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
    ${AXR_TEST_DIR}/utilsTests.cpp
    # ---- Memory ----
    ${AXR_TEST_DIR}/memory/stackAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/doubleStackAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/poolAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/dynamicAllocatorTests.cpp
    ${AXR_TEST_DIR}/memory/memoryUtilsTests.cpp
    # ---- Common - String ----
    ${AXR_TEST_DIR}/common/string/stringTests.cpp
    ${AXR_TEST_DIR}/common/string/stringViewTests.cpp
    ${AXR_TEST_DIR}/common/string/pathTests.cpp
    # ---- Common - Containers ----
    ${AXR_TEST_DIR}/common/containers/arrayTests.cpp
    ${AXR_TEST_DIR}/common/containers/extensionArrayTests.cpp
    ${AXR_TEST_DIR}/common/containers/vector_stackTests.cpp
    ${AXR_TEST_DIR}/common/containers/vector_dynamicTests.cpp
    ${AXR_TEST_DIR}/common/containers/redBlackTree_poolTests.cpp
    ${AXR_TEST_DIR}/common/containers/unorderedMap_dynamicTests.cpp
)

# ---------------------------------------------
# Create Test Executables
# ---------------------------------------------

if(WIN32)
    add_executable(AXR_Test WIN32 ${AXR_TEST_FILES} ${AXR_FILES})
else()
    add_executable(AXR_Test ${AXR_TEST_FILES} ${AXR_FILES})
endif()

target_link_libraries(AXR_Test
    PRIVATE GTest::gtest_main
    PRIVATE AXR_Engine
    PRIVATE SDL3::SDL3
)

set_target_properties(AXR_Test PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

# ---------------------------------------------
# AXR_Test Include Directories
# ---------------------------------------------

target_include_directories(AXR_Test
    PUBLIC ${AXR_SRC_DIR}
)

# ---------------------------------------------
# Set AXR_Test compile definitions
# ---------------------------------------------

target_compile_definitions(AXR_Test
    PRIVATE AXR_TESTING_ENABLED
)

# ---------------------------------------------
# Add Tests
# ---------------------------------------------
add_test(AXR_Engine_Tests AXR_Test)
