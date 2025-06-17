#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>
#include <string>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "shader.hpp"
#include "material.hpp"
#include "model.hpp"
#include "image.hpp"
#include "uniformBuffer.hpp"
#include "imageSampler.hpp"

/// Axr Asset Collection
class AxrAssetCollection {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrAssetCollection();
    /// Copy Constructor
    /// @param src Source AxrAssetCollection to copy from
    AxrAssetCollection(const AxrAssetCollection& src) = delete;
    /// Move Constructor
    /// @param src Source AxrAssetCollection to move from
    AxrAssetCollection(AxrAssetCollection&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrAssetCollection();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAssetCollection to copy from
    AxrAssetCollection& operator=(const AxrAssetCollection& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAssetCollection to move from
    AxrAssetCollection& operator=(AxrAssetCollection&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    // ---- Shader ----

    /// Create a new shader
    /// @param shaderConfig Shader config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createShader(const AxrShaderConfig& shaderConfig);
    /// Create a new engine asset shader
    /// @param engineAssetEnum Engine asset enum
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createShader(AxrEngineAssetEnum engineAssetEnum);

    // ---- Material ----

    /// Create a new material
    /// @param materialConfig Material config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterial(const AxrMaterialConfig& materialConfig);
    /// Create a material using the engine defined material named 'Default Material'
    /// @param materialName Material name
    /// @param materialValues Material values
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterial(
        const std::string& materialName,
        AxrEngineAssetMaterial_DefaultMaterial materialValues
    );

    // ---- Model ----

    /// Create a new model
    /// @param modelConfig Model config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createModel(const AxrModelConfig& modelConfig);
    /// Create a new engine asset model
    /// @param modelName Model name
    /// @param engineAssetEnum Engine asset enum
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createModel(const std::string& modelName, AxrEngineAssetEnum engineAssetEnum);

    // ---- Uniform Buffer ----

    /// Create a new uniform buffer
    /// @param uniformBufferConfig Uniform buffer config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createUniformBuffer(const AxrUniformBufferConfig& uniformBufferConfig);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    // ---- Push Constant Buffer ----

    /// Create a new push constant buffer
    /// @param pushConstantBufferConfig Push constant buffer config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPushConstantBuffer(const AxrPushConstantBufferConfig& pushConstantBufferConfig);
#endif

    // ---- Image ----

    /// Create a new image
    /// @param imageConfig Image config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createImage(const AxrImageConfig& imageConfig);
    /// Create a new engine asset image
    /// @param imageName Image name
    /// @param engineAssetEnum Engine asset enum
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createImage(const std::string& imageName, AxrEngineAssetEnum engineAssetEnum);

    // ---- Image Sampler ----

    /// Create a new image sampler
    /// @param imageSamplerConfig Image sampler config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createImageSampler(const AxrImageSamplerConfig& imageSamplerConfig);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    // ---- All Assets ----

    /// Clean up this class
    void cleanup();

    /// Check if all the assets in the collection have been loaded
    /// @returns True if all assets in the collection have been loaded
    [[nodiscard]] bool isLoaded();
    /// Load all assets in the collection
    /// @param graphicsApi Graphics api to use with these assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadAssets(AxrGraphicsApiEnum graphicsApi);
    /// Unload all assets in the collection
    void unloadAssets();

    /// Find the named shader
    /// @param name Name of the shader
    /// @returns A handle to the shader if it was found. nullptr if it wasn't found
    [[nodiscard]] const AxrShader* findShader(const std::string& name);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Find the named push constant buffer
    /// @param name Name of the push constant buffer
    /// @returns A handle to the push constant buffer if it was found. nullptr if it wasn't found
    [[nodiscard]] const AxrPushConstantBuffer* findPushConstantBuffer(const std::string& name);
#endif

    /// Get the shaders
    /// @returns A map of the shaders 
    [[nodiscard]] const std::unordered_map<std::string, AxrShader>& getShaders();
    /// Get the materials
    /// @returns A map of the materials
    [[nodiscard]] const std::unordered_map<std::string, AxrMaterial>& getMaterials();
    /// Get the models
    /// @returns A map of the models
    [[nodiscard]] const std::unordered_map<std::string, AxrModel>& getModels();
    /// Get the uniform buffers
    /// @returns A map of the uniform buffers
    [[nodiscard]] const std::unordered_map<std::string, AxrUniformBuffer>& getUniformBuffers();
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    /// Get the push constant buffers
    /// @returns A map of the push constant buffers
    [[nodiscard]] const std::unordered_map<std::string, AxrPushConstantBuffer>& getPushConstantBuffers();
#endif
    /// Get the images
    /// @returns A map of the images
    [[nodiscard]] const std::unordered_map<std::string, AxrImage>& getImages();
    /// Get the image samplers
    /// @returns A map of the image samplers
    [[nodiscard]] const std::unordered_map<std::string, AxrImageSampler>& getImageSamplers();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::unordered_map<std::string, AxrShader> m_Shaders;
    std::unordered_map<std::string, AxrMaterial> m_Materials;
    std::unordered_map<std::string, AxrModel> m_Models;
    std::unordered_map<std::string, AxrUniformBuffer> m_UniformBuffers;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    std::unordered_map<std::string, AxrPushConstantBuffer> m_PushConstantBuffers;
#endif
    std::unordered_map<std::string, AxrImage> m_Images;
    std::unordered_map<std::string, AxrImageSampler> m_ImageSamplers;
};
