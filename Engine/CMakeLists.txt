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
# Set Header Files
# ---------------------------------------------
set(AXR_PUBLIC_HEADER_FILES
    # ---- Root ----
    ${AXR_INCLUDE_DIR}/axr/axr.h
    ${AXR_INCLUDE_DIR}/axr/common.h
    # ---- Common ----
    ${AXR_INCLUDE_DIR}/axr/common/defines.h
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------
set(AXR_CPP_FILES
    # ---- Root ----
    ${AXR_SRC_DIR}/axr.cpp
)

# ---------------------------------------------
# Create AXR_Engine library
# ---------------------------------------------
list(APPEND AXR_FILES ${AXR_CPP_FILES} ${AXR_PUBLIC_HEADER_FILES})
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
# Set AXR_Engine compile definitions
# ---------------------------------------------
target_compile_definitions(AXR_Engine PRIVATE
    AXR_BUILD_DLL 
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
