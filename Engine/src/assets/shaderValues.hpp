#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>
#include <string>

/// Shader values RAII wrapper
class AxrShaderValuesRAII {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrShaderValuesRAII();
    /// Constructor
    /// @param values Raw shader values
    AxrShaderValuesRAII(AxrShaderValuesConst_T values);
    /// Copy Constructor
    /// @param src Source AxrShaderValuesRAII to copy from
    AxrShaderValuesRAII(const AxrShaderValuesRAII& src);
    /// Move Constructor
    /// @param src Source AxrShaderValuesRAII to move from
    AxrShaderValuesRAII(AxrShaderValuesRAII&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrShaderValuesRAII();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrShaderValuesRAII to copy from
    AxrShaderValuesRAII& operator=(const AxrShaderValuesRAII& src);
    /// Move Assignment Operator
    /// @param src Source AxrShaderValuesRAII to move from
    AxrShaderValuesRAII& operator=(AxrShaderValuesRAII&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Check if the shader values are valid
    /// @returns True if the shader values are valid
    [[nodiscard]] bool isValid() const;

    /// Get all uniform buffer links
    /// @returns A collection of uniform buffer links
    [[nodiscard]] std::vector<AxrShaderUniformBufferLinkConst_T> getUniformBufferLinks() const;
    /// Get all image sampler buffer links
    /// @returns A collection of image sampler buffer links
    [[nodiscard]] std::vector<AxrShaderImageSamplerBufferLinkConst_T> getImageSamplerBufferLinks() const;

    /// Find the shader uniform buffer at the given binding
    /// @param binding Binding to use
    /// @returns The uniform buffer at the given binding
    [[nodiscard]] AxrShaderUniformBufferLinkConst_T findShaderUniformBuffer(uint32_t binding) const;
    /// Find the shader image sampler buffer at the given binding
    /// @param binding Binding to use
    /// @returns The image sampler buffer at the given binding
    [[nodiscard]] AxrShaderImageSamplerBufferLinkConst_T findShaderImageSamplerBuffer(uint32_t binding) const;

    /// Clean up this class
    void cleanup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrShaderValues_T m_RawShaderValues;

public:
    // ----------------------------------------- //
    // Static Public Functions
    // ----------------------------------------- //

    // ----------------- Shader Values Clone and Destroy -----------------

    /// Clone the given shader values
    /// @param values Shader values to clone
    /// @returns The cloned shader values
    [[nodiscard]] static AxrShaderValues_T clone(AxrShaderValuesConst_T values);
    /// Destroy the given shader values
    /// @param values Shader values to destroy
    static void destroy(AxrShaderValues_T& values);

    // ---- Shader Buffer Links ----

    /// Clone the given shader buffer links
    /// @param shaderBufferLinksCount Number of shader buffer links
    /// @param shaderBufferLinks Shader buffer links to clone
    /// @returns The cloned shader buffer links
    [[nodiscard]] static AxrShaderBufferLink_T* clone(
        uint32_t shaderBufferLinksCount,
        const AxrShaderBufferLinkConst_T* shaderBufferLinks
    );
    /// Destroy the given shader buffer links
    /// @param shaderBufferLinksCount Number of shader buffer links
    /// @param shaderBufferLinks Shader buffer links to destroy
    static void destroy(uint32_t& shaderBufferLinksCount, AxrShaderBufferLink_T*& shaderBufferLinks);
    /// Clone the given shader buffer link
    /// @param shaderBufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    [[nodiscard]] static AxrShaderBufferLink_T clone(AxrShaderBufferLinkConst_T shaderBufferLink);
    /// Destroy the given shader buffer link
    /// @param shaderBufferLink Shader buffer link to destroy
    static void destroy(AxrShaderBufferLink_T& shaderBufferLink);

    /// Clone the given shader uniform buffer link
    /// @param shaderBufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    [[nodiscard]] static AxrShaderUniformBufferLink_T clone(AxrShaderUniformBufferLinkConst_T shaderBufferLink);
    /// Destroy the given shader uniform buffer link
    /// @param shaderBufferLink Shader buffer link to destroy
    static void destroy(AxrShaderUniformBufferLink_T& shaderBufferLink);

    /// Clone the given shader image sampler buffer link
    /// @param shaderBufferLink Shader buffer link to clone
    /// @returns The cloned shader buffer link
    [[nodiscard]] static AxrShaderImageSamplerBufferLink_T clone(
        AxrShaderImageSamplerBufferLinkConst_T shaderBufferLink
    );
    /// Destroy the given shader image sampler buffer link
    /// @param shaderBufferLink Shader buffer link to destroy
    static void destroy(AxrShaderImageSamplerBufferLink_T& shaderBufferLink);

    // ----------------- Shader Value Validation -----------------

    /// Check if the given shader values are valid
    /// @param values Shader values to check
    /// @returns True if the shader values are valid
    [[nodiscard]] static bool isValid(AxrShaderValuesConst_T values);

    /// Check if the given shader buffer links are valid
    /// @param bufferLinksCount Number of shader buffer links
    /// @param bufferLinks Shader buffer links to check
    /// @returns True if the shader buffer links are valid
    [[nodiscard]] static bool isValid(
        uint32_t bufferLinksCount,
        const AxrShaderBufferLinkConst_T* bufferLinks
    );
};
