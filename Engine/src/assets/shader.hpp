#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "shaderProperties.hpp"
#include "axr/graphicsSystem.h"

/// Axr Shader
class AxrShader {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrShader();
    /// Constructor
    /// @param config Shader config
    AxrShader(const AxrShaderConfig& config);
    /// Copy Constructor
    /// @param src Source AxrShader to copy from
    AxrShader(const AxrShader& src);
    /// Move Constructor
    /// @param src Source AxrShader to move from
    AxrShader(AxrShader&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrShader();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrShader to copy from
    AxrShader& operator=(const AxrShader& src);
    /// Move Assignment Operator
    /// @param src Source AxrShader to move from
    AxrShader& operator=(AxrShader&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the shader's name
    /// @returns The shader's name
    [[nodiscard]] const char* getName() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Get the shader file data
    /// @returns The shader file data
    [[nodiscard]] const std::vector<char>& getFileData() const;

    /// Check if this shader is loaded
    /// @returns True if this shader is loaded
    [[nodiscard]] bool isLoaded() const;
    /// Load this shader file
    /// @param graphicsApi The graphics api to use this shader with
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadFile(AxrGraphicsApiEnum graphicsApi) const;
    /// Unload this shader file
    void unloadFile() const;

    /// Get the shader properties
    /// @returns The shader properties
    [[nodiscard]] const AxrShaderPropertiesRAII& getProperties() const;

    /// Check if this shader is valid
    /// @returns True if this shader is valid
    [[nodiscard]] bool isValid() const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Check if the given shaders are compatible and can be used together
    /// @param shader1 First shader to check
    /// @param shader2 Second shader to check
    /// @returns True if the given shaders are compatible
    [[nodiscard]] static bool areCompatible(const AxrShader& shader1, const AxrShader& shader2);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_Name;
    const char* m_FilePath;
    AxrShaderPropertiesRAII m_Properties;
    mutable std::vector<char> m_FileData;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup the shader class
    void cleanup();
};
