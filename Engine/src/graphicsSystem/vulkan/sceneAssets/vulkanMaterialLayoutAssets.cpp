#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanMaterialLayoutAssets.hpp"
#include "axr/logger.h"
#include "../vulkanUtils.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanMaterialLayoutAssets::AxrVulkanMaterialLayoutAssets():
    m_Device(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr),
    m_DescriptorSetLayout(VK_NULL_HANDLE),
    m_PipelineLayout(VK_NULL_HANDLE),
    m_WindowPipeline(VK_NULL_HANDLE) {
}

AxrVulkanMaterialLayoutAssets::AxrVulkanMaterialLayoutAssets(const Config& config):
    m_Name(config.Name),
    m_VertexShaderName(config.VertexShaderName),
    m_FragmentShaderName(config.FragmentShaderName),
    m_Device(config.Device),
    m_DispatchHandle(config.DispatchHandle),
    m_DescriptorSetLayout(VK_NULL_HANDLE),
    m_PipelineLayout(VK_NULL_HANDLE),
    m_WindowPipeline(VK_NULL_HANDLE) {
}

AxrVulkanMaterialLayoutAssets::AxrVulkanMaterialLayoutAssets(AxrVulkanMaterialLayoutAssets&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_VertexShaderName = std::move(src.m_VertexShaderName);
    m_FragmentShaderName = std::move(src.m_FragmentShaderName);
    m_DescriptorSetItemLocations = std::move(src.m_DescriptorSetItemLocations);

    m_Device = src.m_Device;
    m_DispatchHandle = src.m_DispatchHandle;
    m_DescriptorSetLayout = src.m_DescriptorSetLayout;
    m_PipelineLayout = src.m_PipelineLayout;
    m_WindowPipeline = src.m_WindowPipeline;

    src.m_Device = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
    src.m_DescriptorSetLayout = VK_NULL_HANDLE;
    src.m_PipelineLayout = VK_NULL_HANDLE;
    src.m_WindowPipeline = VK_NULL_HANDLE;
}

AxrVulkanMaterialLayoutAssets::~AxrVulkanMaterialLayoutAssets() {
    cleanup();
}

AxrVulkanMaterialLayoutAssets& AxrVulkanMaterialLayoutAssets::operator=(AxrVulkanMaterialLayoutAssets&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_VertexShaderName = std::move(src.m_VertexShaderName);
        m_FragmentShaderName = std::move(src.m_FragmentShaderName);
        m_DescriptorSetItemLocations = std::move(src.m_DescriptorSetItemLocations);

        m_Device = src.m_Device;
        m_DispatchHandle = src.m_DispatchHandle;
        m_DescriptorSetLayout = src.m_DescriptorSetLayout;
        m_PipelineLayout = src.m_PipelineLayout;
        m_WindowPipeline = src.m_WindowPipeline;

        src.m_Device = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
        src.m_DescriptorSetLayout = VK_NULL_HANDLE;
        src.m_PipelineLayout = VK_NULL_HANDLE;
        src.m_WindowPipeline = VK_NULL_HANDLE;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanMaterialLayoutAssets::getName() const {
    return m_Name;
}

const std::string& AxrVulkanMaterialLayoutAssets::getVertexShaderName() const {
    return m_VertexShaderName;
}

const std::string& AxrVulkanMaterialLayoutAssets::getFragmentShaderName() const {
    return m_FragmentShaderName;
}

bool AxrVulkanMaterialLayoutAssets::areAssetsEmpty() const {
    return m_DescriptorSetItemLocations.empty() &&
        m_DescriptorSetLayout == VK_NULL_HANDLE &&
        m_PipelineLayout == VK_NULL_HANDLE;
}

bool AxrVulkanMaterialLayoutAssets::areWindowAssetsEmpty() const {
    return m_WindowPipeline == VK_NULL_HANDLE;
}

AxrResult AxrVulkanMaterialLayoutAssets::createAssets(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!areAssetsEmpty()) {
        axrLogErrorLocation("Material layout assets already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = validateMaterialLayoutShaders(vertexShader, fragmentShader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to validate material layout shaders.");
        destroyAssets();
        return axrResult;
    }

    axrResult = createDescriptorSetLayout(vertexShader, fragmentShader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create descriptor set layout.");
        destroyAssets();
        return axrResult;
    }

    axrResult = createPipelineLayout(vertexShader, fragmentShader);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create pipeline layout.");
        destroyAssets();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutAssets::destroyAssets() {
    destroyWindowAssets();

    destroyDescriptorSetLayout();
    destroyPipelineLayout();
}

AxrResult AxrVulkanMaterialLayoutAssets::createWindowAssets(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!areWindowAssetsEmpty()) {
        axrLogErrorLocation("Material layout window assets already exist.");
        return AXR_ERROR;
    }

    if (areAssetsEmpty()) {
        axrLogErrorLocation("Material layout assets are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createPipeline(vertexShader, fragmentShader, m_WindowPipeline);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create pipeline.");
        destroyWindowAssets();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutAssets::destroyWindowAssets() {
    destroyPipeline(m_WindowPipeline);
}

// ---- Private Functions ----

void AxrVulkanMaterialLayoutAssets::cleanup() {
    destroyWindowAssets();
    destroyAssets();

    m_Name.clear();
    m_VertexShaderName.clear();
    m_FragmentShaderName.clear();
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanMaterialLayoutAssets::validateMaterialLayoutShaders(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader
) {
    if (!vertexShader.isValid()) {
        axrLogErrorLocation("Validation failed for shader named: {0}.", vertexShader.getName());
        return AXR_ERROR;
    }

    if (!fragmentShader.isValid()) {
        axrLogErrorLocation("Validation failed for shader named: {0}.", fragmentShader.getName());
        return AXR_ERROR;
    }

    if (!AxrShader::areCompatible(vertexShader, fragmentShader)) {
        axrLogErrorLocation(
            "Validation failed for shader compatibility between: {0} and {1}.",
            vertexShader.getName(),
            fragmentShader.getName()
        );
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanMaterialLayoutAssets::createDescriptorSetLayout(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
        axrLogErrorLocation("Descriptor set layout already exists.");
        return AXR_ERROR;
    }

    if (!m_DescriptorSetItemLocations.empty()) {
        axrLogErrorLocation("Descriptor set item locations already exist.");
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

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    // ---- Uniform buffer bindings ----

    const std::vector<AxrShaderUniformBufferLayoutConst_T> vertexUniformBufferLayouts =
        vertexShader.getProperties().getUniformBufferLayouts();
    const std::vector<AxrShaderUniformBufferLayoutConst_T> fragmentUniformBufferLayouts =
        fragmentShader.getProperties().getUniformBufferLayouts();

    for (const AxrShaderUniformBufferLayoutConst_T uniformBufferLayout : vertexUniformBufferLayouts) {
        if (uniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            uniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBuffer,
            vk::ShaderStageFlagBits::eVertex,
            bindings,
            m_DescriptorSetItemLocations
        );
    }

    for (const AxrShaderUniformBufferLayoutConst_T uniformBufferLayout : fragmentUniformBufferLayouts) {
        if (uniformBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            uniformBufferLayout->Binding,
            vk::DescriptorType::eUniformBuffer,
            vk::ShaderStageFlagBits::eFragment,
            bindings,
            m_DescriptorSetItemLocations
        );
    }

    // ---- Image sampler bindings ----

    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> vertexImageSamplerBufferLayouts =
        vertexShader.getProperties().getImageSamplerBufferLayouts();
    const std::vector<AxrShaderImageSamplerBufferLayoutConst_T> fragmentImageSamplerBufferLayouts =
        fragmentShader.getProperties().getImageSamplerBufferLayouts();

    for (const AxrShaderImageSamplerBufferLayoutConst_T imageSamplerBufferLayout : vertexImageSamplerBufferLayouts) {
        if (imageSamplerBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            imageSamplerBufferLayout->Binding,
            vk::DescriptorType::eCombinedImageSampler,
            vk::ShaderStageFlagBits::eVertex,
            bindings,
            m_DescriptorSetItemLocations
        );
    }

    for (const AxrShaderImageSamplerBufferLayoutConst_T imageSamplerBufferLayout : fragmentImageSamplerBufferLayouts) {
        if (imageSamplerBufferLayout == nullptr) continue;

        addDescriptorSetLayoutItem(
            imageSamplerBufferLayout->Binding,
            vk::DescriptorType::eCombinedImageSampler,
            vk::ShaderStageFlagBits::eFragment,
            bindings,
            m_DescriptorSetItemLocations
        );
    }

    // ---- Create descriptor set layout ----

    const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
        {},
        static_cast<uint32_t>(bindings.size()),
        bindings.data()
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

void AxrVulkanMaterialLayoutAssets::destroyDescriptorSetLayout() {
    if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
        m_Device.destroyDescriptorSetLayout(m_DescriptorSetLayout, nullptr, *m_DispatchHandle);
        m_DescriptorSetLayout = VK_NULL_HANDLE;
    }
    m_DescriptorSetItemLocations.clear();
}

void AxrVulkanMaterialLayoutAssets::addDescriptorSetLayoutItem(
    const uint32_t binding,
    const vk::DescriptorType descriptorType,
    const vk::ShaderStageFlagBits stageFlag,
    std::vector<vk::DescriptorSetLayoutBinding>& bindings,
    std::vector<DescriptorSetItemLocation>& descriptorSetItemLocations
) {
    // Check if the binding has already been added, and if it has, just include the shader stage
    for (const DescriptorSetItemLocation& itemLocation : descriptorSetItemLocations) {
        if (itemLocation.ShaderBinding == binding) {
            bindings[itemLocation.ItemIndex].stageFlags |= stageFlag;
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

    const DescriptorSetItemLocation descriptorSetItemLocation{
        .BufferLayoutType = AXR_SHADER_BUFFER_LAYOUT_UNIFORM_BUFFER,
        .ShaderBinding = binding,
        .ItemIndex = static_cast<uint32_t>(bindings.size()) - 1
    };

    descriptorSetItemLocations.push_back(descriptorSetItemLocation);
}

AxrResult AxrVulkanMaterialLayoutAssets::createPipelineLayout(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader
) {
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

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::vector<vk::PushConstantRange> pushConstantRanges;

    const AxrShaderPushConstantsBufferLayoutConst_T vertexPushConstantsBufferLayout =
        vertexShader.getProperties().getPushConstantsBufferLayout();
    if (vertexPushConstantsBufferLayout != nullptr) {
        pushConstantRanges.push_back(
            {
                vk::PushConstantRange(
                    vk::ShaderStageFlagBits::eVertex,
                    0,
                    vertexPushConstantsBufferLayout->BufferSize
                )
            }
        );
    }

    const AxrShaderPushConstantsBufferLayoutConst_T fragmentPushConstantsBufferLayout =
        fragmentShader.getProperties().getPushConstantsBufferLayout();
    if (fragmentPushConstantsBufferLayout != nullptr) {
        pushConstantRanges.push_back(
            {
                vk::PushConstantRange(
                    vk::ShaderStageFlagBits::eFragment,
                    0,
                    fragmentPushConstantsBufferLayout->BufferSize
                )
            }
        );
    }

    /* TODO: Allow multiple descriptor sets.
     * https://stackoverflow.com/questions/56928041/what-is-the-purpose-of-multiple-setlayoutcounts-of-vulkan-vkpipelinelayoutcreate
     * "A descriptor set layout describes the layout for a single descriptor set.
     * But a pipeline can have multiple descriptor sets.
     * This is what the layout(set = #) part of the qualifier in GLSL means:
     * it specifies which set that this particular resource gets its descriptor from.
     * The set is an index into the VkPipelineLayoutCreateInfo::pSetLayouts array.
     * The descriptor is the index into that set's list of descriptors.
     * The two of them combined identify the specific descriptor within the pipeline layout."
     *
     * https://www.reddit.com/r/vulkan/comments/zzspjn/need_clarification_regarding_descriptor_sets/
     * "The general idea is that you create a few tables (descriptor sets) of data that a pipeline needs to run.
     * The layout of these is entirely up to you, but you can design it so that your different pipelines can reuse
     * the data by making the layouts of your descriptor sets compatible."
     *
     * I think this means you could have 1 set which contains scene data, and another for object specific data.
     * Then you can reuse the scene descriptor set and data for every object and the object has it's own data.
     * But to accomplish that, would be a bigger change i suspect.
     *
     * I don't think it's even really that necessary for it to share descriptor set layouts.
     * Is there really a downside to just creating them again for difference materials?
     * I'm pretty sure the system is designed in such a way that data (for example scene data) can be shared.
     * And even if we create the same descriptor set layout multiple times, it just means they can differ slightly
     * if desired right?
     * And if they don't differ, is it really worth the extra effort to share them?
     * Maybe it is if it turns out to be fairly simple. I just don't think it will.
     * The most compelling reason is that it will improve performance. 
     *
     * Oh, also, I believe this is vulkan only. OpenGL doesn't have "sets" so layout(set = #) wouldn't work there.
     * So make sure it's a feature that's only available for vulkan. Like push constants.
     *
     * descriptor set layout rules: can't have a missing index?
     * Example: 3 layouts with the indexes 0, 2, 4. Invalid. Missing 1 and 3
     *
     * setLayoutCount must be less than or equal to VkPhysicalDeviceLimits::maxBoundDescriptorSets
     *
     * I think we need to modify vkCmdBindDescriptorSets when this is no longer hard coded to 1.
     *
     * https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.pdf
     * Section 12.2.3. Vulkan Only: Descriptor Sets
     *
     *
     * https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/
     * Section Frequency-based descriptor sets:
     *
     * A more Vulkan centric renderer would organize data that the shaders need to access into groups by frequency
     * of change, and use individual sets for individual frequencies, with set=0 representing least frequent change,
     * and set=3 representing most frequent. For example, a typical setup would involve:
     * Set=0 descriptor set containing uniform buffer with global, per-frame or per-view data,
     * as well as globally available textures such as shadow map texture array/atlas
     * Set=1 descriptor set containing uniform buffer and texture descriptors for per-material data,
     * such as albedo map, Fresnel coefficients, etc.
     * Set=2 descriptor set containing dynamic uniform buffer with per-draw data, such as world transform array
     */
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
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutAssets::destroyPipelineLayout() {
    if (m_PipelineLayout == VK_NULL_HANDLE) return;

    m_Device.destroyPipelineLayout(m_PipelineLayout, nullptr, *m_DispatchHandle);
    m_PipelineLayout = VK_NULL_HANDLE;
}

AxrResult AxrVulkanMaterialLayoutAssets::createPipeline(
    const AxrShader& vertexShader,
    const AxrShader& fragmentShader,
    vk::Pipeline& pipeline
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (pipeline != VK_NULL_HANDLE) {
        axrLogErrorLocation("Pipeline already exists.");
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

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::GraphicsPipelineCreateInfo pipelineCreateInfo(
        {},
        static_cast<uint32_t>(shaderStageCreateInfos.size()),
        shaderStageCreateInfos.data(),
        vertexInputStateCreateInfo,
        inputAssemblyStateCreateInfo,
        {},
        viewportStateCreateInfo,
        rasterizationStateCreateInfo,
        multisampleStateCreateInfo,
        depthStencilStateCreateInfo,
        colorBlendStateCreateInfo,
        dynamicStateCreateInfo,
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
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanMaterialLayoutAssets::destroyPipeline(vk::Pipeline& pipeline) {
    if (pipeline == VK_NULL_HANDLE) return;

    m_Device.destroyPipeline(pipeline, nullptr, *m_DispatchHandle);
    pipeline = VK_NULL_HANDLE;
}

#endif
