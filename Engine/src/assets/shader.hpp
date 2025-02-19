#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "shaderProperties.hpp"

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
    const char* getName() const;
    /// Get the shader's file path
    /// @returns The shader's file path
    const char* getFilePath() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_Name;
    const char* m_FilePath;
    AxrShaderPropertiesRAII m_Properties;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup the shader class
    void cleanup();
};
