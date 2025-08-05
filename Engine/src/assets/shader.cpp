// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <fstream>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shader.hpp"
#include "axr/logger.h"
#include "assetsUtils.hpp"
#include "axr/common/utils.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrShaderConfigIsValid(const AxrShaderConfig* shaderConfig) {
    if (shaderConfig == nullptr) {
        axrLogErrorLocation("`shaderConfig` is null.");
        return false;
    }

    return !axrStringIsEmpty(shaderConfig->Name) &&
        !axrStringIsEmpty(shaderConfig->FilePath) &&
        axrFileExists(shaderConfig->FilePath) &&
        AxrShaderPropertiesRAII::isValid(shaderConfig->Properties);
}

const char* axrShaderGetName(const AxrShaderConst_T shader) {
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

AxrResult AxrShader::loadFile() const {
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

    return readShaderFile(m_FilePath, m_FileData);
}

void AxrShader::unloadFile() const {
    m_FileData.clear();
}

const AxrShaderPropertiesRAII& AxrShader::getProperties() const {
    return m_Properties;
}

bool AxrShader::isValid() const {
    return !m_Name.empty() &&
        !m_FilePath.empty() &&
        axrFileExists(m_FilePath) &&
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

AxrResult AxrShader::readShaderFile(const std::filesystem::path& path, std::vector<char>& data) {
    const std::filesystem::path fullPath = axrGetAbsolutePathToAsset(path);

    // Open the file at the end so we can get the file size easily
    std::ifstream file(fullPath.c_str(), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        axrLogErrorLocation("Failed to open file");
        return AXR_ERROR;
    }

    data = std::vector<char>(file.tellg());
    file.seekg(0);
    file.read(data.data(), static_cast<std::streamsize>(data.size()));
    file.close();

    return AXR_SUCCESS;
}
