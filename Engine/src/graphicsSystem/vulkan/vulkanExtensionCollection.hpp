#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "axr/vulkanApi.h"

template <typename Extension, typename ExtensionTypeEnum>
class AxrVulkanExtensionCollection {
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
    AxrVulkanExtensionCollection() = default;
    /// Copy Constructor
    /// @param src Source AxrVulkanExtensionCollection to copy from
    AxrVulkanExtensionCollection(const AxrVulkanExtensionCollection& src);
    /// Move Constructor
    /// @param src Source AxrVulkanExtensionCollection to move from
    AxrVulkanExtensionCollection(AxrVulkanExtensionCollection&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanExtensionCollection();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanExtensionCollection to copy from
    AxrVulkanExtensionCollection& operator=(const AxrVulkanExtensionCollection& src);
    /// Move Assignment Operator
    /// @param src Source AxrVulkanExtensionCollection to move from
    AxrVulkanExtensionCollection& operator=(AxrVulkanExtensionCollection&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Clear the collection
    void clear();
    /// Check if the collection is empty
    /// @returns True if the collection is empty
    [[nodiscard]] bool empty() const;
    /// Get the underlining collection
    /// @returns The underlining collection
    [[nodiscard]] Collection_T getCollection() const;

    /// Check if the given extension type exists
    /// @param extensionType Extension type to check
    /// @returns True if the given extension type exists 
    [[nodiscard]] bool exists(ExtensionTypeEnum extensionType) const;
    /// Add the given extensions
    /// @param extensions The extensions to add
    void add(const std::vector<Extension>& extensions);
    /// Add the given extensions
    /// @param extensionsCount Number of extensions in the given extensions pointer
    /// @param extensions The extensions to add
    void add(uint32_t extensionsCount, const Extension* extensions);
    /// Add the given extension
    /// @param extension The extension to add
    void add(Extension extension);
    /// Get the extension from the given extension type
    /// @param type Extension type
    /// @returns The extension of the given type or nullptr if it doesn't exist
    [[nodiscard]] Extension get(ExtensionTypeEnum type) const;

    /// Beginning of the collection
    /// @returns Collection iterator
    typename Collection_T::iterator begin();
    /// Beginning of the collection
    /// @returns Collection iterator
    typename Collection_T::const_iterator begin() const;
    /// End of the collection
    /// @returns Collection iterator
    typename Collection_T::iterator end();
    /// End of the collection
    /// @returns Collection iterator
    typename Collection_T::const_iterator end() const;
    /// Erase an item at the given iterator
    /// @param iterator Iterator to remove
    /// @returns An iterator pointing to the new location of the element that followed the last element
    /// erased by the function call. This is the container end if the operation erased the last element in the sequence.
    typename Collection_T::iterator erase(typename Collection_T::const_iterator iterator) noexcept;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Collection_T m_Extensions;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup this class
    void cleanup();
    /// Destroy extensions
    void destroyExtensions();

    /// Clone all the given vulkan extensions
    /// @param extensions Extensions to clone
    /// @returns A collection of cloned extensions
    [[nodiscard]] std::vector<Extension> clone(
        const std::vector<Extension>& extensions
    ) const;
    /// Clone all the given vulkan extensions
    /// @param extensionsCount Number of extensions in the given extensions pointer
    /// @param extensions Extensions to clone
    /// @returns A collection of cloned extensions
    [[nodiscard]] std::vector<Extension> clone(
        uint32_t extensionsCount,
        const Extension* extensions
    ) const;
    /// Clone the given extension
    /// @param extension Extension to clone
    /// @returns A clone of the extension
    [[nodiscard]] Extension clone(Extension extension) const;
};

// ----------------------------------------- //
// Function Implementations
// ----------------------------------------- //

// ---- Special Functions ----

template <typename Extension, typename ExtensionTypeEnum>
AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::AxrVulkanExtensionCollection(
    const AxrVulkanExtensionCollection& src
) {
    m_Extensions = cloneExtensions(src.m_Extensions);
}

template <typename Extension, typename ExtensionTypeEnum>
AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::AxrVulkanExtensionCollection(
    AxrVulkanExtensionCollection&& src
) noexcept {
    m_Extensions = std::move(src.m_Extensions);
}

template <typename Extension, typename ExtensionTypeEnum>
AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::~AxrVulkanExtensionCollection() {
    cleanup();
}

template <typename Extension, typename ExtensionTypeEnum>
AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>& AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::
operator=(const AxrVulkanExtensionCollection& src) {
    if (this != &src) {
        cleanup();

        m_Extensions = cloneExtensions(src.m_Extensions);
    }

    return *this;
}

template <typename Extension, typename ExtensionTypeEnum>
AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>& AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::
operator=(AxrVulkanExtensionCollection&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Extensions = std::move(src.m_Extensions);
    }

    return *this;
}

// ---- Public Functions ----

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::clear() {
    cleanup();
}

template <typename Extension, typename ExtensionTypeEnum>
bool AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::empty() const {
    return m_Extensions.empty();
}

template <typename Extension, typename ExtensionTypeEnum>
typename AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::Collection_T AxrVulkanExtensionCollection<Extension
    , ExtensionTypeEnum>::getCollection() const {
    return m_Extensions;
}

template <typename Extension, typename ExtensionTypeEnum>
bool AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::exists(ExtensionTypeEnum extensionType) const {
    for (const Extension extension : m_Extensions) {
        if (extension != nullptr && extension->Type == extensionType) return true;
    }

    return false;
}

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::add(const std::vector<Extension>& extensions) {
    addExtensions(static_cast<uint32_t>(extensions.size()), extensions.data());
}

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::add(
    const uint32_t extensionsCount,
    const Extension* extensions
) {
    for (uint32_t i = 0; i < extensionsCount; ++i) {
        if (extensions[i] == nullptr) continue;

        add(extensions[i]);
    }
}

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::add(Extension extension) {
    if (extension == nullptr) return;

    if (exists(extension->Type)) {
        return;
    }

    m_Extensions.push_back(clone(extension));
}

template <typename Extension, typename ExtensionTypeEnum>
Extension AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::get(ExtensionTypeEnum type) const {
    for (const Extension extension : m_Extensions) {
        if (extension != nullptr && extension->Type == type) {
            return extension;
        }
    }

    return nullptr;
}

template <typename Extension, typename ExtensionTypeEnum>
typename std::vector<Extension>::iterator AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::begin() {
    return m_Extensions.begin();
}

template <typename Extension, typename ExtensionTypeEnum>
typename std::vector<Extension>::const_iterator AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::
begin() const {
    return m_Extensions.begin();
}

template <typename Extension, typename ExtensionTypeEnum>
typename std::vector<Extension>::iterator AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::end() {
    return m_Extensions.end();
}

template <typename Extension, typename ExtensionTypeEnum>
typename std::vector<Extension>::const_iterator AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::
end() const {
    return m_Extensions.end();
}

template <typename Extension, typename ExtensionTypeEnum>
typename std::vector<Extension>::iterator AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::erase(
    typename Collection_T::const_iterator iterator
) noexcept {
    return m_Extensions.erase(iterator);
}

// ---- Private Functions ----

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::cleanup() {
    destroyExtensions();
}

template <typename Extension, typename ExtensionTypeEnum>
void AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::destroyExtensions() {
    for (Extension& extension : m_Extensions) {
        delete extension;
        extension = nullptr;
    }

    m_Extensions.clear();
}

template <typename Extension, typename ExtensionTypeEnum>
std::vector<Extension> AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::clone(
    const std::vector<Extension>& extensions
) const {
    return cloneExtensions(static_cast<uint32_t>(extensions.size()), extensions.data());
}

template <typename Extension, typename ExtensionTypeEnum>
std::vector<Extension> AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::clone(
    const uint32_t extensionsCount,
    const Extension* extensions
) const {
    std::vector<Extension> clonedExtensions{};

    for (uint32_t i = 0; i < extensionsCount; ++i) {
        if (extensions[i] == nullptr) continue;

        clonedExtensions.push_back(cloneExtension(extensions[i]));
    }

    return clonedExtensions;
}

template <typename Extension, typename ExtensionTypeEnum>
Extension AxrVulkanExtensionCollection<Extension, ExtensionTypeEnum>::clone(Extension extension) const {
    axrLogErrorLocation("Unknown extension type.");
    return nullptr;
}

template <>
inline AxrVulkanExtension_T AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>::clone(
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
inline AxrVulkanApiLayer_T AxrVulkanExtensionCollection<AxrVulkanApiLayer_T, AxrVulkanApiLayerTypeEnum>::clone(
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
