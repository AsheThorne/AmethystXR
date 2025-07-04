// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shaderValues.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

#include "axr/common/utils.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

// ---- Shader Buffer Links ----

AxrShaderBufferLink_T axrShaderBufferLinkClone(const AxrShaderBufferLinkConst_T bufferLink) {
    if (bufferLink == nullptr) return nullptr;

    return AxrShaderValuesRAII::clone(bufferLink);
}

void axrShaderBufferLinkDestroy(AxrShaderBufferLink_T* bufferLink) {
    if (bufferLink == nullptr) return;

    AxrShaderValuesRAII::destroy(*bufferLink);
}

AxrShaderUniformBufferLink_T axrShaderUniformBufferLinkClone(
    const AxrShaderUniformBufferLinkConst_T bufferLink
) {
    if (bufferLink == nullptr) return nullptr;

    return AxrShaderValuesRAII::clone(bufferLink);
}

void axrShaderUniformBufferLinkDestroy(AxrShaderUniformBufferLink_T* bufferLink) {
    if (bufferLink == nullptr) return;

    AxrShaderValuesRAII::destroy(*bufferLink);
}

AxrShaderImageSamplerBufferLink_T axrShaderImageSamplerBufferLinkClone(
    const AxrShaderImageSamplerBufferLinkConst_T bufferLink
) {
    if (bufferLink == nullptr) return nullptr;

    return AxrShaderValuesRAII::clone(bufferLink);
}

void axrShaderImageSamplerBufferLinkDestroy(AxrShaderImageSamplerBufferLink_T* bufferLink) {
    if (bufferLink == nullptr) return;

    AxrShaderValuesRAII::destroy(*bufferLink);
}

// ---- Shader Values ----

bool axrShaderValuesIsValid(const AxrShaderValuesConst_T values) {
    if (values == nullptr) return false;

    return AxrShaderValuesRAII::isValid(values);
}

AxrShaderValues_T axrShaderValuesClone(const AxrShaderValuesConst_T values) {
    if (values == nullptr) return nullptr;

    return AxrShaderValuesRAII::clone(values);
}

void axrShaderValuesDestroy(AxrShaderValues_T* values) {
    if (values == nullptr) return;

    AxrShaderValuesRAII::destroy(*values);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrShaderValuesRAII::AxrShaderValuesRAII():
    m_RawShaderValues(nullptr) {
}

AxrShaderValuesRAII::AxrShaderValuesRAII(const AxrShaderValuesConst_T values):
    m_RawShaderValues(nullptr) {
    if (!isValid(values)) {
        axrLogErrorLocation("Shader values are invalid.");
        return;
    }

    m_RawShaderValues = clone(values);
}

AxrShaderValuesRAII::AxrShaderValuesRAII(const AxrShaderValuesRAII& src) {
    m_RawShaderValues = clone(src.m_RawShaderValues);
}

AxrShaderValuesRAII::AxrShaderValuesRAII(AxrShaderValuesRAII&& src) noexcept {
    m_RawShaderValues = src.m_RawShaderValues;

    src.m_RawShaderValues = nullptr;
}

AxrShaderValuesRAII::~AxrShaderValuesRAII() {
    cleanup();
}

AxrShaderValuesRAII& AxrShaderValuesRAII::operator=(const AxrShaderValuesRAII& src) {
    if (this != &src) {
        cleanup();

        m_RawShaderValues = clone(src.m_RawShaderValues);
    }

    return *this;
}

AxrShaderValuesRAII& AxrShaderValuesRAII::operator=(AxrShaderValuesRAII&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_RawShaderValues = src.m_RawShaderValues;

        src.m_RawShaderValues = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

bool AxrShaderValuesRAII::isValid() const {
    return isValid(m_RawShaderValues);
}

std::vector<AxrShaderUniformBufferLinkConst_T> AxrShaderValuesRAII::getUniformBufferLinks() const {
    if (m_RawShaderValues == nullptr) {
        axrLogErrorLocation("Raw shader values are null.");
        return {};
    }

    std::vector<AxrShaderUniformBufferLinkConst_T> uniformBufferLinks;

    for (uint32_t i = 0; i < m_RawShaderValues->BufferLinkCount; ++i) {
        if (m_RawShaderValues->BufferLinks[i] == nullptr) {
            continue;
        }

        if (m_RawShaderValues->BufferLinks[i]->Type == AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER) {
            uniformBufferLinks.push_back(
                reinterpret_cast<AxrShaderUniformBufferLinkConst_T>(m_RawShaderValues->BufferLinks[i])
            );
        }
    }

    return uniformBufferLinks;
}

std::vector<AxrShaderImageSamplerBufferLinkConst_T> AxrShaderValuesRAII::getImageSamplerBufferLinks() const {
    if (m_RawShaderValues == nullptr) {
        axrLogErrorLocation("Raw shader values are null.");
        return {};
    }

    std::vector<AxrShaderImageSamplerBufferLinkConst_T> imageSamplerBufferLinks;

    for (uint32_t i = 0; i < m_RawShaderValues->BufferLinkCount; ++i) {
        if (m_RawShaderValues->BufferLinks[i] == nullptr) {
            continue;
        }

        if (m_RawShaderValues->BufferLinks[i]->Type == AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER) {
            imageSamplerBufferLinks.push_back(
                reinterpret_cast<AxrShaderImageSamplerBufferLinkConst_T>(m_RawShaderValues->BufferLinks[i])
            );
        }
    }

    return imageSamplerBufferLinks;
}

AxrShaderUniformBufferLinkConst_T AxrShaderValuesRAII::findShaderUniformBuffer(const uint32_t binding) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_RawShaderValues == nullptr) {
        axrLogErrorLocation("Raw shader values are null.");
        return nullptr;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    for (uint32_t i = 0; i < m_RawShaderValues->BufferLinkCount; ++i) {
        if (m_RawShaderValues->BufferLinks[i] == nullptr) continue;
        if (m_RawShaderValues->BufferLinks[i]->Type != AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER) continue;

        const auto uniformBufferLink = reinterpret_cast<AxrShaderUniformBufferLinkConst_T>(
            m_RawShaderValues->BufferLinks[i]
        );
        if (uniformBufferLink->Binding == binding) {
            return uniformBufferLink;
        }
        break;
    }

    return nullptr;
}

AxrShaderImageSamplerBufferLinkConst_T AxrShaderValuesRAII::findShaderImageSamplerBuffer(const uint32_t binding) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_RawShaderValues == nullptr) {
        axrLogErrorLocation("Raw shader values are null.");
        return nullptr;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    for (uint32_t i = 0; i < m_RawShaderValues->BufferLinkCount; ++i) {
        if (m_RawShaderValues->BufferLinks[i] == nullptr) continue;
        if (m_RawShaderValues->BufferLinks[i]->Type != AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER) continue;

        const auto imageSamplerBufferLink = reinterpret_cast<AxrShaderImageSamplerBufferLinkConst_T>(
            m_RawShaderValues->BufferLinks[i]
        );
        if (imageSamplerBufferLink->Binding == binding) {
            return imageSamplerBufferLink;
        }
        break;
    }

    return nullptr;
}

void AxrShaderValuesRAII::cleanup() {
    destroy(m_RawShaderValues);
}

// ---- Public Static Functions ----

AxrShaderValues_T AxrShaderValuesRAII::clone(const AxrShaderValuesConst_T values) {
    if (values == nullptr) return nullptr;

    return new AxrShaderValues{
        .BufferLinkCount = values->BufferLinkCount,
        .BufferLinks = clone(
            values->BufferLinkCount,
            values->BufferLinks
        ),
    };
}

void AxrShaderValuesRAII::destroy(AxrShaderValues_T& values) {
    if (values == nullptr) return;

    destroy(values->BufferLinkCount, values->BufferLinks);

    delete values;
    values = nullptr;
}

AxrShaderBufferLink_T* AxrShaderValuesRAII::clone(
    const uint32_t shaderBufferLinkCount,
    const AxrShaderBufferLinkConst_T* shaderBufferLinks
) {
    if (shaderBufferLinks == nullptr) return nullptr;

    const auto newShaderBufferLinks = new AxrShaderBufferLink_T[shaderBufferLinkCount]{};
    for (uint32_t i = 0; i < shaderBufferLinkCount; ++i) {
        newShaderBufferLinks[i] = clone(shaderBufferLinks[i]);
    }

    return newShaderBufferLinks;
}

void AxrShaderValuesRAII::destroy(
    uint32_t& shaderBufferLinkCount,
    AxrShaderBufferLink_T*& shaderBufferLinks
) {
    if (shaderBufferLinks == nullptr) return;

    for (uint32_t i = 0; i < shaderBufferLinkCount; ++i) {
        destroy(shaderBufferLinks[i]);
    }

    delete[] shaderBufferLinks;
    shaderBufferLinks = nullptr;
    shaderBufferLinkCount = 0;
}

AxrShaderBufferLink_T AxrShaderValuesRAII::clone(const AxrShaderBufferLinkConst_T shaderBufferLink) {
    if (shaderBufferLink == nullptr) return nullptr;

    switch (shaderBufferLink->Type) {
        case AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLink_T>(clone(
                reinterpret_cast<AxrShaderUniformBufferLinkConst_T>(shaderBufferLink)
            ));
        }
        case AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLink_T>(clone(
                reinterpret_cast<AxrShaderImageSamplerBufferLinkConst_T>(shaderBufferLink)
            ));
        }
        case AXR_SHADER_BUFFER_LINK_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader buffer link type.");
            return nullptr;
        }
    }
}

void AxrShaderValuesRAII::destroy(AxrShaderBufferLink_T& shaderBufferLink) {
    if (shaderBufferLink == nullptr) return;

    switch (shaderBufferLink->Type) {
        case AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderUniformBufferLink_T&>(shaderBufferLink));
            break;
        }
        case AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderImageSamplerBufferLink_T&>(shaderBufferLink));
            break;
        }
        case AXR_SHADER_BUFFER_LINK_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader buffer link type.");
            break;
        }
    }
}

AxrShaderUniformBufferLink_T AxrShaderValuesRAII::clone(
    const AxrShaderUniformBufferLinkConst_T shaderBufferLink
) {
    if (shaderBufferLink == nullptr) return nullptr;

    AxrShaderUniformBufferLink* link = new AxrShaderUniformBufferLink{
        .Binding = shaderBufferLink->Binding,
        .BufferName = {},
    };
    if (shaderBufferLink->BufferName != nullptr) {
        strncpy_s(link->BufferName, shaderBufferLink->BufferName, AXR_MAX_ASSET_NAME_SIZE);
    }

    return link;
}

void AxrShaderValuesRAII::destroy(AxrShaderUniformBufferLink_T& shaderBufferLink) {
    if (shaderBufferLink == nullptr) return;

    delete shaderBufferLink;
    shaderBufferLink = nullptr;
}

AxrShaderImageSamplerBufferLink_T AxrShaderValuesRAII::clone(
    const AxrShaderImageSamplerBufferLinkConst_T shaderBufferLink
) {
    if (shaderBufferLink == nullptr) return nullptr;

    AxrShaderImageSamplerBufferLink* link = new AxrShaderImageSamplerBufferLink{
        .Binding = shaderBufferLink->Binding,
        .ImageName = {},
        .ImageSamplerName = {},
    };
    if (shaderBufferLink->ImageName != nullptr) {
        strncpy_s(link->ImageName, shaderBufferLink->ImageName, AXR_MAX_ASSET_NAME_SIZE);
    }
    if (shaderBufferLink->ImageSamplerName != nullptr) {
        strncpy_s(link->ImageSamplerName, shaderBufferLink->ImageSamplerName, AXR_MAX_ASSET_NAME_SIZE);
    }

    return link;
}

void AxrShaderValuesRAII::destroy(AxrShaderImageSamplerBufferLink_T& shaderBufferLink) {
    if (shaderBufferLink == nullptr) return;

    delete shaderBufferLink;
    shaderBufferLink = nullptr;
}

bool AxrShaderValuesRAII::isValid(const AxrShaderValuesConst_T values) {
    if (values == nullptr) {
        axrLogError("Validation for shader values failed. Shader values are null.");
        return false;
    }

    return isValid(values->BufferLinkCount, values->BufferLinks);
}

bool AxrShaderValuesRAII::isValid(
    const uint32_t bufferLinkCount,
    const AxrShaderBufferLinkConst_T* bufferLinks
) {
    if (bufferLinks == nullptr) {
        // Buffer links aren't required
        return true;
    }

    std::unordered_set<uint32_t> bufferBindings;

    for (uint32_t i = 0; i < bufferLinkCount; ++i) {
        if (bufferLinks[i] == nullptr) {
            axrLogError("Validation for shader buffer links failed. Buffer link at index: {0} is null.", i);
            return false;
        }

        switch (bufferLinks[i]->Type) {
            case AXR_SHADER_BUFFER_LINK_UNIFORM_BUFFER: {
                const auto uniformBufferLink = reinterpret_cast<AxrShaderUniformBufferLinkConst_T>(bufferLinks[
                    i]);
                if (bufferBindings.contains(uniformBufferLink->Binding)) {
                    axrLogError(
                        "Validation for shader buffer links failed. Uniform buffer link has a duplicate binding."
                    );
                    return false;
                }
                bufferBindings.insert(uniformBufferLink->Binding);
                break;
            }
            case AXR_SHADER_BUFFER_LINK_IMAGE_SAMPLER_BUFFER: {
                const auto imageSamplerBufferLink = reinterpret_cast<AxrShaderImageSamplerBufferLinkConst_T>(
                    bufferLinks[i]);
                if (bufferBindings.contains(imageSamplerBufferLink->Binding)) {
                    axrLogError(
                        "Validation for shader buffer links failed. Image sampler buffer link has a duplicate binding."
                    );
                    return false;
                }
                bufferBindings.insert(imageSamplerBufferLink->Binding);
                break;
            }
            case AXR_SHADER_BUFFER_LINK_UNDEFINED:
            default: { // NOLINT(clang-diagnostic-covered-switch-default)
                axrLogErrorLocation("Unknown Shader buffer link.");
                return false;
            }
        }
    }

    return true;
}
