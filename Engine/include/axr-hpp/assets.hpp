#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "common/enums.hpp"

namespace axr {
    // ---------------------------------------------------------------------------------- //
    //                               Engine Defined Assets                                //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Axr engine defined assets enum
    enum class EngineAssetEnum {
        Undefined = AXR_ENGINE_ASSET_UNDEFINED,

        // ---- Shaders ----
        ShaderStart = AXR_ENGINE_ASSET_SHADER_START,
        ShaderDefaultVert = AXR_ENGINE_ASSET_SHADER_DEFAULT_VERT,
        ShaderDefaultFrag = AXR_ENGINE_ASSET_SHADER_DEFAULT_FRAG,
        ShaderEnd = AXR_ENGINE_ASSET_SHADER_END,

        // ---- Uniform Buffers ----
        UniformBufferStart = AXR_ENGINE_ASSET_UNIFORM_BUFFER_START,
        UniformBufferSceneData = AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA,
        UniformBufferEnd = AXR_ENGINE_ASSET_UNIFORM_BUFFER_END,

        // ---- Push Constant Buffers ----
        PushConstantBufferStart = AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_START,
        PushConstantBufferModelMatrix = AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX,
        PushConstantBufferEnd = AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_END,

        // ---- Images ----
        ImageStart = AXR_ENGINE_ASSET_IMAGE_START,
        ImageMissingTexture = AXR_ENGINE_ASSET_IMAGE_MISSING_TEXTURE,
        ImageUvTester = AXR_ENGINE_ASSET_IMAGE_UV_TESTER,
        ImageEnd = AXR_ENGINE_ASSET_IMAGE_END,

        // ---- Image Samplers - Max of 8 ----
        ImageSamplerStart = AXR_ENGINE_ASSET_IMAGE_SAMPLER_START,
        ImageSamplerNearestRepeat = AXR_ENGINE_ASSET_IMAGE_SAMPLER_NEAREST_REPEAT,
        ImageSamplerLinearRepeat = AXR_ENGINE_ASSET_IMAGE_SAMPLER_LINEAR_REPEAT,
        ImageSamplerEnd = AXR_ENGINE_ASSET_IMAGE_SAMPLER_END,

        // ---- Models ----
        ModelStart = AXR_ENGINE_ASSET_MODEL_START,
        ModelTriangle = AXR_ENGINE_ASSET_MODEL_TRIANGLE,
        ModelSquare = AXR_ENGINE_ASSET_MODEL_SQUARE,
        ModelCube = AXR_ENGINE_ASSET_MODEL_CUBE,
        ModelEnd = AXR_ENGINE_ASSET_MODEL_END,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    // ---- Uniform Buffers ----

    /// Engine asset uniform buffer named 'Scene Data' structure
    struct EngineAssetUniformBuffer_SceneData {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        alignas(16) glm::mat4 ViewMatrix;
        alignas(16) glm::mat4 ProjectionMatrix;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        EngineAssetUniformBuffer_SceneData() :
            ViewMatrix{},
            ProjectionMatrix{} {
        }

        /// Constructor
        /// @param viewMatrix The view matrix
        /// @param projectionMatrix The projection matrix
        EngineAssetUniformBuffer_SceneData(
            const glm::mat4& viewMatrix,
            const glm::mat4& projectionMatrix
        ) : ViewMatrix(viewMatrix),
            ProjectionMatrix(projectionMatrix) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the EngineAssetUniformBuffer_SceneData as an AxrEngineAssetUniformBuffer_SceneData
        /// @returns This as an AxrEngineAssetUniformBuffer_SceneData
        const AxrEngineAssetUniformBuffer_SceneData* toRaw() const {
            return reinterpret_cast<const AxrEngineAssetUniformBuffer_SceneData*>(this);
        }

        /// Get a handle to the EngineAssetUniformBuffer_SceneData as an AxrEngineAssetUniformBuffer_SceneData
        /// @returns This as an AxrEngineAssetUniformBuffer_SceneData
        AxrEngineAssetUniformBuffer_SceneData* toRaw() {
            return reinterpret_cast<AxrEngineAssetUniformBuffer_SceneData*>(this);
        }
    };

    static_assert(
        sizeof(AxrEngineAssetUniformBuffer_SceneData) == sizeof(axr::EngineAssetUniformBuffer_SceneData),
        "Original type and wrapper have different size!"
    );

    // ---- Push Constant Buffers ----

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Engine asset push constant buffer named 'Model Matrix' structure
    struct EngineAssetPushConstantBuffer_ModelMatrix {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        glm::mat4 ModelMatrix;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        EngineAssetPushConstantBuffer_ModelMatrix() :
            ModelMatrix{} {
        }

        /// Constructor
        /// @param modelMatrix The model matrix
        EngineAssetPushConstantBuffer_ModelMatrix(
            const glm::mat4& modelMatrix
        ) : ModelMatrix(modelMatrix) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the EngineAssetPushConstantBuffer_ModelMatrix as an AxrEngineAssetPushConstantBuffer_ModelMatrix
        /// @returns This as an AxrEngineAssetPushConstantBuffer_ModelMatrix
        const AxrEngineAssetPushConstantBuffer_ModelMatrix* toRaw() const {
            return reinterpret_cast<const AxrEngineAssetPushConstantBuffer_ModelMatrix*>(this);
        }

        /// Get a handle to the EngineAssetPushConstantBuffer_ModelMatrix as an AxrEngineAssetPushConstantBuffer_ModelMatrix
        /// @returns This as an AxrEngineAssetPushConstantBuffer_ModelMatrix
        AxrEngineAssetPushConstantBuffer_ModelMatrix* toRaw() {
            return reinterpret_cast<AxrEngineAssetPushConstantBuffer_ModelMatrix*>(this);
        }
    };

    static_assert(
        sizeof(AxrEngineAssetPushConstantBuffer_ModelMatrix) == sizeof(axr::EngineAssetPushConstantBuffer_ModelMatrix),
        "Original type and wrapper have different size!"
    );
#endif

    // ---- Materials ----

    /// Engine asset material named 'Default Material' values
    struct EngineAssetMaterial_DefaultMaterial {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* ImageName;
        const char* SamplerName;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        EngineAssetMaterial_DefaultMaterial() :
            ImageName{},
            SamplerName{} {
        }

        /// Constructor
        /// @param imageName The image name
        /// @param samplerName The image sampler name
        EngineAssetMaterial_DefaultMaterial(
            const char* imageName,
            const char* samplerName
        ) : ImageName(imageName),
            SamplerName(samplerName) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the EngineAssetMaterial_DefaultMaterial as an AxrEngineAssetMaterial_DefaultMaterial
        /// @returns This as an AxrEngineAssetMaterial_DefaultMaterial
        const AxrEngineAssetMaterial_DefaultMaterial* toRaw() const {
            return reinterpret_cast<const AxrEngineAssetMaterial_DefaultMaterial*>(this);
        }

        /// Get a handle to the EngineAssetMaterial_DefaultMaterial as an AxrEngineAssetMaterial_DefaultMaterial
        /// @returns This as an AxrEngineAssetMaterial_DefaultMaterial
        AxrEngineAssetMaterial_DefaultMaterial* toRaw() {
            return reinterpret_cast<AxrEngineAssetMaterial_DefaultMaterial*>(this);
        }
    };

    static_assert(
        sizeof(AxrEngineAssetMaterial_DefaultMaterial) == sizeof(axr::EngineAssetMaterial_DefaultMaterial),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Function Definitions
    // ----------------------------------------- //

    /// Check if the given name is reserved for an engine asset
    /// @param name Name to check
    /// @returns True if the given name is reserved for an engine asset
    inline bool engineAssetIsNameReserved(const char* name) {
        return axrEngineAssetIsNameReserved(name);
    }

    /// Get the name for the given engine asset
    /// @param engineAssetEnum Engine asset to get the name of
    /// @returns The name of the given engine asset
    inline const char* engineAssetGetName(axr::EngineAssetEnum engineAssetEnum) {
        return axrEngineAssetGetName(static_cast<AxrEngineAssetEnum>(engineAssetEnum));
    }

    // ---- Buffers ----

    /// Get the size for the given uniform buffer engine asset
    /// @param engineAssetEnum Engine asset to use
    /// @returns The size for the given uniform buffer engine asset
    inline uint64_t engineAssetGetUniformBufferSize(axr::EngineAssetEnum engineAssetEnum) {
        return axrEngineAssetGetUniformBufferSize(static_cast<AxrEngineAssetEnum>(engineAssetEnum));
    }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Get the size for the given push constant buffer engine asset
    /// @param engineAssetEnum Engine asset to use
    /// @returns The size for the given push constant buffer engine asset
    inline uint32_t engineAssetGetPushConstantBufferSize(axr::EngineAssetEnum engineAssetEnum) {
        return axrEngineAssetGetPushConstantBufferSize(static_cast<AxrEngineAssetEnum>(engineAssetEnum));
    }
#endif

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
        PushConstantBuffer = AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANT_BUFFER,
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
        uint32_t VertexAttributeCount;
        AxrShaderVertexAttribute* VertexAttributes;
        uint32_t BufferLayoutCount;
        AxrShaderBufferLayout_T* BufferLayouts;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        VertexShaderProperties():
            VertexAttributeCount(0),
            VertexAttributes(nullptr),
            BufferLayoutCount(0),
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
            resizeVertexAttributes(VertexAttributeCount + 1);

            const AxrShaderVertexAttribute vertexAttribute{
                .Type = static_cast<AxrShaderVertexAttributeEnum>(type),
                .Binding = binding,
                .Location = location
            };
            VertexAttributes[VertexAttributeCount - 1] = axrShaderVertexAttributeClone(vertexAttribute);
        }

        /// Clear the vertex attributes
        void clearVertexAttributes() {
            if (VertexAttributes == nullptr) return;

            for (uint32_t i = 0; i < VertexAttributeCount; ++i) {
                axrShaderVertexAttributeDestroy(&VertexAttributes[i]);
            }

            delete[] VertexAttributes;
            VertexAttributes = nullptr;
            VertexAttributeCount = 0;
        }

        // ---- Buffer Layouts ----

        /// Add a uniform buffer layout
        /// @param binding Uniform buffer layout binding
        /// @param bufferSize Uniform buffer layout buffer size
        void addUniformBufferLayout(const uint32_t binding, const uint64_t bufferSize) {
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderUniformBufferLayout bufferLayout{
                .Binding = binding,
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderUniformBufferLayoutClone(&bufferLayout)
            );
        }

        /// Add an image sampler buffer layout
        /// @param binding Image sampler buffer layout binding
        void addImageSamplerBufferLayout(const uint32_t binding) {
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderImageSamplerBufferLayout bufferLayout{
                .Binding = binding,
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderImageSamplerBufferLayoutClone(&bufferLayout)
            );
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Add a push constant buffer layout
        /// @param bufferSize Push constant buffer layout buffer size
        void addPushConstantBufferLayout(const uint32_t bufferSize) {
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderPushConstantBufferLayout bufferLayout{
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderPushConstantBufferLayoutClone(&bufferLayout)
            );
        }
#endif

        /// CLear the buffer layouts
        void clearBufferLayouts() {
            if (BufferLayouts == nullptr) return;

            for (uint32_t i = 0; i < BufferLayoutCount; ++i) {
                if (BufferLayouts[i] == nullptr) continue;

                axrShaderBufferLayoutDestroy(&BufferLayouts[i]);
            }

            delete[] BufferLayouts;
            BufferLayouts = nullptr;
            BufferLayoutCount = 0;
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
            for (uint32_t i = 0; i < std::min(VertexAttributeCount, size); ++i) {
                // Move vertex attributes to new array
                newVertexAttributes[i] = VertexAttributes[i];
                VertexAttributes[i] = {};
            }

            clearVertexAttributes();
            VertexAttributes = newVertexAttributes;
            VertexAttributeCount = size;
        }

        // ---- Buffer Layouts ----

        /// Resize the buffer layouts
        /// @param size New size
        void resizeBufferLayouts(const uint32_t size) {
            auto newBufferLayouts = new AxrShaderBufferLayout_T[size]{};
            for (uint32_t i = 0; i < std::min(BufferLayoutCount, size); ++i) {
                // Move buffer layouts to new array
                newBufferLayouts[i] = BufferLayouts[i];
                BufferLayouts[i] = nullptr;
            }

            clearBufferLayouts();
            BufferLayouts = newBufferLayouts;
            BufferLayoutCount = size;
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
        uint32_t BufferLayoutCount;
        AxrShaderBufferLayout_T* BufferLayouts;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        FragmentShaderProperties():
            BufferLayoutCount(0),
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
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderUniformBufferLayout bufferLayout{
                .Binding = binding,
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderUniformBufferLayoutClone(&bufferLayout)
            );
        }

        /// Add an image sampler buffer layout
        /// @param binding Image sampler buffer layout binding
        void addImageSamplerBufferLayout(const uint32_t binding) {
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderImageSamplerBufferLayout bufferLayout{
                .Binding = binding,
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderImageSamplerBufferLayoutClone(&bufferLayout)
            );
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Add a push constant buffer layout
        /// @param bufferSize Push constant buffer layout buffer size
        void addPushConstantBufferLayout(const uint32_t bufferSize) {
            resizeBufferLayouts(BufferLayoutCount + 1);

            const AxrShaderPushConstantBufferLayout bufferLayout{
                .BufferSize = bufferSize
            };
            BufferLayouts[BufferLayoutCount - 1] = reinterpret_cast<AxrShaderBufferLayout_T>(
                axrShaderPushConstantBufferLayoutClone(&bufferLayout)
            );
        }
#endif

        /// CLear the buffer layouts
        void clearBufferLayouts() {
            if (BufferLayouts == nullptr) return;

            for (uint32_t i = 0; i < BufferLayoutCount; ++i) {
                if (BufferLayouts[i] == nullptr) continue;

                axrShaderBufferLayoutDestroy(&BufferLayouts[i]);
            }

            delete[] BufferLayouts;
            BufferLayouts = nullptr;
            BufferLayoutCount = 0;
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
            for (uint32_t i = 0; i < std::min(BufferLayoutCount, size); ++i) {
                // Move buffer layouts to new array
                newBufferLayouts[i] = BufferLayouts[i];
                BufferLayouts[i] = nullptr;
            }

            clearBufferLayouts();
            BufferLayouts = newBufferLayouts;
            BufferLayoutCount = size;
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
        uint32_t BufferLinkCount;
        AxrShaderBufferLink_T* BufferLinks;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ShaderValues():
            BufferLinkCount(0),
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
            resizeBufferLinks(BufferLinkCount + 1);

            const AxrShaderUniformBufferLink bufferLink{
                .Binding = binding,
                .BufferName = bufferName
            };
            BufferLinks[BufferLinkCount - 1] = reinterpret_cast<AxrShaderBufferLink_T>(
                axrShaderUniformBufferLinkClone(&bufferLink)
            );
        }

        /// Add an image sampler buffer link
        /// @param binding Image sampler buffer binding
        /// @param imageName Image name
        /// @param samplerName Image sampler name
        void addImageSamplerBufferLink(const uint32_t binding, const char* imageName, const char* samplerName) {
            resizeBufferLinks(BufferLinkCount + 1);

            const AxrShaderImageSamplerBufferLink bufferLink{
                .Binding = binding,
                .ImageName = imageName,
                .SamplerName = samplerName
            };
            BufferLinks[BufferLinkCount - 1] = reinterpret_cast<AxrShaderBufferLink_T>(
                axrShaderImageSamplerBufferLinkClone(&bufferLink)
            );
        }

        /// Clear the buffer links 
        void clearBufferLinks() {
            if (BufferLinks == nullptr) return;

            for (uint32_t i = 0; i < BufferLinkCount; ++i) {
                if (BufferLinks[i] == nullptr) continue;

                axrShaderBufferLinkDestroy(&BufferLinks[i]);
            }

            delete[] BufferLinks;
            BufferLinks = nullptr;
            BufferLinkCount = 0;
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
            for (uint32_t i = 0; i < std::min(BufferLinkCount, size); ++i) {
                // Move buffer links to new array
                newBufferLinks[i] = BufferLinks[i];
                BufferLinks[i] = nullptr;
            }

            clearBufferLinks();
            BufferLinks = newBufferLinks;
            BufferLinkCount = size;
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
    // Structs
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
        const char* PushConstantBufferName;
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
            PushConstantBufferName(""),
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
            PushConstantBufferName(""),
#endif
            VertexShaderValues(vertexShaderValues.cloneRaw()),
            FragmentShaderValues(fragmentShaderValues.cloneRaw()) {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Constructor
        /// @param name The material name
        /// @param vertexShaderName The vertex shader name
        /// @param fragmentShaderName The fragment shader name
        /// @param pushConstantBufferName The push constant buffer name
        /// @param vertexShaderValues The vertex shader values to use
        /// @param fragmentShaderValues The fragment shader values to use
        MaterialConfig(
            const char* name,
            const char* vertexShaderName,
            const char* fragmentShaderName,
            const char* pushConstantBufferName,
            const axr::ShaderValues& vertexShaderValues,
            const axr::ShaderValues& fragmentShaderValues
        ) : Name(name),
            VertexShaderName(vertexShaderName),
            FragmentShaderName(fragmentShaderName),
            PushConstantBufferName(pushConstantBufferName),
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
            PushConstantBufferName = src.PushConstantBufferName;
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
            PushConstantBufferName = src.PushConstantBufferName;
#endif
            VertexShaderValues = src.VertexShaderValues;
            FragmentShaderValues = src.FragmentShaderValues;

            src.Name = "";
            src.VertexShaderName = "";
            src.FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            src.PushConstantBufferName = "";
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
                PushConstantBufferName = src.PushConstantBufferName;
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
                PushConstantBufferName = src.PushConstantBufferName;
#endif
                VertexShaderValues = src.VertexShaderValues;
                FragmentShaderValues = src.FragmentShaderValues;

                src.Name = "";
                src.VertexShaderName = "";
                src.FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                src.PushConstantBufferName = "";
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
            PushConstantBufferName = "";
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

    /// Submesh
    struct Submesh {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        uint32_t VertexCount;
        axr::Vertex* Vertices;
        uint32_t IndexCount;
        uint32_t* Indices;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Submesh(): VertexCount(0),
            Vertices(nullptr),
            IndexCount(0),
            Indices(nullptr) {
        }

        /// Constructor
        /// @param vertices Vertices to copy
        /// @param indices Indices to copy
        Submesh(const std::vector<axr::Vertex>& vertices, const std::vector<uint32_t>& indices) {
            VertexCount = static_cast<uint32_t>(vertices.size());
            Vertices = cloneVertices(VertexCount, vertices.data());
            IndexCount = static_cast<uint32_t>(indices.size());
            Indices = cloneIndices(IndexCount, indices.data());
        }

        /// Copy Constructor
        /// @param src Source Submesh to copy from
        Submesh(const Submesh& src) {
            VertexCount = src.VertexCount;
            Vertices = cloneVertices(src.VertexCount, src.Vertices);
            IndexCount = src.IndexCount;
            Indices = cloneIndices(src.IndexCount, src.Indices);
        }

        /// Move Constructor
        /// @param src Source Submesh to move from
        Submesh(Submesh&& src) noexcept {
            VertexCount = src.VertexCount;
            Vertices = src.Vertices;
            IndexCount = src.IndexCount;
            Indices = src.Indices;

            src.VertexCount = 0;
            src.Vertices = nullptr;
            src.IndexCount = 0;
            src.Indices = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~Submesh() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Submesh to copy from
        Submesh& operator=(const Submesh& src) {
            if (this != &src) {
                cleanup();

                VertexCount = src.VertexCount;
                Vertices = cloneVertices(src.VertexCount, src.Vertices);
                IndexCount = src.IndexCount;
                Indices = cloneIndices(src.IndexCount, src.Indices);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Submesh to move from
        Submesh& operator=(Submesh&& src) noexcept {
            if (this != &src) {
                cleanup();

                VertexCount = src.VertexCount;
                Vertices = src.Vertices;
                IndexCount = src.IndexCount;
                Indices = src.Indices;

                src.VertexCount = 0;
                src.Vertices = nullptr;
                src.IndexCount = 0;
                src.Indices = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the Submesh as an AxrSubmesh
        /// @returns This as an AxrSubmesh
        const AxrSubmesh* toRaw() const {
            return reinterpret_cast<const AxrSubmesh*>(this);
        }

        /// Get a handle to the Submesh as an AxrSubmesh
        /// @returns This as an AxrSubmesh
        AxrSubmesh* toRaw() {
            return reinterpret_cast<AxrSubmesh*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrSubmeshDestroyVertices(&VertexCount, reinterpret_cast<AxrVertex**>(&Vertices));
            axrSubmeshDestroyIndices(&IndexCount, &Indices);
        }

        /// Clone the given vertices
        /// @param vertexCount Number of vertices in the given array
        /// @param vertices Vertex array to clone
        /// @returns A cloned array of the given vertices
        axr::Vertex* cloneVertices(const uint32_t vertexCount, const axr::Vertex* vertices) {
            return reinterpret_cast<axr::Vertex*>(axrSubmeshCloneVertices(
                vertexCount,
                reinterpret_cast<const AxrVertex*>(vertices)
            ));
        }

        /// Clone the given indices
        /// @param indexCount Number of indices in the given array
        /// @param indices Index array to clone
        /// @returns A cloned array of the given indices
        uint32_t* cloneIndices(const uint32_t indexCount, const uint32_t* indices) {
            return axrSubmeshCloneIndices(indexCount, indices);
        }
    };

    static_assert(
        sizeof(AxrSubmesh) == sizeof(axr::Submesh),
        "Original type and wrapper have different size!"
    );

    /// Mesh
    struct Mesh {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        uint32_t SubmeshCount;
        axr::Submesh* Submeshes;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Mesh(): SubmeshCount(0),
            Submeshes(nullptr) {
        }

        /// Constructor
        /// @param submeshes Submeshes to copy
        Mesh(const std::vector<axr::Submesh>& submeshes) {
            SubmeshCount = static_cast<uint32_t>(submeshes.size());
            Submeshes = cloneSubmeshes(SubmeshCount, submeshes.data());
        }

        /// Copy Constructor
        /// @param src Source Mesh to copy from
        Mesh(const Mesh& src) {
            SubmeshCount = src.SubmeshCount;
            Submeshes = cloneSubmeshes(src.SubmeshCount, src.Submeshes);
        }

        /// Move Constructor
        /// @param src Source Mesh to move from
        Mesh(Mesh&& src) noexcept {
            SubmeshCount = src.SubmeshCount;
            Submeshes = src.Submeshes;

            src.SubmeshCount = 0;
            src.Submeshes = nullptr;
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

                SubmeshCount = src.SubmeshCount;
                Submeshes = cloneSubmeshes(src.SubmeshCount, src.Submeshes);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source Mesh to move from
        Mesh& operator=(Mesh&& src) noexcept {
            if (this != &src) {
                cleanup();

                SubmeshCount = src.SubmeshCount;
                Submeshes = src.Submeshes;

                src.SubmeshCount = 0;
                src.Submeshes = nullptr;
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
            axrMeshDestroySubmeshes(&SubmeshCount, reinterpret_cast<AxrSubmesh**>(&Submeshes));
        }

        /// Clone the given submeshes
        /// @param submeshCount Number of submeshes in the given array
        /// @param submeshes Submesh array to clone
        /// @returns A cloned array of the given submeshes
        axr::Submesh* cloneSubmeshes(const uint32_t submeshCount, const axr::Submesh* submeshes) {
            return reinterpret_cast<axr::Submesh*>(axrMeshCloneSubmeshes(
                submeshCount,
                reinterpret_cast<const AxrSubmesh*>(submeshes)
            ));
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

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelConfig():
            Name(""),
            FilePath(nullptr) {
        }

        /// Constructor
        /// @param name Name of the model
        /// @param filePath Filepath of the model
        ModelConfig(const char* name, const char* filePath):
            Name(name),
            FilePath(filePath) {
        }

        /// Copy Constructor
        /// @param src Source ModelConfig to copy from
        ModelConfig(const ModelConfig& src) {
            Name = src.Name;
            FilePath = src.FilePath;
        }

        /// Move Constructor
        /// @param src Source ModelConfig to move from
        ModelConfig(ModelConfig&& src) noexcept {
            Name = src.Name;
            FilePath = src.FilePath;

            src.Name = "";
            src.FilePath = nullptr;
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

                src.Name = "";
                src.FilePath = nullptr;
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

        /// Set the mesh data for the model
        /// @param meshCount Number of meshes in the array
        /// @param meshes Meshes array
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] AxrResult setData(const uint32_t meshCount, const AxrMesh* meshes) {
            return axrModelSetData(m_Model, meshCount, meshes);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrModel_T m_Model;
    };

    // ---------------------------------------------------------------------------------- //
    //                                Uniform Buffer Assets                               //
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

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

    // ---------------------------------------------------------------------------------- //
    //                             Push Constant Buffer Assets                            //
    // ---------------------------------------------------------------------------------- //

    /// Push Constant Buffer Config
    struct PushConstantBufferConfig {
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
        PushConstantBufferConfig():
            Name(""),
            DataSize(0),
            Data(nullptr) {
        }

        /// Constructor
        /// @param name Name of the push constant buffer
        /// @param dataSize Data size
        /// @param data Data
        PushConstantBufferConfig(const char* name, const uint32_t dataSize, const void* data):
            Name(name),
            DataSize(dataSize) {
            Data = axrPushConstantBufferCloneData(dataSize, data);
        }

        /// Copy Constructor
        /// @param src Source PushConstantBufferConfig to copy from
        PushConstantBufferConfig(const PushConstantBufferConfig& src) {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = axrPushConstantBufferCloneData(src.DataSize, src.Data);
        }

        /// Move Constructor
        /// @param src Source PushConstantBufferConfig to move from
        PushConstantBufferConfig(PushConstantBufferConfig&& src) noexcept {
            Name = src.Name;
            DataSize = src.DataSize;
            Data = src.Data;

            src.Name = "";
            src.DataSize = 0;
            src.Data = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~PushConstantBufferConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source PushConstantBufferConfig to copy from
        PushConstantBufferConfig& operator=(const PushConstantBufferConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                DataSize = src.DataSize;
                Data = axrPushConstantBufferCloneData(src.DataSize, src.Data);
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source PushConstantBufferConfig to move from
        PushConstantBufferConfig& operator=(PushConstantBufferConfig&& src) noexcept {
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

        /// Get a handle to the PushConstantBufferConfig as an AxrPushConstantBufferConfig
        /// @returns This as an AxrPushConstantBufferConfig
        const AxrPushConstantBufferConfig* toRaw() const {
            return reinterpret_cast<const AxrPushConstantBufferConfig*>(this);
        }

        /// Get a handle to the PushConstantBufferConfig as an AxrPushConstantBufferConfig
        /// @returns This as an AxrPushConstantBufferConfig
        AxrPushConstantBufferConfig* toRaw() {
            return reinterpret_cast<AxrPushConstantBufferConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            axrPushConstantBufferDestroyData(&DataSize, &Data);
        }
    };

    static_assert(
        sizeof(AxrPushConstantBufferConfig) == sizeof(axr::PushConstantBufferConfig),
        "Original type and wrapper have different size!"
    );
#endif

    // ---------------------------------------------------------------------------------- //
    //                               Image Sampler Assets                                 //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Image sampler filter enum
    enum class ImageSamplerFilterEnum {
        Undefined = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED,
        Nearest = AXR_IMAGE_SAMPLER_FILTER_NEAREST,
        Linear = AXR_IMAGE_SAMPLER_FILTER_LINEAR,
    };

    /// Image sampler wrap enum
    enum class ImageSamplerWrapEnum {
        Undefined = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED,
        Repeat = AXR_IMAGE_SAMPLER_WRAP_REPEAT,
        MirroredRepeat = AXR_IMAGE_SAMPLER_WRAP_MIRRORED_REPEAT,
        ClampToEdge = AXR_IMAGE_SAMPLER_WRAP_CLAMP_TO_EDGE,
        ClampToBorder = AXR_IMAGE_SAMPLER_WRAP_CLAMP_TO_BORDER,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Image Sampler Config
    struct ImageSamplerConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        const char* Name;
        axr::ImageSamplerFilterEnum MinFilter;
        axr::ImageSamplerFilterEnum MagFilter;
        axr::ImageSamplerFilterEnum MipmapFilter;
        axr::ImageSamplerWrapEnum WrapU;
        axr::ImageSamplerWrapEnum WrapV;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ImageSamplerConfig():
            Name(""),
            MinFilter(axr::ImageSamplerFilterEnum::Undefined),
            MagFilter(axr::ImageSamplerFilterEnum::Undefined),
            MipmapFilter(axr::ImageSamplerFilterEnum::Undefined),
            WrapU(axr::ImageSamplerWrapEnum::Undefined),
            WrapV(axr::ImageSamplerWrapEnum::Undefined) {
        }

        /// Constructor
        /// @param name Name of the image sampler
        /// @param minFilter Image sampler min filter enum
        /// @param magFilter Image sampler mag filter enum
        /// @param mipmapFilter Image sampler mipmap filter enum
        /// @param wrapU Image sampler U axis wrap enum
        /// @param wrapV Image sampler V axis wrap enum
        ImageSamplerConfig(
            const char* name,
            const axr::ImageSamplerFilterEnum minFilter,
            const axr::ImageSamplerFilterEnum magFilter,
            const axr::ImageSamplerFilterEnum mipmapFilter,
            const axr::ImageSamplerWrapEnum wrapU,
            const axr::ImageSamplerWrapEnum wrapV
        ):
            Name(name),
            MinFilter(minFilter),
            MagFilter(magFilter),
            MipmapFilter(mipmapFilter),
            WrapU(wrapU),
            WrapV(wrapV) {
        }

        /// Copy Constructor
        /// @param src Source ImageSamplerConfig to copy from
        ImageSamplerConfig(const ImageSamplerConfig& src) {
            Name = src.Name;
            MinFilter = src.MinFilter;
            MagFilter = src.MagFilter;
            MipmapFilter = src.MipmapFilter;
            WrapU = src.WrapU;
            WrapV = src.WrapV;
        }

        /// Move Constructor
        /// @param src Source ImageSamplerConfig to move from
        ImageSamplerConfig(ImageSamplerConfig&& src) noexcept {
            Name = src.Name;
            MinFilter = src.MinFilter;
            MagFilter = src.MagFilter;
            MipmapFilter = src.MipmapFilter;
            WrapU = src.WrapU;
            WrapV = src.WrapV;

            src.Name = "";
            MinFilter = axr::ImageSamplerFilterEnum::Undefined;
            MagFilter = axr::ImageSamplerFilterEnum::Undefined;
            MipmapFilter = axr::ImageSamplerFilterEnum::Undefined;
            WrapU = axr::ImageSamplerWrapEnum::Undefined;
            WrapV = axr::ImageSamplerWrapEnum::Undefined;
        }

        // ---- Destructor ----

        /// Destructor
        ~ImageSamplerConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ImageSamplerConfig to copy from
        ImageSamplerConfig& operator=(const ImageSamplerConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                MinFilter = src.MinFilter;
                MagFilter = src.MagFilter;
                MipmapFilter = src.MipmapFilter;
                WrapU = src.WrapU;
                WrapV = src.WrapV;
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ImageSamplerConfig to move from
        ImageSamplerConfig& operator=(ImageSamplerConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                MinFilter = src.MinFilter;
                MagFilter = src.MagFilter;
                MipmapFilter = src.MipmapFilter;
                WrapU = src.WrapU;
                WrapV = src.WrapV;

                src.Name = "";
                MinFilter = axr::ImageSamplerFilterEnum::Undefined;
                MagFilter = axr::ImageSamplerFilterEnum::Undefined;
                MipmapFilter = axr::ImageSamplerFilterEnum::Undefined;
                WrapU = axr::ImageSamplerWrapEnum::Undefined;
                WrapV = axr::ImageSamplerWrapEnum::Undefined;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ImageSamplerConfig as an AxrImageSamplerConfig
        /// @returns This as an AxrImageSamplerConfig
        const AxrImageSamplerConfig* toRaw() const {
            return reinterpret_cast<const AxrImageSamplerConfig*>(this);
        }

        /// Get a handle to the ImageSamplerConfig as an AxrImageSamplerConfig
        /// @returns This as an AxrImageSamplerConfig
        AxrImageSamplerConfig* toRaw() {
            return reinterpret_cast<AxrImageSamplerConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            MinFilter = axr::ImageSamplerFilterEnum::Undefined;
            MagFilter = axr::ImageSamplerFilterEnum::Undefined;
            MipmapFilter = axr::ImageSamplerFilterEnum::Undefined;
            WrapU = axr::ImageSamplerWrapEnum::Undefined;
            WrapV = axr::ImageSamplerWrapEnum::Undefined;
        }
    };

    static_assert(
        sizeof(AxrImageSamplerConfig) == sizeof(axr::ImageSamplerConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Image Sampler Definition
    // ----------------------------------------- //

    /// Image Sampler
    class ImageSampler {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param imageSampler Image sampler handle
        ImageSampler(const AxrImageSampler_T imageSampler):
            m_ImageSampler(imageSampler) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the image sampler's name
        /// @returns The image sampler's name
        [[nodiscard]] const char* getName() const {
            return axrImageSamplerGetName(m_ImageSampler);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrImageSampler_T m_ImageSampler;
    };

    // ---------------------------------------------------------------------------------- //
    //                                   Image Assets                                     //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Image color channels enum
    enum class ImageColorChannelsEnum {
        Undefined = AXR_IMAGE_COLOR_CHANNELS_UNDEFINED,
        Gray = AXR_IMAGE_COLOR_CHANNELS_GRAY,
        GrayAlpha = AXR_IMAGE_COLOR_CHANNELS_GRAY_ALPHA,
        RGB = AXR_IMAGE_COLOR_CHANNELS_RGB,
        RGBAlpha = AXR_IMAGE_COLOR_CHANNELS_RGB_ALPHA,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Image Config
    struct ImageConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        const char* Name;
        const char* FilePath;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ImageConfig():
            Name(""),
            FilePath("") {
        }

        /// Constructor
        /// @param name Name of the image
        /// @param filePath Image file path
        ImageConfig(
            const char* name,
            const char* filePath
        ):
            Name(name),
            FilePath(filePath) {
        }

        /// Copy Constructor
        /// @param src Source ImageConfig to copy from
        ImageConfig(const ImageConfig& src) {
            Name = src.Name;
            FilePath = src.FilePath;
        }

        /// Move Constructor
        /// @param src Source ImageConfig to move from
        ImageConfig(ImageConfig&& src) noexcept {
            Name = src.Name;
            FilePath = src.FilePath;

            src.Name = "";
            src.FilePath = "";
        }

        // ---- Destructor ----

        /// Destructor
        ~ImageConfig() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ImageConfig to copy from
        ImageConfig& operator=(const ImageConfig& src) {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ImageConfig to move from
        ImageConfig& operator=(ImageConfig&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;

                src.Name = "";
                src.FilePath = "";
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ImageConfig as an AxrImageConfig
        /// @returns This as an AxrImageConfig
        const AxrImageConfig* toRaw() const {
            return reinterpret_cast<const AxrImageConfig*>(this);
        }

        /// Get a handle to the ImageConfig as an AxrImageConfig
        /// @returns This as an AxrImageConfig
        AxrImageConfig* toRaw() {
            return reinterpret_cast<AxrImageConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Name = "";
            FilePath = "";
        }
    };

    static_assert(
        sizeof(AxrImageConfig) == sizeof(axr::ImageConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Image Definition
    // ----------------------------------------- //

    /// Image
    class Image {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param image Image handle
        Image(const AxrImage_T image):
            m_Image(image) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the image's name
        /// @returns The image's name
        [[nodiscard]] const char* getName() const {
            return axrImageGetName(m_Image);
        }

        /// Set the image data
        /// @param width Image width
        /// @param height Image height
        /// @param colorChannels Image number of color channels
        /// @param data Image data. Stored from left-to-right, top-to-bottom. Each pixel contains a value for each 'colorChannel', stored with 8-bits
        /// per channel, in the following order: 1=Y, 2=YA, 3=RGB, 4=RGBA. (Y is monochrome color.)
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result setData(
            const uint32_t width,
            const uint32_t height,
            const axr::ImageColorChannelsEnum colorChannels,
            const stbi_uc* data
        ) const {
            return static_cast<axr::Result>(axrImageSetData(
                m_Image,
                width,
                height,
                static_cast<AxrImageColorChannelsEnum>(colorChannels),
                data
            ));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrImage_T m_Image;
    };

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
        [[nodiscard]] axr::Result createShader(axr::EngineAssetEnum engineAssetEnum) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateEngineAssetShader(
                m_AssetCollection,
                static_cast<AxrEngineAssetEnum>(engineAssetEnum)
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
            const axr::EngineAssetMaterial_DefaultMaterial materialValues
        ) const {
            return static_cast<axr::Result>(
                axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
                    m_AssetCollection,
                    materialName,
                    *materialValues.toRaw()
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
            axr::EngineAssetEnum engineAssetEnum
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateEngineAssetModel(
                m_AssetCollection,
                modelName,
                static_cast<AxrEngineAssetEnum>(engineAssetEnum)
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
        // ---- Push Constant Buffer ----

        /// Create a new push constant buffer
        /// @param pushConstantBufferConfig Push constant buffer config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createPushConstantBuffer(
            const axr::PushConstantBufferConfig& pushConstantBufferConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreatePushConstantBuffer(
                m_AssetCollection,
                pushConstantBufferConfig.toRaw()
            ));
        }
#endif

        // ---- Image ----

        /// Create a new image
        /// @param imageConfig Image config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createImage(
            const axr::ImageConfig& imageConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateImage(
                m_AssetCollection,
                imageConfig.toRaw()
            ));
        }

        /// Create a new engine asset image
        /// @param imageName Image name
        /// @param engineAssetEnum Image engine asset
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createImage(
            const char* imageName,
            axr::EngineAssetEnum engineAssetEnum
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateEngineAssetImage(
                m_AssetCollection,
                imageName,
                static_cast<AxrEngineAssetEnum>(engineAssetEnum)
            ));
        }

        // ---- Image Sampler ----

        /// Create a new image sampler
        /// @param imageSamplerConfig Image sampler config
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createImageSampler(
            const axr::ImageSamplerConfig& imageSamplerConfig
        ) const {
            return static_cast<axr::Result>(axrAssetCollectionCreateImageSampler(
                m_AssetCollection,
                imageSamplerConfig.toRaw()
            ));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrAssetCollection_T m_AssetCollection;
    };

    // ---------------------------------------------------------------------------------- //
    //                                    Asset Utils                                     //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Model file image sampler info
    struct ModelFileImageSamplerInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char* Name;
        axr::ImageSamplerFilterEnum MinFilter;
        axr::ImageSamplerFilterEnum MagFilter;
        axr::ImageSamplerFilterEnum MipmapFilter;
        axr::ImageSamplerWrapEnum WrapU;
        axr::ImageSamplerWrapEnum WrapV;
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileImageSamplerInfo() :
            Name(nullptr),
            MinFilter(axr::ImageSamplerFilterEnum::Undefined),
            MagFilter(axr::ImageSamplerFilterEnum::Undefined),
            MipmapFilter(axr::ImageSamplerFilterEnum::Undefined),
            WrapU(axr::ImageSamplerWrapEnum::Undefined),
            WrapV(axr::ImageSamplerWrapEnum::Undefined) {
        }

        /// Constructor
        /// @param src Source ModelFileImageSamplerInfo
        explicit ModelFileImageSamplerInfo(AxrModelFileImageSamplerInfo&& src) {
            Name = src.Name;
            MinFilter = static_cast<axr::ImageSamplerFilterEnum>(src.MinFilter);
            MagFilter = static_cast<axr::ImageSamplerFilterEnum>(src.MagFilter);
            MipmapFilter = static_cast<axr::ImageSamplerFilterEnum>(src.MipmapFilter);
            WrapU = static_cast<axr::ImageSamplerWrapEnum>(src.WrapU);
            WrapV = static_cast<axr::ImageSamplerWrapEnum>(src.WrapV);

            src.Name = nullptr;
            src.MinFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
            src.MagFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
            src.MipmapFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
            src.WrapU = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
            src.WrapV = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
        }

        /// Copy Constructor
        /// @param src Source ModelFileImageSamplerInfo to copy from
        ModelFileImageSamplerInfo(const axr::ModelFileImageSamplerInfo& src) {
            *this = static_cast<axr::ModelFileImageSamplerInfo>(axrModelFileImageSamplerInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileImageSamplerInfo to move from
        ModelFileImageSamplerInfo(ModelFileImageSamplerInfo&& src) noexcept {
            Name = src.Name;
            MinFilter = src.MinFilter;
            MagFilter = src.MagFilter;
            MipmapFilter = src.MipmapFilter;
            WrapU = src.WrapU;
            WrapV = src.WrapV;

            src.Name = nullptr;
            src.MinFilter = axr::ImageSamplerFilterEnum::Undefined;
            src.MagFilter = axr::ImageSamplerFilterEnum::Undefined;
            src.MipmapFilter = axr::ImageSamplerFilterEnum::Undefined;
            src.WrapU = axr::ImageSamplerWrapEnum::Undefined;
            src.WrapV = axr::ImageSamplerWrapEnum::Undefined;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileImageSamplerInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileImageSamplerInfo to copy from
        ModelFileImageSamplerInfo& operator=(const ModelFileImageSamplerInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileImageSamplerInfo>(axrModelFileImageSamplerInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileImageSamplerInfo to move from
        ModelFileImageSamplerInfo& operator=(ModelFileImageSamplerInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                MinFilter = src.MinFilter;
                MagFilter = src.MagFilter;
                MipmapFilter = src.MipmapFilter;
                WrapU = src.WrapU;
                WrapV = src.WrapV;

                src.Name = nullptr;
                src.MinFilter = axr::ImageSamplerFilterEnum::Undefined;
                src.MagFilter = axr::ImageSamplerFilterEnum::Undefined;
                src.MipmapFilter = axr::ImageSamplerFilterEnum::Undefined;
                src.WrapU = axr::ImageSamplerWrapEnum::Undefined;
                src.WrapV = axr::ImageSamplerWrapEnum::Undefined;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileImageSamplerInfo as an AxrModelFileImageSamplerInfo
        /// @returns This as an AxrModelFileImageSamplerInfo
        const AxrModelFileImageSamplerInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileImageSamplerInfo*>(this);
        }

        /// Get a handle to the ModelFileImageSamplerInfo as an AxrModelFileImageSamplerInfo
        /// @returns This as an AxrModelFileImageSamplerInfo
        AxrModelFileImageSamplerInfo* toRaw() {
            return reinterpret_cast<AxrModelFileImageSamplerInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileImageSamplerInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileImageSamplerInfo) == sizeof(axr::ModelFileImageSamplerInfo),
        "Original type and wrapper have different size!"
    );

    /// Model file image info
    struct ModelFileImageInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char* Name;
        char* FilePath;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileImageInfo() :
            Name(nullptr),
            FilePath(nullptr) {
        }

        /// Constructor
        /// @param src Source ModelFileImageInfo
        explicit ModelFileImageInfo(AxrModelFileImageInfo&& src) {
            Name = src.Name;
            FilePath = src.FilePath;

            src.Name = nullptr;
            src.FilePath = nullptr;
        }

        /// Copy Constructor
        /// @param src Source ModelFileImageInfo to copy from
        ModelFileImageInfo(const axr::ModelFileImageInfo& src) {
            *this = static_cast<axr::ModelFileImageInfo>(axrModelFileImageInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileImageInfo to move from
        ModelFileImageInfo(ModelFileImageInfo&& src) noexcept {
            Name = src.Name;
            FilePath = src.FilePath;

            src.Name = nullptr;
            src.FilePath = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileImageInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileImageInfo to copy from
        ModelFileImageInfo& operator=(const ModelFileImageInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileImageInfo>(axrModelFileImageInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileImageInfo to move from
        ModelFileImageInfo& operator=(ModelFileImageInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                FilePath = src.FilePath;

                src.Name = nullptr;
                src.FilePath = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileImageInfo as an AxrModelFileImageInfo
        /// @returns This as an AxrModelFileImageInfo
        const AxrModelFileImageInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileImageInfo*>(this);
        }

        /// Get a handle to the ModelFileImageInfo as an AxrModelFileImageInfo
        /// @returns This as an AxrModelFileImageInfo
        AxrModelFileImageInfo* toRaw() {
            return reinterpret_cast<AxrModelFileImageInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileImageInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileImageInfo) == sizeof(axr::ModelFileImageInfo),
        "Original type and wrapper have different size!"
    );

    /// Model file material info
    struct ModelFileMaterialInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char* Name;
        int32_t ColorImageIndex;
        int32_t ColorImageSamplerIndex;
        glm::vec4 ColorFactor;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileMaterialInfo() :
            Name(nullptr),
            ColorImageIndex(-1),
            ColorImageSamplerIndex(-1),
            ColorFactor({}) {
        }

        /// Constructor
        /// @param src Source ModelFileMaterialInfo
        explicit ModelFileMaterialInfo(AxrModelFileMaterialInfo&& src) {
            Name = src.Name;
            ColorImageIndex = src.ColorImageIndex;
            ColorImageSamplerIndex = src.ColorImageSamplerIndex;
            ColorFactor = src.ColorFactor;

            src.Name = nullptr;
            src.ColorImageIndex = -1;
            src.ColorImageSamplerIndex = -1;
            src.ColorFactor = {};
        }

        /// Copy Constructor
        /// @param src Source ModelFileMaterialInfo to copy from
        ModelFileMaterialInfo(const axr::ModelFileMaterialInfo& src) {
            *this = static_cast<axr::ModelFileMaterialInfo>(axrModelFileMaterialInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileMaterialInfo to move from
        ModelFileMaterialInfo(ModelFileMaterialInfo&& src) noexcept {
            Name = src.Name;
            ColorImageIndex = src.ColorImageIndex;
            ColorImageSamplerIndex = src.ColorImageSamplerIndex;
            ColorFactor = src.ColorFactor;

            src.Name = nullptr;
            src.ColorImageIndex = -1;
            src.ColorImageSamplerIndex = -1;
            src.ColorFactor = {};
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileMaterialInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileMaterialInfo to copy from
        ModelFileMaterialInfo& operator=(const ModelFileMaterialInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileMaterialInfo>(axrModelFileMaterialInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileMaterialInfo to move from
        ModelFileMaterialInfo& operator=(ModelFileMaterialInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                Name = src.Name;
                ColorImageIndex = src.ColorImageIndex;
                ColorImageSamplerIndex = src.ColorImageSamplerIndex;
                ColorFactor = src.ColorFactor;

                src.Name = nullptr;
                src.ColorImageIndex = -1;
                src.ColorImageSamplerIndex = -1;
                src.ColorFactor = {};
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileMaterialInfo as an AxrModelFileMaterialInfo
        /// @returns This as an AxrModelFileMaterialInfo
        const AxrModelFileMaterialInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileMaterialInfo*>(this);
        }

        /// Get a handle to the ModelFileMaterialInfo as an AxrModelFileMaterialInfo
        /// @returns This as an AxrModelFileMaterialInfo
        AxrModelFileMaterialInfo* toRaw() {
            return reinterpret_cast<AxrModelFileMaterialInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileMaterialInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileMaterialInfo) == sizeof(axr::ModelFileMaterialInfo),
        "Original type and wrapper have different size!"
    );

    /// Model file submesh info
    struct ModelFileSubmeshInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        int32_t MaterialIndex;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileSubmeshInfo() :
            MaterialIndex(-1) {
        }

        /// Constructor
        /// @param src Source ModelFileSubmeshInfo
        explicit ModelFileSubmeshInfo(AxrModelFileSubmeshInfo&& src) {
            MaterialIndex = src.MaterialIndex;

            src.MaterialIndex = -1;
        }

        /// Copy Constructor
        /// @param src Source ModelFileSubmeshInfo to copy from
        ModelFileSubmeshInfo(const axr::ModelFileSubmeshInfo& src) {
            *this = static_cast<axr::ModelFileSubmeshInfo>(axrModelFileSubmeshInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileSubmeshInfo to move from
        ModelFileSubmeshInfo(ModelFileSubmeshInfo&& src) noexcept {
            MaterialIndex = src.MaterialIndex;

            src.MaterialIndex = -1;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileSubmeshInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileSubmeshInfo to copy from
        ModelFileSubmeshInfo& operator=(const ModelFileSubmeshInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileSubmeshInfo>(axrModelFileSubmeshInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileSubmeshInfo to move from
        ModelFileSubmeshInfo& operator=(ModelFileSubmeshInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                MaterialIndex = src.MaterialIndex;

                src.MaterialIndex = -1;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileSubmeshInfo as an AxrModelFileSubmeshInfo
        /// @returns This as an AxrModelFileSubmeshInfo
        const AxrModelFileSubmeshInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileSubmeshInfo*>(this);
        }

        /// Get a handle to the ModelFileSubmeshInfo as an AxrModelFileSubmeshInfo
        /// @returns This as an AxrModelFileSubmeshInfo
        AxrModelFileSubmeshInfo* toRaw() {
            return reinterpret_cast<AxrModelFileSubmeshInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileSubmeshInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileSubmeshInfo) == sizeof(axr::ModelFileSubmeshInfo),
        "Original type and wrapper have different size!"
    );

    /// Model file mesh info
    struct ModelFileMeshInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t SubmeshCount;
        axr::ModelFileSubmeshInfo* Submeshes;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileMeshInfo() :
            SubmeshCount(0),
            Submeshes(nullptr) {
        }

        /// Constructor
        /// @param src Source ModelFileMeshInfo
        explicit ModelFileMeshInfo(AxrModelFileMeshInfo&& src) {
            SubmeshCount = src.SubmeshCount;
            Submeshes = reinterpret_cast<axr::ModelFileSubmeshInfo*>(src.Submeshes);

            src.SubmeshCount = 0;
            src.Submeshes = nullptr;
        }

        /// Copy Constructor
        /// @param src Source ModelFileMeshInfo to copy from
        ModelFileMeshInfo(const axr::ModelFileMeshInfo& src) {
            *this = static_cast<axr::ModelFileMeshInfo>(axrModelFileMeshInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileMeshInfo to move from
        ModelFileMeshInfo(ModelFileMeshInfo&& src) noexcept {
            SubmeshCount = src.SubmeshCount;
            Submeshes = src.Submeshes;

            src.SubmeshCount = 0;
            src.Submeshes = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileMeshInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileMeshInfo to copy from
        ModelFileMeshInfo& operator=(const ModelFileMeshInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileMeshInfo>(axrModelFileMeshInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileMeshInfo to move from
        ModelFileMeshInfo& operator=(ModelFileMeshInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                SubmeshCount = src.SubmeshCount;
                Submeshes = src.Submeshes;

                src.SubmeshCount = 0;
                src.Submeshes = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileMeshInfo as an AxrModelFileMeshInfo
        /// @returns This as an AxrModelFileMeshInfo
        const AxrModelFileMeshInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileMeshInfo*>(this);
        }

        /// Get a handle to the ModelFileMeshInfo as an AxrModelFileMeshInfo
        /// @returns This as an AxrModelFileMeshInfo
        AxrModelFileMeshInfo* toRaw() {
            return reinterpret_cast<AxrModelFileMeshInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileMeshInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileMeshInfo) == sizeof(axr::ModelFileMeshInfo),
        "Original type and wrapper have different size!"
    );

    /// Model file info
    struct ModelFileInfo {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t ImageSamplerCount;
        axr::ModelFileImageSamplerInfo* ImageSamplers;
        uint32_t ImageCount;
        axr::ModelFileImageInfo* Images;
        uint32_t MaterialCount;
        axr::ModelFileMaterialInfo* Materials;
        uint32_t MeshCount;
        axr::ModelFileMeshInfo* Meshes;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ModelFileInfo() :
            ImageSamplerCount(0),
            ImageSamplers(nullptr),
            ImageCount(0),
            Images(nullptr),
            MaterialCount(0),
            Materials(nullptr),
            MeshCount(0),
            Meshes(nullptr) {
        }

        /// Constructor
        /// @param src Source ModelFileInfo
        explicit ModelFileInfo(AxrModelFileInfo&& src) {
            ImageSamplerCount = src.ImageSamplerCount;
            ImageSamplers = reinterpret_cast<axr::ModelFileImageSamplerInfo*>(src.ImageSamplers);
            ImageCount = src.ImageCount;
            Images = reinterpret_cast<axr::ModelFileImageInfo*>(src.Images);
            MaterialCount = src.MaterialCount;
            Materials = reinterpret_cast<axr::ModelFileMaterialInfo*>(src.Materials);
            MeshCount = src.MeshCount;
            Meshes = reinterpret_cast<axr::ModelFileMeshInfo*>(src.Meshes);

            src.ImageSamplerCount = 0;
            src.ImageSamplers = nullptr;
            src.ImageCount = 0;
            src.Images = nullptr;
            src.MaterialCount = 0;
            src.Materials = nullptr;
            src.MeshCount = 0;
            src.Meshes = nullptr;
        }

        /// Copy Constructor
        /// @param src Source ModelFileInfo to copy from
        ModelFileInfo(const axr::ModelFileInfo& src) {
            *this = static_cast<axr::ModelFileInfo>(axrModelFileInfoClone(src.toRaw()));
        }

        /// Move Constructor
        /// @param src Source ModelFileInfo to move from
        ModelFileInfo(ModelFileInfo&& src) noexcept {
            ImageSamplerCount = src.ImageSamplerCount;
            ImageSamplers = src.ImageSamplers;
            ImageCount = src.ImageCount;
            Images = src.Images;
            MaterialCount = src.MaterialCount;
            Materials = src.Materials;
            MeshCount = src.MeshCount;
            Meshes = src.Meshes;

            src.ImageSamplerCount = 0;
            src.ImageSamplers = nullptr;
            src.ImageCount = 0;
            src.Images = nullptr;
            src.MaterialCount = 0;
            src.Materials = nullptr;
            src.MeshCount = 0;
            src.Meshes = nullptr;
        }

        // ---- Destructor ----

        /// Destructor
        ~ModelFileInfo() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source ModelFileInfo to copy from
        ModelFileInfo& operator=(const ModelFileInfo& src) {
            if (this != &src) {
                cleanup();

                *this = static_cast<axr::ModelFileInfo>(axrModelFileInfoClone(src.toRaw()));
            }

            return *this;
        }

        /// Move Assignment Operator
        /// @param src Source ModelFileInfo to move from
        ModelFileInfo& operator=(ModelFileInfo&& src) noexcept {
            if (this != &src) {
                cleanup();

                ImageSamplerCount = src.ImageSamplerCount;
                ImageSamplers = src.ImageSamplers;
                ImageCount = src.ImageCount;
                Images = src.Images;
                MaterialCount = src.MaterialCount;
                Materials = src.Materials;
                MeshCount = src.MeshCount;
                Meshes = src.Meshes;

                src.ImageSamplerCount = 0;
                src.ImageSamplers = nullptr;
                src.ImageCount = 0;
                src.Images = nullptr;
                src.MaterialCount = 0;
                src.Materials = nullptr;
                src.MeshCount = 0;
                src.Meshes = nullptr;
            }

            return *this;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ModelFileInfo as an AxrModelFileInfo
        /// @returns This as an AxrModelFileInfo
        const AxrModelFileInfo* toRaw() const {
            return reinterpret_cast<const AxrModelFileInfo*>(this);
        }

        /// Get a handle to the ModelFileInfo as an AxrModelFileInfo
        /// @returns This as an AxrModelFileInfo
        AxrModelFileInfo* toRaw() {
            return reinterpret_cast<AxrModelFileInfo*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            axrModelFileInfoDestroy(this->toRaw());
        }
    };

    static_assert(
        sizeof(AxrModelFileInfo) == sizeof(axr::ModelFileInfo),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Function Definitions
    // ----------------------------------------- //

    /// Get a model's file info
    /// @param path The model's file path
    /// @param modelFileInfo Output model file info
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] inline AxrResult getModelFileData(
        const char* path,
        axr::ModelFileInfo& modelFileInfo
    ) {
        AxrModelFileInfo rawModelFileInfo;
        const AxrResult axrResult = axrGetModelFileInfo(path, &rawModelFileInfo);
        if (AXR_FAILED(axrResult)) {
            return axrResult;
        }
        modelFileInfo = axr::ModelFileInfo(std::move(rawModelFileInfo));

        return AXR_SUCCESS;
    }
}
