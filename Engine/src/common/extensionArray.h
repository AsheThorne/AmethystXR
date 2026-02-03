#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "array.h"

/// Check if the given extension is valid for the AxrExtensionArray
/// @tparam Extension Extension
template<typename Extension>
concept AxrIsValidExtension = requires(Extension extension) { extension.Type; };

/// Axr Extension Array. Intended for Vulkan/Xr extensions/api layers.
/// @tparam Extension Extension
/// @tparam Cap Array capacity
template<typename Extension, size_t Cap>
    requires AxrIsValidExtension<Extension>
class AxrExtensionArray final : public AxrArray<Extension, Cap> {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Extension type enum
    using ExtensionType = std::remove_const_t<decltype(Extension::Type)>;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Add a new extension to the end of the array. If a duplicate extension type is added, the existing one is
    /// overwritten.
    /// @param extension New extension to add
    void pushBack(const Extension& extension) {
        const size_t existingIndex = findIndex(extension.Type);
        if (existingIndex != AxrArray<Extension, Cap>::Capacity) {
            axrLogWarning("Extension already exists. Overwriting existing entry with given data.");

            AxrArray<Extension, Cap>::Data[existingIndex] = extension;
            return;
        }

        AxrArray<Extension, Cap>::pushBack(extension);
    }

    /// Find the index for the given extension type
    /// @param type Extension type
    /// @return The index of the found type or `Capacity` if it wasn't found;
    [[nodiscard]] size_t findIndex(const ExtensionType type) const {
        for (size_t i = 0; i < AxrArray<Extension, Cap>::Size; ++i) {
            if (AxrArray<Extension, Cap>::Data[i].Type == type) {
                return i;
            }
        }

        return AxrArray<Extension, Cap>::Capacity;
    }

    /// Check if the given extension type exists
    /// @param type Extension type
    /// @return True if the given extension type exists
    [[nodiscard]] bool exists(const ExtensionType type) const {
        const size_t index = findIndex(type);
        return index != AxrArray<Extension, Cap>::Capacity;
    }
};
