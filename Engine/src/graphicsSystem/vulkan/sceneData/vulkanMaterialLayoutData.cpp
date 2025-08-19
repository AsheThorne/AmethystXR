#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanMaterialLayoutData.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanMaterialLayoutData::AxrVulkanMaterialLayoutData():
    m_VertexShaderHandle(nullptr),
    m_FragmentShaderHandle(nullptr),
    m_Device(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr),
    m_DescriptorSetLayout(VK_NULL_HANDLE),
    m_PipelineLayout(VK_NULL_HANDLE),
    m_PushConstantShaderStage(0) {
}

AxrVulkanMaterialLayoutData::AxrVulkanMaterialLayoutData(const Config& config):
    m_Name(config.Name),
    m_VertexShaderHandle(config.VertexShaderHandle),
    m_FragmentShaderHandle(config.FragmentShaderHandle),
    m_Device(config.Device),
    m_DispatchHandle(config.DispatchHandle),
    m_DescriptorSetLayout(VK_NULL_HANDLE),
    m_PipelineLayout(VK_NULL_HANDLE),
    m_PushConstantShaderStage(0) {
}

AxrVulkanMaterialLayoutData::AxrVulkanMaterialLayoutData(AxrVulkanMaterialLayoutData&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_DescriptorSetLayoutBindings = std::move(src.m_DescriptorSetLayoutBindings);

    m_VertexShaderHandle = src.m_VertexShaderHandle;
    m_FragmentShaderHandle = src.m_FragmentShaderHandle;
    m_Device = src.m_Device;
    m_DispatchHandle = src.m_DispatchHandle;
    m_DescriptorSetLayout = src.m_DescriptorSetLayout;
    m_PipelineLayout = src.m_PipelineLayout;
    m_PushConstantShaderStage = src.m_PushConstantShaderStage;

    src.m_VertexShaderHandle = nullptr;
    src.m_FragmentShaderHandle = nullptr;
    src.m_Device = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
    src.m_DescriptorSetLayout = VK_NULL_HANDLE;
    src.m_PipelineLayout = VK_NULL_HANDLE;
    src.m_PushConstantShaderStage = static_cast<vk::ShaderStageFlagBits>(0);
}

AxrVulkanMaterialLayoutData::~AxrVulkanMaterialLayoutData() {
    cleanup();
}

AxrVulkanMaterialLayoutData& AxrVulkanMaterialLayoutData::operator=(AxrVulkanMaterialLayoutData&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_DescriptorSetLayoutBindings = std::move(src.m_DescriptorSetLayoutBindings);

        m_VertexShaderHandle = src.m_VertexShaderHandle;
        m_FragmentShaderHandle = src.m_FragmentShaderHandle;
        m_Device = src.m_Device;
        m_DispatchHandle = src.m_DispatchHandle;
        m_DescriptorSetLayout = src.m_DescriptorSetLayout;
        m_PipelineLayout = src.m_PipelineLayout;
        m_PushConstantShaderStage = src.m_PushConstantShaderStage;

        src.m_VertexShaderHandle = nullptr;
        src.m_FragmentShaderHandle = nullptr;
        src.m_Device = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
        src.m_DescriptorSetLayout = VK_NULL_HANDLE;
        src.m_PipelineLayout = VK_NULL_HANDLE;
        src.m_PushConstantShaderStage = static_cast<vk::ShaderStageFlagBits>(0);
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanMaterialLayoutData::getName() const {
    return m_Name;
}

const vk::PipelineLayout& AxrVulkanMaterialLayoutData::getPipelineLayout() const {
    return m_PipelineLayout;
}

const vk::ShaderStageFlags& AxrVulkanMaterialLayoutData::getPushConstantShaderStages() const {
    return m_PushConstantShaderStage;
}

const vk::DescriptorSetLayout& AxrVulkanMaterialLayoutData::getDescriptorSetLayout() const {
    return m_DescriptorSetLayout;
}

const std::vector<vk::DescriptorSetLayoutBinding>& AxrVulkanMaterialLayoutData::getDescriptorSetLayoutBindings() const {
    return m_DescriptorSetLayoutBindings;
}

const AxrShader* AxrVulkanMaterialLayoutData::getVertexShaderHandle() const {
    return m_VertexShaderHandle;
}

const AxrShader* AxrVulkanMaterialLayoutData::getFragmentShaderHandle() const {
    return m_FragmentShaderHandle;
}

bool AxrVulkanMaterialLayoutData::doesDataExist() const {
    return !m_DescriptorSetLayoutBindings.empty() ||
        m_DescriptorSetLayout != VK_NULL_HANDLE ||
        m_PipelineLayout != VK_NULL_HANDLE;
}

AxrResult AxrVulkanMaterialLayoutData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesDataExist()) {
        axrLogErrorLocation("Material layout data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = validateMaterialLayoutShaders();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to validate material layout shaders.");
        destroyData();
        return axrResult;
    }

    axrResult = createDescriptorSetLayout();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create descriptor set layout.");
        destroyData();
        return axrResult;
    }

    axrResult = createPipelineLayout();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create pipeline layout.");
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutData::destroyData() {
    destroyDescriptorSetLayout();
    destroyPipelineLayout();
}

// ---- Private Functions ----

void AxrVulkanMaterialLayoutData::cleanup() {
    destroyData();

    m_Name.clear();
    m_VertexShaderHandle = nullptr;
    m_FragmentShaderHandle = nullptr;
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanMaterialLayoutData::validateMaterialLayoutShaders() const {
    if (m_VertexShaderHandle == nullptr) {
        axrLogErrorLocation("Vertex shader handle is null.");
        return AXR_ERROR;
    }

    if (m_FragmentShaderHandle == nullptr) {
        axrLogErrorLocation("Fragment shader handle is null.");
        return AXR_ERROR;
    }

    if (!m_VertexShaderHandle->isValid()) {
        axrLogErrorLocation("Validation failed for shader named: {0}.", m_VertexShaderHandle->getName());
        return AXR_ERROR;
    }

    if (!m_FragmentShaderHandle->isValid()) {
        axrLogErrorLocation("Validation failed for shader named: {0}.", m_FragmentShaderHandle->getName());
        return AXR_ERROR;
    }

    if (!AxrShader::areCompatible(*m_VertexShaderHandle, *m_FragmentShaderHandle)) {
        axrLogErrorLocation(
            "Validation failed for shader compatibility between: {0} and {1}.",
            m_VertexShaderHandle->getName(),
            m_FragmentShaderHandle->getName()
        );
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanMaterialLayoutData::createDescriptorSetLayout() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
        axrLogErrorLocation("Descriptor set layout already exists.");
        return AXR_ERROR;
    }

    if (!m_DescriptorSetLayoutBindings.empty()) {
        axrLogErrorLocation("Descriptor set layout bindings already exist.");
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

    // ---- Uniform buffer bindings ----

    const std::vector<AxrShaderUniformBufferLayoutConst_T> vertexUniformBufferLayouts =
        m_VertexShaderHandle->getProperties().getUniformBufferLayouts();
    const std::vector<AxrShaderUniformBufferLayoutConst_T> fragmentUniformBufferLayouts =
        m_FragmentShaderHandle->getProperties().getUniformBufferLayouts();

    for (const AxrShaderUniformBufferLayoutConst_T uniformBufferLayout : vertexUniformBufferLayouts) {
        if (uniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            uniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBuffer,
            vk::ShaderStageFlagBits::eVertex,
            m_DescriptorSetLayoutBindings
        );
    }

    for (const AxrShaderUniformBufferLayoutConst_T uniformBufferLayout : fragmentUniformBufferLayouts) {
        if (uniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            uniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBuffer,
            vk::ShaderStageFlagBits::eFragment,
            m_DescriptorSetLayoutBindings
        );
    }

    // ---- Dynamic Uniform buffer bindings ----

    const std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> vertexDynamicUniformBufferLayouts =
        m_VertexShaderHandle->getProperties().getDynamicUniformBufferLayouts();
    const std::vector<AxrShaderDynamicUniformBufferLayoutConst_T> fragmentDynamicUniformBufferLayouts =
        m_FragmentShaderHandle->getProperties().getDynamicUniformBufferLayouts();

    for (const AxrShaderDynamicUniformBufferLayoutConst_T dynamicUniformBufferLayout :
         vertexDynamicUniformBufferLayouts) {
        if (dynamicUniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            dynamicUniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBufferDynamic,
            vk::ShaderStageFlagBits::eVertex,
            m_DescriptorSetLayoutBindings
        );
    }

    for (const AxrShaderDynamicUniformBufferLayoutConst_T dynamicUniformBufferLayout :
         fragmentDynamicUniformBufferLayouts) {
        if (dynamicUniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            dynamicUniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBufferDynamic,
            vk::ShaderStageFlagBits::eFragment,
            m_DescriptorSetLayoutBindings
        );
    }

    // ---- Image sampler bindings ----

    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> vertexImageSamplerBufferLayouts =
        m_VertexShaderHandle->getProperties().getImageSamplerBufferLayouts();
    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> fragmentImageSamplerBufferLayouts =
        m_FragmentShaderHandle->getProperties().getImageSamplerBufferLayouts();

    for (const AxrShaderImageSamplerBufferLayoutConst_T imageSamplerBufferLayout : vertexImageSamplerBufferLayouts) {
        if (imageSamplerBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            imageSamplerBufferLayout->Binding,
            vk::DescriptorType::eCombinedImageSampler,
            vk::ShaderStageFlagBits::eVertex,
            m_DescriptorSetLayoutBindings
        );
    }

    for (const AxrShaderImageSamplerBufferLayoutConst_T imageSamplerBufferLayout : fragmentImageSamplerBufferLayouts) {
        if (imageSamplerBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            imageSamplerBufferLayout->Binding,
            vk::DescriptorType::eCombinedImageSampler,
            vk::ShaderStageFlagBits::eFragment,
            m_DescriptorSetLayoutBindings
        );
    }

    // ---- Create descriptor set layout ----

    const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
        {},
        static_cast<uint32_t>(m_DescriptorSetLayoutBindings.size()),
        m_DescriptorSetLayoutBindings.data()
    );

    const vk::Result vkResult = m_Device.createDescriptorSetLayout(
        &descriptorSetLayoutCreateInfo,
        nullptr,
        &m_DescriptorSetLayout,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.createDescriptorSetLayout");
    if (VK_FAILED(vkResult)) {
        destroyDescriptorSetLayout();
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutData::destroyDescriptorSetLayout() {
    if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
        m_Device.destroyDescriptorSetLayout(m_DescriptorSetLayout, nullptr, *m_DispatchHandle);
        m_DescriptorSetLayout = VK_NULL_HANDLE;
    }
    m_DescriptorSetLayoutBindings.clear();
}

void AxrVulkanMaterialLayoutData::addDescriptorSetLayoutItem(
    const uint32_t binding,
    const vk::DescriptorType descriptorType,
    const vk::ShaderStageFlagBits stageFlag,
    std::vector<vk::DescriptorSetLayoutBinding>& bindings
) const {
    // Check if the binding has already been added, and if it has, just include the shader stage
    for (vk::DescriptorSetLayoutBinding& layout : bindings) {
        if (layout.binding == binding) {
            layout.stageFlags |= stageFlag;
            return;
        }
    }

    const vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding(
        binding,
        descriptorType,
        1,
        stageFlag
    );
    bindings.push_back(descriptorSetLayoutBinding);
}

AxrResult AxrVulkanMaterialLayoutData::createPipelineLayout() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PipelineLayout != VK_NULL_HANDLE) {
        axrLogErrorLocation("Pipeline layout already exists.");
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

    if (m_DescriptorSetLayout == VK_NULL_HANDLE) {
        axrLogErrorLocation("Descriptor set layout is null.");
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

    std::vector<vk::PushConstantRange> pushConstantRanges;

    const AxrShaderPushConstantBufferLayoutConst_T vertexPushConstantBufferLayout =
        m_VertexShaderHandle->getProperties().getPushConstantBufferLayout();
    if (vertexPushConstantBufferLayout != nullptr) {
        pushConstantRanges.push_back(
            {
                vk::PushConstantRange(
                    vk::ShaderStageFlagBits::eVertex,
                    0,
                    vertexPushConstantBufferLayout->BufferSize
                )
            }
        );
    }

    const AxrShaderPushConstantBufferLayoutConst_T fragmentPushConstantBufferLayout =
        m_FragmentShaderHandle->getProperties().getPushConstantBufferLayout();
    if (fragmentPushConstantBufferLayout != nullptr) {
        pushConstantRanges.push_back(
            {
                vk::PushConstantRange(
                    vk::ShaderStageFlagBits::eFragment,
                    0,
                    fragmentPushConstantBufferLayout->BufferSize
                )
            }
        );
    }

    for (const vk::PushConstantRange& pushConstantRange : pushConstantRanges) {
        m_PushConstantShaderStage |= pushConstantRange.stageFlags;
    }

    const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo(
        {},
        1,
        &m_DescriptorSetLayout,
        static_cast<uint32_t>(pushConstantRanges.size()),
        pushConstantRanges.data()
    );

    const vk::Result vkResult = m_Device.createPipelineLayout(
        &pipelineLayoutCreateInfo,
        nullptr,
        &m_PipelineLayout,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.createPipelineLayout");
    if (VK_FAILED(vkResult)) {
        destroyPipelineLayout();
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutData::destroyPipelineLayout() {
    if (m_PipelineLayout == VK_NULL_HANDLE) return;

    m_Device.destroyPipelineLayout(m_PipelineLayout, nullptr, *m_DispatchHandle);
    m_PipelineLayout = VK_NULL_HANDLE;
    m_PushConstantShaderStage = static_cast<vk::ShaderStageFlagBits>(0);
}

#endif
