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
        typename AxrArray<Extension, Cap>::Iterator iterator = find(extension.Type);
        if (iterator != AxrArray<Extension, Cap>::end()) {
            axrLogWarning("Extension already exists. Overwriting existing entry with given data.");

            *iterator = extension;
            return;
        }

        AxrArray<Extension, Cap>::pushBack(extension);
    }

    /// Find an iterator to the first instance of the extension with the given type
    /// @param type Extension type
    /// @return An iterator to the first instance of the extension with the given type. Or end() if it wasn't found
    [[nodiscard]] AxrArray<Extension, Cap>::Iterator find(const ExtensionType type) {
        for (typename AxrArray<Extension, Cap>::Iterator it = AxrArray<Extension, Cap>::begin(),
                                                         e = AxrArray<Extension, Cap>::end();
             it != e;
             ++it) {
            if (it->Type == type) {
                return it;
            }
        }

        return AxrArray<Extension, Cap>::end();
    }

    /// Find an iterator to the first instance of the extension with the given type
    /// @param type Extension type
    /// @return An iterator to the first instance of the extension with the given type. Or end() if it wasn't found
    [[nodiscard]] AxrArray<Extension, Cap>::ConstIterator find(const ExtensionType type) const {
        for (typename AxrArray<Extension, Cap>::ConstIterator it = AxrArray<Extension, Cap>::begin(),
                                                              e = AxrArray<Extension, Cap>::end();
             it != e;
             ++it) {
            if (it->Type == type) {
                return it;
            }
        }

        return AxrArray<Extension, Cap>::end();
    }

    /// Check if the given extension type exists
    /// @param type Extension type
    /// @return True if the given extension type exists
    [[nodiscard]] bool exists(const ExtensionType type) const {
        typename AxrArray<Extension, Cap>::ConstIterator iterator = find(type);
        return iterator != AxrArray<Extension, Cap>::end();
    }
};
