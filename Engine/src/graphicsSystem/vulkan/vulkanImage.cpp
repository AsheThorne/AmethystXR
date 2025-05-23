#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanImage.hpp"
#include "axr/logger.h"
#include "vulkanBuffer.hpp"
#include "vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanImage::AxrVulkanImage():
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_DispatchHandle(VK_NULL_HANDLE),
    m_Image(VK_NULL_HANDLE),
    m_ImageMemory(VK_NULL_HANDLE) {
}

AxrVulkanImage::AxrVulkanImage(const Config& config):
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle),
    m_Image(VK_NULL_HANDLE),
    m_ImageMemory(VK_NULL_HANDLE) {
}

AxrVulkanImage::AxrVulkanImage(AxrVulkanImage&& src) noexcept {
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_TransferCommandPool = src.m_TransferCommandPool;
    m_TransferQueue = src.m_TransferQueue;
    m_DispatchHandle = src.m_DispatchHandle;
    m_Image = src.m_Image;
    m_ImageMemory = src.m_ImageMemory;

    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_DispatchHandle = VK_NULL_HANDLE;
    m_Image = VK_NULL_HANDLE;
    m_ImageMemory = VK_NULL_HANDLE;
}

AxrVulkanImage::~AxrVulkanImage() {
    cleanup();
}

AxrVulkanImage& AxrVulkanImage::operator=(AxrVulkanImage&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_TransferCommandPool = src.m_TransferCommandPool;
        m_TransferQueue = src.m_TransferQueue;
        m_DispatchHandle = src.m_DispatchHandle;
        m_Image = src.m_Image;
        m_ImageMemory = src.m_ImageMemory;

        m_PhysicalDevice = VK_NULL_HANDLE;
        m_Device = VK_NULL_HANDLE;
        m_TransferCommandPool = VK_NULL_HANDLE;
        m_TransferQueue = VK_NULL_HANDLE;
        m_DispatchHandle = VK_NULL_HANDLE;
        m_Image = VK_NULL_HANDLE;
        m_ImageMemory = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

bool AxrVulkanImage::isEmpty() const {
    return m_Image == VK_NULL_HANDLE &&
        m_ImageMemory == VK_NULL_HANDLE;
}

vk::Image AxrVulkanImage::getImage() const {
    return m_Image;
}

AxrResult AxrVulkanImage::createImage(const AxrImage_T image) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (image == nullptr) {
        axrLogErrorLocation("Image is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("DispatchHandle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    if (!image->isLoaded()) {
        axrResult = image->loadFile();

        if (AXR_FAILED(axrResult)) {
            destroyImage();
            return axrResult;
        }
    }

    const vk::DeviceSize imageSizeInBytes = image->getPixelData().size() * sizeof(stbi_uc);

    const AxrVulkanBuffer::Config bufferConfig{
        .PhysicalDevice = m_PhysicalDevice,
        .Device = m_Device,
        .TransferCommandPool = m_TransferCommandPool,
        .TransferQueue = m_TransferQueue,
        .DispatchHandle = m_DispatchHandle,
    };
    AxrVulkanBuffer buffer(bufferConfig);
    axrResult = buffer.createBuffer(false, imageSizeInBytes, vk::BufferUsageFlagBits::eTransferSrc);
    if (AXR_FAILED(axrResult)) {
        buffer.destroyBuffer();
        destroyImage();
        return axrResult;
    }

    axrResult = buffer.setBufferData(0, imageSizeInBytes, image->getPixelData().data());
    if (AXR_FAILED(axrResult)) {
        buffer.destroyBuffer();
        destroyImage();
        return axrResult;
    }

    const uint32_t mipLevelCount = countImageMipLevels(image->getWidth(), image->getHeight());

    axrResult = createImage(
        m_Device,
        m_PhysicalDevice,
        vk::Extent2D(image->getWidth(), image->getHeight()),
        mipLevelCount,
        vk::SampleCountFlagBits::e1,
        getImageFormat(image->getColorChannels()),
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_Image,
        m_ImageMemory,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        buffer.destroyBuffer();
        destroyImage();
        return axrResult;
    }

    axrResult = copyBufferToImage(
        buffer.getBuffer(),
        m_Image,
        image->getWidth(),
        image->getHeight(),
        mipLevelCount
    );
    if (AXR_FAILED(axrResult)) {
        buffer.destroyBuffer();
        destroyImage();
        return axrResult;
    }

    // We're done with the buffer now
    buffer.destroyBuffer();

    return AXR_SUCCESS;
}

void AxrVulkanImage::destroyImage() {
    if (m_Image && m_ImageMemory == VK_NULL_HANDLE) return;

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Failed to destroy image. Dispatch is null.");
        return;
    }

    destroyImage(m_Device, m_Image, m_ImageMemory, *m_DispatchHandle);
}

AxrResult AxrVulkanImage::createImage(
    const vk::Device device,
    const vk::PhysicalDevice physicalDevice,
    const vk::Extent2D extent,
    const uint32_t mipLevelCount,
    const vk::SampleCountFlagBits sampleCount,
    const vk::Format format,
    const vk::ImageTiling imageTiling,
    const vk::ImageUsageFlags imageUsage,
    const vk::MemoryPropertyFlags memoryProperties,
    vk::Image& image,
    vk::DeviceMemory& imageMemory,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (image != VK_NULL_HANDLE || imageMemory != VK_NULL_HANDLE) {
        axrLogErrorLocation("Image already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("PhysicalDevice is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::ImageCreateInfo imageCreateInfo(
        {},
        vk::ImageType::e2D,
        format,
        vk::Extent3D(extent, 0),
        mipLevelCount,
        1,
        sampleCount,
        imageTiling,
        imageUsage,
        vk::SharingMode::eExclusive,
        {},
        {},
        vk::ImageLayout::eUndefined
    );
    vk::Result vkResult = device.createImage(&imageCreateInfo, nullptr, &image, dispatch);
    axrLogVkResult(vkResult, "device.createImage");
    if (VK_FAILED(vkResult)) {
        destroyImage(device, image, imageMemory, dispatch);
        return AXR_ERROR;
    }

    const vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image, dispatch);
    uint32_t memoryTypeIndex = 0;
    const AxrResult axrResult = axrFindMemoryTypeIndex(
        physicalDevice,
        memoryRequirements.memoryTypeBits,
        memoryProperties,
        memoryTypeIndex,
        dispatch
    );
    if (AXR_FAILED(axrResult)) {
        destroyImage(device, image, imageMemory, dispatch);
        return AXR_ERROR;
    }

    const vk::MemoryAllocateInfo memoryAllocateInfo(
        memoryRequirements.size,
        memoryTypeIndex
    );
    vkResult = device.allocateMemory(
        &memoryAllocateInfo,
        nullptr,
        &imageMemory,
        dispatch
    );
    axrLogVkResult(vkResult, "device.allocateMemory");
    if (VK_FAILED(vkResult)) {
        destroyImage(device, image, imageMemory, dispatch);
        return AXR_ERROR;
    }

    vkResult = device.bindImageMemory(
        image,
        imageMemory,
        0,
        dispatch
    );
    axrLogVkResult(vkResult, "device.bindImageMemory");
    if (VK_FAILED(vkResult)) {
        destroyImage(device, image, imageMemory, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImage::destroyImage(
    const vk::Device device,
    vk::Image& image,
    vk::DeviceMemory& imageMemory,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (image != VK_NULL_HANDLE) {
        device.destroyImage(image, nullptr, dispatch);
        image = VK_NULL_HANDLE;
    }

    if (imageMemory != VK_NULL_HANDLE) {
        device.freeMemory(imageMemory, nullptr, dispatch);
        imageMemory = VK_NULL_HANDLE;
    }
}

// ---- Private Functions ----

void AxrVulkanImage::cleanup() {
    destroyImage();

    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_DispatchHandle = VK_NULL_HANDLE;
}

vk::Format AxrVulkanImage::getImageFormat(const uint32_t colorChannels) const {
    switch (colorChannels) {
        case 1: {
            return vk::Format::eR8Srgb;
        }
        case 2: {
            return vk::Format::eR8G8Srgb;
        }
        case 3: {
            return vk::Format::eR8G8B8Srgb;
        }
        case 4: {
            return vk::Format::eR8G8B8A8Srgb;
        }
        default: {
            axrLogErrorLocation("Unknown number of color channels.");
            return vk::Format::eUndefined;
        }
    }
}

uint32_t AxrVulkanImage::countImageMipLevels(const uint32_t width, const uint32_t height) const {
    return 1;
    // TODO: After implementing mip levels, uncomment this
    // return static_cast<uint32_t>(
    //     std::floor(std::log2(std::max(width, height)))
    // ) + 1;
}

AxrResult AxrVulkanImage::copyBufferToImage(
    const vk::Buffer buffer,
    const vk::Image image,
    const uint32_t imageWidth,
    const uint32_t imageHeight,
    const uint32_t mipLevelCount
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device == VK_NULL_HANDLE) {
        axrLogError("Device is null.");
        return AXR_ERROR;
    }

    if (m_TransferCommandPool == VK_NULL_HANDLE) {
        axrLogError("Transfer command pool is null.");
        return AXR_ERROR;
    }

    if (m_TransferQueue == VK_NULL_HANDLE) {
        axrLogError("Transfer queue is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogError("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    vk::CommandBuffer commandBuffer;
    axrResult = axrBeginSingleTimeCommand(
        m_Device,
        // TODO: Might need the graphics queue...
        m_TransferCommandPool,
        commandBuffer,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    vk::ImageMemoryBarrier imageMemoryBarrier(
        vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eTransferWrite,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal,
        vk::QueueFamilyIgnored,
        vk::QueueFamilyIgnored,
        image,
        vk::ImageSubresourceRange(
            vk::ImageAspectFlagBits::eColor,
            0,
            mipLevelCount,
            0,
            1
        )
    );

    // TODO: Make sure this equals 0
    auto test = vk::DependencyFlags();

    commandBuffer.pipelineBarrier(
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eTransfer,
        vk::DependencyFlags(),
        0,
        nullptr,
        0,
        nullptr,
        1,
        &imageMemoryBarrier,
        *m_DispatchHandle
    );

    const vk::BufferImageCopy bufferImageCopy(
        0,
        0,
        0,
        vk::ImageSubresourceLayers(
            vk::ImageAspectFlagBits::eColor,
            0,
            0,
            1
        ),
        vk::Offset3D(0, 0, 0),
        vk::Extent3D(imageWidth, imageHeight, 0)
    );

    commandBuffer.copyBufferToImage(
        buffer,
        image,
        vk::ImageLayout::eTransferDstOptimal,
        1,
        &bufferImageCopy,
        *m_DispatchHandle
    );

    imageMemoryBarrier = vk::ImageMemoryBarrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::eShaderReadOnlyOptimal,
        vk::QueueFamilyIgnored,
        vk::QueueFamilyIgnored,
        image,
        vk::ImageSubresourceRange(
            vk::ImageAspectFlagBits::eColor,
            0,
            mipLevelCount,
            0,
            1
        )
    );

    commandBuffer.pipelineBarrier(
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::DependencyFlags(),
        0,
        nullptr,
        0,
        nullptr,
        1,
        &imageMemoryBarrier,
        *m_DispatchHandle
    );

    // TODO: generate mipmaps

    axrResult = axrEndSingleTimeCommand(
        m_Device,
        // TODO: Might need the graphics queue. if it works like this, leave it
        m_TransferCommandPool,
        m_TransferQueue,
        commandBuffer,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

#endif
