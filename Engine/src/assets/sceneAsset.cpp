// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "sceneAsset.h"

#include <utility>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrSceneAsset::AxrSceneAsset() = default;

AxrSceneAsset::AxrSceneAsset(const AxrSceneAssetConfig& config) :
    m_ID(config.ID) {
}

AxrSceneAsset::AxrSceneAsset(AxrSceneAsset&& src) noexcept {
    move_internal(std::move(src), true);
}

AxrSceneAsset::~AxrSceneAsset() {
    cleanup();
}

AxrSceneAsset& AxrSceneAsset::operator=(AxrSceneAsset&& src) noexcept {
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

void AxrSceneAsset::cleanup() {
}

void AxrSceneAsset::move_internal(AxrSceneAsset&& src, bool useConstructor) {
    m_ID = src.m_ID;
}
