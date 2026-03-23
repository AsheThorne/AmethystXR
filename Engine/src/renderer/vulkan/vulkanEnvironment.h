#pragma once
#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/containers/vector_dynamic.h"
#include "axr/vulkanApi.h"
#include "vulkanImage.h"
#include "vulkanQueueFamilies.h"

#include <vulkan/vulkan_core.h>

/// Vulkan environment (XR System, Desktop) related functions
class AxrVulkanEnvironment {
public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    struct SetupConfig {
        const AxrVulkanRendererDesktopConfig* DesktopConfig;
        VkInstance Instance;
        VkPhysicalDevice PhysicalDevice;
        VkDevice Device;
        VkCommandPool GraphicsCommandPool;
        const AxrVulkanQueueFamilies& QueueFamilies;
        /// Ordered from most desired to the least desired
        const AxrVector_Stack<VkFormat>& SwapchainColorFormatOptions;
        /// Ordered from most desired to the least desired
        const AxrVector_Stack<VkFormat>& SwapchainDepthFormatOptions;
        uint32_t MaxFramesInFlight;
    };

    struct DesktopSwapchainContext {
        AxrVector_Dynamic<VkImage> ColorImages = AxrVector_Dynamic<VkImage>();
        AxrVector_Dynamic<VkImageView> ColorImageViews = AxrVector_Dynamic<VkImageView>();
        AxrVector_Dynamic<AxrVulkanImage> DepthImages = AxrVector_Dynamic<AxrVulkanImage>();
        AxrVector_Dynamic<AxrVulkanImage> MsaaImages = AxrVector_Dynamic<AxrVulkanImage>();
        VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
        VkExtent2D Extent = {};
        VkFormat ColorFormat = VK_FORMAT_UNDEFINED;
        VkFormat DepthFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR ColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
        AxrVulkanPresentationModeEnum PreferredPresentationMode = AXR_VULKAN_PRESENTATION_MODE_UNDEFINED;
        VkPresentModeKHR PresentationMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
    };

    struct DesktopContext {
        AxrVector_Dynamic<VkSemaphore> ImageAvailableSemaphores = AxrVector_Dynamic<VkSemaphore>();
        AxrVector_Dynamic<VkSemaphore> RenderingFinishedSemaphores = AxrVector_Dynamic<VkSemaphore>();
        AxrVector_Dynamic<VkFence> RenderingFences = AxrVector_Dynamic<VkFence>();
        AxrVector_Dynamic<VkCommandBuffer> RenderingCommandBuffers = AxrVector_Dynamic<VkCommandBuffer>();
        AxrVector_Dynamic<VkFramebuffer> Framebuffers = AxrVector_Dynamic<VkFramebuffer>();
        DesktopSwapchainContext SwapchainContext;
        VkInstance Instance = VK_NULL_HANDLE;
        VkDevice Device = VK_NULL_HANDLE;
        VkSurfaceKHR Surface = VK_NULL_HANDLE;
        VkRenderPass RenderPass = VK_NULL_HANDLE;
        VkCommandPool GraphicsCommandPool = VK_NULL_HANDLE;
        VkSampleCountFlagBits MsaaSampleCount = VK_SAMPLE_COUNT_1_BIT;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanEnvironment() = delete;
    /// Copy Constructor
    /// @param src Source AxrVulkanEnvironment to copy from
    AxrVulkanEnvironment(const AxrVulkanEnvironment& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanEnvironment to move from
    AxrVulkanEnvironment(AxrVulkanEnvironment&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanEnvironment() = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanEnvironment to copy from
    AxrVulkanEnvironment& operator=(const AxrVulkanEnvironment& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanEnvironment to move from
    AxrVulkanEnvironment& operator=(AxrVulkanEnvironment&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the desktop environment context
    /// @param config Setup config
    /// @param context Output desktop environment context
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setupDesktopContext(const SetupConfig& config, DesktopContext& context);
    /// Destroy the given desktop environment context
    /// @param context Desktop environment context to destroy
    static void destroyDesktopContext(DesktopContext& context);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Format ----

    /// Set the color and depth swapchain formats to use for the desktop environment
    /// @param surface Surface to use
    /// @param physicalDevice Physical device to use
    /// @param swapchainColorFormatOptions Swapchain color format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param swapchainDepthFormatOptions Swapchain depth format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param colorFormat Output color format
    /// @param colorSpace Output color space
    /// @param depthFormat Output depth format
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setDesktopSwapchainFormats(
        const VkSurfaceKHR& surface,
        const VkPhysicalDevice& physicalDevice,
        const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
        const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
        VkFormat& colorFormat,
        VkColorSpaceKHR& colorSpace,
        VkFormat& depthFormat);
    /// Reset the given color space and the color and depth swapchain formats
    /// @param colorFormat Color format to reset
    /// @param colorSpace Color space to reset
    /// @param depthFormat Depth format to reset
    static void resetDesktopSwapchainFormats(VkFormat& colorFormat, VkColorSpaceKHR& colorSpace, VkFormat& depthFormat);

    /// Get the supported formats for the given surface
    /// @param surface Surface to use
    /// @param physicalDevice Physical device to use
    /// @param supportedFormats Output supported formats
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedSurfaceFormats(const VkSurfaceKHR& surface,
                                                              const VkPhysicalDevice& physicalDevice,
                                                              AxrVector_Stack<VkSurfaceFormatKHR>& supportedFormats);

    /// Set the color and depth swapchain formats to use
    /// @param physicalDevice Physical device to use
    /// @param swapchainColorFormatOptions Swapchain color format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param swapchainDepthFormatOptions Swapchain depth format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param supportedSwapchainFormats Supported swapchain formats
    /// @param colorFormat Output color format
    /// @param depthFormat Output depth format
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setSwapchainFormats(const VkPhysicalDevice& physicalDevice,
                                                       const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                       const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                       const AxrVector_Stack<VkFormat>& supportedSwapchainFormats,
                                                       VkFormat& colorFormat,
                                                       VkFormat& depthFormat);

    /// Check if the given format supports the given features
    /// @param format Format to check against
    /// @param imageTiling Image tiling option
    /// @param features Features to check
    /// @param physicalDevice Physical device to use
    /// @returns True if the given format supports the given features
    [[nodiscard]] static bool areFormatFeaturesSupported(VkFormat format,
                                                         VkImageTiling imageTiling,
                                                         VkFormatFeatureFlags features,
                                                         const VkPhysicalDevice& physicalDevice);

    // ---- Render Pass ----

    /// Create a render pass
    /// @param device Device to use
    /// @param colorFormat Color format to use
    /// @param depthStencilFormat Depth stencil format to use
    /// @param msaaSampleCount Msaa sample count
    /// @param renderPass Output created render pass
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createRenderPass(const VkDevice& device,
                                                    VkFormat colorFormat,
                                                    VkFormat depthStencilFormat,
                                                    VkSampleCountFlagBits msaaSampleCount,
                                                    VkRenderPass& renderPass);
    /// Destroy the given render pass
    /// @param device Device to use
    /// @param renderPass Render pass to destroy
    static void destroyRenderPass(const VkDevice& device, VkRenderPass& renderPass);

    // ---- Sync Objects ----

    /// Create the rendering sync objects for the desktop environment
    /// @param device Device to use
    /// @param maxFramesInFlight The max frames in flight
    /// @param imageAvailableSemaphores Output image available semaphores
    /// @param renderingFinishedSemaphores Output rendering finished semaphores
    /// @param renderingFences Output rendering fences
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createDesktopSyncObjects(const VkDevice& device,
                                                            uint32_t maxFramesInFlight,
                                                            AxrVector_Dynamic<VkSemaphore>& imageAvailableSemaphores,
                                                            AxrVector_Dynamic<VkSemaphore>& renderingFinishedSemaphores,
                                                            AxrVector_Dynamic<VkFence>& renderingFences);
    /// Destroy the rendering sync objects for the desktop environment
    /// @param device Device to use
    /// @param imageAvailableSemaphores Image available semaphores to destroy
    /// @param renderingFinishedSemaphores Rendering finished semaphores to destroy
    /// @param renderingFences Rendering fences to destroy
    void static destroyDesktopSyncObjects(const VkDevice& device,
                                          AxrVector_Dynamic<VkSemaphore>& imageAvailableSemaphores,
                                          AxrVector_Dynamic<VkSemaphore>& renderingFinishedSemaphores,
                                          AxrVector_Dynamic<VkFence>& renderingFences);

    /// Fill the given vector with newly created semaphores
    /// @param device Device to use
    /// @param semaphores Output created semaphores
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createSemaphores(const VkDevice& device, AxrVector_Dynamic<VkSemaphore>& semaphores);
    /// Destroy the given semaphores
    /// @param device Device to use
    /// @param semaphores Semaphores to destroy
    /// @returns AXR_SUCCESS if the function succeeded
    static void destroySemaphores(const VkDevice& device, AxrVector_Dynamic<VkSemaphore>& semaphores);

    /// Fill the given vector with newly created fences
    /// @param device Device to use
    /// @param flags Fence create info flags to use
    /// @param fences Output created fences
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createFences(const VkDevice& device,
                                                VkFenceCreateFlags flags,
                                                AxrVector_Dynamic<VkFence>& fences);
    /// Destroy the given fences
    /// @param device Device to use
    /// @param fences Fences to destroy
    /// @returns AXR_SUCCESS if the function succeeded
    static void destroyFences(const VkDevice& device, AxrVector_Dynamic<VkFence>& fences);

    // ---- Command Buffers ----

    /// Fill the given vector with newly created command buffers
    /// @param device Device to use
    /// @param commandPool Command pool to use
    /// @param maxFramesInFlight The max frames in flight
    /// @param commandBuffers Output created command buffers
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createCommandBuffers(const VkDevice& device,
                                                        const VkCommandPool& commandPool,
                                                        uint32_t maxFramesInFlight,
                                                        AxrVector_Dynamic<VkCommandBuffer>& commandBuffers);
    /// Destroy the given command buffers
    /// @param device Device to use
    /// @param commandPool Command pool that was used to create the command buffers
    /// @param commandBuffers Command buffers to destroy
    static void destroyCommandBuffers(const VkDevice& device,
                                      const VkCommandPool& commandPool,
                                      AxrVector_Dynamic<VkCommandBuffer>& commandBuffers);

    // ---- Swapchain ----

    /// Set up the swapchain and all objects that depend on it for the desktop environment.
    /// Please note that the swapchain context `ColorFormat`, `DepthFormat` and `ColorSpace` is NOT set in this
    /// function. They must be set prior to using it here.
    /// @param physicalDevice Physical device to use
    /// @param device Device to use
    /// @param surface Surface to use
    /// @param renderPass Render pass to use
    /// @param graphicsCommandPool Graphics command pool to use
    /// @param queueFamilies Queue families to use
    /// @param msaaSampleCount Msaa sample count to use
    /// @param swapchainContext Input/Output swapchain context
    /// @param framebuffers Output Framebuffers to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setupDesktopSwapchain(const VkPhysicalDevice& physicalDevice,
                                                         const VkDevice& device,
                                                         const VkSurfaceKHR& surface,
                                                         const VkRenderPass& renderPass,
                                                         const VkCommandPool& graphicsCommandPool,
                                                         const AxrVulkanQueueFamilies& queueFamilies,
                                                         VkSampleCountFlagBits msaaSampleCount,
                                                         DesktopSwapchainContext& swapchainContext,
                                                         AxrVector_Dynamic<VkFramebuffer>& framebuffers);
    /// Reset the swapchain and all objects that depend on it for the desktop environment.
    /// Please note that the swapchain context `ColorFormat`, `DepthFormat` and `ColorSpace` is NOT reset in this
    /// function.
    /// @param device Logical device to use
    /// @param swapchainContext Swapchain context to reset
    /// @param framebuffers Framebuffers to reset
    static void resetSetupDesktopSwapchain(const VkDevice& device,
                                           DesktopSwapchainContext& swapchainContext,
                                           AxrVector_Dynamic<VkFramebuffer>& framebuffers);

    /// Set the swapchain presentation mode for the desktop environment
    /// @param physicalDevice Physical device to use
    /// @param surface Surface to use
    /// @param preferredPresentationMode Preferred swapchain presentation mode to use
    /// @param presentationMode Output selected swapchain presentation mode
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setDesktopSwapchainPresentationMode(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        AxrVulkanPresentationModeEnum preferredPresentationMode,
        VkPresentModeKHR& presentationMode);
    /// Reset the swapchain presentation mode for the desktop environment
    /// @param presentationMode Swapchain presentation mode to reset
    static void resetDesktopSwapchainPresentationMode(VkPresentModeKHR& presentationMode);

    /// Get the supported presentation modes for the given surface
    /// @param physicalDevice Physical device to use
    /// @param surface Surface to use
    /// @param supportedPresentationModes Output supported presentation modes
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedSurfacePresentationModes(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        AxrVector_Stack<VkPresentModeKHR>& supportedPresentationModes);

    /// @param physicalDevice Physical device to use
    /// @param surface Surface to use
    /// @param capabilities Output swapchain capabilities
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSurfaceCapabilities(const VkPhysicalDevice& physicalDevice,
                                                          const VkSurfaceKHR& surface,
                                                          VkSurfaceCapabilitiesKHR& capabilities);

    /// Set the swapchain extent for the desktop environment
    /// @param physicalDevice Physical device to use
    /// @param surface Surface to use
    /// @param extent Output swapchain extent
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setDesktopSwapchainExtent(const VkPhysicalDevice& physicalDevice,
                                                             const VkSurfaceKHR& surface,
                                                             VkExtent2D& extent);
    /// Reset the swapchain extent
    /// @param extent Swapchain extent to reset
    static void resetSwapchainExtent(VkExtent2D& extent);

    /// Create the swapchain for the desktop environment.
    /// Please note that only the swapchain context `Swapchain` variable is set in this function. `Extent`,
    /// `ColorFormat`, `ColorSpace` and `PresentationMode` must be set prior to using this function.
    /// @param physicalDevice Physical device to use
    /// @param device Logical device to use
    /// @param surface Surface to use
    /// @param queueFamilies Queue families to use
    /// @param swapchainContext Input/Output swapchain context
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createDesktopSwapchain(const VkPhysicalDevice& physicalDevice,
                                                          const VkDevice& device,
                                                          const VkSurfaceKHR& surface,
                                                          const AxrVulkanQueueFamilies& queueFamilies,
                                                          DesktopSwapchainContext& swapchainContext);
    /// Destroy the swapchain for the desktop environment
    /// @param device Logical device to use
    /// @param swapchain Swapchain to destroy
    static void destroyDesktopSwapchain(const VkDevice& device, VkSwapchainKHR& swapchain);

    /// Get the swapchain color images for the desktop environment
    /// @param device Device to use
    /// @param swapchain Swapchain to get the images of
    /// @param swapchainColorFormat Swapchain color format to use
    /// @param images Output swapchain images
    /// @param imageViews Output swapchain image views
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getDesktopSwapchainImages(const VkDevice& device,
                                                             const VkSwapchainKHR& swapchain,
                                                             VkFormat swapchainColorFormat,
                                                             AxrVector_Dynamic<VkImage>& images,
                                                             AxrVector_Dynamic<VkImageView>& imageViews);
    /// Reset the swapchain color images for the desktop environment
    /// @param device Device to use
    /// @param images Swapchain images to reset
    /// @param imageViews Swapchain image views to reset
    static void resetDesktopSwapchainImages(const VkDevice& device,
                                            AxrVector_Dynamic<VkImage>& images,
                                            AxrVector_Dynamic<VkImageView>& imageViews);

    /// Create swapchain depth images
    /// @param physicalDevice Physical device to use
    /// @param device Device to use
    /// @param graphicsCommandPool Graphics command pool to use
    /// @param graphicsQueue Graphics Queue to use
    /// @param swapchainExtent Swapchain extent
    /// @param imageCount Number of depth images to create
    /// @param msaaSampleCount Msaa sample count
    /// @param imageFormat Swapchain depth image format
    /// @param images Output created swapchain depth images
    [[nodiscard]] static AxrResult createSwapchainDepthImages(const VkPhysicalDevice& physicalDevice,
                                                              const VkDevice& device,
                                                              const VkCommandPool& graphicsCommandPool,
                                                              const VkQueue& graphicsQueue,
                                                              VkExtent2D swapchainExtent,
                                                              uint32_t imageCount,
                                                              VkSampleCountFlagBits msaaSampleCount,
                                                              VkFormat imageFormat,
                                                              AxrVector_Dynamic<AxrVulkanImage>& images);
    /// Create swapchain msaa images
    /// @param physicalDevice Physical device to use
    /// @param device Device to use
    /// @param graphicsCommandPool Graphics command pool to use
    /// @param graphicsQueue Graphics Queue to use
    /// @param swapchainExtent Swapchain extent
    /// @param imageCount Number of msaa images to create
    /// @param msaaSampleCount Msaa sample count
    /// @param imageFormat Swapchain msaa image format
    /// @param images Output created swapchain msaa images
    [[nodiscard]] static AxrResult createSwapchainMsaaImages(const VkPhysicalDevice& physicalDevice,
                                                             const VkDevice& device,
                                                             const VkCommandPool& graphicsCommandPool,
                                                             const VkQueue& graphicsQueue,
                                                             VkExtent2D swapchainExtent,
                                                             uint32_t imageCount,
                                                             VkSampleCountFlagBits msaaSampleCount,
                                                             VkFormat imageFormat,
                                                             AxrVector_Dynamic<AxrVulkanImage>& images);

    /// Destroy the given vulkan images
    /// @param images Vulkan images to destroy
    static void destroyVulkanImages(AxrVector_Dynamic<AxrVulkanImage>& images);

    // ---- Framebuffer ----

    /// Create a framebuffer for each swapchain image
    /// @param device Device to use
    /// @param renderPass Render pass to use
    /// @param swapchainImageCount Number of swapchain images in each swapchainImageViews array
    /// @param swapchainColorImageViews Swapchain color image views array
    /// @param swapchainDepthImageViews Swapchain depth image views array
    /// @param swapchainMsaaImageViews Swapchain msaa image views array
    /// @param swapchainExtent Swapchain extent
    /// @param msaaSampleCount Msaa sample count
    /// @param framebuffers Output created framebuffers
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createFramebuffers(const VkDevice& device,
                                                      const VkRenderPass& renderPass,
                                                      uint32_t swapchainImageCount,
                                                      const VkImageView* swapchainColorImageViews,
                                                      const VkImageView* swapchainDepthImageViews,
                                                      const VkImageView* swapchainMsaaImageViews,
                                                      VkExtent2D swapchainExtent,
                                                      VkSampleCountFlagBits msaaSampleCount,
                                                      AxrVector_Dynamic<VkFramebuffer>& framebuffers);
    /// Destroy the given framebuffers
    /// @param device Device to use
    /// @param framebuffers Framebuffers to destroy
    static void destroyFramebuffers(const VkDevice& device, AxrVector_Dynamic<VkFramebuffer>& framebuffers);
};

#endif
