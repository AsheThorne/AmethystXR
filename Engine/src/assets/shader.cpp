// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shader.hpp"
#include "axr/logger.h"

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

const char* axrShaderGetFilePath(const AxrShader_T shader) {
    if (shader == nullptr) {
        axrLogErrorLocation("`shader` is null.");
        return "";
    }

    return shader->getFilePath();
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
}

AxrShader::AxrShader(AxrShader&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_FilePath = std::move(src.m_FilePath);
    m_Properties = std::move(src.m_Properties);
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
    }

    return *this;
}

AxrShader& AxrShader::operator=(AxrShader&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_FilePath = std::move(src.m_FilePath);
        m_Properties = std::move(src.m_Properties);
    }

    return *this;
}

// ---- Public Functions ----

const char* AxrShader::getName() const {
    return m_Name;
}

const char* AxrShader::getFilePath() const {
    return m_FilePath;
}

// ---- Private Functions ----

void AxrShader::cleanup() {
    m_Name = "";
    m_FilePath = "";
    m_Properties.cleanup();
}
