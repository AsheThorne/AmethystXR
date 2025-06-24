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
    m_Device(VK_NULL_HANDLE),
    m_MaxSamplerAnisotropy(0.0f),
    m_DispatchHandle(nullptr),
    m_Sampler(VK_NULL_HANDLE) {
}

AxrVulkanImageSamplerData::AxrVulkanImageSamplerData(const Config& config):
    m_Name(config.Name),
    m_ImageSamplerHandle(config.ImageSamplerHandle),
    m_Device(config.Device),
    m_MaxSamplerAnisotropy(config.MaxSamplerAnisotropy),
    m_DispatchHandle(config.DispatchHandle),
    m_Sampler(VK_NULL_HANDLE) {
}

AxrVulkanImageSamplerData::AxrVulkanImageSamplerData(AxrVulkanImageSamplerData&& src) noexcept {
    m_Name = std::move(src.m_Name);

    m_ImageSamplerHandle = src.m_ImageSamplerHandle;
    m_Device = src.m_Device;
    m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
    m_DispatchHandle = src.m_DispatchHandle;
    m_Sampler = src.m_Sampler;

    src.m_ImageSamplerHandle = nullptr;
    src.m_Device = VK_NULL_HANDLE;
    src.m_MaxSamplerAnisotropy = 0.0f;
    src.m_DispatchHandle = nullptr;
    src.m_Sampler = VK_NULL_HANDLE;
}

AxrVulkanImageSamplerData::~AxrVulkanImageSamplerData() {
    cleanup();
}

AxrVulkanImageSamplerData& AxrVulkanImageSamplerData::operator=(AxrVulkanImageSamplerData&& src) noexcept {
    if (this != &src) {
        m_Name = std::move(src.m_Name);

        m_ImageSamplerHandle = src.m_ImageSamplerHandle;
        m_Device = src.m_Device;
        m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
        m_DispatchHandle = src.m_DispatchHandle;
        m_Sampler = src.m_Sampler;

        src.m_ImageSamplerHandle = nullptr;
        src.m_Device = VK_NULL_HANDLE;
        src.m_MaxSamplerAnisotropy = 0.0f;
        src.m_DispatchHandle = nullptr;
        src.m_Sampler = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanImageSamplerData::getName() const {
    return m_Name;
}

const vk::Sampler& AxrVulkanImageSamplerData::getSampler() const {
    return m_Sampler;
}

bool AxrVulkanImageSamplerData::doesDataExist() const {
    return m_Sampler != VK_NULL_HANDLE;
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

    axrResult = createSampler(m_Sampler);
    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImageSamplerData::destroyData() {
    destroySampler(m_Sampler);
}

// ---- Private Functions ----

void AxrVulkanImageSamplerData::cleanup() {
    destroyData();

    m_Name.clear();
    m_ImageSamplerHandle = nullptr;
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanImageSamplerData::createSampler(vk::Sampler& sampler) const {
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
        axrToVkFilter(m_ImageSamplerHandle->getMagFilter()),
        axrToVkFilter(m_ImageSamplerHandle->getMinFilter()),
        axrToVkSamplerMipmapMode(m_ImageSamplerHandle->getMipmapFilter()),
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
