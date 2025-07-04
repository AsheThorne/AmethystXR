#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "axr/xrSystem.h"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "axr/vulkanApi.h"
#endif

template <typename Extension, typename ExtensionTypeEnum>
class AxrExtensionCollection {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// The collection type
    using Collection_T = std::vector<Extension>;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrExtensionCollection() = default;

    /// Copy Constructor
    /// @param src Source AxrExtensionCollection to copy from
    AxrExtensionCollection(const AxrExtensionCollection& src) {
        m_Extensions = clone(src.m_Extensions);
    }

    /// Move Constructor
    /// @param src Source AxrExtensionCollection to move from
    AxrExtensionCollection(AxrExtensionCollection&& src) noexcept {
        m_Extensions = std::move(src.m_Extensions);
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrExtensionCollection() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrExtensionCollection to copy from
    AxrExtensionCollection& operator=(const AxrExtensionCollection& src) {
        if (this != &src) {
            cleanup();

            m_Extensions = clone(src.m_Extensions);
        }

        return *this;
    }

    /// Move Assignment Operator
    /// @param src Source AxrExtensionCollection to move from
    AxrExtensionCollection& operator=(AxrExtensionCollection&& src) noexcept {
        if (this != &src) {
            cleanup();

            m_Extensions = std::move(src.m_Extensions);
        }

        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Clear the collection
    void clear() {
        destroyExtensions();
    }

    /// Check if the collection is empty
    /// @returns True if the collection is empty
    [[nodiscard]] bool empty() const {
        return m_Extensions.empty();
    }

    /// Get the underlining collection
    /// @returns The underlining collection
    [[nodiscard]] Collection_T getCollection() const {
        return m_Extensions;
    }

    /// Check if the given extension type exists
    /// @param extensionType Extension type to check
    /// @returns True if the given extension type exists 
    [[nodiscard]] bool exists(const ExtensionTypeEnum extensionType) const {
        for (const Extension& extension : m_Extensions) {
            if (extension != nullptr && extension->Type == extensionType) return true;
        }

        return false;
    }

    /// Add the given extensions
    /// @param extensions The extensions to add
    void add(const std::vector<Extension>& extensions) {
        add(static_cast<uint32_t>(extensions.size()), extensions.data());
    }

    /// Add the given extensions
    /// @param extensionCount Number of extensions in the given extensions pointer
    /// @param extensions The extensions to add
    void add(const uint32_t extensionCount, const Extension* extensions) {
        for (uint32_t i = 0; i < extensionCount; ++i) {
            if (extensions[i] == nullptr) continue;

            add(extensions[i]);
        }
    }

    /// Add the given extension
    /// @param extension The extension to add
    void add(Extension extension) {
        if (extension == nullptr) return;

        if (exists(extension->Type)) {
            return;
        }

        m_Extensions.push_back(clone(extension));
    }

    /// Get the extension from the given extension type
    /// @param type Extension type
    /// @returns The extension of the given type or nullptr if it doesn't exist
    [[nodiscard]] Extension get(const ExtensionTypeEnum type) const {
        for (const Extension extension : m_Extensions) {
            if (extension != nullptr && extension->Type == type) {
                return extension;
            }
        }

        return nullptr;
    }

    /// Beginning of the collection
    /// @returns Collection iterator
    typename Collection_T::iterator begin() {
        return m_Extensions.begin();
    }

    /// Beginning of the collection
    /// @returns Collection iterator
    typename Collection_T::const_iterator begin() const {
        return m_Extensions.begin();
    }

    /// End of the collection
    /// @returns Collection iterator
    typename Collection_T::iterator end() {
        return m_Extensions.end();
    }

    /// End of the collection
    /// @returns Collection iterator
    typename Collection_T::const_iterator end() const {
        return m_Extensions.end();
    }

    /// Erase an item at the given iterator
    /// @param iterator Iterator to remove
    /// @returns An iterator pointing to the new location of the element that followed the last element
    /// erased by the function call. This is the container end if the operation erased the last element in the sequence.
    typename Collection_T::iterator erase(typename Collection_T::const_iterator iterator) noexcept {
        return m_Extensions.erase(iterator);
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Collection_T m_Extensions;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup this class
    void cleanup() {
        destroyExtensions();
    }

    /// Destroy extensions
    void destroyExtensions() {
        for (Extension& extension : m_Extensions) {
            delete extension;
            extension = nullptr;
        }

        m_Extensions.clear();
    }

    /// Clone all the given extensions
    /// @param extensions Extensions to clone
    /// @returns A collection of cloned extensions
    [[nodiscard]] std::vector<Extension> clone(
        const std::vector<Extension>& extensions
    ) const {
        return clone(static_cast<uint32_t>(extensions.size()), extensions.data());
    }

    /// Clone all the given extensions
    /// @param extensionCount Number of extensions in the given extensions pointer
    /// @param extensions Extensions to clone
    /// @returns A collection of cloned extensions
    [[nodiscard]] std::vector<Extension> clone(
        const uint32_t extensionCount,
        const Extension* extensions
    ) const {
        std::vector<Extension> clonedExtensions{};

        for (uint32_t i = 0; i < extensionCount; ++i) {
            if (extensions[i] == nullptr) continue;

            clonedExtensions.push_back(cloneExtension(extensions[i]));
        }

        return clonedExtensions;
    }

    /// Clone the given extension
    /// @param extension Extension to clone
    /// @returns A clone of the extension
    [[nodiscard]] Extension clone(Extension extension) const {
        axrLogErrorLocation("Unknown extension type.");
        return nullptr;
    }
};

// ----------------------------------------- //
// Specialized Function Templates
// ----------------------------------------- //

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
template <>
inline AxrVulkanExtension_T AxrExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>::clone(
    const AxrVulkanExtension_T extension
) const {
    if (extension == nullptr) return nullptr;

    switch (extension->Type) {
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionDebugUtils*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionDebugUtils(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionSwapchain*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionSwapchain(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionSurface*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionSurface(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
#ifdef AXR_USE_PLATFORM_WIN32
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionWin32Surface*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionWin32Surface(castExtension));
#else
            axrLogErrorLocation("Win32 surface extension is not supported.");
            return nullptr;
#endif
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}

template <>
inline AxrVulkanApiLayer_T AxrExtensionCollection<AxrVulkanApiLayer_T, AxrVulkanApiLayerTypeEnum>::clone(
    const AxrVulkanApiLayer_T extension
) const {
    if (extension == nullptr) return nullptr;

    switch (extension->Type) {
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            const auto castExtension = *reinterpret_cast<AxrVulkanApiLayerCoreValidation*>(extension);
            return reinterpret_cast<AxrVulkanApiLayer_T>(new AxrVulkanApiLayerCoreValidation(castExtension));
        }
        case AXR_VULKAN_API_LAYER_TYPE_END:
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}
#endif

template <>
inline AxrXrExtension_T AxrExtensionCollection<AxrXrExtension_T, AxrXrExtensionTypeEnum>::clone(
    const AxrXrExtension_T extension
) const {
    if (extension == nullptr) return nullptr;

    switch (extension->Type) {
        case AXR_XR_EXTENSION_TYPE_DEBUG_UTILS: {
            const auto castExtension = *reinterpret_cast<AxrXrExtensionDebugUtils*>(extension);
            return reinterpret_cast<AxrXrExtension_T>(new AxrXrExtensionDebugUtils(castExtension));
        }
        case AXR_XR_EXTENSION_TYPE_VULKAN_ENABLE: {
            const auto castExtension = *reinterpret_cast<AxrXrExtensionVulkanEnable*>(extension);
            return reinterpret_cast<AxrXrExtension_T>(new AxrXrExtensionVulkanEnable(castExtension));
        }
        case AXR_XR_EXTENSION_TYPE_END:
        case AXR_XR_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}

template <>
inline AxrXrApiLayer_T AxrExtensionCollection<AxrXrApiLayer_T, AxrXrApiLayerTypeEnum>::clone(
    const AxrXrApiLayer_T extension
) const {
    if (extension == nullptr) return nullptr;

    switch (extension->Type) {
        case AXR_XR_API_LAYER_TYPE_CORE_VALIDATION: {
            const auto castExtension = *reinterpret_cast<AxrXrApiLayerCoreValidation*>(extension);
            return reinterpret_cast<AxrXrApiLayer_T>(new AxrXrApiLayerCoreValidation(castExtension));
        }
        case AXR_XR_API_LAYER_TYPE_END:
        case AXR_XR_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}
