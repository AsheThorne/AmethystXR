#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "shaderValues.hpp"

/// Axr Material
class AxrMaterial {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrMaterial();
    /// Constructor
    /// @param config Material config
    AxrMaterial(const AxrMaterialConfig& config);
    /// Copy Constructor
    /// @param src Source AxrMaterial to copy from
    AxrMaterial(const AxrMaterial& src);
    /// Move Constructor
    /// @param src Source AxrMaterial to move from
    AxrMaterial(AxrMaterial&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrMaterial();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrMaterial to copy from
    AxrMaterial& operator=(const AxrMaterial& src);
    /// Move Assignment Operator
    /// @param src Source AxrMaterial to move from
    AxrMaterial& operator=(AxrMaterial&& src) noexcept;

    // ----------------------------------------- //
    // Public Headers
    // ----------------------------------------- //

    /// Get the material's name
    /// @returns The material's name
    [[nodiscard]] const char* getName() const;
    
    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Get the vertex shader's name
    /// @returns The vertex shader's name
    [[nodiscard]] const char* getVertexShaderName() const;
    /// Get the fragment shader's name
    /// @returns The fragment shader's name
    [[nodiscard]] const char* getFragmentShaderName() const;

    /// Get the material layout's name
    /// @returns The material layout's name
    [[nodiscard]] std::string getMaterialLayoutName() const;

    /// Check if this material is valid
    /// @returns True if this material is valid
    [[nodiscard]] bool isValid() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_Name;
    const char* m_VertexShaderName;
    const char* m_FragmentShaderName;
    AxrShaderValuesRAII m_VertexShaderValues;
    AxrShaderValuesRAII m_FragmentShaderValues;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
