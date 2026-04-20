// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "shaderAsset.h"

#include <utility>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrShaderAsset::AxrShaderAsset() = default;

AxrShaderAsset::AxrShaderAsset(const AxrShaderAssetConfig& config) {
}

AxrShaderAsset::AxrShaderAsset(AxrShaderAsset&& src) noexcept {
    move_internal(std::move(src), true);
}

AxrShaderAsset::~AxrShaderAsset() {
    cleanup();
}

AxrShaderAsset& AxrShaderAsset::operator=(AxrShaderAsset&& src) noexcept {
    if (this != &src) {
        cleanup();

        move_internal(std::move(src), false);
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrShaderAsset::cleanup() {
}

void AxrShaderAsset::move_internal(AxrShaderAsset&& src, const bool useConstructor) {
}
