// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <filesystem>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shader.hpp"
#include "axr/logger.h"
#include "assetsUtils.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrShaderGetName(const AxrShader_T shader) {
    if (shader == nullptr) {
        axrLogErrorLocation("`shader` is null.");
        return "";
    }

    return shader->getName();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrShader::AxrShader():
    m_Name(""),
    m_FilePath("") {
}

AxrShader::AxrShader(const AxrShaderConfig& config):
    m_Name(config.Name),
    m_FilePath(config.FilePath),
    m_Properties(config.Properties) {
}

AxrShader::AxrShader(const AxrShader& src) {
    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;
    m_Properties = src.m_Properties;
    m_FileData = src.m_FileData;
}

AxrShader::AxrShader(AxrShader&& src) noexcept {
    m_Properties = std::move(src.m_Properties);
    m_FileData = std::move(src.m_FileData);

    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;

    src.m_Name = "";
    src.m_FilePath = "";
}

AxrShader::~AxrShader() {
    cleanup();
}

AxrShader& AxrShader::operator=(const AxrShader& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;
        m_Properties = src.m_Properties;
        m_FileData = src.m_FileData;
    }

    return *this;
}

AxrShader& AxrShader::operator=(AxrShader&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Properties = std::move(src.m_Properties);
        m_FileData = std::move(src.m_FileData);

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;

        src.m_Name = "";
        src.m_FilePath = "";
    }

    return *this;
}

// ---- Public Functions ----

const char* AxrShader::getName() const {
    return m_Name;
}

bool AxrShader::isLoaded() const {
    return !m_FileData.empty();
}

AxrResult AxrShader::loadFile(const AxrGraphicsApiEnum graphicsApi) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (isLoaded()) {
        return AXR_SUCCESS;
    }

    if (std::strcmp(m_FilePath, "") == 0) {
        axrLogErrorLocation("File path is empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::filesystem::path path(m_FilePath);
    const std::string extension = path.extension().string();

    if (graphicsApi == AXR_GRAPHICS_API_VULKAN && extension != ".spv") {
        path.replace_extension(extension + ".spv");
    }

    return axrReadFileBytes(path, m_FileData);
}

void AxrShader::unloadFile() {
    m_FileData.clear();
}

const AxrShaderPropertiesRAII& AxrShader::getProperties() const {
    return m_Properties;
}

bool AxrShader::isValid() const {
    return m_Properties.isValid();
}

// ---- Public Static Functions ----

bool AxrShader::areCompatible(const AxrShader& shader1, const AxrShader& shader2) {
    return AxrShaderPropertiesRAII::areCompatible(shader1.getProperties(), shader2.getProperties());
}

// ---- Private Functions ----

void AxrShader::cleanup() {
    unloadFile();

    m_Name = "";
    m_FilePath = "";
    m_Properties.cleanup();
}
