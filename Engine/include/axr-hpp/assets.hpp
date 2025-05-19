#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "common/enums.hpp"
#include "axr/assets.h"

namespace axr {
    // ---------------------------------------------------------------------------------- //
    //                                  Shader Properties                                 //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    // ---- Shader Properties ----

    /// Shader stage enum
    enum class ShaderStageEnum {
        Undefined = AXR_SHADER_STAGE_UNDEFINED,
        Vertex = AXR_SHADER_STAGE_VERTEX,
        Fragment = AXR_SHADER_STAGE_FRAGMENT,
    };

    // ---- Shader Buffer Layout ----

    /// Shader buffer layout enum
    enum class ShaderBufferLayoutEnum {
        Undefined = AXR_SHADER_BUFFER_LAYOUT_UNDEFINED,
        UniformBuffer = AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER,
        ImageSamplerBuffer = AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER,
        PushConstantsBuffer = AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER,
    };

    // ---- Vertex Attribute ----

    /// Shader vertex attribute enum
    enum class ShaderVertexAttributeEnum {
        Undefined = AXR_SHADER_VERTEX_ATTRIBUTE_UNDEFINED,
        Position = AXR_SHADER_VERTEX_ATTRIBUTE_POSITION,
        Color = AXR_SHADER_VERTEX_ATTRIBUTE_COLOR,
        TexCoords = AXR_SHADER_VERTEX_ATTRIBUTE_TEX_COORDS,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    // ---- Shader Properties ----

    /// Vertex shader properties
    struct VertexShaderProperties {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const AxrShaderStageEnum Type = AXR_SHADER_STAGE_VERTEX;
        uint32_t VertexAttributesCount;
        AxrShaderVertexAttribute* VertexAttributes;
        uint32_t BufferLayoutsCount;
        AxrShaderBufferLayout_T* BufferLayouts;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        VertexShaderProperties():
            VertexAttributesCount(0),
            VertexAttributes(nullptr),
            BufferLayoutsCount(0),
            BufferLayouts(nullptr) {
        }

        /// Copy Constructor
        /// @param src Source VertexShaderProperties to copy from
        VertexShaderProperties(const VertexShaderProperties& src) = delete;
        /// Move Constructor
        /// @param src Source VertexShaderProperties to move from
        VertexShaderProperties(VertexShaderProperties&& src) noexcept = delete;

        // ---- Destructor ----

        /// Destructor
        ~VertexShaderProperties() {
            clearVertexAttributes();
            clearBufferLayouts();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source VertexShaderProperties to copy from
        VertexShaderProperties& operator=(const VertexShaderProperties& src) = delete;
        /// Move Assignment Operator
        /// @param src Source VertexShaderProperties to move from
        VertexShaderProperties& operator=(VertexShaderProperties&& src) noexcept = delete;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if these vertex shader properties are valid
        /// @returns True if these shader properties are valid
        [[nodiscard]] bool isValid() const {
            return axrVertexShaderPropertiesIsValid(toRaw());
        }

        /// Clone the raw vertex shader properties
        /// @returns The cloned raw shader properties
        [[nodiscard]] AxrVertexShaderProperties_T cloneRaw() const {
            return axrVertexShaderPropertiesClone(toRaw());
        }

        /// Get a handle to the VertexShaderProperties as an AxrVertexShaderProperties
        /// @returns This as an AxrVertexShaderProperties
        const AxrVertexShaderProperties* toRaw() const {
            return reinterpret_cast<const AxrVertexShaderProperties*>(this);
        }

        /// Get a handle to the VertexShaderProperties as an AxrVertexShaderProperties
        /// @returns This as an AxrVertexShaderProperties
        AxrVertexShaderProperties* toRaw() {
            return reinterpret_cast<AxrVertexShaderProperties*>(this);
        }

        // ---- Vertex Attributes ----

        /// Add a vertex attribute
        /// @param type Vertex attribute type
        /// @param binding Vertex attribute binding
        /// @param location Vertex attribute location
        void addVertexAttribute(
            const axr::ShaderVertexAttributeEnum type,
            const uint32_t binding,
            const uint32_t location
        ) {
            resizeVertexAttributes(VertexAttributesCount + 1);

            const AxrShaderVertexAttribute vertexAttribute{
                .Type = static_cast<AxrShaderVertexAttributeEnum>(type),
                .Binding = binding,
                .Location = location
            };
            VertexAttributes[VertexAttributesCount - 1] = axrShaderVertexAttributeClone(vertexAttribute);
        }

        /// Clear the vertex attributes
        void clearVertexAttributes() {
            if (VertexAttributes == nullptr) return;

            for (uint32_t i = 0; i < VertexAttributesCount; ++i) {
                axrShaderVertexAttributeDestroy(&VertexAttributes[i]);
            }

            delete[] VertexAttributes;
            VertexAttributes = nullptr;
            VertexAttributesCount = 0;
        }

        // ---- Buffer Layouts ----

        /// Add a uniform buffer layout
        /// @param binding Uniform buffer layout binding
        /// @param bufferSize Uniform buffer layout buffer size
        void addUniformBufferLayout(const uint32_t binding, const uint64_t bufferSize) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderUniformBufferLayout bufferLayout{
                .Binding = binding,
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderUniformBufferLayoutClone(&bufferLayout)
            );
        }

        /// Add an image sampler buffer layout
        /// @param binding Image sampler buffer layout binding
        void addImageSamplerBufferLayout(const uint32_t binding) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderImageSamplerBufferLayout bufferLayout{
                .Binding = binding,
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderImageSamplerBufferLayoutClone(&bufferLayout)
            );
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Add a push constants buffer layout
        /// @param bufferSize Push constants buffer layout buffer size
        void addPushConstantsBufferLayout(const uint32_t bufferSize) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderPushConstantsBufferLayout bufferLayout{
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderPushConstantsBufferLayoutClone(&bufferLayout)
            );
        }
#endif

        /// CLear the buffer layouts
        void clearBufferLayouts() {
            if (BufferLayouts == nullptr) return;

            for (uint32_t i = 0; i < BufferLayoutsCount; ++i) {
                if (BufferLayouts[i] == nullptr) continue;

                axrShaderBufferLayoutDestroy(&BufferLayouts[i]);
            }

            delete[] BufferLayouts;
            BufferLayouts = nullptr;
            BufferLayoutsCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        // ---- Vertex Attributes ----

        /// Resize the vertex attributes
        /// @param size New size
        void resizeVertexAttributes(const uint32_t size) {
            auto newVertexAttributes = new AxrShaderVertexAttribute[size]{};
            for (uint32_t i = 0; i < std::min(VertexAttributesCount, size); ++i) {
                // Move vertex attributes to new array
                newVertexAttributes[i] = VertexAttributes[i];
                VertexAttributes[i] = {};
            }

            clearVertexAttributes();
            VertexAttributes = newVertexAttributes;
            VertexAttributesCount = size;
        }

        // ---- Buffer Layouts ----

        /// Resize the buffer layouts
        /// @param size New size
        void resizeBufferLayouts(const uint32_t size) {
            auto newBufferLayouts = new AxrShaderBufferLayout_T[size]{};
            for (uint32_t i = 0; i < std::min(BufferLayoutsCount, size); ++i) {
                // Move buffer layouts to new array
                newBufferLayouts[i] = BufferLayouts[i];
                BufferLayouts[i] = nullptr;
            }

            clearBufferLayouts();
            BufferLayouts = newBufferLayouts;
            BufferLayoutsCount = size;
        }
    };

    static_assert(
        sizeof(AxrVertexShaderProperties) == sizeof(axr::VertexShaderProperties),
        "Original type and wrapper have different size!"
    );

    /// Fragment shader properties
    struct FragmentShaderProperties {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const AxrShaderStageEnum Type = AXR_SHADER_STAGE_FRAGMENT;
        uint32_t BufferLayoutsCount;
        AxrShaderBufferLayout_T* BufferLayouts;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        FragmentShaderProperties():
            BufferLayoutsCount(0),
            BufferLayouts(nullptr) {
        }

        /// Copy Constructor
        /// @param src Source FragmentShaderProperties to copy from
        FragmentShaderProperties(const FragmentShaderProperties& src) = delete;
        /// Move Constructor
        /// @param src Source FragmentShaderProperties to move from
        FragmentShaderProperties(FragmentShaderProperties&& src) noexcept = delete;

        // ---- Destructor ----

        /// Destructor
        ~FragmentShaderProperties() {
            clearBufferLayouts();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source FragmentShaderProperties to copy from
        FragmentShaderProperties& operator=(const FragmentShaderProperties& src) = delete;
        /// Move Assignment Operator
        /// @param src Source FragmentShaderProperties to move from
        FragmentShaderProperties& operator=(FragmentShaderProperties&& src) noexcept = delete;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if these fragment shader properties are valid
        /// @returns True if these shader properties are valid
        [[nodiscard]] bool isValid() const {
            return axrFragmentShaderPropertiesIsValid(toRaw());
        }

        /// Clone the raw fragment shader properties
        /// @returns The cloned raw shader properties
        [[nodiscard]] AxrFragmentShaderProperties_T cloneRaw() const {
            return axrFragmentShaderPropertiesClone(toRaw());
        }

        /// Get a handle to the FragmentShaderProperties as an AxrFragmentShaderProperties
        /// @returns This as an AxrFragmentShaderProperties
        const AxrFragmentShaderProperties* toRaw() const {
            return reinterpret_cast<const AxrFragmentShaderProperties*>(this);
        }

        /// Get a handle to the FragmentShaderProperties as an AxrFragmentShaderProperties
        /// @returns This as an AxrFragmentShaderProperties
        AxrFragmentShaderProperties* toRaw() {
            return reinterpret_cast<AxrFragmentShaderProperties*>(this);
        }

        // ---- Buffer Layouts ----

        /// Add a uniform buffer layout
        /// @param binding Uniform buffer layout binding
        /// @param bufferSize Uniform buffer layout buffer size
        void addUniformBufferLayout(const uint32_t binding, const uint64_t bufferSize) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderUniformBufferLayout bufferLayout{
                .Binding = binding,
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderUniformBufferLayoutClone(&bufferLayout)
            );
        }

        /// Add an image sampler buffer layout
        /// @param binding Image sampler buffer layout binding
        void addImageSamplerBufferLayout(const uint32_t binding) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderImageSamplerBufferLayout bufferLayout{
                .Binding = binding,
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderImageSamplerBufferLayoutClone(&bufferLayout)
            );
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Add a push constants buffer layout
        /// @param bufferSize Push constants buffer layout buffer size
        void addPushConstantsBufferLayout(const uint32_t bufferSize) {
            resizeBufferLayouts(BufferLayoutsCount + 1);

            const AxrShaderPushConstantsBufferLayout bufferLayout{
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutsCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderPushConstantsBufferLayoutClone(&bufferLayout)
            );
        }
#endif

        /// CLear the buffer layouts
        void clearBufferLayouts() {
            if (BufferLayouts == nullptr) return;

            for (uint32_t i = 0; i < BufferLayoutsCount; ++i) {
                if (BufferLayouts[i] == nullptr) continue;

                axrShaderBufferLayoutDestroy(&BufferLayouts[i]);
            }

            delete[] BufferLayouts;
            BufferLayouts = nullptr;
            BufferLayoutsCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        // ---- Buffer Layouts ----

        /// Resize the buffer layouts
        /// @param size New size
        void resizeBufferLayouts(const uint32_t size) {
            auto newBufferLayouts = new AxrShaderBufferLayout_T[size]{};
            for (uint32_t i = 0; i < std::min(BufferLayoutsCount, size); ++i) {
                // Move buffer layouts to new array
                newBufferLayouts[i] = BufferLayouts[i];
                BufferLayouts[i] = nullptr;
            }

            clearBufferLayouts();
            BufferLayouts = newBufferLayouts;
            BufferLayoutsCount = size;
        }
    };

    static_assert(
        sizeof(AxrFragmentShaderProperties) == sizeof(axr::FragmentShaderProperties),
        "Original type and wrapper have different size!"
    );

    // ---------------------------------------------------------------------------------- //
    //                                   Shader Assets                                    //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Shader Config Definition
    // ----------------------------------------- //

    /// Shader Config
    struct ShaderConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* Name;
        const char* FilePath;
        AxrShaderProperties_T Properties;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ShaderConfig() :
            Name(""),
            FilePath(""),
            Properties(nullptr) {
        }

        /// Constructor
        /// @param name The shader name
        /// @param filePath The shader file path
        /// @param vertexShaderProperties The vertex shader properties to use
        ShaderConfig(
            const char* name,
            const char* filePath,
            const axr::VertexShaderProperties& vertexShaderProperties
        ) : Name(name),
            FilePath(filePath),
            Properties(reinterpret_cast<AxrShaderProperties_T>(vertexShaderProperties.cloneRaw())) {
        }

        /// Constructor
        /// @param name The shader name
        /// @param filePath The shader file path
        /// @param fragmentShaderProperties The fragment shader properties to use
        ShaderConfig(
            const char* name,
            const char* filePath,
            const axr::FragmentShaderProperties& fragmentShaderProperties
        ) : Name(name),
            FilePath(filePath),
            Properties(reinterpret_cast<AxrShaderProperties_T>(fragmentShaderProperties.cloneRaw())) {
        }

        /// Copy Constructor
        /// @param src Source ShaderConfig to copy from
        ShaderConfig(const ShaderConfig& src) {
            Name = src.Name;
            FilePath = src.FilePath;
            if (src.Properties != nullptr) {
                Properties = axrShaderPropertiesClone(src.Properties);
            } else {
                Properties = nullptr;
            }
        }

        /// Move Constructor
        /// @param src Source ShaderConfig to move from
        ShaderConfig(ShaderConfig&& src) noexcept {
            Name = src.Name;
            FilePath = src.FilePath;
            Properties = src.Properties;

            src.Name = "";
            src.FilePath = "";
            src.Properties = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~ShaderConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ShaderConfig to copy from
        ShaderConfig& operator=(const ShaderConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;
                if (src.Properties != nullptr) {
                    Properties = axrShaderPropertiesClone(src.Properties);
                } else {
                    Properties = nullptr;
                }
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ShaderConfig to move from
        ShaderConfig& operator=(ShaderConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;
                Properties = src.Properties;

                src.Name = "";
                src.FilePath = "";
                src.Properties = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if this shader config is valid
        /// @returns True if this shader config is valid
        [[nodiscard]] bool isValid() const {
            return axrShaderConfigIsValid(toRaw());
        }

        /// Get a handle to the ShaderConfig as an AxrShaderConfig
        /// @returns This as an AxrShaderConfig
        const AxrShaderConfig* toRaw() const {
            return reinterpret_cast<const AxrShaderConfig*>(this);
        }

        /// Get a handle to the ShaderConfig as an AxrShaderConfig
        /// @returns This as an AxrShaderConfig
        AxrShaderConfig* toRaw() {
            return reinterpret_cast<AxrShaderConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            if (Properties != nullptr) {
                axrShaderPropertiesDestroy(&Properties);
            }

            Name = "";
            FilePath = "";
        }
    };

    static_assert(
        sizeof(AxrShaderConfig) == sizeof(axr::ShaderConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Shader Definition
    // ----------------------------------------- //

    /// Shader
    class Shader {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param shader Shader handle
        Shader(const AxrShader_T shader):
            m_Shader(shader) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the shader's name
        /// @returns The shader's name
        [[nodiscard]] const char* getName() const {
            return axrShaderGetName(m_Shader);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrShader_T m_Shader;
    };

    // ---------------------------------------------------------------------------------- //
    //                                   Shader Values                                    //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Shader buffer link type enum
    enum class ShaderBufferLinkEnum {
        Undefined = AXR_SHADER_BUFFER_LINK_UNDEFINED,
        UniformBuffer = AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER,
        ImageSamplerBuffer = AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Shader values
    struct ShaderValues {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t BufferLinksCount;
        AxrShaderBufferLink_T* BufferLinks;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ShaderValues():
            BufferLinksCount(0),
            BufferLinks(nullptr) {
        }

        /// Copy Constructor
        /// @param src Source ShaderValues to copy from
        ShaderValues(const ShaderValues& src) = delete;
        /// Move Constructor
        /// @param src Source ShaderValues to move from
        ShaderValues(ShaderValues&& src) noexcept = delete;

        // ---- Destructor ----

        /// Destructor
        ~ShaderValues() {
            clearBufferLinks();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ShaderValues to copy from
        ShaderValues& operator=(const ShaderValues& src) = delete;
        /// Move Assignment Operator
        /// @param src Source ShaderValues to move from
        ShaderValues& operator=(ShaderValues&& src) noexcept = delete;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if these shader values are valid
        /// @returns True if these shader values are valid
        [[nodiscard]] bool isValid() const {
            return axrShaderValuesIsValid(toRaw());
        }

        /// Clone the raw shader values
        /// @returns The cloned raw shader values
        [[nodiscard]] AxrShaderValues_T cloneRaw() const {
            return axrShaderValuesClone(toRaw());
        }

        /// Get a handle to the ShaderValues as an AxrShaderValues
        /// @returns This as an AxrShaderValues
        const AxrShaderValues* toRaw() const {
            return reinterpret_cast<const AxrShaderValues*>(this);
        }

        /// Get a handle to the ShaderValues as an AxrShaderValues
        /// @returns This as an AxrShaderValues
        AxrShaderValues* toRaw() {
            return reinterpret_cast<AxrShaderValues*>(this);
        }

        // ---- Buffer Links ----

        /// Add a uniform buffer link
        /// @param binding Uniform buffer binding
        /// @param bufferName Uniform buffer name 
        void addUniformBufferLink(const uint32_t binding, const char* bufferName) {
            resizeBufferLinks(BufferLinksCount + 1);

            const AxrShaderUniformBufferLink bufferLink{
                .Binding = binding,
                .BufferName = bufferName
            };
            BufferLinks[BufferLinksCount - 1] = reinterpret_cast<AxrShaderBufferLink_T>(
                axrShaderUniformBufferLinkClone(&bufferLink)
            );
        }

        /// Add an image sampler buffer link
        /// @param binding Image sampler buffer binding
        /// @param imageName Image sampler name
        void addImageSamplerBufferLink(const uint32_t binding, const char* imageName) {
            resizeBufferLinks(BufferLinksCount + 1);

            const AxrShaderImageSamplerBufferLink bufferLink{
                .Binding = binding,
                .ImageName = imageName
            };
            BufferLinks[BufferLinksCount - 1] = reinterpret_cast<AxrShaderBufferLink_T>(
                axrShaderImageSamplerBufferLinkClone(&bufferLink)
            );
        }

        /// Clear the buffer links 
        void clearBufferLinks() {
            if (BufferLinks == nullptr) return;

            for (uint32_t i = 0; i < BufferLinksCount; ++i) {
                if (BufferLinks[i] == nullptr) continue;

                axrShaderBufferLinkDestroy(&BufferLinks[i]);
            }

            delete[] BufferLinks;
            BufferLinks = nullptr;
            BufferLinksCount = 0;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        // ---- Buffer Links ----

        /// Resize the buffer links
        /// @param size New size
        void resizeBufferLinks(const uint32_t size) {
            auto newBufferLinks = new AxrShaderBufferLink_T[size]{};
            for (uint32_t i = 0; i < std::min(BufferLinksCount, size); ++i) {
                // Move buffer links to new array
                newBufferLinks[i] = BufferLinks[i];
                BufferLinks[i] = nullptr;
            }

            clearBufferLinks();
            BufferLinks = newBufferLinks;
            BufferLinksCount = size;
        }
    };

    static_assert(
        sizeof(AxrShaderValues) == sizeof(axr::ShaderValues),
        "Original type and wrapper have different size!"
    );

    // ---------------------------------------------------------------------------------- //
    //                                  Material Assets                                   //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Material Config definition
    // ----------------------------------------- //

    /// Material Config
    struct MaterialConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* Name;
        const char* VertexShaderName;
        const char* FragmentShaderName;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        const char* PushConstantsBufferName;
#endif
        AxrShaderValues_T VertexShaderValues;
        AxrShaderValues_T FragmentShaderValues;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        MaterialConfig() :
            Name(""),
            VertexShaderName(""),
            FragmentShaderName(""),
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            PushConstantsBufferName(""),
#endif
            VertexShaderValues(nullptr),
            FragmentShaderValues(nullptr) {
        }

        /// Constructor
        /// @param name The material name
        /// @param vertexShaderName The vertex shader name
        /// @param fragmentShaderName The fragment shader name
        /// @param vertexShaderValues The vertex shader values to use
        /// @param fragmentShaderValues The fragment shader values to use
        MaterialConfig(
            const char* name,
            const char* vertexShaderName,
            const char* fragmentShaderName,
            const axr::ShaderValues& vertexShaderValues,
            const axr::ShaderValues& fragmentShaderValues
        ) : Name(name),
            VertexShaderName(vertexShaderName),
            FragmentShaderName(fragmentShaderName),
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            PushConstantsBufferName(""),
#endif
            VertexShaderValues(vertexShaderValues.cloneRaw()),
            FragmentShaderValues(fragmentShaderValues.cloneRaw()) {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Constructor
        /// @param name The material name
        /// @param vertexShaderName The vertex shader name
        /// @param fragmentShaderName The fragment shader name
        /// @param pushConstantsBufferName The push constants buffer name
        /// @param vertexShaderValues The vertex shader values to use
        /// @param fragmentShaderValues The fragment shader values to use
        MaterialConfig(
            const char* name,
            const char* vertexShaderName,
            const char* fragmentShaderName,
            const char* pushConstantsBufferName,
            const axr::ShaderValues& vertexShaderValues,
            const axr::ShaderValues& fragmentShaderValues
        ) : Name(name),
            VertexShaderName(vertexShaderName),
            FragmentShaderName(fragmentShaderName),
            PushConstantsBufferName(pushConstantsBufferName),
            VertexShaderValues(vertexShaderValues.cloneRaw()),
            FragmentShaderValues(fragmentShaderValues.cloneRaw()) {
        }
#endif

        /// Copy Constructor
        /// @param src Source MaterialConfig to copy from
        MaterialConfig(const axr::MaterialConfig& src) {
            Name = src.Name;
            VertexShaderName = src.VertexShaderName;
            FragmentShaderName = src.FragmentShaderName;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            PushConstantsBufferName = src.PushConstantsBufferName;
#endif

            if (src.VertexShaderValues != nullptr) {
                VertexShaderValues = axrShaderValuesClone(src.VertexShaderValues);
            } else {
                VertexShaderValues = nullptr;
            }

            if (src.FragmentShaderValues != nullptr) {
                FragmentShaderValues = axrShaderValuesClone(src.FragmentShaderValues);
            } else {
                FragmentShaderValues = nullptr;
            }
        }

        /// Move Constructor
        /// @param src Source MaterialConfig to move from
        MaterialConfig(MaterialConfig&& src) noexcept {
            Name = src.Name;
            VertexShaderName = src.VertexShaderName;
            FragmentShaderName = src.FragmentShaderName;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            PushConstantsBufferName = src.PushConstantsBufferName;
#endif
            VertexShaderValues = src.VertexShaderValues;
            FragmentShaderValues = src.FragmentShaderValues;

            src.Name = "";
            src.VertexShaderName = "";
            src.FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            src.PushConstantsBufferName = "";
#endif
            src.VertexShaderValues = nullptr;
            src.FragmentShaderValues = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~MaterialConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source MaterialConfig to copy from
        MaterialConfig& operator=(const MaterialConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                VertexShaderName = src.VertexShaderName;
                FragmentShaderName = src.FragmentShaderName;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                PushConstantsBufferName = src.PushConstantsBufferName;
#endif

                if (src.VertexShaderValues != nullptr) {
                    VertexShaderValues = axrShaderValuesClone(src.VertexShaderValues);
                } else {
                    VertexShaderValues = nullptr;
                }

                if (src.FragmentShaderValues != nullptr) {
                    FragmentShaderValues = axrShaderValuesClone(src.FragmentShaderValues);
                } else {
                    FragmentShaderValues = nullptr;
                }
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source MaterialConfig to move from
        MaterialConfig& operator=(MaterialConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                VertexShaderName = src.VertexShaderName;
                FragmentShaderName = src.FragmentShaderName;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                PushConstantsBufferName = src.PushConstantsBufferName;
#endif
                VertexShaderValues = src.VertexShaderValues;
                FragmentShaderValues = src.FragmentShaderValues;

                src.Name = "";
                src.VertexShaderName = "";
                src.FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                src.PushConstantsBufferName = "";
#endif
                src.VertexShaderValues = nullptr;
                src.FragmentShaderValues = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if this material config is valid
        /// @returns True if this material config is valid
        [[nodiscard]] bool isValid() const {
            return axrMaterialConfigIsValid(toRaw());
        }

        /// Get a handle to the MaterialConfig as an AxrMaterialConfig
        /// @returns This as an AxrMaterialConfig
        const AxrMaterialConfig* toRaw() const {
            return reinterpret_cast<const AxrMaterialConfig*>(this);
        }

        /// Get a handle to the MaterialConfig as an AxrMaterialConfig
        /// @returns This as an AxrMaterialConfig
        AxrMaterialConfig* toRaw() {
            return reinterpret_cast<AxrMaterialConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            if (VertexShaderValues != nullptr) {
                axrShaderValuesDestroy(&VertexShaderValues);
            }
            if (FragmentShaderValues != nullptr) {
                axrShaderValuesDestroy(&FragmentShaderValues);
            }

            Name = "";
            VertexShaderName = "";
            FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            PushConstantsBufferName = "";
#endif
        }
    };

    static_assert(
        sizeof(AxrMaterialConfig) == sizeof(axr::MaterialConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Material Definition
    // ----------------------------------------- //

    /// Material
    class Material {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param material Material handle
        Material(const AxrMaterial_T material):
            m_Material(material) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the material's name
        /// @returns The material's name
        [[nodiscard]] const char* getName() const {
            return axrMaterialGetName(m_Material);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrMaterial_T m_Material;
    };

    // ---------------------------------------------------------------------------------- //
    //                                   Model Assets                                     //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vertex
    struct Vertex {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 TexCoords;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Vertex(): Position(0.0f),
            Color(0.0f),
            TexCoords(0.0f) {
        }

        /// Constructor
        /// @param position Vertex position
        /// @param color Vertex color
        /// @param texCoords Vertex Texture Coordinates
        Vertex(const glm::vec3 position, const glm::vec3 color, const glm::vec2 texCoords):
            Position(position),
            Color(color),
            TexCoords(texCoords) {
        }

        /// Copy Constructor
        /// @param src Source Vertex to copy from
        Vertex(const Vertex& src) {
            Position = src.Position;
            Color = src.Color;
            TexCoords = src.TexCoords;
        }

        /// Move Constructor
        /// @param src Source Vertex to move from
        Vertex(Vertex&& src) noexcept {
            Position = src.Position;
            Color = src.Color;
            TexCoords = src.TexCoords;

            src.Position = glm::vec3(0.0f);
            src.Color = glm::vec3(0.0f);
            src.TexCoords = glm::vec3(0.0f);
        }

        // ---- Destructor ----

        /// Destructor
        ~Vertex() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Vertex to copy from
        Vertex& operator=(const Vertex& src) {
            if (this != &src) {
                cleanup();

                Position = src.Position;
                Color = src.Color;
                TexCoords = src.TexCoords;
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Vertex to move from
        Vertex& operator=(Vertex&& src) noexcept {
            if (this != &src) {
                cleanup();

                Position = src.Position;
                Color = src.Color;
                TexCoords = src.TexCoords;

                src.Position = glm::vec3(0.0f);
                src.Color = glm::vec3(0.0f);
                src.TexCoords = glm::vec3(0.0f);
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Vertex as an AxrVertex
        /// @returns This as an AxrVertex
        const AxrVertex* toRaw() const {
            return reinterpret_cast<const AxrVertex*>(this);
        }

        /// Get a handle to the Vertex as an AxrVertex
        /// @returns This as an AxrVertex
        AxrVertex* toRaw() {
            return reinterpret_cast<AxrVertex*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Position = glm::vec3(0.0f);
            Color = glm::vec3(0.0f);
            TexCoords = glm::vec3(0.0f);
        }
    };

    static_assert(
        sizeof(AxrVertex) == sizeof(axr::Vertex),
        "Original type and wrapper have different size!"
    );

    /// Mesh
    struct Mesh {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        uint32_t VerticesCount;
        axr::Vertex* Vertices;
        uint32_t IndicesCount;
        uint32_t* Indices;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Mesh(): VerticesCount(0),
            Vertices(nullptr),
            IndicesCount(0),
            Indices(nullptr) {
        }

        /// Constructor
        /// @param vertices Vertices to copy
        /// @param indices Indices to copy
        Mesh(const std::vector<axr::Vertex>& vertices, const std::vector<uint32_t>& indices) {
            VerticesCount = static_cast<uint32_t>(vertices.size());
            Vertices = cloneVertices(VerticesCount, vertices.data());
            IndicesCount = static_cast<uint32_t>(indices.size());
            Indices = cloneIndices(IndicesCount, indices.data());
        }

        /// Copy Constructor
        /// @param src Source Mesh to copy from
        Mesh(const Mesh& src) {
            VerticesCount = src.VerticesCount;
            Vertices = cloneVertices(src.VerticesCount, src.Vertices);
            IndicesCount = src.IndicesCount;
            Indices = cloneIndices(src.IndicesCount, src.Indices);
        }

        /// Move Constructor
        /// @param src Source Mesh to move from
        Mesh(Mesh&& src) noexcept {
            VerticesCount = src.VerticesCount;
            Vertices = src.Vertices;
            IndicesCount = src.IndicesCount;
            Indices = src.Indices;

            src.VerticesCount = 0;
            src.Vertices = nullptr;
            src.IndicesCount = 0;
            src.Indices = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~Mesh() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Mesh to copy from
        Mesh& operator=(const Mesh& src) {
            if (this != &src) {
                cleanup();

                VerticesCount = src.VerticesCount;
                Vertices = cloneVertices(src.VerticesCount, src.Vertices);
                IndicesCount = src.IndicesCount;
                Indices = cloneIndices(src.IndicesCount, src.Indices);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Mesh to move from
        Mesh& operator=(Mesh&& src) noexcept {
            if (this != &src) {
                cleanup();

                VerticesCount = src.VerticesCount;
                Vertices = src.Vertices;
                IndicesCount = src.IndicesCount;
                Indices = src.Indices;

                src.VerticesCount = 0;
                src.Vertices = nullptr;
                src.IndicesCount = 0;
                src.Indices = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Mesh as an AxrMesh
        /// @returns This as an AxrMesh
        const AxrMesh* toRaw() const {
            return reinterpret_cast<const AxrMesh*>(this);
        }

        /// Get a handle to the Mesh as an AxrMesh
        /// @returns This as an AxrMesh
        AxrMesh* toRaw() {
            return reinterpret_cast<AxrMesh*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrMeshDestroyVertices(&VerticesCount, reinterpret_cast<AxrVertex**>(&Vertices));
            axrMeshDestroyIndices(&IndicesCount, &Indices);
        }

        /// Clone the given vertices
        /// @param verticesCount Number of vertices in the given array
        /// @param vertices Vertex array to clone
        /// @returns A cloned array of the given vertices
        axr::Vertex* cloneVertices(const uint32_t verticesCount, const axr::Vertex* vertices) {
            return reinterpret_cast<axr::Vertex*>(axrMeshCloneVertices(
                verticesCount,
                reinterpret_cast<const AxrVertex*>(vertices)
            ));
        }

        /// Clone the given indices
        /// @param indicesCount Number of indices in the given array
        /// @param indices Index array to clone
        /// @returns A cloned array of the given indices
        uint32_t* cloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
            return axrMeshCloneIndices(indicesCount, indices);
        }
    };

    static_assert(
        sizeof(AxrMesh) == sizeof(axr::Mesh),
        "Original type and wrapper have different size!"
    );

    /// Model Config
    struct ModelConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        const char* Name;
        const char* FilePath;
        uint32_t MeshesCount;
        axr::Mesh* Meshes;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelConfig():
            Name(""),
            FilePath(nullptr),
            MeshesCount(0),
            Meshes(nullptr) {
        }

        /// Constructor
        /// @param name Name of the model
        /// @param meshes Model meshes
        ModelConfig(const char* name, const std::vector<axr::Mesh>& meshes):
            Name(name),
            FilePath(nullptr) {
            MeshesCount = static_cast<uint32_t>(meshes.size());
            Meshes = cloneMeshes(MeshesCount, meshes.data());
        }

        /// Constructor
        /// @param name Name of the model
        /// @param filePath Filepath of the model
        ModelConfig(const char* name, const char* filePath):
            Name(name),
            FilePath(filePath),
            MeshesCount(0),
            Meshes(nullptr) {
        }

        /// Copy Constructor
        /// @param src Source ModelConfig to copy from
        ModelConfig(const ModelConfig& src) {
            Name = src.Name;
            FilePath = src.FilePath;
            MeshesCount = src.MeshesCount;
            Meshes = cloneMeshes(src.MeshesCount, src.Meshes);
        }

        /// Move Constructor
        /// @param src Source ModelConfig to move from
        ModelConfig(ModelConfig&& src) noexcept {
            Name = src.Name;
            FilePath = src.FilePath;
            MeshesCount = src.MeshesCount;
            Meshes = src.Meshes;

            src.Name = "";
            src.FilePath = nullptr;
            src.MeshesCount = 0;
            src.Meshes = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelConfig to copy from
        ModelConfig& operator=(const ModelConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;
                MeshesCount = src.MeshesCount;
                Meshes = cloneMeshes(src.MeshesCount, src.Meshes);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelConfig to move from
        ModelConfig& operator=(ModelConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;
                MeshesCount = src.MeshesCount;
                Meshes = src.Meshes;

                src.Name = "";
                src.FilePath = nullptr;
                src.MeshesCount = 0;
                src.Meshes = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelConfig as an AxrModelConfig
        /// @returns This as an AxrModelConfig
        const AxrModelConfig* toRaw() const {
            return reinterpret_cast<const AxrModelConfig*>(this);
        }

        /// Get a handle to the ModelConfig as an AxrModelConfig
        /// @returns This as an AxrModelConfig
        AxrModelConfig* toRaw() {
            return reinterpret_cast<AxrModelConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            FilePath = nullptr;
            axrModelDestroyMeshes(&MeshesCount, reinterpret_cast<AxrMesh**>(&Meshes));
        }

        /// Clone the given meshes
        /// @param meshesCount Number of meshes in the given array
        /// @param meshes Mesh array to clone
        /// @returns A cloned array of the given meshes
        axr::Mesh* cloneMeshes(const uint32_t meshesCount, const axr::Mesh* meshes) const {
            return reinterpret_cast<axr::Mesh*>(axrModelCloneMeshes(
                meshesCount,
                reinterpret_cast<const AxrMesh*>(meshes)
            ));
        }
    };

    static_assert(
        sizeof(AxrModelConfig) == sizeof(axr::ModelConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Model Definition
    // ----------------------------------------- //

    /// Model
    class Model {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param model Model handle
        Model(const AxrModel_T model):
            m_Model(model) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the model's name
        /// @returns The model's name
        [[nodiscard]] const char* getName() const {
            return axrModelGetName(m_Model);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrModel_T m_Model;
    };

    // ---------------------------------------------------------------------------------- //
    //                                    Buffer Assets                                   //
    // ---------------------------------------------------------------------------------- //

    /// Uniform Buffer Config
    struct UniformBufferConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        const char* Name;
        uint64_t DataSize;
        void* Data;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        UniformBufferConfig():
            Name(""),
            DataSize(0),
            Data(nullptr) {
        }

        /// Constructor
        /// @param name Name of the uniform buffer
        /// @param dataSize Data size
        /// @param data Data
        UniformBufferConfig(const char* name, const uint64_t dataSize, const void* data):
            Name(name),
            DataSize(dataSize) {
            Data = axrUniformBufferCloneData(dataSize, data);
        }

        /// Copy Constructor
        /// @param src Source UniformBufferConfig to copy from
        UniformBufferConfig(const UniformBufferConfig& src) {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = axrUniformBufferCloneData(src.DataSize, src.Data);
        }

        /// Move Constructor
        /// @param src Source UniformBufferConfig to move from
        UniformBufferConfig(UniformBufferConfig&& src) noexcept {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = src.Data;

            src.Name = "";
            src.DataSize = 0;
            src.Data = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~UniformBufferConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source UniformBufferConfig to copy from
        UniformBufferConfig& operator=(const UniformBufferConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                DataSize = src.DataSize;
                Data = axrUniformBufferCloneData(src.DataSize, src.Data);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source UniformBufferConfig to move from
        UniformBufferConfig& operator=(UniformBufferConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                DataSize = src.DataSize;
                Data = src.Data;

                src.Name = "";
                src.DataSize = 0;
                src.Data = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the UniformBufferConfig as an AxrUniformBufferConfig
        /// @returns This as an AxrUniformBufferConfig
        const AxrUniformBufferConfig* toRaw() const {
            return reinterpret_cast<const AxrUniformBufferConfig*>(this);
        }

        /// Get a handle to the UniformBufferConfig as an AxrUniformBufferConfig
        /// @returns This as an AxrUniformBufferConfig
        AxrUniformBufferConfig* toRaw() {
            return reinterpret_cast<AxrUniformBufferConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            axrUniformBufferDestroyData(&DataSize, &Data);
        }
    };

    static_assert(
        sizeof(AxrUniformBufferConfig) == sizeof(axr::UniformBufferConfig),
        "Original type and wrapper have different size!"
    );

    /// Push Constants Buffer Config
    struct PushConstantsBufferConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        const char* Name;
        uint32_t DataSize;
        void* Data;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        PushConstantsBufferConfig():
            Name(""),
            DataSize(0),
            Data(nullptr) {
        }

        /// Constructor
        /// @param name Name of the push constants buffer
        /// @param dataSize Data size
        /// @param data Data
        PushConstantsBufferConfig(const char* name, const uint32_t dataSize, const void* data):
            Name(name),
            DataSize(dataSize) {
            Data = axrPushConstantsBufferCloneData(dataSize, data);
        }

        /// Copy Constructor
        /// @param src Source PushConstantsBufferConfig to copy from
        PushConstantsBufferConfig(const PushConstantsBufferConfig& src) {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = axrPushConstantsBufferCloneData(src.DataSize, src.Data);
        }

        /// Move Constructor
        /// @param src Source PushConstantsBufferConfig to move from
        PushConstantsBufferConfig(PushConstantsBufferConfig&& src) noexcept {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = src.Data;

            src.Name = "";
            src.DataSize = 0;
            src.Data = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~PushConstantsBufferConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source PushConstantsBufferConfig to copy from
        PushConstantsBufferConfig& operator=(const PushConstantsBufferConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                DataSize = src.DataSize;
                Data = axrPushConstantsBufferCloneData(src.DataSize, src.Data);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source PushConstantsBufferConfig to move from
        PushConstantsBufferConfig& operator=(PushConstantsBufferConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                DataSize = src.DataSize;
                Data = src.Data;

                src.Name = "";
                src.DataSize = 0;
                src.Data = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the PushConstantsBufferConfig as an AxrPushConstantsBufferConfig
        /// @returns This as an AxrPushConstantsBufferConfig
        const AxrPushConstantsBufferConfig* toRaw() const {
            return reinterpret_cast<const AxrPushConstantsBufferConfig*>(this);
        }

        /// Get a handle to the PushConstantsBufferConfig as an AxrPushConstantsBufferConfig
        /// @returns This as an AxrPushConstantsBufferConfig
        AxrPushConstantsBufferConfig* toRaw() {
            return reinterpret_cast<AxrPushConstantsBufferConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            axrPushConstantsBufferDestroyData(&DataSize, &Data);
        }
    };

    static_assert(
        sizeof(AxrPushConstantsBufferConfig) == sizeof(axr::PushConstantsBufferConfig),
        "Original type and wrapper have different size!"
    );

    // ---------------------------------------------------------------------------------- //
    //                               Engine Defined Assets                                //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    // ---- Shaders ----

    /// Engine defined shader enum
    enum class ShaderEngineAssetEnum {
        Undefined = AXR_SHADER_ENGINE_ASSET_UNDEFINED,
        DefaultVert = AXR_SHADER_ENGINE_ASSET_DEFAULT_VERT,
        DefaultFrag = AXR_SHADER_ENGINE_ASSET_DEFAULT_FRAG,
    };

    // ---- Buffers ----

    /// Engine defined uniform buffer enum
    enum class UniformBufferEngineAssetEnum {
        Undefined = AXR_UNIFORM_BUFFER_ENGINE_ASSET_UNDEFINED,
        SceneData = AXR_UNIFORM_BUFFER_ENGINE_ASSET_SCENE_DATA,
    };

    /// Engine defined push constants buffer enum
    enum class PushConstantsBufferEngineAssetEnum {
        Undefined = AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_UNDEFINED,
        ModelMatrix = AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX,
    };

    // ---- Models ----

    /// Engine defined model enum
    enum class ModelEngineAssetEnum {
        Undefined = AXR_MODEL_ENGINE_ASSET_UNDEFINED,
        Triangle = AXR_MODEL_ENGINE_ASSET_TRIANGLE,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    // ---- Materials ----

    /// Engine asset material named 'Default Material' values
    struct MaterialEngineAsset_DefaultMaterial {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* ImageName;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        MaterialEngineAsset_DefaultMaterial() :
            ImageName{} {
        }

        /// Constructor
        /// @param imageName The image name
        MaterialEngineAsset_DefaultMaterial(
            const char* imageName
        ) : ImageName(imageName) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the MaterialEngineAsset_DefaultMaterial as an AxrMaterialEngineAsset_DefaultMaterial
        /// @returns This as an AxrMaterialEngineAsset_DefaultMaterial
        const AxrMaterialEngineAsset_DefaultMaterial* toRaw() const {
            return reinterpret_cast<const AxrMaterialEngineAsset_DefaultMaterial*>(this);
        }

        /// Get a handle to the MaterialEngineAsset_DefaultMaterial as an AxrMaterialEngineAsset_DefaultMaterial
        /// @returns This as an AxrMaterialEngineAsset_DefaultMaterial
        AxrMaterialEngineAsset_DefaultMaterial* toRaw() {
            return reinterpret_cast<AxrMaterialEngineAsset_DefaultMaterial*>(this);
        }
    };

    static_assert(
        sizeof(AxrMaterialEngineAsset_DefaultMaterial) == sizeof(axr::MaterialEngineAsset_DefaultMaterial),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Function Definitions
    // ----------------------------------------- //

    // ---- Shaders ----

    /// Check if the given name is reserved as a shader engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a shader engine asset name
    inline bool isShaderNameReserved(const char* name) {
        return axrIsShaderNameReserved(name);
    }

    /// Get the name for the given shader engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    inline const char* getShaderEngineAssetName(axr::ShaderEngineAssetEnum engineAssetEnum) {
        return axrGetShaderEngineAssetName(static_cast<AxrShaderEngineAssetEnum>(engineAssetEnum));
    }

    // ---- Buffers ----

    /// Check if the given name is reserved for any engine asset buffer
    /// @param name Name to check
    /// @returns True if the given name is reserved for any engine asset buffer
    inline bool isBufferNameReserved(const char* name) {
        return axrIsBufferNameReserved(name);
    }

    /// Check if the given name is reserved for a uniform buffer engine asset
    /// @param name Name to check
    /// @returns True if the given name is reserved for a uniform buffer engine asset
    inline bool isUniformBufferNameReserved(const char* name) {
        return axrIsUniformBufferNameReserved(name);
    }

    /// Get the name for the given uniform buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    inline const char* getUniformBufferEngineAssetName(axr::UniformBufferEngineAssetEnum engineAssetEnum) {
        return axrGetUniformBufferEngineAssetName(static_cast<AxrUniformBufferEngineAssetEnum>(engineAssetEnum));
    }

    /// Get the data size for the given uniform buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The data size of the given engine asset
    inline uint64_t getUniformBufferEngineAssetDataSize(axr::UniformBufferEngineAssetEnum engineAssetEnum) {
        return axrGetUniformBufferEngineAssetDataSize(static_cast<AxrUniformBufferEngineAssetEnum>(engineAssetEnum));
    }

    /// Check if the given name is reserved for a push constants buffer engine asset
    /// @param name Name to check
    /// @returns True if the given name is reserved for a push constants buffer engine asset
    inline bool isPushConstantsBufferNameReserved(const char* name) {
        return axrIsPushConstantsBufferNameReserved(name);
    }

    /// Get the name for the given push constants buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    inline const char* getPushConstantsBufferEngineAssetName(axr::PushConstantsBufferEngineAssetEnum engineAssetEnum) {
        return axrGetPushConstantsBufferEngineAssetName(
            static_cast<AxrPushConstantsBufferEngineAssetEnum>(engineAssetEnum)
        );
    }

    /// Get the data size for the given push constants buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The data size of the given engine asset
    inline uint32_t getPushConstantsBufferEngineAssetDataSize(
        const axr::PushConstantsBufferEngineAssetEnum engineAssetEnum
    ) {
        return axrGetPushConstantsBufferEngineAssetDataSize(
            static_cast<AxrPushConstantsBufferEngineAssetEnum>(engineAssetEnum)
        );
    }

    // ---------------------------------------------------------------------------------- //
    //                                  Asset Collection                                  //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Asset Collection Definition
    // ----------------------------------------- //

    /// Asset Collection
    class AssetCollection {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param assetCollection Asset collection handle
        AssetCollection(const AxrAssetCollection_T assetCollection):
            m_AssetCollection(assetCollection) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        // ---- Shaders ----

        /// Create a new shader
        /// @param shaderConfig Shader config to use
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createShader(const axr::ShaderConfig& shaderConfig) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateShader(m_AssetCollection, shaderConfig.toRaw()));
        }

        /// Create a new engine asset shader
        /// @param engineAssetEnum Shader engine asset
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createShader(axr::ShaderEngineAssetEnum engineAssetEnum) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateEngineAssetShader(
                m_AssetCollection,
                static_cast<AxrShaderEngineAssetEnum>(engineAssetEnum)
            ));
        }

        // ---- Materials ----

        /// Create a new material
        /// @param materialConfig Material config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createMaterial(
            const axr::MaterialConfig& materialConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateMaterial(
                m_AssetCollection,
                materialConfig.toRaw()
            ));
        }

        /// Create a material using the engine defined material named 'Default Material'
        /// @param materialName Material name
        /// @param materialValues Material values
        /// @returns AXR_SUCCESS if the function succeeded
        axr::Result createMaterial(
            const char* materialName,
            const AxrMaterialEngineAsset_DefaultMaterial materialValues
        ) const {
            return static_cast<axr::Result>(
                axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
                    m_AssetCollection,
                    materialName,
                    materialValues
                )
            );
        }

        // ---- Model ----

        /// Create a new model
        /// @param modelConfig Model config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createModel(
            const axr::ModelConfig& modelConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateModel(
                m_AssetCollection,
                modelConfig.toRaw()
            ));
        }

        /// Create a new engine asset model
        /// @param modelName Model name
        /// @param engineAssetEnum Model engine asset
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createModel(
            const char* modelName,
            axr::ModelEngineAssetEnum engineAssetEnum
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateEngineAssetModel(
                m_AssetCollection,
                modelName,
                static_cast<AxrModelEngineAssetEnum>(engineAssetEnum)
            ));
        }

        // ---- Uniform Buffer ----

        /// Create a new uniform buffer
        /// @param uniformBufferConfig Unuform buffer config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createUniformBuffer(
            const axr::UniformBufferConfig& uniformBufferConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateUniformBuffer(
                m_AssetCollection,
                uniformBufferConfig.toRaw()
            ));
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        // ---- Push Constants Buffer ----

        /// Create a new push constants buffer
        /// @param pushConstantsBufferConfig Push constants buffer config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createPushConstantsBuffer(
            const axr::PushConstantsBufferConfig& pushConstantsBufferConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreatePushConstantsBuffer(
                m_AssetCollection,
                pushConstantsBufferConfig.toRaw()
            ));
        }
#endif

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrAssetCollection_T m_AssetCollection;
    };
}
