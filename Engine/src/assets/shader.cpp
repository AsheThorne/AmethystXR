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
#include "axr/utils.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrShaderConfigIsValid(const AxrShaderConfig* shaderConfig) {
    if (shaderConfig == nullptr) {
        axrLogErrorLocation("`shaderConfig` is null.");
        return false;
    }

    // TODO: Maybe check that the file path is a valid path and the file exists
    return !axrStringIsEmpty(shaderConfig->Name) &&
        !axrStringIsEmpty(shaderConfig->FilePath) &&
        AxrShaderPropertiesRAII::isValid(shaderConfig->Properties);
}

const char* axrShaderGetName(const AxrShader_T shader) {
    if (shader == nullptr) {
        axrLogErrorLocation("`shader` is null.");
        return "";
    }

    return shader->getName().c_str();
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
    if (!axrShaderConfigIsValid(&config)) {
        axrLogErrorLocation("Shader config is invalid.");
    }
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

const std::string& AxrShader::getName() const {
    return m_Name;
}

const std::vector<char>& AxrShader::getFileData() const {
    return m_FileData;
}

bool AxrShader::isLoaded() const {
    return !m_FileData.empty();
}

AxrResult AxrShader::loadFile(const AxrGraphicsApiEnum graphicsApi) const {
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

    std::filesystem::path path(m_FilePath);
    const std::string extension = path.extension().string();

    if (graphicsApi == AXR_GRAPHICS_API_VULKAN && extension != ".spv") {
        path.replace_extension(extension + ".spv");
    }

    //  TODO: Should we move this just to this class? I think so.
    return axrReadFileBytes(path, m_FileData);
}

void AxrShader::unloadFile() const {
    m_FileData.clear();
}

const AxrShaderPropertiesRAII& AxrShader::getProperties() const {
    return m_Properties;
}

bool AxrShader::isValid() const {
    // TODO: Maybe check that the file path is a valid path and the file exists
    return !m_Name.empty() &&
        !m_FilePath.empty() &&
        m_Properties.isValid();
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
