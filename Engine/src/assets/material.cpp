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

    return material->getName().c_str();
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
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBufferName(config.PushConstantBufferName),
#endif
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
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBufferName = src.m_PushConstantBufferName;
#endif
    m_VertexShaderValues = src.m_VertexShaderValues;
    m_FragmentShaderValues = src.m_FragmentShaderValues;
}

AxrMaterial::AxrMaterial(AxrMaterial&& src) noexcept {
    m_VertexShaderValues = std::move(src.m_VertexShaderValues);
    m_FragmentShaderValues = std::move(src.m_FragmentShaderValues);
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBufferName = std::move(src.m_PushConstantBufferName);
#endif

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
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        m_PushConstantBufferName = src.m_PushConstantBufferName;
#endif
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
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        m_PushConstantBufferName = std::move(src.m_PushConstantBufferName);
#endif

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

const std::string& AxrMaterial::getName() const {
    return m_Name;
}

const std::string& AxrMaterial::getVertexShaderName() const {
    return m_VertexShaderName;
}

const std::string& AxrMaterial::getFragmentShaderName() const {
    return m_FragmentShaderName;
}

const AxrShaderValuesRAII& AxrMaterial::getVertexShaderValues() const {
    return m_VertexShaderValues;
}

const AxrShaderValuesRAII& AxrMaterial::getFragmentShaderValues() const {
    return m_FragmentShaderValues;
}

std::string AxrMaterial::findShaderBufferName(const uint32_t binding) const {
    std::string bufferName = m_VertexShaderValues.findShaderBufferName(binding);
    if (!bufferName.empty()) {
        return bufferName;
    }

    bufferName = m_FragmentShaderValues.findShaderBufferName(binding);
    if (!bufferName.empty()) {
        return bufferName;
    }

    return "";
}

std::string AxrMaterial::getMaterialLayoutName() const {
    return std::string(std::string(m_VertexShaderName) + '_' + m_FragmentShaderName);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
const std::string& AxrMaterial::getPushConstantBufferName() const {
    return m_PushConstantBufferName;
}
#endif

bool AxrMaterial::isValid() const {
    return !m_Name.empty() &&
        !m_VertexShaderName.empty() &&
        !m_FragmentShaderName.empty() &&
        m_VertexShaderValues.isValid() &&
        m_FragmentShaderValues.isValid();
}

void AxrMaterial::cleanup() {
    m_Name = "";
    m_VertexShaderName = "";
    m_FragmentShaderName = "";
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    m_PushConstantBufferName.clear();
#endif

    m_VertexShaderValues.cleanup();
    m_FragmentShaderValues.cleanup();
}
