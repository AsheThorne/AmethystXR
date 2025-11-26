# ---------------------------------------------
# Create Sandbox Project
# ---------------------------------------------
cmake_minimum_required(VERSION 3.25.0)
project(Sandbox VERSION 0.1.0)

set(AXR_SANDBOX_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# ---------------------------------------------
# Set Header Files
# ---------------------------------------------
set(AXR_SANDBOX_HEADER_FILES
    # ---- Root ----
    ${AXR_SANDBOX_SRC_DIR}/sandbox.h
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------
set(AXR_SANDBOX_CPP_FILES
    # ---- Root ----
    ${AXR_SANDBOX_SRC_DIR}/sandbox.cpp
    # ---- Platform ----
    ${AXR_SANDBOX_SRC_DIR}/platform/entrypoint_${AXR_PLATFORM}.cpp
)

# ---------------------------------------------
# Set Sandbox linker options
# ---------------------------------------------

add_link_options(-municode)

# ---------------------------------------------
# Create Sandbox Executable
# ---------------------------------------------
list(APPEND AXR_SANDBOX_FILES ${AXR_SANDBOX_CPP_FILES})

if(WIN32)
    add_executable(Sandbox WIN32 ${AXR_SANDBOX_FILES})
elseif(LINUX)
    add_executable(Sandbox ${AXR_SANDBOX_FILES})
endif()

set_target_properties(Sandbox PROPERTIES 
    LINKER_LANGUAGE CXX 
    CXX_STANDARD 20
)

# ---------------------------------------------
# Link libraries to Sandbox 
# ---------------------------------------------
target_link_libraries(Sandbox 
    PUBLIC AXR_Engine
)

# ---------------------------------------------
# Set Sandbox compile definitions
# ---------------------------------------------
add_compile_definitions(UNICODE)

if(WIN32)
    add_compile_definitions(AXR_PLATFORM_WIN32)
    add_compile_definitions(NOMINMAX)
elseif(LINUX)
    add_compile_definitions(AXR_PLATFORM_LINUX)
endif()
