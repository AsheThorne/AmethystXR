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
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
    ${AXR_INCLUDE_DIR}/axr/common/enums.h
    ${AXR_INCLUDE_DIR}/axr/common/callback.h
)

# ---------------------------------------------
# Set Source Files
# ---------------------------------------------
set(AXR_SOURCE_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/lifecycle.cpp
    # ---- Memory ----
    ${AXR_SRC_DIR}/memory/allocator.h
    ${AXR_SRC_DIR}/memory/allocator.cpp
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
elseif(LINUX)
    target_compile_definitions(AXR_Engine PRIVATE
        AXR_PLATFORM_LINUX
    )
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
    ${AXR_TEST_DIR}/test.cpp
)

# ---------------------------------------------
# Create Test Executables
# ---------------------------------------------

if(WIN32)
    add_executable(AXR_Test WIN32 ${AXR_TEST_FILES})
elseif(LINUX)
    add_executable(AXR_Test ${AXR_TEST_FILES})
endif()

target_link_libraries(AXR_Test PRIVATE
    GTest::gtest_main
    AXR_Engine
)

set_target_properties(AXR_Test PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

# ---------------------------------------------
# Add Tests
# ---------------------------------------------
add_test(AXR_Engine_Tests AXR_Test)
