#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>

/// Shader properties RAII wrapper
class AxrShaderPropertiesRAII {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrShaderPropertiesRAII();
    /// Constructor
    /// @param properties Shader properties
    AxrShaderPropertiesRAII(AxrShaderPropertiesConst_T properties);
    /// Copy Constructor
    /// @param src Source AxrShaderPropertiesRAII to copy from
    AxrShaderPropertiesRAII(const AxrShaderPropertiesRAII& src);
    /// Move Constructor
    /// @param src Source AxrShaderPropertiesRAII to move from
    AxrShaderPropertiesRAII(AxrShaderPropertiesRAII&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrShaderPropertiesRAII();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrShaderPropertiesRAII to copy from
    AxrShaderPropertiesRAII& operator=(const AxrShaderPropertiesRAII& src);
    /// Move Assignment Operator
    /// @param src Source AxrShaderPropertiesRAII to move from
    AxrShaderPropertiesRAII& operator=(AxrShaderPropertiesRAII&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Check if the shader properties are valid
    /// @returns True if the shader properties are valid
    [[nodiscard]] bool isValid() const;

    /// Get all uniform buffer layouts
    /// @returns A collection of uniform buffer layouts
    [[nodiscard]] std::vector<AxrShaderUniformBufferLayoutConst_T> getUniformBufferLayouts() const;
    /// Get all image sampler buffer layouts
    /// @returns A collection of image sampler buffer layouts
    [[nodiscard]] std::vector<AxrShaderImageSamplerBufferLayoutConst_T> getImageSamplerBufferLayouts() const;
    /// Get the push constants buffer layout
    /// @returns The push constants buffer layout
    [[nodiscard]] AxrShaderPushConstantsBufferLayoutConst_T getPushConstantsBufferLayout() const;

    /// Clean up this class
    void cleanup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrShaderProperties_T m_RawProperties;

public:
    // ----------------------------------------- //
    // Static Public Functions
    // ----------------------------------------- //

    // ----------------- Property Clone and Destroy -----------------

    /// Clone the given shader properties
    /// @param properties Properties to clone
    /// @returns The cloned properties
    [[nodiscard]] static AxrShaderProperties_T clone(AxrShaderPropertiesConst_T properties);
    /// Destroy the given shader properties
    /// @param properties Properties to destroy
    static void destroy(AxrShaderProperties_T& properties);

    // ---- Vertex Shader Properties ----

    /// Clone the given vertex shader properties
    /// @param properties Properties to clone
    /// @returns The cloned properties
    [[nodiscard]] static AxrVertexShaderProperties_T clone(AxrVertexShaderPropertiesConst_T properties);
    /// Destroy the given vertex shader properties
    /// @param properties Properties to destroy
    static void destroy(AxrVertexShaderProperties_T& properties);

    // ---- Fragment Shader Properties ----

    /// Clone the given fragment shader properties
    /// @param properties Properties to clone
    /// @returns The cloned properties
    [[nodiscard]] static AxrFragmentShaderProperties_T clone(AxrFragmentShaderPropertiesConst_T properties);
    /// Destroy the given fragment shader properties
    /// @param properties Properties to destroy
    static void destroy(AxrFragmentShaderProperties_T& properties);

    // ---- Shader Buffer Layouts ----

    /// Clone the given shader buffer layouts
    /// @param shaderBufferLayouts Shader buffer layouts to clone
    /// @param shaderBufferLayoutsCount Number of shader buffer layouts
    /// @returns The cloned shader buffer layouts
    [[nodiscard]] static AxrShaderBufferLayout_T* clone(
        const AxrShaderBufferLayoutConst_T* shaderBufferLayouts,
        uint32_t shaderBufferLayoutsCount
    );
    /// Destroy the given shader buffer layouts
    /// @param shaderBufferLayouts Shader buffer layouts to destroy
    /// @param shaderBufferLayoutsCount Number of shader buffer layouts
    static void destroy(AxrShaderBufferLayout_T*& shaderBufferLayouts, uint32_t& shaderBufferLayoutsCount);
    /// Clone the given shader buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderBufferLayout_T clone(AxrShaderBufferLayoutConst_T shaderBufferLayout);
    /// Destroy the given shader buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderBufferLayout_T& shaderBufferLayout);

    /// Clone the given shader uniform buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderUniformBufferLayout_T clone(AxrShaderUniformBufferLayoutConst_T shaderBufferLayout);
    /// Destroy the given shader uniform buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderUniformBufferLayout_T& shaderBufferLayout);

    /// Clone the given shader image sampler buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderImageSamplerBufferLayout_T clone(
        AxrShaderImageSamplerBufferLayoutConst_T shaderBufferLayout
    );
    /// Destroy the given shader image sampler buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderImageSamplerBufferLayout_T& shaderBufferLayout);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Clone the given shader push constants buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderPushConstantsBufferLayout_T clone(
        AxrShaderPushConstantsBufferLayoutConst_T shaderBufferLayout
    );
    /// Destroy the given shader push constants buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderPushConstantsBufferLayout_T& shaderBufferLayout);
#endif

    // ---- Vertex Attribute ----

    /// Clone the given shader vertex attributes
    /// @param vertexAttributes Shader vertex attributes to clone
    /// @param vertexAttributesCount Number of shader vertex attributes
    /// @returns The cloned shader vertex attributes
    [[nodiscard]] static AxrShaderVertexAttribute* clone(
        const AxrShaderVertexAttribute* vertexAttributes,
        uint32_t vertexAttributesCount
    );
    /// Destroy the given shader vertex attributes
    /// @param vertexAttributes Shader vertex attributes to destroy
    /// @param vertexAttributesCount Number of shader vertex attributes
    static void destroy(AxrShaderVertexAttribute*& vertexAttributes, uint32_t& vertexAttributesCount);
    /// Clone the given shader vertex attribute
    /// @param vertexAttribute Shader vertex attribute to clone
    /// @returns The cloned shader vertex attribute
    [[nodiscard]] static AxrShaderVertexAttribute clone(AxrShaderVertexAttribute vertexAttribute);
    /// Destroy the given shader vertex attribute
    /// @param vertexAttribute Shader vertex attribute to destroy
    static void destroy(AxrShaderVertexAttribute& vertexAttribute);

    // ----------------- Property Validation -----------------

    /// Check if the given shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the shader properties are valid
    [[nodiscard]] static bool isValid(AxrShaderPropertiesConst_T properties);
    /// Check if the given vertex shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the vertex shader properties are valid
    [[nodiscard]] static bool isValid(AxrVertexShaderPropertiesConst_T properties);
    /// Check if the given fragment shader properties are valid
    /// @param properties Properties to check
    /// @returns True if the fragment shader properties are valid
    [[nodiscard]] static bool isValid(AxrFragmentShaderPropertiesConst_T properties);

    /// Check if the given shader buffer layouts are valid
    /// @param bufferLayouts Shader buffer layouts to check
    /// @param bufferLayoutsCount Number of shader buffer layouts
    /// @returns True if the shader buffer layouts are valid
    [[nodiscard]] static bool isValid(
        const AxrShaderBufferLayoutConst_T* bufferLayouts,
        uint32_t bufferLayoutsCount
    );

    /// Check if the given shader vertex attributes are valid
    /// @param vertexAttributes Shader vertex attributes to check
    /// @param vertexAttributesCount Number of shader vertex attributes
    /// @returns True if the shader vertex attributes are valid
    [[nodiscard]] static bool isValid(
        const AxrShaderVertexAttribute* vertexAttributes,
        uint32_t vertexAttributesCount
    );

    /// Check if the given shader properties are compatible and can be used together
    /// @param properties1 First shader properties to check
    /// @param properties2 Second shader properties to check
    /// @returns True if the given shader properties are compatible
    [[nodiscard]] static bool areCompatible(
        const AxrShaderPropertiesRAII& properties1,
        const AxrShaderPropertiesRAII& properties2
    );

    // ----------------- Get Buffer Layouts -----------------

    /// Get all uniform buffer layouts from the given shader buffer layouts
    /// @param bufferLayouts Buffer layouts array
    /// @param bufferLayoutsCount Buffer layouts array length
    /// @returns A collection of uniform buffer layouts
    [[nodiscard]] static std::vector<AxrShaderUniformBufferLayoutConst_T> getUniformBufferLayouts(
        const AxrShaderBufferLayoutConst_T* bufferLayouts,
        uint32_t bufferLayoutsCount
    );
    /// Get all image sampler buffer layouts from the given shader buffer layouts
    /// @param bufferLayouts Buffer layouts array
    /// @param bufferLayoutsCount Buffer layouts array length
    /// @returns A collection of image sampler buffer layouts
    [[nodiscard]] static std::vector<AxrShaderImageSamplerBufferLayoutConst_T> getImageSamplerBufferLayouts(
        const AxrShaderBufferLayoutConst_T* bufferLayouts,
        uint32_t bufferLayoutsCount
    );
    /// Get the push constants buffer layout from the given shader buffer layouts
    /// @param bufferLayouts Buffer layouts array
    /// @param bufferLayoutsCount Buffer layouts array length
    /// @returns The push constants buffer layout
    [[nodiscard]] static AxrShaderPushConstantsBufferLayoutConst_T getPushConstantsBufferLayout(
        const AxrShaderBufferLayoutConst_T* bufferLayouts,
        uint32_t bufferLayoutsCount
    );

private:
    // ----------------------------------------- //
    // Static Private Functions
    // ----------------------------------------- //

    /// Check if the given uniform buffer layout has a duplicate binding but with a different buffer size in the given collection
    /// @param uniformBufferLayout Uniform buffer layout to check
    /// @param uniformBufferBindings Collection to check within
    /// @returns True if the given uniform buffer layout is compatible with the given collection
    [[nodiscard]] static bool isUniformBufferLayoutBindingValid(
        AxrShaderUniformBufferLayoutConst_T uniformBufferLayout,
        std::unordered_map<uint32_t, uint64_t>& uniformBufferBindings
    );
};
