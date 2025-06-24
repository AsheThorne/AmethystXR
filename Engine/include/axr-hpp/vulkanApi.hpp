#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/vulkanApi.h"
#include "flags.hpp"
#include "logger.hpp"

namespace axr {
    // ---------------------------------------------------------------------------------- //
    //                                     Api Layers                                     //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Vulkan Api Layer Type
    enum class VulkanApiLayerTypeEnum {
        Undefined = AXR_VULKAN_API_LAYER_TYPE_UNDEFINED,
        CoreValidation = AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION,
        End = AXR_VULKAN_API_LAYER_TYPE_END,
    };

    // ---------------------------------------------------------------------------------- //
    //                                     Extensions                                     //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Vulkan Extension Type
    enum class VulkanExtensionTypeEnum {
        Undefined = AXR_VULKAN_EXTENSION_TYPE_UNDEFINED,
        DebugUtils = AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS,
        Swapchain = AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN,
        Surface = AXR_VULKAN_EXTENSION_TYPE_SURFACE,
        Win32Surface = AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE,
        End = AXR_VULKAN_EXTENSION_TYPE_END,
    };

    /// Vulkan Extension Level
    enum class VulkanExtensionLevelEnum {
        Undefined = AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED,
        Instance = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE,
        Device = AXR_VULKAN_EXTENSION_LEVEL_DEVICE,
    };

    // Vulkan Extension Debug Utils Severity Flag Bits. See: VkDebugUtilsMessageSeverityFlagBitsEXT
    enum class VulkanDebugUtilsSeverityFlagBits : AxrVulkanDebugUtilsSeverityFlags_T {
        Verbose = AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_VERBOSE_BIT,
        Info = AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_INFO_BIT,
        Warning = AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_WARNING_BIT,
        Error = AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_ERROR_BIT,
    };

    /// Vulkan Extension Debug Utils Severity Flags Type
    using VulkanDebugUtilsSeverityFlags_T = axr::Flags<VulkanDebugUtilsSeverityFlagBits>;

    // Vulkan Extension Debug Utils Type Flag Bits. See: VkDebugUtilsMessageTypeFlagBitsEXT
    enum class VulkanDebugUtilsTypeFlagBits : AxrVulkanDebugUtilsTypeFlags_T {
        General = AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_GENERAL_BIT,
        Validation = AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_VALIDATION_BIT,
        Performance = AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_PERFORMANCE_BIT,
        DeviceAddressBinding = AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_DEVICE_ADDRESS_BINDING_BIT,
    };

    /// Vulkan Extension Debug Utils Type Flags Type
    using VulkanDebugUtilsTypeFlags_T = axr::Flags<VulkanDebugUtilsTypeFlagBits>;

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    template <>
    struct FlagTraits<axr::VulkanDebugUtilsSeverityFlagBits> {
        static constexpr bool isBitmask = true;
        static constexpr VulkanDebugUtilsSeverityFlags_T allFlags =
            VulkanDebugUtilsSeverityFlagBits::Verbose | VulkanDebugUtilsSeverityFlagBits::Info |
            VulkanDebugUtilsSeverityFlagBits::Warning | VulkanDebugUtilsSeverityFlagBits::Error;
    };

    template <>
    struct FlagTraits<axr::VulkanDebugUtilsTypeFlagBits> {
        static constexpr bool isBitmask = true;
        static constexpr VulkanDebugUtilsTypeFlags_T allFlags =
            VulkanDebugUtilsTypeFlagBits::General | VulkanDebugUtilsTypeFlagBits::Validation |
            VulkanDebugUtilsTypeFlagBits::Performance | VulkanDebugUtilsTypeFlagBits::DeviceAddressBinding;
    };

    // ---------------------------------------------------------------------------------- //
    //                                   Vulkan Config                                    //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Vulkan Presentation Mode Enum
    enum class VulkanPresentationModeEnum {
        Undefined = AXR_VULKAN_PRESENTATION_MODE_UNDEFINED,
        Immediate = AXR_VULKAN_PRESENTATION_MODE_IMMEDIATE,
        Mailbox = AXR_VULKAN_PRESENTATION_MODE_MAILBOX,
        Fifo = AXR_VULKAN_PRESENTATION_MODE_FIFO,
        FifoRelaxed = AXR_VULKAN_PRESENTATION_MODE_FIFO_RELAXED,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan Window Graphics Config
    struct VulkanWindowConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::VulkanPresentationModeEnum PresentationMode;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        VulkanWindowConfig():
            PresentationMode(axr::VulkanPresentationModeEnum::Fifo) {
        }

        /// Constructor
        VulkanWindowConfig(const axr::VulkanPresentationModeEnum presentationMode):
            PresentationMode(presentationMode) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the VulkanWindowConfig as an AxrVulkanWindowConfig
        /// @returns This as an AxrVulkanWindowConfig
        const AxrVulkanWindowConfig* toRaw() const {
            return reinterpret_cast<const AxrVulkanWindowConfig*>(this);
        }

        /// Get a handle to the VulkanWindowConfig as an AxrVulkanWindowConfig
        /// @returns This as an AxrVulkanWindowConfig
        AxrVulkanWindowConfig* toRaw() {
            return reinterpret_cast<AxrVulkanWindowConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrVulkanWindowConfig) == sizeof(axr::VulkanWindowConfig),
        "Original type and wrapper have different size!"
    );

    /// Vulkan Api Graphics System Config
    struct VulkanApiConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t ApiLayerCount;
        AxrVulkanApiLayer_T* ApiLayers;
        uint32_t ExtensionCount;
        AxrVulkanExtension_T* Extensions;
        axr::VulkanWindowConfig* WindowConfig;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        VulkanApiConfig():
            ApiLayerCount(0),
            // equal to the same number of available options in VulkanApiLayerTypeEnum
            ApiLayers(new AxrVulkanApiLayer_T[static_cast<uint32_t>(VulkanApiLayerTypeEnum::End) - 1]{}),
            ExtensionCount(0),
            // equal to the same number of available options in VulkanExtensionTypeEnum
            Extensions(new AxrVulkanExtension_T[static_cast<uint32_t>(VulkanExtensionTypeEnum::End) - 1]{}),
            WindowConfig(nullptr) {
        }

        /// Constructor
        VulkanApiConfig(axr::VulkanWindowConfig* windowConfig):
            ApiLayerCount(0),
            // equal to the same number of available options in VulkanApiLayerTypeEnum
            ApiLayers(new AxrVulkanApiLayer_T[static_cast<uint32_t>(VulkanApiLayerTypeEnum::End) - 1]{}),
            ExtensionCount(0),
            // equal to the same number of available options in VulkanExtensionTypeEnum
            Extensions(new AxrVulkanExtension_T[static_cast<uint32_t>(VulkanExtensionTypeEnum::End) - 1]{}),
            WindowConfig(windowConfig) {
        }

        /// Copy Constructor
        /// @param src Source VulkanApiConfig to copy from
        VulkanApiConfig(const VulkanApiConfig& src) = delete;
        /// Move Constructor
        /// @param src Source VulkanApiConfig to move from
        VulkanApiConfig(VulkanApiConfig&& src) noexcept = delete;

        // ---- Destructor ----

        /// Destructor
        ~VulkanApiConfig() {
            clearApiLayers();
            delete[] ApiLayers;
            ApiLayers = nullptr;

            clearExtensions();
            delete[] Extensions;
            Extensions = nullptr;
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source VulkanApiConfig to copy from
        VulkanApiConfig& operator=(const VulkanApiConfig& src) = delete;
        /// Move Assignment Operator
        /// @param src Source VulkanApiConfig to move from
        VulkanApiConfig& operator=(VulkanApiConfig&& src) noexcept = delete;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if the given api layer exists
        /// @param apiLayerType The api layer type
        /// @returns True if the given api layer exists
        [[nodiscard]] bool apiLayerExists(axr::VulkanApiLayerTypeEnum apiLayerType) const {
            for (uint32_t i = 0; i < ApiLayerCount; ++i) {
                if (ApiLayers[i] != nullptr &&
                    ApiLayers[i]->Type == static_cast<AxrVulkanApiLayerTypeEnum>(apiLayerType)) {
                    return true;
                }
            }

            return false;
        }

        /// Check if the given extension exists
        /// @param extensionType The extension type
        /// @returns True if the given extension exists
        [[nodiscard]] bool extensionExists(axr::VulkanExtensionTypeEnum extensionType) const {
            for (uint32_t i = 0; i < ExtensionCount; ++i) {
                if (Extensions[i] != nullptr &&
                    Extensions[i]->Type == static_cast<AxrVulkanExtensionTypeEnum>(extensionType)) {
                    return true;
                }
            }

            return false;
        }

        /// Add the Core Validation api layer
        void addApiLayerCoreValidation() {
            const auto apiLayer = new AxrVulkanApiLayerCoreValidation();
            addApiLayer(reinterpret_cast<AxrVulkanApiLayer_T>(apiLayer));
        }

        /// Add the Debug Utils extension
        /// @param severityFlags The debug severity flags to log
        /// @param typeFlags The debug type flags to log
        void addExtensionDebugUtils(
            const axr::VulkanDebugUtilsSeverityFlags_T severityFlags,
            const axr::VulkanDebugUtilsTypeFlags_T typeFlags
        ) {
            const auto extension = new AxrVulkanExtensionDebugUtils(
                {
                    .SeverityFlags = static_cast<AxrVulkanDebugUtilsSeverityFlags_T>(severityFlags),
                    .TypeFlags = static_cast<AxrVulkanDebugUtilsTypeFlags_T>(typeFlags)
                }
            );
            addExtension(reinterpret_cast<AxrVulkanExtension_T>(extension));
        }

        /// Add the Swapchain extension
        void addExtensionSwapchain() {
            const auto extension = new AxrVulkanExtensionSwapchain();
            addExtension(reinterpret_cast<AxrVulkanExtension_T>(extension));
        }

        /// Add the Surface extension
        void addExtensionSurface() {
            const auto extension = new AxrVulkanExtensionSurface();
            addExtension(reinterpret_cast<AxrVulkanExtension_T>(extension));
        }

#ifdef AXR_USE_PLATFORM_WIN32
        /// Add the Win32 Surface extension
        void addExtensionWin32Surface() {
            const auto extension = new AxrVulkanExtensionWin32Surface();
            addExtension(reinterpret_cast<AxrVulkanExtension_T>(extension));
        }
#endif

        /// Get a handle to the VulkanApiConfig as an AxrVulkanApiConfig
        /// @returns This as an AxrVulkanApiConfig
        const AxrVulkanApiConfig* toRaw() const {
            return reinterpret_cast<const AxrVulkanApiConfig*>(this);
        }

        /// Get a handle to the VulkanApiConfig as an AxrVulkanApiConfig
        /// @returns This as an AxrVulkanApiConfig
        AxrVulkanApiConfig* toRaw() {
            return reinterpret_cast<AxrVulkanApiConfig*>(this);
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Add the given api layer
        /// @param apiLayer Api layer to add
        void addApiLayer(const AxrVulkanApiLayer_T apiLayer) {
            if (apiLayer == nullptr) {
                axr::logErrorLocation("Api Layer is null.");
                return;
            }

            if (apiLayerExists(static_cast<axr::VulkanApiLayerTypeEnum>(apiLayer->Type))) {
                axr::logErrorLocation("Api Layer already exists.");
                return;
            }

            if (ApiLayerCount + 1 > static_cast<uint32_t>(VulkanApiLayerTypeEnum::End) - 1) {
                axr::logErrorLocation("Api Layers array is full.");
                return;
            }

            if (ApiLayers[ApiLayerCount] != nullptr) {
                axr::logErrorLocation(
                    "This slot should be free. If this error triggered, something went really wrong."
                );
                return;
            }

            ApiLayers[ApiLayerCount] = apiLayer;
            ApiLayerCount++;
        }

        /// Add the given extension
        /// @param extension Extension to add
        void addExtension(const AxrVulkanExtension_T extension) {
            if (extension == nullptr) {
                axr::logErrorLocation("Extension is null.");
                return;
            }

            if (extensionExists(static_cast<axr::VulkanExtensionTypeEnum>(extension->Type))) {
                axr::logErrorLocation("Extension already exists.");
                return;
            }

            if (ExtensionCount + 1 > static_cast<uint32_t>(VulkanExtensionTypeEnum::End) - 1) {
                axr::logErrorLocation("Extensions array is full.");
                return;
            }

            if (Extensions[ExtensionCount] != nullptr) {
                axr::logErrorLocation(
                    "This slot should be free. If this error triggered, something went really wrong."
                );
                return;
            }

            Extensions[ExtensionCount] = extension;
            ExtensionCount++;
        }

        /// Clear all the api layers
        void clearApiLayers() {
            for (uint32_t i = 0; i < static_cast<uint32_t>(VulkanApiLayerTypeEnum::End) - 1; ++i) {
                if (ApiLayers[i] != nullptr) {
                    delete ApiLayers[i];
                    ApiLayers[i] = nullptr;
                }
            }

            ApiLayerCount = 0;
        }

        /// Clear all the extensions
        void clearExtensions() {
            for (uint32_t i = 0; i < static_cast<uint32_t>(VulkanExtensionTypeEnum::End) - 1; ++i) {
                if (Extensions[i] != nullptr) {
                    delete Extensions[i];
                    Extensions[i] = nullptr;
                }
            }

            ExtensionCount = 0;
        }
    };

    static_assert(
        sizeof(AxrVulkanApiConfig) == sizeof(axr::VulkanApiConfig),
        "Original type and wrapper have different size!"
    );
}

#endif
