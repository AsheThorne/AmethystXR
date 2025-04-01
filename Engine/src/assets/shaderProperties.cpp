// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "shaderProperties.hpp"
#include "axr/logger.h"

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

AxrShaderPushConstantsBufferLayout_T axrShaderPushConstantsBufferLayoutClone(
    const AxrShaderPushConstantsBufferLayoutConst_T bufferLayout
) {
    if (bufferLayout == nullptr) return nullptr;

    return AxrShaderPropertiesRAII::clone(bufferLayout);
}

void axrShaderPushConstantsBufferLayoutDestroy(AxrShaderPushConstantsBufferLayout_T* bufferLayout) {
    if (bufferLayout == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*bufferLayout);
}
#endif


// ---- Shader Vertex Property ----

AxrShaderVertexProperty axrShaderVertexPropertyClone(const AxrShaderVertexProperty vertexProperty) {
    return AxrShaderPropertiesRAII::clone(vertexProperty);
}

void axrShaderVertexPropertyDestroy(AxrShaderVertexProperty* vertexProperty) {
    if (vertexProperty == nullptr) return;

    AxrShaderPropertiesRAII::destroy(*vertexProperty);
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

std::vector<AxrShaderUniformBufferLayoutConst_T> AxrShaderPropertiesRAII::getUniformBufferLayouts() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getUniformBufferLayouts(vertexProperties->BufferLayouts, vertexProperties->BufferLayoutsCount);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getUniformBufferLayouts(fragmentProperties->BufferLayouts, fragmentProperties->BufferLayoutsCount);
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
            return getImageSamplerBufferLayouts(vertexProperties->BufferLayouts, vertexProperties->BufferLayoutsCount);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getImageSamplerBufferLayouts(
                fragmentProperties->BufferLayouts,
                fragmentProperties->BufferLayoutsCount
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}

AxrShaderPushConstantsBufferLayoutConst_T AxrShaderPropertiesRAII::getPushConstantsBufferLayout() const {
    if (m_RawProperties == nullptr) {
        axrLogErrorLocation("Raw properties are null.");
        return {};
    }

    switch (m_RawProperties->Type) {
        case AXR_SHADER_STAGE_VERTEX: {
            const auto vertexProperties = reinterpret_cast<AxrVertexShaderPropertiesConst_T>(m_RawProperties);
            return getPushConstantsBufferLayout(vertexProperties->BufferLayouts, vertexProperties->BufferLayoutsCount);
        }
        case AXR_SHADER_STAGE_FRAGMENT: {
            const auto fragmentProperties = reinterpret_cast<AxrFragmentShaderPropertiesConst_T>(m_RawProperties);
            return getPushConstantsBufferLayout(
                fragmentProperties->BufferLayouts,
                fragmentProperties->BufferLayoutsCount
            );
        }
        case AXR_SHADER_STAGE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown shader properties type.");
            return {};
        }
    }
}

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
        .VertexProperties = clone(
            properties->VertexProperties,
            properties->VertexPropertiesCount
        ),
        .VertexPropertiesCount = properties->VertexPropertiesCount,
        .BufferLayouts = clone(
            properties->BufferLayouts,
            properties->BufferLayoutsCount
        ),
        .BufferLayoutsCount = properties->BufferLayoutsCount,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrVertexShaderProperties_T& properties) {
    if (properties == nullptr) return;

    destroy(properties->VertexProperties, properties->VertexPropertiesCount);
    destroy(properties->BufferLayouts, properties->BufferLayoutsCount);

    delete properties;
    properties = nullptr;
}

AxrFragmentShaderProperties_T AxrShaderPropertiesRAII::clone(const AxrFragmentShaderPropertiesConst_T properties) {
    if (properties == nullptr) return nullptr;

    return new AxrFragmentShaderProperties{
        .BufferLayouts = clone(
            properties->BufferLayouts,
            properties->BufferLayoutsCount
        ),
        .BufferLayoutsCount = properties->BufferLayoutsCount,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrFragmentShaderProperties_T& properties) {
    if (properties == nullptr) return;

    destroy(properties->BufferLayouts, properties->BufferLayoutsCount);

    delete properties;
    properties = nullptr;
}

AxrShaderBufferLayout_T* AxrShaderPropertiesRAII::clone(
    const AxrShaderBufferLayoutConst_T* shaderBufferLayouts,
    const uint32_t shaderBufferLayoutsCount
) {
    if (shaderBufferLayouts == nullptr) return nullptr;

    const auto newShaderBufferLayouts = new AxrShaderBufferLayout_T[shaderBufferLayoutsCount]{};
    for (uint32_t i = 0; i < shaderBufferLayoutsCount; ++i) {
        newShaderBufferLayouts[i] = clone(shaderBufferLayouts[i]);
    }

    return newShaderBufferLayouts;
}

void AxrShaderPropertiesRAII::destroy(
    AxrShaderBufferLayout_T*& shaderBufferLayouts,
    uint32_t& shaderBufferLayoutsCount
) {
    if (shaderBufferLayouts == nullptr) return;

    for (uint32_t i = 0; i < shaderBufferLayoutsCount; ++i) {
        destroy(shaderBufferLayouts[i]);
    }

    delete[] shaderBufferLayouts;
    shaderBufferLayouts = nullptr;
    shaderBufferLayoutsCount = 0;
}

AxrShaderBufferLayout_T AxrShaderPropertiesRAII::clone(const AxrShaderBufferLayoutConst_T shaderBufferLayout) {
    if (shaderBufferLayout == nullptr) return nullptr;

    switch (shaderBufferLayout->Type) {
        case AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderUniformBufferLayoutConst_T>(shaderBufferLayout)
            ));
        }
        case AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER: {
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderImageSamplerBufferLayoutConst_T>(shaderBufferLayout)
            ));
        }
        case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            return reinterpret_cast<AxrShaderBufferLayout_T>(clone(
                reinterpret_cast<AxrShaderPushConstantsBufferLayoutConst_T>(shaderBufferLayout)
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
        case AXR_SHADER_BUFFER_LAYOUT_IMAGE_SAMPLER_BUFFER: {
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderImageSamplerBufferLayout_T&>(shaderBufferLayout));
            break;
        }
        case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            destroy(reinterpret_cast<AxrShaderPushConstantsBufferLayout_T&>(shaderBufferLayout));
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

    return new AxrShaderUniformBufferLayout{
        .Binding = shaderBufferLayout->Binding,
        .BufferSize = shaderBufferLayout->BufferSize,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderUniformBufferLayout_T& shaderBufferLayout) {
    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}

AxrShaderImageSamplerBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderImageSamplerBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    return new AxrShaderImageSamplerBufferLayout{
        .Binding = shaderBufferLayout->Binding,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderImageSamplerBufferLayout_T& shaderBufferLayout) {
    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrShaderPushConstantsBufferLayout_T AxrShaderPropertiesRAII::clone(
    const AxrShaderPushConstantsBufferLayoutConst_T shaderBufferLayout
) {
    if (shaderBufferLayout == nullptr) return nullptr;

    return new AxrShaderPushConstantsBufferLayout{
        .BufferSize = shaderBufferLayout->BufferSize,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderPushConstantsBufferLayout_T& shaderBufferLayout) {
    delete shaderBufferLayout;
    shaderBufferLayout = nullptr;
}
#endif

AxrShaderVertexProperty* AxrShaderPropertiesRAII::clone(
    const AxrShaderVertexProperty* vertexProperties,
    const uint32_t vertexPropertiesCount
) {
    if (vertexProperties == nullptr) return nullptr;

    const auto newVertexProperties = new AxrShaderVertexProperty[vertexPropertiesCount]{};
    for (uint32_t i = 0; i < vertexPropertiesCount; ++i) {
        newVertexProperties[i] = clone(vertexProperties[i]);
    }

    return newVertexProperties;
}

void AxrShaderPropertiesRAII::destroy(
    AxrShaderVertexProperty*& vertexProperties,
    uint32_t& vertexPropertiesCount
) {
    if (vertexProperties == nullptr) return;

    for (uint32_t i = 0; i < vertexPropertiesCount; ++i) {
        destroy(vertexProperties[i]);
    }

    delete[] vertexProperties;
    vertexProperties = nullptr;
    vertexPropertiesCount = 0;
}

AxrShaderVertexProperty AxrShaderPropertiesRAII::clone(const AxrShaderVertexProperty vertexProperty) {
    return AxrShaderVertexProperty{
        .Type = vertexProperty.Type,
        .Binding = vertexProperty.Binding,
        .Location = vertexProperty.Location,
    };
}

void AxrShaderPropertiesRAII::destroy(AxrShaderVertexProperty& vertexProperty) {
    vertexProperty = {};
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

    // TODO: Rename vertexProperties to vertexAttributes. and all other related stuff
    return isValid(properties->VertexProperties, properties->VertexPropertiesCount) &&
        isValid(properties->BufferLayouts, properties->BufferLayoutsCount);
}

bool AxrShaderPropertiesRAII::isValid(const AxrFragmentShaderPropertiesConst_T properties) {
    if (properties == nullptr) {
        axrLogError("Validation for shader properties failed. Properties are null.");
        return false;
    }

    return isValid(properties->BufferLayouts, properties->BufferLayoutsCount);
}

bool AxrShaderPropertiesRAII::isValid(
    const AxrShaderBufferLayoutConst_T* bufferLayouts,
    const uint32_t bufferLayoutsCount
) {
    if (bufferLayouts == nullptr) {
        // Buffer layouts aren't required
        return true;
    }

    uint32_t pushConstantsCount = 0;
    std::unordered_set<uint32_t> bufferBindings;

    for (uint32_t i = 0; i < bufferLayoutsCount; ++i) {
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
            case AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER: {
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
                pushConstantsCount++;
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

    if (pushConstantsCount > 1) {
        axrLogError(
            "Validation for shader buffer layouts failed. More than 1 push constants buffer was found."
        );
        return false;
    }

    return true;
}

bool AxrShaderPropertiesRAII::isValid(
    const AxrShaderVertexProperty* vertexProperties,
    const uint32_t vertexPropertiesCount
) {
    if (vertexProperties == nullptr) {
        // Vertex properties aren't required
        return true;
    }

    std::unordered_set<AxrShaderVertexPropertyEnum> vertexPropertyTypes;
    std::unordered_map<uint32_t, std::unordered_set<uint32_t>> vertexPropertiesBindingsLocation;

    for (uint32_t i = 0; i < vertexPropertiesCount; ++i) {
        if (vertexPropertyTypes.contains(vertexProperties[i].Type)) {
            axrLogError("Validation for shader vertex properties failed. Vertex properties have a duplicate type.");
            return false;
        }
        vertexPropertyTypes.insert(vertexProperties[i].Type);

        auto foundVertexPropertyBindingIt = vertexPropertiesBindingsLocation.find(vertexProperties[i].Binding);
        if (foundVertexPropertyBindingIt != vertexPropertiesBindingsLocation.end()) {
            if (foundVertexPropertyBindingIt->second.contains(vertexProperties[i].Location)) {
                axrLogError(
                    "Validation for shader vertex properties failed. Vertex properties have a duplicate binding and location."
                );
                return false;
            }
            foundVertexPropertyBindingIt->second.insert(vertexProperties[i].Location);
        } else {
            vertexPropertiesBindingsLocation.insert(
                std::pair(
                    vertexProperties[i].Binding,
                    std::unordered_set({vertexProperties[i].Location})
                )
            );
        }
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
    // Check that there aren't uniform buffers that share the same binding but have different buffer sizes. 

    const std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts1 = properties1.
        getUniformBufferLayouts();
    const std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts2 = properties2.
        getUniformBufferLayouts();

    // Key is binding, Value is buffer size
    std::unordered_map<uint32_t, uint64_t> uniformBufferBindings;
    bool isUniformBufferValid = true;

    for (const AxrShaderUniformBufferLayoutConst_T bufferLayout : uniformBufferLayouts1) {
        isUniformBufferValid = isUniformBufferLayoutBindingValid(bufferLayout, uniformBufferBindings);
        if (!isUniformBufferValid) {
            break;
        }

        uniformBufferBindings.insert(std::pair(bufferLayout->Binding, bufferLayout->BufferSize));
    }

    if (isUniformBufferValid) {
        for (const AxrShaderUniformBufferLayoutConst_T bufferLayout : uniformBufferLayouts2) {
            isUniformBufferValid = isUniformBufferLayoutBindingValid(bufferLayout, uniformBufferBindings);
            if (!isUniformBufferValid) {
                break;
            }

            uniformBufferBindings.insert(std::pair(bufferLayout->Binding, bufferLayout->BufferSize));
        }
    }

    if (!isUniformBufferValid) {
        axrLogError(
            "Validation for shader compatibility failed. Incompatibility found within the uniform buffers."
        );
        return false;
    }

    // ---- Image Samplers ----
    // Check that image samplers don't share a binding with the uniform buffers

    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts1 = properties1.
        getImageSamplerBufferLayouts();
    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts2 = properties2.
        getImageSamplerBufferLayouts();

    AxrResult axrResult = AXR_SUCCESS;

    for (const AxrShaderImageSamplerBufferLayoutConst_T bufferLayout : imageSamplerBufferLayouts1) {
        if (uniformBufferBindings.contains(bufferLayout->Binding)) {
            axrResult = AXR_ERROR;
            break;
        }
    }

    if (AXR_SUCCEEDED(axrResult)) {
        for (const AxrShaderImageSamplerBufferLayoutConst_T bufferLayout : imageSamplerBufferLayouts2) {
            if (uniformBufferBindings.contains(bufferLayout->Binding)) {
                axrResult = AXR_ERROR;
                break;
            }
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogError(
            "Validation for shader compatibility failed. Duplicate bindings with different buffer types were found."
        );
        return false;
    }

    return true;
}

std::vector<AxrShaderUniformBufferLayoutConst_T> AxrShaderPropertiesRAII::getUniformBufferLayouts(
    const AxrShaderBufferLayoutConst_T* bufferLayouts,
    const uint32_t bufferLayoutsCount
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    std::vector<AxrShaderUniformBufferLayoutConst_T> uniformBufferLayouts;

    for (uint32_t i = 0; i < bufferLayoutsCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER) {
            uniformBufferLayouts.push_back(reinterpret_cast<AxrShaderUniformBufferLayoutConst_T>(bufferLayouts[i]));
        }
    }

    return uniformBufferLayouts;
}

std::vector<AxrShaderImageSamplerBufferLayoutConst_T> AxrShaderPropertiesRAII::getImageSamplerBufferLayouts(
    const AxrShaderBufferLayoutConst_T* bufferLayouts,
    const uint32_t bufferLayoutsCount
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    std::vector<AxrShaderImageSamplerBufferLayoutConst_T> imageSamplerBufferLayouts;

    for (uint32_t i = 0; i < bufferLayoutsCount; ++i) {
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

AxrShaderPushConstantsBufferLayoutConst_T AxrShaderPropertiesRAII::getPushConstantsBufferLayout(
    const AxrShaderBufferLayoutConst_T* bufferLayouts,
    const uint32_t bufferLayoutsCount
) {
    if (bufferLayouts == nullptr) {
        return {};
    }

    for (uint32_t i = 0; i < bufferLayoutsCount; ++i) {
        if (bufferLayouts[i] == nullptr) {
            continue;
        }

        if (bufferLayouts[i]->Type == AXR_SHADER_BUFFER_LAYOUT_PUSH_CONSTANTS_BUFFER) {
            return reinterpret_cast<AxrShaderPushConstantsBufferLayoutConst_T>(bufferLayouts[i]);
        }
    }

    return nullptr;
}

// ---- Static Private Functions ----

bool AxrShaderPropertiesRAII::isUniformBufferLayoutBindingValid(
    const AxrShaderUniformBufferLayoutConst_T uniformBufferLayout,
    std::unordered_map<uint32_t, uint64_t>& uniformBufferBindings
) {
    if (uniformBufferLayout == nullptr) return false;

    const auto foundUniformBufferBindingSize = uniformBufferBindings.find(uniformBufferLayout->Binding);
    // If a duplicate uniform buffer is found with a different size...
    if (foundUniformBufferBindingSize != uniformBufferBindings.end() &&
        foundUniformBufferBindingSize->second != uniformBufferLayout->BufferSize) {
        axrLogErrorLocation("Duplicate bindings with different buffer sizes were found.");
        return false;
    }

    return true;
}
