#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanImageSamplerData.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanImageSamplerData::AxrVulkanImageSamplerData():
    m_ImageSamplerHandle(nullptr),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_MaxSamplerAnisotropy(0.0f),
    m_DispatchHandle(nullptr),
    m_MainSampler(VK_NULL_HANDLE),
    m_BackupSampler(VK_NULL_HANDLE) {
}

AxrVulkanImageSamplerData::AxrVulkanImageSamplerData(const Config& config):
    m_ImageSamplerHandle(config.ImageSamplerHandle),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_MaxSamplerAnisotropy(config.MaxSamplerAnisotropy),
    m_DispatchHandle(config.DispatchHandle),
    m_MainSampler(VK_NULL_HANDLE),
    m_BackupSampler(VK_NULL_HANDLE) {
}

AxrVulkanImageSamplerData::AxrVulkanImageSamplerData(AxrVulkanImageSamplerData&& src) noexcept {
    m_ImageSamplerHandle = src.m_ImageSamplerHandle;
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
    m_DispatchHandle = src.m_DispatchHandle;
    m_MainSampler = src.m_MainSampler;
    m_BackupSampler = src.m_BackupSampler;

    src.m_ImageSamplerHandle = nullptr;
    src.m_PhysicalDevice = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_MaxSamplerAnisotropy = 0.0f;
    src.m_DispatchHandle = nullptr;
    src.m_MainSampler = VK_NULL_HANDLE;
    src.m_BackupSampler = VK_NULL_HANDLE;
}

AxrVulkanImageSamplerData::~AxrVulkanImageSamplerData() {
    cleanup();
}

AxrVulkanImageSamplerData& AxrVulkanImageSamplerData::operator=(AxrVulkanImageSamplerData&& src) noexcept {
    if (this != &src) {
        m_ImageSamplerHandle = src.m_ImageSamplerHandle;
        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
        m_DispatchHandle = src.m_DispatchHandle;
        m_MainSampler = src.m_MainSampler;
        m_BackupSampler = src.m_BackupSampler;

        src.m_ImageSamplerHandle = nullptr;
        src.m_PhysicalDevice = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_MaxSamplerAnisotropy = 0.0f;
        src.m_DispatchHandle = nullptr;
        src.m_MainSampler = VK_NULL_HANDLE;
        src.m_BackupSampler = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanImageSamplerData::getName() const {
    if (m_ImageSamplerHandle == nullptr) {
        return m_DummyName;
    }

    return m_ImageSamplerHandle->getName();
}

const vk::Sampler& AxrVulkanImageSamplerData::getSampler(const vk::Format imageFormat) const {
    // If there's no backup sampler, then the main should be compatible for all formats
    if (m_BackupSampler == VK_NULL_HANDLE) {
        return m_MainSampler;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE || m_DispatchHandle == nullptr) {
        // Can't check if it's supported. So just use the backup
        return m_BackupSampler;
    }

    if (axrAreFormatFeaturesSupported(
        imageFormat,
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eSampledImage | vk::FormatFeatureFlagBits::eSampledImageFilterLinear,
        m_PhysicalDevice,
        *m_DispatchHandle
    )) {
        return m_MainSampler;
    }

    return m_BackupSampler;
}

bool AxrVulkanImageSamplerData::doesDataExist() const {
    return m_MainSampler != VK_NULL_HANDLE;
}

AxrResult AxrVulkanImageSamplerData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesDataExist()) {
        axrLogErrorLocation("Image sampler data already exists.");
        return AXR_ERROR;
    }

    if (m_ImageSamplerHandle == nullptr) {
        axrLogErrorLocation("Image handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createSamplers(m_MainSampler, m_BackupSampler);
    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImageSamplerData::destroyData() {
    destroySampler(m_BackupSampler);
    destroySampler(m_MainSampler);
}

// ---- Private Functions ----

void AxrVulkanImageSamplerData::cleanup() {
    destroyData();

    m_ImageSamplerHandle = nullptr;
    m_Device = VK_NULL_HANDLE;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanImageSamplerData::createSamplers(vk::Sampler& mainSampler, vk::Sampler& backupSampler) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (mainSampler != VK_NULL_HANDLE) {
        axrLogErrorLocation("Main sampler already exists.");
        return AXR_ERROR;
    }

    if (backupSampler != VK_NULL_HANDLE) {
        axrLogErrorLocation("Backup sampler already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    vk::Filter magFilter = axrToVkFilter(m_ImageSamplerHandle->getMagFilter());
    vk::Filter minFilter = axrToVkFilter(m_ImageSamplerHandle->getMinFilter());
    vk::SamplerMipmapMode mipmapMode = axrToVkSamplerMipmapMode(m_ImageSamplerHandle->getMipmapFilter());

    AxrResult axrResult = createSampler(magFilter, minFilter, mipmapMode, mainSampler);
    if (AXR_FAILED(axrResult)) return axrResult;

    /// If the sampler has components which *may* not be supported, create a backup that will be supported by all image formats
    if (magFilter != vk::Filter::eLinear &&
        minFilter != vk::Filter::eLinear &&
        mipmapMode != vk::SamplerMipmapMode::eLinear) {
        return AXR_SUCCESS;
    }

    if (magFilter == vk::Filter::eLinear) {
        magFilter = vk::Filter::eNearest;
    }
    if (minFilter == vk::Filter::eLinear) {
        minFilter = vk::Filter::eNearest;
    }
    if (mipmapMode == vk::SamplerMipmapMode::eLinear) {
        mipmapMode = vk::SamplerMipmapMode::eNearest;
    }

    axrResult = createSampler(magFilter, minFilter, mipmapMode, backupSampler);
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}

AxrResult AxrVulkanImageSamplerData::createSampler(
    const vk::Filter magFilter,
    const vk::Filter minFilter,
    const vk::SamplerMipmapMode mipmapMode,
    vk::Sampler& sampler
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (sampler != VK_NULL_HANDLE) {
        axrLogErrorLocation("Sampler already exists.");
        return AXR_ERROR;
    }

    if (m_ImageSamplerHandle == nullptr) {
        axrLogErrorLocation("Image sampler handle is null.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::SamplerCreateInfo samplerCreateInfo(
        {},
        magFilter,
        minFilter,
        mipmapMode,
        axrToVkSamplerAddressMode(m_ImageSamplerHandle->getWrapU()),
        axrToVkSamplerAddressMode(m_ImageSamplerHandle->getWrapV()),
        vk::SamplerAddressMode::eRepeat,
        0.0f,
        m_MaxSamplerAnisotropy > 1.0f ? vk::True : vk::False,
        m_MaxSamplerAnisotropy,
        vk::False,
        vk::CompareOp::eAlways,
        0.0f,
        vk::LodClampNone,
        vk::BorderColor::eIntOpaqueBlack,
        vk::False
    );

    const vk::Result vkResult = m_Device.createSampler(
        &samplerCreateInfo,
        nullptr,
        &sampler,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.createSampler");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImageSamplerData::destroySampler(vk::Sampler& sampler) const {
    if (sampler == VK_NULL_HANDLE) return;

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return;
    }

    m_Device.destroySampler(sampler, nullptr, *m_DispatchHandle);
    sampler = VK_NULL_HANDLE;
}

#endif
