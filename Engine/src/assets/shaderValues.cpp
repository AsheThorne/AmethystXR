// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shaderValues.hpp"
#include "axr/logger.h"

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

void AxrShaderValuesRAII::cleanup() {
    destroy(m_RawShaderValues);
}

// ---- Public Static Functions ----

AxrShaderValues_T AxrShaderValuesRAII::clone(const AxrShaderValuesConst_T values) {
    if (values == nullptr) return nullptr;

    return new AxrShaderValues{
        .BufferLinksCount = values->BufferLinksCount,
        .BufferLinks = clone(
            values->BufferLinksCount,
            values->BufferLinks
        ),
    };
}

void AxrShaderValuesRAII::destroy(AxrShaderValues_T& values) {
    if (values == nullptr) return;

    destroy(values->BufferLinksCount, values->BufferLinks);

    delete values;
    values = nullptr;
}

AxrShaderBufferLink_T* AxrShaderValuesRAII::clone(
    const uint32_t shaderBufferLinksCount,
    const AxrShaderBufferLinkConst_T* shaderBufferLinks
) {
    if (shaderBufferLinks == nullptr) return nullptr;

    const auto newShaderBufferLinks = new AxrShaderBufferLink_T[shaderBufferLinksCount]{};
    for (uint32_t i = 0; i < shaderBufferLinksCount; ++i) {
        newShaderBufferLinks[i] = clone(shaderBufferLinks[i]);
    }

    return newShaderBufferLinks;
}

void AxrShaderValuesRAII::destroy(
    uint32_t& shaderBufferLinksCount,
    AxrShaderBufferLink_T*& shaderBufferLinks
) {
    if (shaderBufferLinks == nullptr) return;

    for (uint32_t i = 0; i < shaderBufferLinksCount; ++i) {
        destroy(shaderBufferLinks[i]);
    }

    delete[] shaderBufferLinks;
    shaderBufferLinks = nullptr;
    shaderBufferLinksCount = 0;
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

    return new AxrShaderUniformBufferLink{
        .Binding = shaderBufferLink->Binding,
        .BufferName = shaderBufferLink->BufferName,
    };
}

void AxrShaderValuesRAII::destroy(AxrShaderUniformBufferLink_T& shaderBufferLink) {
    delete shaderBufferLink;
    shaderBufferLink = nullptr;
}

AxrShaderImageSamplerBufferLink_T AxrShaderValuesRAII::clone(
    const AxrShaderImageSamplerBufferLinkConst_T shaderBufferLink
) {
    if (shaderBufferLink == nullptr) return nullptr;

    return new AxrShaderImageSamplerBufferLink{
        .Binding = shaderBufferLink->Binding,
        .ImageName = shaderBufferLink->ImageName,
    };
}

void AxrShaderValuesRAII::destroy(AxrShaderImageSamplerBufferLink_T& shaderBufferLink) {
    delete shaderBufferLink;
    shaderBufferLink = nullptr;
}

bool AxrShaderValuesRAII::isValid(const AxrShaderValuesConst_T values) {
    if (values == nullptr) {
        axrLogError("Validation for shader values failed. Shader values are null.");
        return false;
    }

    return isValid(values->BufferLinksCount, values->BufferLinks);
}

bool AxrShaderValuesRAII::isValid(
    const uint32_t bufferLinksCount,
    const AxrShaderBufferLinkConst_T* bufferLinks
) {
    if (bufferLinks == nullptr) {
        // Buffer links aren't required
        return true;
    }

    std::unordered_set<uint32_t> bufferBindings;

    for (uint32_t i = 0; i < bufferLinksCount; ++i) {
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
