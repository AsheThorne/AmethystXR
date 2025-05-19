// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "material.hpp"
#include "axr/logger.h"
#include "../utils.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrMaterialConfigIsValid(const AxrMaterialConfig* materialConfig) {
    if (materialConfig == nullptr) {
        axrLogErrorLocation("`materialConfig` is null.");
        return false;
    }

    return !axrStringIsEmpty(materialConfig->Name) &&
        !axrStringIsEmpty(materialConfig->VertexShaderName) &&
        !axrStringIsEmpty(materialConfig->FragmentShaderName) &&
        AxrShaderValuesRAII::isValid(materialConfig->VertexShaderValues) &&
        AxrShaderValuesRAII::isValid(materialConfig->FragmentShaderValues);
}

const char* axrMaterialGetName(const AxrMaterial_T material) {
    if (material == nullptr) {
        axrLogErrorLocation("`material` is null.");
        return "";
    }

    return material->getName();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrMaterial::AxrMaterial():
    m_Name(""),
    m_VertexShaderName(""),
    m_FragmentShaderName("") {
}

AxrMaterial::AxrMaterial(const AxrMaterialConfig& config):
    m_Name(config.Name),
    m_VertexShaderName(config.VertexShaderName),
    m_FragmentShaderName(config.FragmentShaderName),
    m_PushConstantsBufferName(config.PushConstantsBufferName),
    m_VertexShaderValues(config.VertexShaderValues),
    m_FragmentShaderValues(config.FragmentShaderValues) {
    if (!axrMaterialConfigIsValid(&config)) {
        axrLogErrorLocation("Material config is invalid.");
    }
}

AxrMaterial::AxrMaterial(const AxrMaterial& src) {
    m_Name = src.m_Name;
    m_VertexShaderName = src.m_VertexShaderName;
    m_FragmentShaderName = src.m_FragmentShaderName;
    m_PushConstantsBufferName = src.m_PushConstantsBufferName;
    m_VertexShaderValues = src.m_VertexShaderValues;
    m_FragmentShaderValues = src.m_FragmentShaderValues;
}

AxrMaterial::AxrMaterial(AxrMaterial&& src) noexcept {
    m_VertexShaderValues = std::move(src.m_VertexShaderValues);
    m_FragmentShaderValues = std::move(src.m_FragmentShaderValues);
    m_PushConstantsBufferName = std::move(src.m_PushConstantsBufferName);

    m_Name = src.m_Name;
    m_VertexShaderName = src.m_VertexShaderName;
    m_FragmentShaderName = src.m_FragmentShaderName;

    src.m_Name = "";
    src.m_VertexShaderName = "";
    src.m_FragmentShaderName = "";
}

AxrMaterial::~AxrMaterial() {
    cleanup();
}

AxrMaterial& AxrMaterial::operator=(const AxrMaterial& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_VertexShaderName = src.m_VertexShaderName;
        m_FragmentShaderName = src.m_FragmentShaderName;
        m_PushConstantsBufferName = src.m_PushConstantsBufferName;
        m_VertexShaderValues = src.m_VertexShaderValues;
        m_FragmentShaderValues = src.m_FragmentShaderValues;
    }

    return *this;
}

AxrMaterial& AxrMaterial::operator=(AxrMaterial&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_VertexShaderValues = std::move(src.m_VertexShaderValues);
        m_FragmentShaderValues = std::move(src.m_FragmentShaderValues);
        m_PushConstantsBufferName = std::move(src.m_PushConstantsBufferName);

        m_Name = src.m_Name;
        m_VertexShaderName = src.m_VertexShaderName;
        m_FragmentShaderName = src.m_FragmentShaderName;

        src.m_Name = "";
        src.m_VertexShaderName = "";
        src.m_FragmentShaderName = "";
    }

    return *this;
}

// ---- Public Functions ----

const char* AxrMaterial::getName() const {
    return m_Name;
}

const char* AxrMaterial::getVertexShaderName() const {
    return m_VertexShaderName;
}

const char* AxrMaterial::getFragmentShaderName() const {
    return m_FragmentShaderName;
}

const AxrShaderValuesRAII& AxrMaterial::getVertexShaderValues() const {
    return m_VertexShaderValues;
}

const AxrShaderValuesRAII& AxrMaterial::getFragmentShaderValues() const {
    return m_FragmentShaderValues;
}

const char* AxrMaterial::findShaderBufferName(const uint32_t binding) const {
    const char* bufferName = m_VertexShaderValues.findShaderBufferName(binding);
    if (!axrStringIsEmpty(bufferName)) {
        return bufferName;
    }

    bufferName = m_FragmentShaderValues.findShaderBufferName(binding);
    if (!axrStringIsEmpty(bufferName)) {
        return bufferName;
    }

    return "";
}

std::string AxrMaterial::getMaterialLayoutName() const {
    return std::string(std::string(m_VertexShaderName) + '_' + m_FragmentShaderName);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const std::string& AxrMaterial::getPushConstantsBufferName() const {
    return m_PushConstantsBufferName;
}
#endif

bool AxrMaterial::isValid() const {
    return !axrStringIsEmpty(m_Name) &&
        !axrStringIsEmpty(m_VertexShaderName) &&
        !axrStringIsEmpty(m_FragmentShaderName) &&
        m_VertexShaderValues.isValid() &&
        m_FragmentShaderValues.isValid();
}

void AxrMaterial::cleanup() {
    m_Name = "";
    m_VertexShaderName = "";
    m_FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantsBufferName.clear();
#endif

    m_VertexShaderValues.cleanup();
    m_FragmentShaderValues.cleanup();
}
