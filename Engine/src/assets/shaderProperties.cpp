// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shaderProperties.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

// ---- Shader Buffer Layouts ----

AxrShaderBufferLayout_T axrShaderBufferLayoutClone(const AxrShaderBufferLayoutConst_T bufferLayout) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderBufferLayoutDestroy(AxrShaderBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}

AxrShaderUniformBufferLayout_T axrShaderUniformBufferLayoutClone(
    const AxrShaderUniformBufferLayoutConst_T bufferLayout
) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderUniformBufferLayoutDestroy(AxrShaderUniformBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}

AxrShaderDynamicUniformBufferLayout_T axrShaderDynamicUniformBufferLayoutClone(
    const AxrShaderDynamicUniformBufferLayoutConst_T bufferLayout
) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderDynamicUniformBufferLayoutDestroy(AxrShaderDynamicUniformBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}


AxrShaderImageSamplerBufferLayout_T axrShaderImageSamplerBufferLayoutClone(
    const AxrShaderImageSamplerBufferLayoutConst_T bufferLayout
) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderImageSamplerBufferLayoutDestroy(AxrShaderImageSamplerBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrShaderPushConstantBufferLayout_T axrShaderPushConstantBufferLayoutClone(
    const AxrShaderPushConstantBufferLayoutConst_T bufferLayout
) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderPushConstantBufferLayoutDestroy(AxrShaderPushConstantBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}
#endif


// ---- Shader Vertex Attribute ----

AxrShaderVertexAttribute axrShaderVertexAttributeClone(const AxrShaderVertexAttribute vertexAttribute) {
    return AxrShaderPropertiesRAII::clone(vertexAttribute);
}

void axrShaderVertexAttributeDestroy(AxrShaderVertexAttribute* vertexAttribute) {
    if (vertexAttribute == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*vertexAttribute);
}

// ---- Shader Properties ----

bool axrShaderPropertiesIsValid(const AxrShaderPropertiesConst_T properties) {
    if (properties == nullptr) return false;

    return AxrShaderPropertiesRAII::isValid(properties);
}

AxrShaderProperties_T axrShaderPropertiesClone(const AxrShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(properties);
}

void axrShaderPropertiesDestroy(AxrShaderProperties_T* properties) {
    if (properties == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*properties);
}

bool axrVertexShaderPropertiesIsValid(const AxrVertexShaderPropertiesConst_T properties) {
    if (properties == nullptr) return false;

    return AxrShaderPropertiesRAII::isValid(properties);
}

AxrVertexShaderProperties_T axrVertexShaderPropertiesClone(const AxrVertexShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(properties);
}

void axrVertexShaderPropertiesDestroy(AxrVertexShaderProperties_T* properties) {
    if (properties == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*properties);
}

bool axrFragmentShaderPropertiesIsValid(const AxrFragmentShaderPropertiesConst_T properties) {
    if (properties == nullptr) return false;

    return AxrShaderPropertiesRAII::isValid(properties);
}

AxrFragmentShaderProperties_T axrFragmentShaderPropertiesClone(
    const AxrFragmentShaderPropertiesConst_T properties
) {
    if (properties == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(properties);
}

void axrFragmentShaderPropertiesDestroy(AxrFragmentShaderProperties_T* properties) {
    if (properties == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*properties);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrShaderPropertiesRAII::AxrShaderPropertiesRAII():
    m_RawProperties(nullptr) {
}

AxrShaderPropertiesRAII::AxrShaderPropertiesRAII(const AxrShaderPropertiesConst_T properties):
    m_RawProperties(nullptr) {
    if (!isValid(properties)) {
        axrLogErrorLocation("Shader properties are invalid.");
        return;
    }

    m_RawProperties = clone(properties);
}

AxrShaderPropertiesRAII::AxrShaderPropertiesRAII(const AxrShaderPropertiesRAII& src) {
    m_RawProperties = clone(src.m_RawProperties);
}

AxrShaderPropertiesRAII::AxrShaderPropertiesRAII(AxrShaderPropertiesRAII&& src) noexcept {
    m_RawProperties = src.m_RawProperties;

    src.m_RawProperties = nullptr;
}

AxrShaderPropertiesRAII::~AxrShaderPropertiesRAII() {
    cleanup();
}

AxrShaderPropertiesRAII& AxrShaderPropertiesRAII::operator=(const AxrShaderPropertiesRAII& src) {
    if (this != &src) {
        cleanup();

        m_RawProperties = clone(src.m_RawProperties);
    }

    return *this;
}

AxrShaderPropertiesRAII& AxrShaderPropertiesRAII::operator=(AxrShaderPropertiesRAII&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_RawProperties = src.m_RawProperties;

        src.m_RawProperties = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

bool AxrShaderPropertiesRAII::isValid() const {
    return isValid(m_RawProperties);
}

std::vector<AxrShaderVertexAttribute> AxrShaderPropertiesRAII::getVertexAttributes() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    if (m_RawProperties->Type != AXR_SHADER_STAGE_VERTEX) return {};

    const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
    if (vertexProperties->VertexAttributes == nullptr) return {};

    return {
        vertexProperties->VertexAttributes,
        vertexProperties->VertexAttributes + vertexProperties->VertexAttributeCount
    };
}

std::vector<AxrShaderUniformBufferLayoutConst_T> AxrShaderPropertiesRAII::getUniformBufferLayouts() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getUniformBufferLayouts(vertexProperties->BufferLayoutCount, vertexProperties->BufferLayouts);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getUniformBufferLayouts(fragmentProperties->BufferLayoutCount, fragmentProperties->BufferLayouts);
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}

std::vector<AxrShaderDynamicUniformBufferLayoutConst_T>
AxrShaderPropertiesRAII::getDynamicUniformBufferLayouts() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getDynamicUniformBufferLayouts(vertexProperties->BufferLayoutCount, vertexProperties->BufferLayouts);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getDynamicUniformBufferLayouts(
                fragmentProperties->BufferLayoutCount,
                fragmentProperties->BufferLayouts
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}

std::vector<AxrShaderImageSamplerBufferLayoutConst_T> AxrShaderPropertiesRAII::getImageSamplerBufferLayouts() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getImageSamplerBufferLayouts(vertexProperties->BufferLayoutCount, vertexProperties->BufferLayouts);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getImageSamplerBufferLayouts(
                fragmentProperties->BufferLayoutCount,
                fragmentProperties->BufferLayouts
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrShaderPushConstantBufferLayoutConst_T AxrShaderPropertiesRAII::getPushConstantBufferLayout() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getPushConstantBufferLayout(vertexProperties->BufferLayoutCount, vertexProperties->BufferLayouts);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getPushConstantBufferLayout(
                fragmentProperties->BufferLayoutCount,
                fragmentProperties->BufferLayouts
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}
#endif

void AxrShaderPropertiesRAII::cleanup() {
    destroy(m_RawProperties);
}

// ---- Public Static Functions ----

AxrShaderProperties_T AxrShaderPropertiesRAII::clone(const AxrShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    switch (properties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            return reinterpret_cast<AxrShaderProperties_T>(
                clone(reinterpret_cast<AxrVertexShaderPropertiesConst_T>(properties))
            );
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            return reinterpret_cast<AxrShaderProperties_T>(
                clone(reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(properties))
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return nullptr;
        }
    }
}

void AxrShaderPropertiesRAII::destroy(AxrShaderProperties_T& properties) {
    if (properties == nullptr) return;

    switch (properties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrVertexShaderProperties_T&>(properties));
            break;
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrFragmentShaderProperties_T&>(properties));
            break;
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            break;
        }
    }
}

AxrVertexShaderProperties_T AxrShaderPropertiesRAII::clone(const AxrVertexShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    return new AxrVertexShaderProperties{
        .VertexAttributeCount = properties->VertexAttributeCount,
        // No memory leak was found here, but we still get warnings.
        // ReSharper disable once CppDFAMemoryLeak
        .VertexAttributes = clone(
            properties->VertexAttributeCount,
            properties->VertexAttributes
        ),
        .BufferLayoutCount = properties->BufferLayoutCount,
        // No memory leak was found here, but we still get warnings.
        // ReSharper disable once CppDFAMemoryLeak
        .BufferLayouts = clone(
            properties->BufferLayoutCount,
            properties->BufferLayouts
        ),
    };
}

void AxrShaderPropertiesRAII::destroy(AxrVertexShaderProperties_T& properties) {
    if (properties == nullptr) return;

    destroy(properties->VertexAttributeCount, properties->VertexAttributes);
    destroy(properties->BufferLayoutCount, properties->BufferLayouts);

    delete properties;
    properties = nullptr;
}

AxrFragmentShaderProperties_T AxrShaderPropertiesRAII::clone(const AxrFragmentShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    return new AxrFragmentShaderProperties{
        .BufferLayoutCount = properties->BufferLayoutCount,
        // No memory leak was found here, but we still get warnings.
        // ReSharper disable once CppDFAMemoryLeak
        .BufferLayouts = clone(
            properties->BufferLayoutCount,
            properties->BufferLayouts
        ),
    };
}

void AxrShaderPropertiesRAII::destroy(AxrFragmentShaderProperties_T& properties) {
    if (properties == nullptr) return;

    destroy(properties->BufferLayoutCount, properties->BufferLayouts);

    delete properties;
    properties = nullptr;
}

AxrShaderBufferLayout_T* AxrShaderPropertiesRAII::clone(
    const uint32_t shaderBufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* shaderBufferLayouts
) {
    if (shaderBufferLayoutCount == 0 || shaderBufferLayouts == nullptr) return nullptr;

    // No memory leak was found here, but we still get warnings.
    // It was checked by increasing a count for every time this `clone` function was called, against
    // a count of every time the counterpart `destroy` function was called. And it was equal.
    // ReSharper disable CppDFAMemoryLeak
    const auto newShaderBufferLayouts = new AxrShaderBufferLayout_T[shaderBufferLayoutCount]{};
    for (uint32_t i = 0; i < shaderBufferLayoutCount; ++i) {
        newShaderBufferLayouts[i] = clone(shaderBufferLayouts[i]);
    }
    // ReSharper restore CppDFAMemoryLeak

    return newShaderBufferLayouts;
}

void AxrShaderPropertiesRAII::destroy(
    uint32_t& shaderBufferLayoutCount,
    AxrShaderBufferLayout_T*& shaderBufferLayouts
) {
    if (shaderBufferLayouts == nullptr) return;

    for (uint32_t i = 0; i < shaderBufferLayoutCount; ++i) {
        destroy(shaderBufferLayouts[i]);
    }

    delete[] shaderBufferLayouts;
    shaderBufferLayouts = nullptr;
    shaderBufferLayoutCount = 0;
}

AxrShaderBufferLayout_T AxrShaderPropertiesRAII::clone(const AxrShaderBufferLayoutConst_T shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return nullptr;

    switch (shaderBufferLayout->Type) {
        case AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderUniformBufferLayoutConst_T>(shaderBufferLayout)
            ));
        }
        case AXR_SHADER_BUFFER_LAYOUT_DYNAMIC_UNIFORM_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderDynamicUniformBufferLayoutConst_T>(shaderBufferLayout)
            ));
        }
        case AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderImageSamplerBufferLayoutConst_T>(shaderBufferLayout)
            ));
        }
        case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANT_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderPushConstantBufferLayoutConst_T>(shaderBufferLayout)
            ));
#else
            axrLogErrorLocation("Vulkan not supported.");
            return nullptr;
#endif
        }
        case AXR_SHADER_BUFFER_LAYOUT_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader buffer layout type.");
            return nullptr;
        }
    }
}

void AxrShaderPropertiesRAII::destroy(AxrShaderBufferLayout_T& shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return;

    switch (shaderBufferLayout->Type) {
        case AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderUniformBufferLayout_T&>(shaderBufferLayout));
            break;
        }
        case AXR_SHADER_BUFFER_LAYOUT_DYNAMIC_UNIFORM_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderDynamicUniformBufferLayout_T&>(shaderBufferLayout));
            break;
        }
        case AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderImageSamplerBufferLayout_T&>(shaderBufferLayout));
            break;
        }
        case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANT_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderPushConstantBufferLayout_T&>(shaderBufferLayout));
            break;
#else
            axrLogErrorLocation("Vulkan not supported.");
            break;
#endif
        }
        case AXR_SHADER_BUFFER_LAYOUT_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader buffer layout type.");
            break;
        }
    }
}

AxrShaderUniformBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderUniformBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    // No memory leak was found here, but we still get warnings.
    // It was checked by increasing a count for every time this `clone` function was called, against
    // a count of every time the counterpart `destroy` function was called. And it was equal.
    // ReSharper disable once CppDFAMemoryLeak
    return new AxrShaderUniformBufferLayout{
        .Binding = shaderBufferLayout->Binding,
        .BufferSize = shaderBufferLayout->BufferSize,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderUniformBufferLayout_T& shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return;

    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}

AxrShaderDynamicUniformBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderDynamicUniformBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    return new AxrShaderDynamicUniformBufferLayout{
        .Binding = shaderBufferLayout->Binding,
        .InstanceSize = shaderBufferLayout->InstanceSize,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderDynamicUniformBufferLayout_T& shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return;

    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}

AxrShaderImageSamplerBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderImageSamplerBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    // No memory leak was found here, but we still get warnings.
    // It was checked by increasing a count for every time this `clone` function was called, against
    // a count of every time the counterpart `destroy` function was called. And it was equal.
    // ReSharper disable once CppDFAMemoryLeak
    return new AxrShaderImageSamplerBufferLayout{
        .Binding = shaderBufferLayout->Binding,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderImageSamplerBufferLayout_T& shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return;

    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrShaderPushConstantBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderPushConstantBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    // No memory leak was found here, but we still get warnings.
    // It was checked by increasing a count for every time this `clone` function was called, against
    // a count of every time the counterpart `destroy` function was called. And it was equal.
    // ReSharper disable once CppDFAMemoryLeak
    return new AxrShaderPushConstantBufferLayout{
        .BufferSize = shaderBufferLayout->BufferSize,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderPushConstantBufferLayout_T& shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return;

    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}
#endif

AxrShaderVertexAttribute* AxrShaderPropertiesRAII::clone(
    const uint32_t vertexAttributeCount,
    const AxrShaderVertexAttribute* vertexAttributes
) {
    if (vertexAttributeCount == 0 || vertexAttributes == nullptr) return nullptr;

    // No memory leak was found here, but we still get warnings.
    // It was checked by increasing a count for every time this `clone` function was called, against
    // a count of every time the counterpart `destroy` function was called. And it was equal.
    // ReSharper disable once CppDFAMemoryLeak
    const auto newVertexAttributes = new AxrShaderVertexAttribute[vertexAttributeCount]{};
    for (uint32_t i = 0; i < vertexAttributeCount; ++i) {
        newVertexAttributes[i] = clone(vertexAttributes[i]);
    }

    return newVertexAttributes;
}

void AxrShaderPropertiesRAII::destroy(
    uint32_t& vertexAttributeCount,
    AxrShaderVertexAttribute*& vertexAttributes
) {
    if (vertexAttributes == nullptr) return;

    for (uint32_t i = 0; i < vertexAttributeCount; ++i) {
        destroy(vertexAttributes[i]);
    }

    delete[] vertexAttributes;
    vertexAttributes = nullptr;
    vertexAttributeCount = 0;
}

AxrShaderVertexAttribute AxrShaderPropertiesRAII::clone(const AxrShaderVertexAttribute vertexAttribute) {
    return AxrShaderVertexAttribute{
        .Type = vertexAttribute.Type,
        .Location = vertexAttribute.Location,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderVertexAttribute& vertexAttribute) {
    vertexAttribute = {};
}

bool AxrShaderPropertiesRAII::isValid(const AxrShaderPropertiesConst_T properties) {
    if (properties == nullptr) {
        axrLogError("Validation for shader properties failed. Properties are null.");
        return false;
    }

    switch (properties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            return isValid(reinterpret_cast<AxrVertexShaderPropertiesConst_T>(properties));
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            return isValid(reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(properties));
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return false;
        }
    }
}

bool AxrShaderPropertiesRAII::isValid(const AxrVertexShaderPropertiesConst_T properties) {
    if (properties == nullptr) {
        axrLogError("Validation for shader properties failed. Properties are null.");
        return false;
    }

    return isValid(properties->VertexAttributeCount, properties->VertexAttributes) &&
        isValid(properties->BufferLayoutCount, properties->BufferLayouts);
}

bool AxrShaderPropertiesRAII::isValid(const AxrFragmentShaderPropertiesConst_T properties) {
    if (properties == nullptr) {
        axrLogError("Validation for shader properties failed. Properties are null.");
        return false;
    }

    return isValid(properties->BufferLayoutCount, properties->BufferLayouts);
}

bool AxrShaderPropertiesRAII::isValid(
    const uint32_t bufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* bufferLayouts
) {
    if (bufferLayouts == nullptr) {
        // Buffer layouts aren't required
        return true;
    }

    uint32_t pushConstantCount = 0;
    std::unordered_set<uint32_t> bufferBindings;

    for (uint32_t i = 0; i < bufferLayoutCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            axrLogError("Validation for shader buffer layouts failed. Buffer layout at index: {0} is null.", i);
            return false;
        }

        switch (bufferLayouts[i]->Type) {
            case AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER: {
                const auto uniformBufferLayout = reinterpret_cast<AxrShaderUniformBufferLayoutConst_T>(bufferLayouts[
                    i]);
                if (bufferBindings.contains(uniformBufferLayout->Binding)) {
                    axrLogError(
                        "Validation for shader buffer layouts failed. Uniform buffer layout has a duplicate binding."
                    );
                    return false;
                }
                bufferBindings.insert(uniformBufferLayout->Binding);
                break;
            }
            case AXR_SHADER_BUFFER_LAYOUT_DYNAMIC_UNIFORM_BUFFER: {
                const auto uniformBufferLayout = reinterpret_cast<AxrShaderDynamicUniformBufferLayoutConst_T>(
                    bufferLayouts[
                        i]);
                if (bufferBindings.contains(uniformBufferLayout->Binding)) {
                    axrLogError(
                        "Validation for shader buffer layouts failed. Dynamic uniform buffer layout has a duplicate binding."
                    );
                    return false;
                }
                bufferBindings.insert(uniformBufferLayout->Binding);
                break;
            }
            case AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER: {
                const auto imageSamplerBufferLayout = reinterpret_cast<AxrShaderImageSamplerBufferLayoutConst_T>(
                    bufferLayouts[i]);
                if (bufferBindings.contains(imageSamplerBufferLayout->Binding)) {
                    axrLogError(
                        "Validation for shader buffer layouts failed. Image sampler buffer layout has a duplicate binding."
                    );
                    return false;
                }
                bufferBindings.insert(imageSamplerBufferLayout->Binding);
                break;
            }
            case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANT_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                pushConstantCount++;
                break;
#else
                axrLogErrorLocation("Vulkan not supported.");
                return false;
#endif
            }
            case AXR_SHADER_BUFFER_LAYOUT_UNDEFINED:
            default: { // NOLINT(clang-diagnostic-covered-switch-default)
                axrLogErrorLocation("Unknown Shader buffer layout.");
                return false;
            }
        }
    }

    if (pushConstantCount > 1) {
        axrLogError(
            "Validation for shader buffer layouts failed. More than 1 push constant buffer was found."
        );
        return false;
    }

    return true;
}

bool AxrShaderPropertiesRAII::isValid(
    const uint32_t vertexAttributeCount,
    const AxrShaderVertexAttribute* vertexAttributes
) {
    if (vertexAttributes == nullptr) {
        // Vertex attributes aren't required
        return true;
    }

    std::unordered_set<uint32_t> vertexAttributeLocations;

    for (uint32_t i = 0; i < vertexAttributeCount; ++i) {
        if (vertexAttributeLocations.contains(vertexAttributes[i].Location)) {
            axrLogError("Validation for shader vertex attributes failed. Vertex attributes have a duplicate location.");
            return false;
        }
        vertexAttributeLocations.insert(vertexAttributes[i].Location);
    }

    return true;
}

bool AxrShaderPropertiesRAII::areCompatible(
    const AxrShaderPropertiesRAII& properties1,
    const AxrShaderPropertiesRAII& properties2
) {
    if (properties1.m_RawProperties == nullptr) {
        axrLogError("Validation for shader properties failed. `properties1` is null.");
        return false;
    }

    if (properties2.m_RawProperties == nullptr) {
        axrLogError("Validation for shader properties failed. `properties2` is null.");
        return false;
    }

    // ---- Uniform Buffers ----
    // Find all uniform buffer bindings

    const std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts1 = properties1.
        getUniformBufferLayouts();
    const std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts2 = properties2.
        getUniformBufferLayouts();

    std::unordered_set<uint32_t> uniformBufferBindings;

    for (const AxrShaderUniformBufferLayoutConst_T bufferLayout : uniformBufferLayouts1) {
        uniformBufferBindings.insert(bufferLayout->Binding);
    }

    for (const AxrShaderUniformBufferLayoutConst_T bufferLayout : uniformBufferLayouts2) {
        uniformBufferBindings.insert(bufferLayout->Binding);
    }

    // ---- Dynamic Uniform Buffers ----
    // Check that a dynamic uniform buffer doesn't share a binding with a normal uniform buffer.

    const std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> dynamicUniformBufferLayouts1 = properties1.
        getDynamicUniformBufferLayouts();
    const std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> dynamicUniformBufferLayouts2 = properties2.
        getDynamicUniformBufferLayouts();

    std::unordered_set<uint32_t> dynamicUniformBufferBindings;
    const char* dynamicUniformBufferError = nullptr;

    for (const AxrShaderDynamicUniformBufferLayoutConst_T bufferLayout : dynamicUniformBufferLayouts1) {
        if (uniformBufferBindings.contains(bufferLayout->Binding)) {
            dynamicUniformBufferError =
                "Duplicate binding found for dynamic uniform buffer and standard uniform buffer.";
            break;
        }

        dynamicUniformBufferBindings.insert(bufferLayout->Binding);
    }

    if (dynamicUniformBufferError == nullptr) {
        for (const AxrShaderDynamicUniformBufferLayoutConst_T bufferLayout : dynamicUniformBufferLayouts2) {
            if (uniformBufferBindings.contains(bufferLayout->Binding)) {
                dynamicUniformBufferError =
                    "Duplicate binding found for dynamic uniform buffer and standard uniform buffer.";
                break;
            }

            dynamicUniformBufferBindings.insert(bufferLayout->Binding);
        }
    }

    if (dynamicUniformBufferError != nullptr) {
        axrLogError(
            "Validation for shader compatibility failed. {0}.",
            dynamicUniformBufferError
        );
        return false;
    }

    // ---- Image Samplers ----
    // Check that image samplers don't share a binding with the uniform buffers

    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts1 = properties1.
        getImageSamplerBufferLayouts();
    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts2 = properties2.
        getImageSamplerBufferLayouts();

    const char* imageSamplerBufferError = nullptr;

    for (const AxrShaderImageSamplerBufferLayoutConst_T bufferLayout : imageSamplerBufferLayouts1) {
        if (uniformBufferBindings.contains(bufferLayout->Binding)) {
            imageSamplerBufferError =
                "Duplicate binding found for image sampler buffer and standard uniform buffer.";
            break;
        }

        if (dynamicUniformBufferBindings.contains(bufferLayout->Binding)) {
            imageSamplerBufferError =
                "Duplicate binding found for image sampler buffer and dynamic uniform buffer.";
            break;
        }
    }

    if (imageSamplerBufferError == nullptr) {
        for (const AxrShaderImageSamplerBufferLayoutConst_T bufferLayout : imageSamplerBufferLayouts2) {
            if (uniformBufferBindings.contains(bufferLayout->Binding)) {
                imageSamplerBufferError =
                    "Duplicate binding found for image sampler buffer and standard uniform buffer.";
                break;
            }

            if (dynamicUniformBufferBindings.contains(bufferLayout->Binding)) {
                imageSamplerBufferError =
                    "Duplicate binding found for image sampler buffer and dynamic uniform buffer.";
                break;
            }
        }
    }

    if (imageSamplerBufferError != nullptr) {
        axrLogError(
            "Validation for shader compatibility failed. {0}.",
            imageSamplerBufferError
        );
        return false;
    }

    return true;
}

std::vector<AxrShaderUniformBufferLayoutConst_T> AxrShaderPropertiesRAII::getUniformBufferLayouts(
    const uint32_t bufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* bufferLayouts
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts;

    for (uint32_t i = 0; i < bufferLayoutCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER) {
            uniformBufferLayouts.push_back(reinterpret_cast<AxrShaderUniformBufferLayoutConst_T>(bufferLayouts[i]));
        }
    }

    return uniformBufferLayouts;
}

std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> AxrShaderPropertiesRAII::getDynamicUniformBufferLayouts(
    const uint32_t bufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* bufferLayouts
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> dynamicUniformBufferLayouts;

    for (uint32_t i = 0; i < bufferLayoutCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_DYNAMIC_UNIFORM_BUFFER) {
            dynamicUniformBufferLayouts.push_back(
                reinterpret_cast<AxrShaderDynamicUniformBufferLayoutConst_T>(bufferLayouts[i])
            );
        }
    }

    return dynamicUniformBufferLayouts;
}

std::vector<AxrShaderImageSamplerBufferLayoutConst_T> AxrShaderPropertiesRAII::getImageSamplerBufferLayouts(
    const uint32_t bufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* bufferLayouts
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts;

    for (uint32_t i = 0; i < bufferLayoutCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER) {
            imageSamplerBufferLayouts.push_back(
                reinterpret_cast<AxrShaderImageSamplerBufferLayoutConst_T>(bufferLayouts[i])
            );
        }
    }

    return imageSamplerBufferLayouts;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrShaderPushConstantBufferLayoutConst_T AxrShaderPropertiesRAII::getPushConstantBufferLayout(
    const uint32_t bufferLayoutCount,
    const AxrShaderBufferLayoutConst_T* bufferLayouts
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    for (uint32_t i = 0; i < bufferLayoutCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANT_BUFFER) {
            return reinterpret_cast<AxrShaderPushConstantBufferLayoutConst_T>(bufferLayouts[i]);
        }
    }

    return nullptr;
}
#endif
