#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>
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

    /// Get all vertex attributes
    /// @returns A collection of vertex attributes
    [[nodiscard]] std::vector<AxrShaderVertexAttribute> getVertexAttributes() const;

    /// Get all uniform buffer layouts
    /// @returns A collection of uniform buffer layouts
    [[nodiscard]] std::vector<AxrShaderUniformBufferLayoutConst_T> getUniformBufferLayouts() const;
    /// Get all dynamic uniform buffer layouts
    /// @returns A collection of dynamic uniform buffer layouts
    [[nodiscard]] std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> getDynamicUniformBufferLayouts() const;
    /// Get all image sampler buffer layouts
    /// @returns A collection of image sampler buffer layouts
    [[nodiscard]] std::vector<AxrShaderImageSamplerBufferLayoutConst_T> getImageSamplerBufferLayouts() const;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Get the push constant buffer layout
    /// @returns The push constant buffer layout
    [[nodiscard]] AxrShaderPushConstantBufferLayoutConst_T getPushConstantBufferLayout() const;
#endif

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
    /// @param shaderBufferLayoutCount Number of shader buffer layouts
    /// @param shaderBufferLayouts Shader buffer layouts to clone
    /// @returns The cloned shader buffer layouts
    [[nodiscard]] static AxrShaderBufferLayout_T* clone(
        uint32_t shaderBufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* shaderBufferLayouts
    );
    /// Destroy the given shader buffer layouts
    /// @param shaderBufferLayoutCount Number of shader buffer layouts
    /// @param shaderBufferLayouts Shader buffer layouts to destroy
    static void destroy(uint32_t& shaderBufferLayoutCount, AxrShaderBufferLayout_T*& shaderBufferLayouts);
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

    /// Clone the given shader dynamic uniform buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderDynamicUniformBufferLayout_T clone(AxrShaderDynamicUniformBufferLayoutConst_T shaderBufferLayout);
    /// Destroy the given shader dynamic uniform buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderDynamicUniformBufferLayout_T& shaderBufferLayout);

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
    /// Clone the given shader push constant buffer layout
    /// @param shaderBufferLayout Shader buffer layout to clone
    /// @returns The cloned shader buffer layout
    [[nodiscard]] static AxrShaderPushConstantBufferLayout_T clone(
        AxrShaderPushConstantBufferLayoutConst_T shaderBufferLayout
    );
    /// Destroy the given shader push constant buffer layout
    /// @param shaderBufferLayout Shader buffer layout to destroy
    static void destroy(AxrShaderPushConstantBufferLayout_T& shaderBufferLayout);
#endif

    // ---- Vertex Attribute ----

    /// Clone the given shader vertex attributes
    /// @param vertexAttributeCount Number of shader vertex attributes
    /// @param vertexAttributes Shader vertex attributes to clone
    /// @returns The cloned shader vertex attributes
    [[nodiscard]] static AxrShaderVertexAttribute* clone(
        uint32_t vertexAttributeCount,
        const AxrShaderVertexAttribute* vertexAttributes
    );
    /// Destroy the given shader vertex attributes
    /// @param vertexAttributeCount Number of shader vertex attributes
    /// @param vertexAttributes Shader vertex attributes to destroy
    static void destroy(uint32_t& vertexAttributeCount, AxrShaderVertexAttribute*& vertexAttributes);
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
    /// @param bufferLayoutCount Number of shader buffer layouts
    /// @param bufferLayouts Shader buffer layouts to check
    /// @returns True if the shader buffer layouts are valid
    [[nodiscard]] static bool isValid(
        uint32_t bufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* bufferLayouts
    );

    /// Check if the given shader vertex attributes are valid
    /// @param vertexAttributeCount Number of shader vertex attributes
    /// @param vertexAttributes Shader vertex attributes to check
    /// @returns True if the shader vertex attributes are valid
    [[nodiscard]] static bool isValid(
        uint32_t vertexAttributeCount,
        const AxrShaderVertexAttribute* vertexAttributes
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
    /// @param bufferLayoutCount Buffer layouts array length
    /// @param bufferLayouts Buffer layouts array
    /// @returns A collection of uniform buffer layouts
    [[nodiscard]] static std::vector<AxrShaderUniformBufferLayoutConst_T> getUniformBufferLayouts(
        uint32_t bufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* bufferLayouts
    );
    /// Get all dynamic uniform buffer layouts from the given shader buffer layouts
    /// @param bufferLayoutCount Buffer layouts array length
    /// @param bufferLayouts Buffer layouts array
    /// @returns A collection of dynamic uniform buffer layouts
    [[nodiscard]] static std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> getDynamicUniformBufferLayouts(
        uint32_t bufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* bufferLayouts
    );
    /// Get all image sampler buffer layouts from the given shader buffer layouts
    /// @param bufferLayoutCount Buffer layouts array length
    /// @param bufferLayouts Buffer layouts array
    /// @returns A collection of image sampler buffer layouts
    [[nodiscard]] static std::vector<AxrShaderImageSamplerBufferLayoutConst_T> getImageSamplerBufferLayouts(
        uint32_t bufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* bufferLayouts
    );
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Get the push constant buffer layout from the given shader buffer layouts
    /// @param bufferLayoutCount Buffer layouts array length
    /// @param bufferLayouts Buffer layouts array
    /// @returns The push constant buffer layout
    [[nodiscard]] static AxrShaderPushConstantBufferLayoutConst_T getPushConstantBufferLayout(
        uint32_t bufferLayoutCount,
        const AxrShaderBufferLayoutConst_T* bufferLayouts
    );
#endif

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
    /// Check if the given dynamic uniform buffer layout has a duplicate binding but with a different buffer size in the given collection
    /// @param uniformBufferLayout Dynamic uniform buffer layout to check
    /// @param uniformBufferBindings Collection to check within
    /// @returns True if the given dynamic uniform buffer layout is compatible with the given collection
    [[nodiscard]] static bool isDynamicUniformBufferLayoutBindingValid(
        AxrShaderDynamicUniformBufferLayoutConst_T uniformBufferLayout,
        std::unordered_map<uint32_t, uint64_t>& uniformBufferBindings
    );
};
