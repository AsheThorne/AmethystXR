#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "common/result.hpp"
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

    // ---- Vertex Property ----

    /// Shader vertex property enum
    enum class ShaderVertexPropertyEnum {
        Undefined = AXR_SHADER_VERTEX_PROPERTY_UNKNOWN,
        Position = AXR_SHADER_VERTEX_PROPERTY_POSITION,
        Color = AXR_SHADER_VERTEX_PROPERTY_COLOR,
        TexCoords = AXR_SHADER_VERTEX_PROPERTY_TEX_COORDS,
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
        AxrShaderVertexProperty* VertexProperties;
        uint32_t VertexPropertiesCount;
        AxrShaderBufferLayout_T* BufferLayouts;
        uint32_t BufferLayoutsCount;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        VertexShaderProperties():
            VertexProperties(nullptr),
            VertexPropertiesCount(0),
            BufferLayouts(nullptr),
            BufferLayoutsCount(0) {
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
            clearVertexProperties();
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

        // ---- Vertex Properties ----

        /// Add a vertex property
        /// @param type Vertex property type
        /// @param binding Vertex property binding
        /// @param location Vertex property location
        void addVertexProperty(
            const axr::ShaderVertexPropertyEnum type,
            const uint32_t binding,
            const uint32_t location
        ) {
            resizeVertexProperties(VertexPropertiesCount + 1);

            const AxrShaderVertexProperty vertexProperty{
                .Type = static_cast<AxrShaderVertexPropertyEnum>(type),
                .Binding = binding,
                .Location = location
            };
            VertexProperties[VertexPropertiesCount - 1] = axrShaderVertexPropertyClone(vertexProperty);
        }

        /// Clear the vertex properties
        void clearVertexProperties() {
            if (VertexProperties == nullptr) return;

            for (uint32_t i = 0; i < VertexPropertiesCount; ++i) {
                axrShaderVertexPropertyDestroy(&VertexProperties[i]);
            }

            delete[] VertexProperties;
            VertexProperties = nullptr;
            VertexPropertiesCount = 0;
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

        // ---- Vertex Properties ----

        /// Resize the vertex properties
        /// @param size New size
        void resizeVertexProperties(const uint32_t size) {
            auto newVertexProperties = new AxrShaderVertexProperty[size]{};
            for (uint32_t i = 0; i < std::min(VertexPropertiesCount, size); ++i) {
                // Move vertex properties to new array
                newVertexProperties[i] = VertexProperties[i];
                VertexProperties[i] = {};
            }

            clearVertexProperties();
            VertexProperties = newVertexProperties;
            VertexPropertiesCount = size;
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
        AxrShaderBufferLayout_T* BufferLayouts;
        uint32_t BufferLayoutsCount;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        FragmentShaderProperties():
            BufferLayouts(nullptr),
            BufferLayoutsCount(0) {
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
            // TODO: Maybe check that the file path is valid too
            return axrShaderPropertiesIsValid(Properties);
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
    enum class ShaderBufferLinkTypeEnum {
        Undefined = AXR_SHADER_BUFFER_LINK_TYPE_UNDEFINED,
        UniformBuffer = AXR_SHADER_BUFFER_LINK_TYPE_UNIFORM_BUFFER,
        ImageSamplerBuffer = AXR_SHADER_BUFFER_LINK_TYPE_IMAGE_SAMPLER_BUFFER,
        PushConstantsBuffer = AXR_SHADER_BUFFER_LINK_TYPE_PUSH_CONSTANTS_BUFFER,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Shader values
    struct ShaderValues {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        AxrShaderBufferLink_T* BufferLinks;
        uint32_t BufferLinksCount;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ShaderValues():
            BufferLinks(nullptr),
            BufferLinksCount(0) {
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

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Add a push constants buffer link
        /// @param bufferName Push constants buffer name
        void addPushConstantsBufferLink(const char* bufferName) {
            resizeBufferLinks(BufferLinksCount + 1);

            const AxrShaderPushConstantsBufferLink bufferLink{
                .BufferName = bufferName
            };
            BufferLinks[BufferLinksCount - 1] = reinterpret_cast<AxrShaderBufferLink_T>(
                axrShaderPushConstantsBufferLinkClone(&bufferLink)
            );
        }
#endif

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
            VertexShaderValues(vertexShaderValues.cloneRaw()),
            FragmentShaderValues(fragmentShaderValues.cloneRaw()) {
        }

        /// Copy Constructor
        /// @param src Source MaterialConfig to copy from
        MaterialConfig(const axr::MaterialConfig& src) {
            Name = src.Name;
            VertexShaderName = src.VertexShaderName;
            FragmentShaderName = src.FragmentShaderName;

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
            VertexShaderValues = src.VertexShaderValues;
            FragmentShaderValues = src.FragmentShaderValues;

            src.Name = "";
            src.VertexShaderName = "";
            src.FragmentShaderName = "";
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
                VertexShaderValues = src.VertexShaderValues;
                FragmentShaderValues = src.FragmentShaderValues;

                src.Name = "";
                src.VertexShaderName = "";
                src.FragmentShaderName = "";
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
            // TODO: Replace with a materialConfigIsValid function instead of these 2?
            //  If we do, then replace the isValid functions in the assetCollection too with it
            return axrShaderValuesIsValid(VertexShaderValues) &&
                axrShaderValuesIsValid(FragmentShaderValues);
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
        ViewProjMatrices = AXR_UNIFORM_BUFFER_ENGINE_ASSET_VIEW_PROJ_MATRICES,
    };

    /// Engine defined push constants buffer enum
    enum class PushConstantsBufferEngineAssetEnum {
        Undefined = AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_UNDEFINED,
        ModelMatrix = AXR_PUSH_CONSTANTS_BUFFER_ENGINE_ASSET_MODEL_MATRIX,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

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

    /// Check if the given name is reserved as a uniform buffer engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a uniform buffer engine asset name
    inline bool isUniformBufferNameReserved(const char* name) {
        return axrIsUniformBufferNameReserved(name);
    }

    /// Get the name for the given uniform buffer engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    inline const char* getUniformBufferEngineAssetName(axr::UniformBufferEngineAssetEnum engineAssetEnum) {
        return axrGetUniformBufferEngineAssetName(static_cast<AxrUniformBufferEngineAssetEnum>(engineAssetEnum));
    }

    /// Check if the given name is reserved as a push constants buffer engine asset name
    /// @param name Name to check
    /// @returns True if the given name is reserved as a push constants buffer engine asset name
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

        // ---- All Assets ----

        /// Check if all the assets in the collection have been loaded
        /// @returns True if all assets in the collection have been loaded
        bool isLoaded() const {
            return axrAssetCollectionIsLoaded(m_AssetCollection);
        }

        /// Load all assets in the collection
        /// @param graphicsApi Graphics api to use with these assets
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result loadAssets(
            axr::GraphicsApiEnum graphicsApi
        ) {
            return static_cast<axr::Result>(axrAssetCollectionLoadAssets(
                m_AssetCollection,
                static_cast<AxrGraphicsApiEnum>(graphicsApi)
            ));
        }

        /// Unload all assets in the collection
        void unloadAssets() {
            axrAssetCollectionUnloadAssets(m_AssetCollection);
        }

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
        ) {
            return static_cast<axr::Result>(axrAssetCollectionCreateMaterial(
                m_AssetCollection,
                materialConfig.toRaw()
            ));
        }

        /// Create a material using the engine defined material named 'Default Material'
        /// @param materialName Material name
        /// @param materialValues Material values
        /// @returns AXR_SUCCESS if the function succeeded
        axr::Result assetCollectionCreateMaterial(
            const char* materialName,
            const AxrMaterialEngineAsset_DefaultMaterial materialValues
        ) {
            return static_cast<axr::Result>(
                axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
                    m_AssetCollection,
                    materialName,
                    materialValues
                )
            );
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrAssetCollection_T m_AssetCollection;
    };
}
