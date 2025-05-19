# ---------------------------------------------
# Create Sandbox Project
# ---------------------------------------------
project(Sandbox VERSION 1.0.0)

set(AXR_SANDBOX_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(AXR_SANDBOX_ASSETS_DIR ${CMAKE_CURRENT_LIST_DIR}/assets)
set(AXR_SANDBOX_SHADERS_DIR ${AXR_SANDBOX_ASSETS_DIR}/shaders)

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
# Set Asset Files
# ---------------------------------------------

set(AXR_SANDBOX_ASSET_FILES
    # ---- Shaders ----
    ${AXR_SANDBOX_SHADERS_DIR}/shader.vert
    ${AXR_SANDBOX_SHADERS_DIR}/shader.frag
)

# ---------------------------------------------
# Set Sandbox linker options
# ---------------------------------------------

add_link_options(-municode)

# ---------------------------------------------
# Create Sandbox Executable
# ---------------------------------------------

# ---- Combine cpp, header and asset files into one list called AXR_SANDBOX_FILES ----
list(APPEND AXR_SANDBOX_SOURCE_FILES ${AXR_SANDBOX_HEADER_FILES} ${AXR_SANDBOX_CPP_FILES})
list(APPEND AXR_SANDBOX_FILES ${AXR_SANDBOX_SOURCE_FILES} ${AXR_SANDBOX_ASSET_FILES})

add_executable(Sandbox WIN32 ${AXR_SANDBOX_FILES})

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

# ---------------------------------------------
# Set Sandbox compile definitions
# ---------------------------------------------
add_compile_definitions(UNICODE)

if(WIN32)
    add_compile_definitions(AXR_USE_PLATFORM_WIN32)
    add_compile_definitions(NOMINMAX)
endif()

if(Vulkan_FOUND)
    add_compile_definitions(AXR_SUPPORTED_GRAPHICS_VULKAN)
endif()

# ---------------------------------------------
# Generate vulkan shaders
# ---------------------------------------------

generate_vulkan_shaders(${AXR_SANDBOX_SHADERS_DIR} Sandbox)

# ---------------------------------------------
# Copy assets to output folder
# ---------------------------------------------

set(AXR_SANDBOX_OUTPUT_ASSETS_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

# Delete old assets folder
add_custom_command(TARGET Sandbox POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E remove_directory ${AXR_SANDBOX_OUTPUT_ASSETS_DIR}
)

# Copy assets folder to output directory
add_custom_command(TARGET Sandbox POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${AXR_SANDBOX_ASSETS_DIR} ${AXR_SANDBOX_OUTPUT_ASSETS_DIR}
)