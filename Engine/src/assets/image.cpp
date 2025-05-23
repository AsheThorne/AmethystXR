// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "image.hpp"
#include "assetsUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrImageGetName(const AxrImage_T image) {
    if (image == nullptr) {
        axrLogErrorLocation("`image` is null.");
        return "";
    }

    return image->getName().c_str();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrImage::AxrImage() = default;

AxrImage::AxrImage(const AxrImageConfig& config):
    m_Name(config.Name),
    m_FilePath(config.FilePath),
    m_Data() {
}

AxrImage::AxrImage(const AxrImage& src) {
    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;
    m_Data = src.m_Data;
}

AxrImage::AxrImage(AxrImage&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_FilePath = std::move(src.m_FilePath);
    m_Data = std::move(src.m_Data);
}

AxrImage::~AxrImage() {
    cleanup();
}

AxrImage& AxrImage::operator=(const AxrImage& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;
        m_Data = src.m_Data;
    }

    return *this;
}

AxrImage& AxrImage::operator=(AxrImage&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_FilePath = std::move(src.m_FilePath);
        m_Data = std::move(src.m_Data);
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrImage::getName() const {
    return m_Name;
}

bool AxrImage::isLoaded() const {
    return !m_Data.Pixels.empty();
}

AxrResult AxrImage::loadFile() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (isLoaded()) {
        return AXR_SUCCESS;
    }

    if (m_FilePath.empty()) {
        axrLogErrorLocation("File path is empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    return loadImage(m_FilePath, m_Data);
}

void AxrImage::unloadFile() const {
    // Don't clear the image data if there is no original data file to be loaded again
    if (m_FilePath.empty()) return;

    m_Data.clear();
}

const std::vector<stbi_uc>& AxrImage::getPixelData() const {
    return m_Data.Pixels;
}

uint32_t AxrImage::getWidth() const {
    return m_Data.Width;
}

uint32_t AxrImage::getHeight() const {
    return m_Data.Height;
}

uint32_t AxrImage::getColorChannels() const {
    return m_Data.ColorChannels;
}

// ---- Private Functions ----

void AxrImage::cleanup() {
    m_Name.clear();
    m_FilePath.clear();
    m_Data.clear();
}

AxrResult AxrImage::loadImage(const std::string& path, Data& imageData) {
    int width;
    int height;
    int colorChannels;
    const std::filesystem::path fullPath = axrGetAbsolutePathToAsset(path);

    stbi_uc* pixelData = stbi_load(
        fullPath.string().c_str(),
        &width,
        &height,
        &colorChannels,
        0
    );

    if (pixelData == nullptr) {
        axrLogErrorLocation("Failed to read image data. {0}", stbi_failure_reason());
        return AXR_ERROR;
    }

    imageData.Width = width;
    imageData.Height = height;
    imageData.ColorChannels = colorChannels;

    const size_t pixelsSize =
        static_cast<uint64_t>(imageData.Width) *
        static_cast<uint64_t>(imageData.Height) *
        imageData.ColorChannels;

    imageData.Pixels.assign(pixelsSize, *pixelData);

    stbi_image_free(pixelData);
    return AXR_SUCCESS;
}
