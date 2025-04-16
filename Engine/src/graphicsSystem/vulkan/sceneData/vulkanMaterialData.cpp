#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanMaterialData.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanMaterialData::AxrVulkanMaterialData():
    m_VertexShaderHandle(nullptr),
    m_FragmentShaderHandle(nullptr),
    m_PipelineLayout(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr),
    m_WindowPipeline(VK_NULL_HANDLE) {
}

AxrVulkanMaterialData::AxrVulkanMaterialData(const Config& config):
    m_Name(config.Name),
    m_VertexShaderHandle(config.VertexShaderHandle),
    m_FragmentShaderHandle(config.FragmentShaderHandle),
    m_PipelineLayout(config.PipelineLayout),
    m_Device(config.Device),
    m_DispatchHandle(config.DispatchHandle),
    m_WindowPipeline(VK_NULL_HANDLE) {
}

AxrVulkanMaterialData::AxrVulkanMaterialData(AxrVulkanMaterialData&& src) noexcept {
    m_Name = std::move(src.m_Name);

    m_VertexShaderHandle = src.m_VertexShaderHandle;
    m_FragmentShaderHandle = src.m_FragmentShaderHandle;
    m_PipelineLayout = src.m_PipelineLayout;
    m_Device = src.m_Device;
    m_DispatchHandle = src.m_DispatchHandle;
    m_WindowPipeline = src.m_WindowPipeline;

    src.m_VertexShaderHandle = nullptr;
    src.m_FragmentShaderHandle = nullptr;
    src.m_PipelineLayout = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
    src.m_WindowPipeline = VK_NULL_HANDLE;
}

AxrVulkanMaterialData::~AxrVulkanMaterialData() {
    cleanup();
}

AxrVulkanMaterialData& AxrVulkanMaterialData::operator=(AxrVulkanMaterialData&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);

        m_VertexShaderHandle = src.m_VertexShaderHandle;
        m_FragmentShaderHandle = src.m_FragmentShaderHandle;
        m_PipelineLayout = src.m_PipelineLayout;
        m_Device = src.m_Device;
        m_DispatchHandle = src.m_DispatchHandle;
        m_WindowPipeline = src.m_WindowPipeline;

        src.m_VertexShaderHandle = nullptr;
        src.m_FragmentShaderHandle = nullptr;
        src.m_PipelineLayout = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
        src.m_WindowPipeline = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanMaterialData::getName() const {
    return m_Name;
}

const vk::PipelineLayout& AxrVulkanMaterialData::getPipelineLayout() const {
    return m_PipelineLayout;
}

const vk::Pipeline& AxrVulkanMaterialData::getWindowPipeline() const {
    return m_WindowPipeline;
}

bool AxrVulkanMaterialData::doesDataExist() const {
    // NOTE: When we have general data, check for it here
    return true;
}

bool AxrVulkanMaterialData::doesWindowDataExist() const {
    return m_WindowPipeline != VK_NULL_HANDLE;
}

AxrResult AxrVulkanMaterialData::createData() {
    // NOTE: We don't have general data yet
    return AXR_SUCCESS;
}

void AxrVulkanMaterialData::destroyData() {
    destroyWindowData();

    // NOTE: Destroy general data here when we have some
}

AxrResult AxrVulkanMaterialData::createWindowData(
    const vk::RenderPass renderPass
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesWindowDataExist()) {
        axrLogErrorLocation("Material window data already exists.");
        return AXR_ERROR;
    }

    if (!doesDataExist()) {
        axrLogErrorLocation("Material data is missing.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createPipeline(renderPass, m_WindowPipeline);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create pipeline.");
        destroyWindowData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialData::destroyWindowData() {
    destroyPipeline(m_WindowPipeline);
}

// ---- Private Functions ----

void AxrVulkanMaterialData::cleanup() {
    destroyWindowData();
    destroyData();

    m_Name.clear();
    m_VertexShaderHandle = nullptr;
    m_FragmentShaderHandle = nullptr;
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanMaterialData::createPipeline(
    const vk::RenderPass renderPass,
    vk::Pipeline& pipeline
) const {
    // TODO: When creating a pipeline (For OpenXR or Window)
    //  Check if any pipeline already exists
    //  Check if the existing pipeline was created with the same parameters as what is passed in here.
    //  (Check the render pass. Not the output pipeline).
    //  If an existing pipeline was already created with the same parameters, then just reuse it. Don't create a new one
    //  -
    //  To make this worth it, it needs to be possible for OpenXR and the window to use the same render pass.
    //  Which will need to be checked similar to this function. By checking the parameters during it's creation.
    //  -
    //  Also check if there are other objects that can be reused too.

    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (pipeline != VK_NULL_HANDLE) {
        axrLogErrorLocation("Pipeline already exists.");
        return AXR_ERROR;
    }

    if (m_PipelineLayout == VK_NULL_HANDLE) {
        axrLogErrorLocation("Pipeline layout is null.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return AXR_ERROR;
    }

    if (m_VertexShaderHandle == nullptr) {
        axrLogErrorLocation("Vertex shader handle is null.");
        return AXR_ERROR;
    }

    if (m_FragmentShaderHandle == nullptr) {
        axrLogErrorLocation("Fragment shader handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    // ---- Shader Stage ----

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStageCreateInfos(2);
    std::vector<vk::ShaderModule> shaderModules(2);

    if (!m_VertexShaderHandle->isLoaded()) {
        axrResult = m_VertexShaderHandle->loadFile(AXR_GRAPHICS_API_VULKAN);
        if (AXR_FAILED(axrResult)) {
            cleanupPipelineCreationData(shaderModules);
            return axrResult;
        }
    }

    axrResult = createShaderModule(m_VertexShaderHandle->getFileData(), shaderModules[0]);
    if (AXR_FAILED(axrResult)) {
        cleanupPipelineCreationData(shaderModules);
        return axrResult;
    }

    shaderStageCreateInfos[0] = vk::PipelineShaderStageCreateInfo(
        {},
        vk::ShaderStageFlagBits::eVertex,
        shaderModules[0],
        "main"
    );

    if (!m_FragmentShaderHandle->isLoaded()) {
        axrResult = m_FragmentShaderHandle->loadFile(AXR_GRAPHICS_API_VULKAN);
        if (AXR_FAILED(axrResult)) {
            cleanupPipelineCreationData(shaderModules);
            return axrResult;
        }
    }

    axrResult = createShaderModule(m_FragmentShaderHandle->getFileData(), shaderModules[1]);
    if (AXR_FAILED(axrResult)) {
        cleanupPipelineCreationData(shaderModules);
        return axrResult;
    }

    shaderStageCreateInfos[1] = vk::PipelineShaderStageCreateInfo(
        {},
        vk::ShaderStageFlagBits::eFragment,
        shaderModules[1],
        "main"
    );

    // ---- Vertex Input State ----

    std::vector<AxrShaderVertexAttribute> vertexAttributes = m_VertexShaderHandle->getProperties().
        getVertexAttributes();

    std::unordered_set<uint32_t> vertexAttributeBindings;
    for (const AxrShaderVertexAttribute& vertexAttribute : vertexAttributes) {
        vertexAttributeBindings.insert(vertexAttribute.Binding);
    }

    std::vector<vk::VertexInputBindingDescription> vertexBindingDescriptions(vertexAttributeBindings.size());
    for (uint32_t vertexAttributeBindingIndex = 0; uint32_t vertexAttributeBinding : vertexAttributeBindings) {
        vertexBindingDescriptions[vertexAttributeBindingIndex] = vk::VertexInputBindingDescription(
            vertexAttributeBinding,
            sizeof(AxrVertex),
            vk::VertexInputRate::eVertex
        );
        ++vertexAttributeBindingIndex;
    }

    std::vector<vk::VertexInputAttributeDescription> vertexAttributeDescriptions(vertexAttributes.size());
    for (size_t i = 0; i < vertexAttributeDescriptions.size(); ++i) {
        vertexAttributeDescriptions[i] = vk::VertexInputAttributeDescription(
            vertexAttributes[i].Location,
            vertexAttributes[i].Binding,
            getVertexAttributeFormat(vertexAttributes[i].Type),
            getVertexAttributeOffset(vertexAttributes[i].Type)
        );
    }

    const vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(
        {},
        static_cast<uint32_t>(vertexBindingDescriptions.size()),
        vertexBindingDescriptions.data(),
        static_cast<uint32_t>(vertexAttributeDescriptions.size()),
        vertexAttributeDescriptions.data()
    );

    // ---- Input Assembly State ----

    constexpr vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo(
        {},
        vk::PrimitiveTopology::eTriangleList,
        vk::False
    );

    // ---- Viewport State ----

    constexpr vk::PipelineViewportStateCreateInfo viewportStateCreateInfo(
        {},
        1,
        {},
        1,
        {}
    );

    // ---- Rasterization State ----

    constexpr vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(
        {},
        vk::False,
        vk::False,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        vk::False,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    // ---- Multisample State ----

    // TODO: Implement multisampling
    constexpr vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo(
        {},
        vk::SampleCountFlagBits::e1,
        vk::False,
        1.0f,
        nullptr,
        vk::False,
        vk::False
    );

    // ---- Depth Stencil State ----

    constexpr vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo(
        {},
        vk::True,
        vk::True,
        vk::CompareOp::eLess,
        vk::False,
        vk::False,
        {},
        {},
        0.0f,
        1.0f
    );

    // ---- Color Blend State ----

    constexpr vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        vk::False,
        vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusSrcAlpha,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA
    );

    constexpr std::array blendConstants{
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };

    const vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo(
        {},
        vk::False,
        vk::LogicOp::eCopy,
        1,
        &colorBlendAttachment,
        blendConstants
    );

    // ---- Dynamic State ----

    constexpr std::array dynamicStates{
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };

    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo(
        {},
        static_cast<uint32_t>(dynamicStates.size()),
        dynamicStates.data()
    );

    // ---- Pipeline Creation ----

    const vk::GraphicsPipelineCreateInfo pipelineCreateInfo(
        {},
        static_cast<uint32_t>(shaderStageCreateInfos.size()),
        shaderStageCreateInfos.data(),
        &vertexInputStateCreateInfo,
        &inputAssemblyStateCreateInfo,
        {},
        &viewportStateCreateInfo,
        &rasterizationStateCreateInfo,
        &multisampleStateCreateInfo,
        &depthStencilStateCreateInfo,
        &colorBlendStateCreateInfo,
        &dynamicStateCreateInfo,
        m_PipelineLayout,
        renderPass,
        0,
        VK_NULL_HANDLE,
        -1
    );

    const vk::Result vkResult = m_Device.createGraphicsPipelines(
        VK_NULL_HANDLE,
        1,
        &pipelineCreateInfo,
        nullptr,
        &pipeline,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.createGraphicsPipelines");

    cleanupPipelineCreationData(shaderModules);

    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialData::destroyPipeline(vk::Pipeline& pipeline) {
    if (pipeline == VK_NULL_HANDLE) return;

    m_Device.destroyPipeline(pipeline, nullptr, *m_DispatchHandle);
    pipeline = VK_NULL_HANDLE;
}

void AxrVulkanMaterialData::cleanupPipelineCreationData(std::vector<vk::ShaderModule>& shaderModules) const {
    for (vk::ShaderModule& shaderModule : shaderModules) {
        destroyShaderModule(shaderModule);
    }
    shaderModules.clear();
}

AxrResult AxrVulkanMaterialData::createShaderModule(
    const std::vector<char>& shaderFileData,
    vk::ShaderModule& shaderModule
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::ShaderModuleCreateInfo shaderModuleCreateInfo(
        {},
        shaderFileData.size(),
        reinterpret_cast<const uint32_t*>(shaderFileData.data())
    );

    const vk::Result vkResult = m_Device.createShaderModule(
        &shaderModuleCreateInfo,
        nullptr,
        &shaderModule,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.createShaderModule");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialData::destroyShaderModule(vk::ShaderModule& shaderModule) const {
    if (shaderModule == VK_NULL_HANDLE) return;

    m_Device.destroyShaderModule(shaderModule, nullptr, *m_DispatchHandle);
    shaderModule = VK_NULL_HANDLE;
}

vk::Format AxrVulkanMaterialData::getVertexAttributeFormat(const AxrShaderVertexAttributeEnum vertexAttribute) const {
    switch (vertexAttribute) {
        case AXR_SHADER_VERTEX_ATTRIBUTE_POSITION:
        case AXR_SHADER_VERTEX_ATTRIBUTE_COLOR: {
            return vk::Format::eR32G32B32Sfloat;
        }
        case AXR_SHADER_VERTEX_ATTRIBUTE_TEX_COORDS: {
            return vk::Format::eR32G32Sfloat;
        }
        case AXR_SHADER_VERTEX_ATTRIBUTE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return vk::Format::eUndefined;
        }
    }
}

uint32_t AxrVulkanMaterialData::getVertexAttributeOffset(const AxrShaderVertexAttributeEnum vertexAttribute) const {
    switch (vertexAttribute) {
        case AXR_SHADER_VERTEX_ATTRIBUTE_POSITION: {
            return offsetof(AxrVertex, Position);
        }
        case AXR_SHADER_VERTEX_ATTRIBUTE_COLOR: {
            return offsetof(AxrVertex, Color);
        }
        case AXR_SHADER_VERTEX_ATTRIBUTE_TEX_COORDS: {
            return offsetof(AxrVertex, TexCoords);
        }
        case AXR_SHADER_VERTEX_ATTRIBUTE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return 0;
        }
    }
}

#endif
