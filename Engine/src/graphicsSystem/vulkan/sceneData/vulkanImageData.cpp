#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanImageData.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanImageData::AxrVulkanImageData():
    m_ImageHandle(nullptr) {
}

AxrVulkanImageData::AxrVulkanImageData(const Config& config):
    m_ImageHandle(config.ImageHandle),
    m_Image(
        {
            .PhysicalDevice = config.PhysicalDevice,
            .Device = config.Device,
            .GraphicsCommandPool = config.GraphicsCommandPool,
            .GraphicsQueue = config.GraphicsQueue,
            .DispatchHandle = config.DispatchHandle,
        }
    ) {
}

AxrVulkanImageData::AxrVulkanImageData(AxrVulkanImageData&& src) noexcept {
    m_Image = std::move(src.m_Image);

    m_ImageHandle = src.m_ImageHandle;

    src.m_ImageHandle = nullptr;
}

AxrVulkanImageData::~AxrVulkanImageData() {
    cleanup();
}

AxrVulkanImageData& AxrVulkanImageData::operator=(AxrVulkanImageData&& src) noexcept {
    if (this != &src) {
        m_Image = std::move(src.m_Image);

        m_ImageHandle = src.m_ImageHandle;

        src.m_ImageHandle = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanImageData::getName() const {
    if (m_ImageHandle == nullptr) {
        return m_DummyName;
    }

    return m_ImageHandle->getName();
}

const vk::ImageView& AxrVulkanImageData::getImageView() const {
    return m_Image.getImageView();
}

bool AxrVulkanImageData::doesDataExist() const {
    return !m_Image.isEmpty();
}

AxrResult AxrVulkanImageData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesDataExist()) {
        axrLogErrorLocation("Image data already exists.");
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

    return AXR_SUCCESS;
}

void AxrVulkanImageData::destroyData() {
    m_Image.destroyImage();
}

// ---- Private Functions ----

void AxrVulkanImageData::cleanup() {
    destroyData();

    m_ImageHandle = nullptr;
}

#endif
