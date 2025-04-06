#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/windowSystem.h"
#include "vulkanExtensionCollection.hpp"
#include "vulkanQueueFamilies.hpp"
#include "sceneData/vulkanLoadedScenesCollection.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Window Graphics
class AxrVulkanWindowGraphics {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrVulkanWindowGraphics Config
    struct Config {
        AxrWindowSystem& WindowSystem;
        vk::DispatchLoaderDynamic& Dispatch;
        AxrVulkanLoadedScenesCollection& LoadedScenes;
        AxrVulkanPresentationModeEnum PresentationMode;
    };

    /// AxrVulkanWindowGraphics Setup Config
    struct SetupConfig {
        vk::Instance Instance;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        const AxrVulkanQueueFamilies& QueueFamilies;
        /// Ordered from most desired to the least desired
        const std::vector<vk::SurfaceFormatKHR>& SwapchainColorFormatOptions;
        /// Ordered from most desired to the least desired
        const std::vector<vk::Format>& SwapchainDepthFormatOptions;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanWindowGraphics(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanWindowGraphics to copy from
    AxrVulkanWindowGraphics(const AxrVulkanWindowGraphics& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanWindowGraphics to move from
    AxrVulkanWindowGraphics(AxrVulkanWindowGraphics&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanWindowGraphics();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanWindowGraphics to copy from
    AxrVulkanWindowGraphics& operator=(const AxrVulkanWindowGraphics& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanWindowGraphics to move from
    AxrVulkanWindowGraphics& operator=(AxrVulkanWindowGraphics&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Add the instance extensions required for window graphics to the given extension collection
    /// @param extensions The extension collection to add to
    void addRequiredInstanceExtensions(
        AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>& extensions
    ) const;
    /// Add the device extensions required for window graphics to the given extension collection
    /// @param extensions The extension collection to add to
    void addRequiredDeviceExtensions(
        AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>& extensions
    ) const;

    /// Set up vulkan window graphics
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config ----
    AxrWindowSystem& m_WindowSystem;
    vk::DispatchLoaderDynamic& m_Dispatch;
    AxrVulkanLoadedScenesCollection& m_LoadedScenes;
    AxrVulkanPresentationModeEnum m_PreferredPresentationMode;

    // ---- Setup Config ----
    vk::Instance m_Instance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    AxrVulkanQueueFamilies m_QueueFamilies;
    /// Ordered from most desired to the least desired
    std::vector<vk::SurfaceFormatKHR> m_SwapchainColorFormatOptions;
    /// Ordered from most desired to the least desired
    std::vector<vk::Format> m_SwapchainDepthFormatOptions;

    vk::SurfaceKHR m_Surface;
    vk::SurfaceFormatKHR m_SwapchainColorFormat;
    vk::Format m_SwapchainDepthFormat;
    vk::PresentModeKHR m_SwapchainPresentationMode;
    vk::Extent2D m_SwapchainExtent;
    vk::SwapchainKHR m_Swapchain;
    std::vector<vk::Image> m_SwapchainImages;
    std::vector<vk::ImageView> m_SwapchainImageViews;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Set the SetupConfig variables
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSetupConfigVariables(const SetupConfig& config);
    /// Reset the SetupConfig variables
    void resetSetupConfigVariables();

    /// Set the swapchain color and depth format options
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainFormatOptions(const SetupConfig& config);
    /// Reset the setSwapchainFormatOptions() function 
    void resetSwapchainFormatOptions();

    /// Configure window graphics
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult configureWindowGraphics();
    /// Reset the configureWindowGraphics() function 
    void resetWindowConfiguration();

    // ---- Surface ----

    /// Create the vulkan surface
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSurface();
    /// Destroy the vulkan surface
    void destroySurface();

#ifdef AXR_USE_PLATFORM_WIN32
    /// Create the vulkan win32 surface
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWin32Surface();
#endif

    // ---- Swapchain ----

    /// Set the swapchain color and depth formats
    /// @param surfaceFormats Collection of surface formats that are available to us
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainFormats(const std::vector<vk::SurfaceFormatKHR>& surfaceFormats);
    /// Reset the setSwapchainFormats() function
    void resetSwapchainFormats();

    /// Set the swapchain presentation mode
    /// @param surfacePresentationModes Collection of surface presentation modes that are available to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainPresentationMode(
        const std::vector<vk::PresentModeKHR>& surfacePresentationModes
    );
    /// Reset the setSwapchainPresentationMode() function
    void resetSwapchainPresentationMode();

    /// Set the swapchain extent
    /// @param surfaceCapabilities Surface capabilities
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSwapchainExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
    /// Reset the setSwapchainExtent() function
    void resetSwapchainExtent();

    /// Create the vulkan swapchain
    /// @param surfaceCapabilities Surface capabilities
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSwapchain(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
    /// Destroy the createSwapchain() function
    void destroySwapchain();

    /// Get the swapchain images
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult getSwapchainImages();
    /// Reset the getSwapchainImages() function
    void resetSwapchainImages();

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //

    /// 'Configure window graphics' callback function
    /// @param userData User data
    /// @param isWindowOpen If true, the graphics should be configured. If false, the graphics configuration should be reset.
    /// @returns AXR_SUCCESS if the function succeeded
    static AxrResult configureWindowGraphicsCallback(void* userData, bool isWindowOpen);
};


#endif
