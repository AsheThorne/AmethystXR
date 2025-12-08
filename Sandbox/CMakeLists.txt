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
# Set Sandbox linker options
# ---------------------------------------------

if(WIN32 AND MINGW)
    target_link_options(Sandbox PRIVATE 
        "-Wl,--subsystem,console" 
        -municode
    )
endif()

# ---------------------------------------------
# Set AXR_Engine compile options
# ---------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Sandbox PRIVATE
        $<$<CONFIG:Debug>:-Wall>
        $<$<CONFIG:Debug>:-Werror>
    )
endif()

# ---------------------------------------------
# Link libraries to Sandbox 
# ---------------------------------------------
target_link_libraries(Sandbox 
    PUBLIC AXR_Engine
)

# ---------------------------------------------
# Set Sandbox compile definitions
# ---------------------------------------------
target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:AXR_LOG_SOURCE_LOCATION>
)

if(WIN32)
    target_compile_definitions(Sandbox PRIVATE
        AXR_PLATFORM_WIN32 
        NOMINMAX 
        UNICODE 
        _UNICODE
    )
elseif(LINUX)
    target_compile_definitions(Sandbox PRIVATE
        AXR_PLATFORM_LINUX
    )
endif()
