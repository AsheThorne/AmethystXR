// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "assetCollection.hpp"
#include "axr/logger.h"
#include "engineAssets.hpp"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "pushConstantBuffer.hpp"
#endif

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrResult axrAssetCollectionCreateShader(
    const AxrAssetCollection_T assetCollection,
    const AxrShaderConfig* shaderConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (shaderConfig == nullptr) {
        axrLogErrorLocation("`shaderConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createShader(*shaderConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetShader(
    const AxrAssetCollection_T assetCollection,
    const AxrEngineAssetEnum engineAssetEnum
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createShader(engineAssetEnum);
}

AxrResult axrAssetCollectionCreateMaterial(
    const AxrAssetCollection_T assetCollection,
    const AxrMaterialConfig* materialConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (materialConfig == nullptr) {
        axrLogErrorLocation("`materialConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createMaterial(*materialConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetMaterial_DefaultMaterial(
    const AxrAssetCollection_T assetCollection,
    const char* materialName,
    const AxrEngineAssetMaterial_DefaultMaterial materialValues
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createMaterial(materialName, materialValues);
}

AxrResult axrAssetCollectionCreateModel(
    const AxrAssetCollection_T assetCollection,
    const AxrModelConfig* modelConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (modelConfig == nullptr) {
        axrLogErrorLocation("`modelConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createModel(*modelConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetModel(
    const AxrAssetCollection_T assetCollection,
    const char* modelName,
    const AxrEngineAssetEnum engineAssetEnum
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createModel(modelName, engineAssetEnum);
}

AxrResult axrAssetCollectionCreateUniformBuffer(
    const AxrAssetCollection_T assetCollection,
    const AxrUniformBufferConfig* uniformBufferConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (uniformBufferConfig == nullptr) {
        axrLogErrorLocation("`uniformBufferConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createUniformBuffer(*uniformBufferConfig);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrResult axrAssetCollectionCreatePushConstantBuffer(
    const AxrAssetCollection_T assetCollection,
    const AxrPushConstantBufferConfig* pushConstantBufferConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (pushConstantBufferConfig == nullptr) {
        axrLogErrorLocation("`pushConstantBufferConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createPushConstantBuffer(*pushConstantBufferConfig);
}
#endif

AxrResult axrAssetCollectionCreateImage(
    const AxrAssetCollection_T assetCollection,
    const AxrImageConfig* imageConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (imageConfig == nullptr) {
        axrLogErrorLocation("`imageConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createImage(*imageConfig);
}

AxrResult axrAssetCollectionCreateEngineAssetImage(
    const AxrAssetCollection_T assetCollection,
    const char* imageName,
    const AxrEngineAssetEnum engineAssetEnum
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createImage(imageName, engineAssetEnum);
}

AxrResult axrAssetCollectionCreateImageSampler(
    const AxrAssetCollection_T assetCollection,
    const AxrImageSamplerConfig* imageSamplerConfig
) {
    if (assetCollection == nullptr) {
        axrLogErrorLocation("`assetCollection` is null.");
        return AXR_ERROR;
    }

    if (imageSamplerConfig == nullptr) {
        axrLogErrorLocation("`imageConfig` is null.");
        return AXR_ERROR;
    }

    return assetCollection->createImageSampler(*imageSamplerConfig);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrAssetCollection::AxrAssetCollection() = default;

AxrAssetCollection::AxrAssetCollection(AxrAssetCollection&& src) noexcept {
    OnMaterialCreatedCallbackGraphics = std::move(src.OnMaterialCreatedCallbackGraphics);
    OnModelCreatedCallbackGraphics = std::move(src.OnModelCreatedCallbackGraphics);
    OnUniformBufferCreatedCallbackGraphics = std::move(src.OnUniformBufferCreatedCallbackGraphics);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    OnPushConstantBufferCreatedCallbackGraphics = std::move(src.OnPushConstantBufferCreatedCallbackGraphics);
#endif
    OnImageCreatedCallbackGraphics = std::move(src.OnImageCreatedCallbackGraphics);
    OnImageSamplerCreatedCallbackGraphics = std::move(src.OnImageSamplerCreatedCallbackGraphics);

    m_Shaders = std::move(src.m_Shaders);
    m_Materials = std::move(src.m_Materials);
    m_Models = std::move(src.m_Models);
    m_UniformBuffers = std::move(src.m_UniformBuffers);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBuffers = std::move(src.m_PushConstantBuffers);
#endif
    m_Images = std::move(src.m_Images);
}

AxrAssetCollection::~AxrAssetCollection() {
    cleanup();
}

AxrAssetCollection& AxrAssetCollection::operator=(AxrAssetCollection&& src) noexcept {
    if (this == &src) {
        cleanup();

        OnMaterialCreatedCallbackGraphics = std::move(src.OnMaterialCreatedCallbackGraphics);
        OnModelCreatedCallbackGraphics = std::move(src.OnModelCreatedCallbackGraphics);
        OnUniformBufferCreatedCallbackGraphics = std::move(src.OnUniformBufferCreatedCallbackGraphics);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        OnPushConstantBufferCreatedCallbackGraphics = std::move(src.OnPushConstantBufferCreatedCallbackGraphics);
#endif
        OnImageCreatedCallbackGraphics = std::move(src.OnImageCreatedCallbackGraphics);
        OnImageSamplerCreatedCallbackGraphics = std::move(src.OnImageSamplerCreatedCallbackGraphics);

        m_Shaders = std::move(src.m_Shaders);
        m_Materials = std::move(src.m_Materials);
        m_Models = std::move(src.m_Models);
        m_UniformBuffers = std::move(src.m_UniformBuffers);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        m_PushConstantBuffers = std::move(src.m_PushConstantBuffers);
#endif
        m_Images = std::move(src.m_Images);
    }

    return *this;
}

// ---- Public Functions ----

AxrResult AxrAssetCollection::createShader(const AxrShaderConfig& shaderConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (axrEngineAssetIsShaderNameReserved(shaderConfig.Name)) {
        axrLogError("Unable to create shader. The shader name: {0} is reserved by the engine.", shaderConfig.Name);
        return AXR_ERROR;
    }

    if (m_Shaders.contains(shaderConfig.Name)) {
        axrLogError("Unable to create shader. A shader named: {0} already exists.", shaderConfig.Name);
        return AXR_ERROR;
    }

    if (!axrShaderConfigIsValid(&shaderConfig)) {
        axrLogError("Unable to create shader. Shader config is invalid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Shaders.insert(std::pair(shaderConfig.Name, AxrShader(shaderConfig)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert shader.");
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createShader(const AxrEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!axrEngineAssetIsShader(engineAssetEnum)) {
        axrLogError("Unable to create shader. Engine asset is not a shader.");
        return AXR_ERROR;
    }

    const std::string& shaderName = axrEngineAssetGetName(engineAssetEnum);
    if (shaderName.empty()) {
        axrLogError("Unable to create shader. Unknown shader engine asset name.");
        return AXR_ERROR;
    }

    if (m_Shaders.contains(shaderName)) {
        axrLogError("Unable to create shader. A shader named: {0} already exists.", shaderName.c_str());
        return AXR_ERROR;
    }

    AxrShader shader;
    const AxrResult axrResult = axrEngineAssetCreateShader(engineAssetEnum, shader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create shader engine asset.");
        return axrResult;
    }

    if (!shader.isValid()) {
        axrLogError("Unable to create shader. Shader is invalid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Shaders.insert(std::pair(shaderName, std::move(shader)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert shader.");
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createMaterial(const AxrMaterialConfig& materialConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Materials.contains(materialConfig.Name)) {
        axrLogError("Unable to create material. A material named: {0} already exists.", materialConfig.Name);
        return AXR_ERROR;
    }

    if (!axrMaterialConfigIsValid(&materialConfig)) {
        axrLogError("Unable to create material. Material config is invalid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Materials.insert(std::pair(materialConfig.Name, AxrMaterial(materialConfig)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert material.");
        return AXR_ERROR;
    }

    OnMaterialCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createMaterial(
    const std::string& materialName,
    const AxrEngineAssetMaterial_DefaultMaterial materialValues
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Materials.contains(materialName)) {
        axrLogError("Unable to create material. A material named: {0} already exists.", materialName.c_str());
        return AXR_ERROR;
    }

    AxrMaterial material;
    const AxrResult axrResult = axrEngineAssetCreateMaterial_DefaultMaterial(
        materialName,
        materialValues,
        material
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create material engine asset.");
        return axrResult;
    }

    if (!material.isValid()) {
        axrLogError("Unable to create material. Material is invalid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Materials.insert(std::pair(materialName, std::move(material)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert material.");
        return AXR_ERROR;
    }

    OnMaterialCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createModel(const AxrModelConfig& modelConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Models.contains(modelConfig.Name)) {
        axrLogError("Unable to create model. A model named: {0} already exists.", modelConfig.Name);
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Models.insert(std::pair(modelConfig.Name, AxrModel(modelConfig)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert model.");
        return AXR_ERROR;
    }

    OnModelCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createModel(const std::string& modelName, const AxrEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!axrEngineAssetIsModel(engineAssetEnum)) {
        axrLogError("Unable to create model. Engine asset is not a model.");
        return AXR_ERROR;
    }

    if (m_Models.contains(modelName)) {
        axrLogError("Unable to create model. A model named: {0} already exists.", modelName.c_str());
        return AXR_ERROR;
    }

    AxrModel model;
    const AxrResult axrResult = axrEngineAssetCreateModel(modelName, engineAssetEnum, model);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create model engine asset.");
        return axrResult;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Models.insert(std::pair(modelName, std::move(model)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert model.");
        return AXR_ERROR;
    }

    OnModelCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createUniformBuffer(const AxrUniformBufferConfig& uniformBufferConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (axrEngineAssetIsUniformBufferNameReserved(uniformBufferConfig.Name)) {
        axrLogError(
            "Unable to create uniform buffer. The uniform buffer name: {0} is reserved by the engine.",
            uniformBufferConfig.Name
        );
        return AXR_ERROR;
    }

    if (m_UniformBuffers.contains(uniformBufferConfig.Name)) {
        axrLogError(
            "Unable to create uniform buffer. A uniform buffer named: {0} already exists.",
            uniformBufferConfig.Name
        );
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_UniformBuffers.insert(
        std::pair(uniformBufferConfig.Name, AxrUniformBuffer(uniformBufferConfig))
    );
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert uniform buffer.");
        return AXR_ERROR;
    }

    OnUniformBufferCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrResult AxrAssetCollection::createPushConstantBuffer(const AxrPushConstantBufferConfig& pushConstantBufferConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (axrEngineAssetIsPushConstantBufferNameReserved(pushConstantBufferConfig.Name)) {
        axrLogError(
            "Unable to create push constant buffer. The push constant buffer name: {0} is reserved by the engine.",
            pushConstantBufferConfig.Name
        );
        return AXR_ERROR;
    }

    if (m_PushConstantBuffers.contains(pushConstantBufferConfig.Name)) {
        axrLogError(
            "Unable to create push constant buffer. A push constant buffer named: {0} already exists.",
            pushConstantBufferConfig.Name
        );
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_PushConstantBuffers.insert(
        std::pair(pushConstantBufferConfig.Name, AxrPushConstantBuffer(pushConstantBufferConfig))
    );
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert push constant buffer.");
        return AXR_ERROR;
    }

    OnPushConstantBufferCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}
#endif

AxrResult AxrAssetCollection::createImage(const AxrImageConfig& imageConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (axrEngineAssetIsImageNameReserved(imageConfig.Name)) {
        axrLogError("Unable to create image. The image name: {0} is reserved by the engine.", imageConfig.Name);
        return AXR_ERROR;
    }

    if (m_Images.contains(imageConfig.Name)) {
        axrLogError("Unable to create image. An image named: {0} already exists.", imageConfig.Name);
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Images.insert(std::pair(imageConfig.Name, AxrImage(imageConfig)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert image.");
        return AXR_ERROR;
    }

    OnImageCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createImage(const std::string& imageName, const AxrEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!axrEngineAssetIsImage(engineAssetEnum)) {
        axrLogError("Unable to create image. Engine asset is not an image.");
        return AXR_ERROR;
    }

    if (axrEngineAssetIsImageNameReserved(imageName.c_str())) {
        axrLogError("Unable to create image. The image name: {0} is reserved by the engine.", imageName.c_str());
        return AXR_ERROR;
    }

    if (m_Images.contains(imageName)) {
        axrLogError("Unable to create image. An image named: {0} already exists.", imageName.c_str());
        return AXR_ERROR;
    }

    AxrImage image;
    const AxrResult axrResult = axrEngineAssetCreateImage(imageName, engineAssetEnum, image);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create image engine asset.");
        return axrResult;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Images.insert(std::pair(imageName, std::move(image)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert image.");
        return AXR_ERROR;
    }

    OnImageCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createImageSampler(const AxrImageSamplerConfig& imageSamplerConfig) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_ImageSamplers.contains(imageSamplerConfig.Name)) {
        axrLogError(
            "Unable to create image sampler. An image sampler named: {0} already exists.",
            imageSamplerConfig.Name
        );
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_ImageSamplers.insert(
        std::pair(imageSamplerConfig.Name, AxrImageSampler(imageSamplerConfig))
    );
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert image sampler.");
        return AXR_ERROR;
    }

    OnImageSamplerCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createImage(const AxrEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!axrEngineAssetIsImage(engineAssetEnum)) {
        axrLogError("Unable to create image. Engine asset is not an image.");
        return AXR_ERROR;
    }

    const std::string& imageName = axrEngineAssetGetName(engineAssetEnum);
    if (imageName.empty()) {
        axrLogError("Unable to create image. Unknown image engine asset name.");
        return AXR_ERROR;
    }

    if (m_Images.contains(imageName)) {
        axrLogError("Unable to create image. An image named: {0} already exists.", imageName.c_str());
        return AXR_ERROR;
    }

    AxrImage image;
    const AxrResult axrResult = axrEngineAssetCreateImage(imageName, engineAssetEnum, image);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create image engine asset.");
        return axrResult;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_Images.insert(std::pair(imageName, std::move(image)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert image.");
        return AXR_ERROR;
    }

    OnImageCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

AxrResult AxrAssetCollection::createImageSampler(const AxrEngineAssetEnum engineAssetEnum) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!axrEngineAssetIsImageSampler(engineAssetEnum)) {
        axrLogError("Unable to create image sampler. Engine asset is not an image sampler.");
        return AXR_ERROR;
    }

    const std::string& imageSamplerName = axrEngineAssetGetName(engineAssetEnum);
    if (imageSamplerName.empty()) {
        axrLogError("Unable to create image sampler. Unknown image sampler engine asset name.");
        return AXR_ERROR;
    }

    if (m_ImageSamplers.contains(imageSamplerName)) {
        axrLogError("Unable to create image sampler. An image sampler named: {0} already exists.", imageSamplerName.c_str());
        return AXR_ERROR;
    }

    AxrImageSampler imageSampler;
    const AxrResult axrResult = axrEngineAssetCreateImageSampler(imageSamplerName, engineAssetEnum, imageSampler);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create image sampler engine asset.");
        return axrResult;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto insertResult = m_ImageSamplers.insert(std::pair(imageSamplerName, std::move(imageSampler)));
    if (!insertResult.second) {
        axrLogErrorLocation("Failed to insert image sampler.");
        return AXR_ERROR;
    }

    OnImageSamplerCreatedCallbackGraphics(&insertResult.first->second);

    return AXR_SUCCESS;
}

void AxrAssetCollection::cleanup() {
    unloadAssets();

    OnMaterialCreatedCallbackGraphics.reset();
    OnModelCreatedCallbackGraphics.reset();
    OnUniformBufferCreatedCallbackGraphics.reset();
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    OnPushConstantBufferCreatedCallbackGraphics.reset();
#endif
    OnImageCreatedCallbackGraphics.reset();
    OnImageSamplerCreatedCallbackGraphics.reset();

    m_Shaders.clear();
    m_Materials.clear();
    m_Models.clear();
    m_UniformBuffers.clear();
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBuffers.clear();
#endif
    m_Images.clear();
}

bool AxrAssetCollection::isLoaded() {
    for (auto& [shaderName, shader] : m_Shaders) {
        if (!shader.isLoaded()) {
            return false;
        }
    }

    for (auto& [modelName, model] : m_Models) {
        if (!model.isLoaded()) {
            return false;
        }
    }

    for (auto& [imageName, image] : m_Images) {
        if (!image.isLoaded()) {
            return false;
        }
    }

    return true;
}

AxrResult AxrAssetCollection::loadAssets(const AxrGraphicsApiEnum graphicsApi) {
    for (auto& [shaderName, shader] : m_Shaders) {
        const AxrResult axrResult = shader.loadFile(graphicsApi);
        if (AXR_FAILED(axrResult)) {
            unloadAssets();
            return axrResult;
        }
    }

    for (auto& [modelName, model] : m_Models) {
        const AxrResult axrResult = model.loadFile();
        if (AXR_FAILED(axrResult)) {
            unloadAssets();
            return axrResult;
        }
    }

    for (auto& [imageName, image] : m_Images) {
        const AxrResult axrResult = image.loadFile();
        if (AXR_FAILED(axrResult)) {
            unloadAssets();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrAssetCollection::unloadAssets() {
    for (auto& [shaderName, shader] : m_Shaders) {
        shader.unloadFile();
    }

    for (auto& [modelName, model] : m_Models) {
        model.unloadFile();
    }

    for (auto& [imageName, image] : m_Images) {
        image.unloadFile();
    }
}

const AxrShader* AxrAssetCollection::findShader(const std::string& name) {
    const auto foundShaderIterator = m_Shaders.find(name);
    if (foundShaderIterator == m_Shaders.end()) {
        return nullptr;
    }

    return &foundShaderIterator->second;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const AxrPushConstantBuffer* AxrAssetCollection::findPushConstantBuffer(const std::string& name) {
    const auto foundPushConstantBufferIterator = m_PushConstantBuffers.find(name);
    if (foundPushConstantBufferIterator == m_PushConstantBuffers.end()) {
        return nullptr;
    }

    return &foundPushConstantBufferIterator->second;
}
#endif

const std::unordered_map<std::string, AxrShader>& AxrAssetCollection::getShaders() {
    return m_Shaders;
}

const std::unordered_map<std::string, AxrMaterial>& AxrAssetCollection::getMaterials() {
    return m_Materials;
}

const std::unordered_map<std::string, AxrModel>& AxrAssetCollection::getModels() {
    return m_Models;
}

const std::unordered_map<std::string, AxrUniformBuffer>& AxrAssetCollection::getUniformBuffers() {
    return m_UniformBuffers;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const std::unordered_map<std::string, AxrPushConstantBuffer>& AxrAssetCollection::getPushConstantBuffers() {
    return m_PushConstantBuffers;
}
#endif

const std::unordered_map<std::string, AxrImage>& AxrAssetCollection::getImages() {
    return m_Images;
}

const std::unordered_map<std::string, AxrImageSampler>& AxrAssetCollection::getImageSamplers() {
    return m_ImageSamplers;
}
