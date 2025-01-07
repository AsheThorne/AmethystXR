# ---------------------------------------------
# Create Sandbox Project
# ---------------------------------------------
project(Sandbox VERSION 1.0.0)

set(AXR_SANDBOX_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# ---------------------------------------------
# Set Header Files
# ---------------------------------------------

set(AXR_SANDBOX_HEADER_FILES
)

# ---------------------------------------------
# Set CPP Files
# ---------------------------------------------
set(AXR_SANDBOX_CPP_FILES
    # ---- Root ----
    ${AXR_SANDBOX_SRC_DIR}/main.cpp
)

# ---------------------------------------------
# Create Sandbox Executable
# ---------------------------------------------

# ---- Combine cpp and header files into one list called AXR_SANDBOX_SOURCE_FILES ----
list(APPEND AXR_SANDBOX_SOURCE_FILES ${AXR_SANDBOX_HEADER_FILES} ${AXR_SANDBOX_CPP_FILES})

add_executable(Sandbox WIN32 ${AXR_SANDBOX_SOURCE_FILES})

set_target_properties(Sandbox PROPERTIES LINKER_LANGUAGE CXX)

# ---------------------------------------------
# Link libraries to Sandbox 
# ---------------------------------------------
target_link_libraries(Sandbox 
    PUBLIC AXR_Engine
    axr_compiler_flags
    Vulkan::Vulkan
    OpenXR::openxr_loader
)

add_compile_definitions(OPENXR_HPP_NO_EXCEPTIONS)
