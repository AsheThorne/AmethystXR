#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanImageData.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanImageData::AxrVulkanImageData():
    m_ImageHandle(nullptr),
    m_Device(VK_NULL_HANDLE),
    m_MaxSamplerAnisotropy(0.0f),
    m_DispatchHandle(nullptr),
    m_Sampler(VK_NULL_HANDLE) {
}

AxrVulkanImageData::AxrVulkanImageData(const Config& config):
    m_Name(config.Name),
    m_ImageHandle(config.ImageHandle),
    m_Device(config.Device),
    m_MaxSamplerAnisotropy(config.MaxSamplerAnisotropy),
    m_DispatchHandle(config.DispatchHandle),
    m_Image(
        {
            .PhysicalDevice = config.PhysicalDevice,
            .Device = config.Device,
            .GraphicsCommandPool = config.GraphicsCommandPool,
            .GraphicsQueue = config.GraphicsQueue,
            .DispatchHandle = config.DispatchHandle,
        }
    ),
    m_Sampler(VK_NULL_HANDLE) {
}

AxrVulkanImageData::AxrVulkanImageData(AxrVulkanImageData&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_Image = std::move(src.m_Image);

    m_ImageHandle = src.m_ImageHandle;
    m_Device = src.m_Device;
    m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
    m_DispatchHandle = src.m_DispatchHandle;
    m_Sampler = src.m_Sampler;

    src.m_ImageHandle = nullptr;
    src.m_Device = VK_NULL_HANDLE;
    src.m_MaxSamplerAnisotropy = 0.0f;
    src.m_DispatchHandle = nullptr;
    src.m_Sampler = VK_NULL_HANDLE;
}

AxrVulkanImageData::~AxrVulkanImageData() {
    cleanup();
}

AxrVulkanImageData& AxrVulkanImageData::operator=(AxrVulkanImageData&& src) noexcept {
    if (this != &src) {
        m_Name = std::move(src.m_Name);
        m_Image = std::move(src.m_Image);

        m_ImageHandle = src.m_ImageHandle;
        m_Device = src.m_Device;
        m_MaxSamplerAnisotropy = src.m_MaxSamplerAnisotropy;
        m_DispatchHandle = src.m_DispatchHandle;
        m_Sampler = src.m_Sampler;

        src.m_ImageHandle = nullptr;
        src.m_Device = VK_NULL_HANDLE;
        src.m_MaxSamplerAnisotropy = 0.0f;
        src.m_DispatchHandle = nullptr;
        src.m_Sampler = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanImageData::getName() const {
    return m_Name;
}

bool AxrVulkanImageData::doesDataExist() const {
    return !m_Image.isEmpty() && m_Sampler != VK_NULL_HANDLE;
}

AxrResult AxrVulkanImageData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesDataExist()) {
        axrLogErrorLocation("Model data already exists.");
        return AXR_ERROR;
    }

    if (m_ImageHandle == nullptr) {
        axrLogErrorLocation("Image handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_Image.createImage(m_ImageHandle);
    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    axrResult = createSampler(
        m_Image.getMipLevelCount(),
        m_ImageHandle->getSamplerFilter(),
        m_ImageHandle->getSamplerWrapping(),
        m_Sampler
    );
    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImageData::destroyData() {
    destroySampler(m_Sampler);
    m_Image.destroyImage();
}

// ---- Private Functions ----

void AxrVulkanImageData::cleanup() {
    destroyData();

    m_Name.clear();
    m_ImageHandle = nullptr;
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanImageData::createSampler(
    const uint32_t mipLevelCount,
    const AxrImageSamplerFilterEnum imageSamplerFilter,
    const AxrImageSamplerWrappingEnum imageSamplerWrapping,
    vk::Sampler& sampler
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (sampler != VK_NULL_HANDLE) {
        axrLogErrorLocation("Sampler already exists.");
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

    const vk::Filter filter = axrToVkFilter(imageSamplerFilter);
    const vk::SamplerAddressMode addressMode = axrToVkSamplerAddressMode(imageSamplerWrapping);

    const vk::SamplerCreateInfo samplerCreateInfo(
        {},
        filter,
        filter,
        vk::SamplerMipmapMode::eLinear,
        addressMode,
        addressMode,
        addressMode,
        0.0f,
        m_MaxSamplerAnisotropy > 1.0f ? vk::True : vk::False,
        m_MaxSamplerAnisotropy,
        vk::False,
        vk::CompareOp::eAlways,
        0.0f,
        static_cast<float>(mipLevelCount),
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

void AxrVulkanImageData::destroySampler(vk::Sampler& sampler) const {
    if (sampler == VK_NULL_HANDLE) return;

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return;
    }

    m_Device.destroySampler(sampler, nullptr, *m_DispatchHandle);
    sampler = VK_NULL_HANDLE;
}

#endif
