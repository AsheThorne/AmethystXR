#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../../xrSystem/xrSystem.hpp"
#include "vulkanQueueFamilies.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Xr Graphics
class AxrVulkanXrGraphics {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrVulkanXrGraphics config
    struct Config {
        AxrXrSystem& XrSystem;
        vk::DispatchLoaderDynamic& Dispatch;
        uint32_t MaxFramesInFlight;
    };

    /// AxrVulkanXrGraphics Setup Config
    struct SetupConfig {
        vk::Instance Instance;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool GraphicsCommandPool;
        const AxrVulkanQueueFamilies& QueueFamilies;
        /// Ordered from most desired to the least desired
        const std::vector<vk::Format>& SwapchainColorFormatOptions;
        /// Ordered from most desired to the least desired
        const std::vector<vk::Format>& SwapchainDepthFormatOptions;
    };

    // Per view data
    struct View {
        /// One semaphore per frame in flight
        std::vector<vk::Semaphore> RenderingFinishedSemaphores;
        /// One fence per frame in flight
        std::vector<vk::Fence> RenderingFences;
        /// One command buffer per frame in flight
        std::vector<vk::CommandBuffer> RenderingCommandBuffers;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Xr graphics config
    AxrVulkanXrGraphics(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics(const AxrVulkanXrGraphics& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics(AxrVulkanXrGraphics&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanXrGraphics();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics& operator=(const AxrVulkanXrGraphics& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics& operator=(AxrVulkanXrGraphics&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up vulkan xr graphics
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

    /// Check if the xr session graphics are ready for rendering
    /// @returns True if the xr session graphics are ready for rendering
    [[nodiscard]] bool isReady() const;

    /// Create the vulkan instance to use.
    /// @param createInfo The VkInstanceCreateInfo
    /// @param vkInstance Output created vkInstance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createVulkanInstance(
        const vk::InstanceCreateInfo& createInfo,
        vk::Instance& vkInstance
    ) const;

    /// Get the vulkan physical device to use
    /// @param vkInstance The vkInstance to use
    /// @param vkPhysicalDevice The Output VkPhysicalDevice
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult getVulkanPhysicalDevice(
        vk::Instance vkInstance,
        vk::PhysicalDevice& vkPhysicalDevice
    ) const;

    /// Create the vulkan device to use
    /// @param vkPhysicalDevice VkPhysicalDevice to use
    /// @param createInfo The VkDeviceCreateInfo
    /// @param vkDevice Output created vkDevice
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createVulkanDevice(
        vk::PhysicalDevice vkPhysicalDevice,
        const vk::DeviceCreateInfo& createInfo,
        vk::Device& vkDevice
    ) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config ----
    AxrXrSystem& m_XrSystem;
    vk::DispatchLoaderDynamic& m_Dispatch;
    uint32_t m_MaxFramesInFlight;

    // ---- Setup Config ----
    vk::Instance m_Instance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_GraphicsCommandPool;
    AxrVulkanQueueFamilies m_QueueFamilies;
    /// Ordered from most desired to the least desired
    std::vector<vk::Format> m_SwapchainColorFormatOptions;
    /// Ordered from most desired to the least desired
    std::vector<vk::Format> m_SwapchainDepthFormatOptions;

    // ---- Data ----
    bool m_IsReady;
    vk::Format m_SwapchainColorFormat;
    vk::Format m_SwapchainDepthFormat;
    vk::ImageLayout m_SwapchainImageLayout;
    vk::RenderPass m_RenderPass;
    std::vector<View> m_Views;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Set the xr session graphics binding
    void setXrGraphicsBinding() const;
    /// Reset setXrGraphicsBinding()
    void resetXrGraphicsBinding() const;

    /// Set up the xr session graphics
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupXrSessionGraphics();
    /// Reset the setupXrSessionGraphics() function 
    void resetSetupXrSessionGraphics();

    // ---- Swapchain ----

    /// Set the swapchain color and depth format options
    /// @param physicalDevice Physical device to use
    /// @param swapchainColorFormatOptions Swapchain color format options to choose from
    /// @param swapchainDepthFormatOptions Swapchain depth format options to choose from
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainFormatOptions(
        const vk::PhysicalDevice& physicalDevice,
        const std::vector<vk::Format>& swapchainColorFormatOptions,
        const std::vector<vk::Format>& swapchainDepthFormatOptions
    );
    /// Reset the setSwapchainFormatOptions() function 
    void resetSwapchainFormatOptions();

    /// Set the swapchain color and depth formats
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainFormats();
    /// Reset the setSwapchainFormats() function
    void resetSwapchainFormats();

    // ---- Render pass ----

    /// Create the render pass
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createRenderPass();
    /// Destroy the render pass
    void destroyRenderPass();

    // ---- View ----

    /// Set up all view related data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupAllViews();
    /// Reset setupViews();
    void resetSetupAllViews();

    /// Set up the given view
    /// @param xrView Xr system view data
    /// @param view Output view graphics data
    [[nodiscard]] AxrResult setupView(const AxrXrSystem::View& xrView, View& view);
    /// Reset the given view
    /// @param view View data
    void resetSetupView(View& view);

    // ---- Sync Objects ----

    /// Create the rendering sync objects for the given view
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSyncObjects(View& view) const;
    /// Destroy the rendering sync objects for the given view
    void destroySyncObjects(View& view) const;

    // ---- Command Buffers ----

    /// Create command buffers for the given view
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createCommandBuffers(View& view) const;
    /// Destroy command buffers for the given view
    void destroyCommandBuffers(View& view) const;

    // ---- Callbacks ----

    /// 'On xr session state changed' callback function
    /// @param isSessionRunning If true, the xr session is running. If false, the xr session is not running.
    /// @returns AXR_SUCCESS if the function succeeded
    AxrResult onXrSessionStateChangedCallback(bool isSessionRunning);
};

#endif
