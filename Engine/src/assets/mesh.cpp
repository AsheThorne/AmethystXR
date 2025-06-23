// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "mesh.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrSubmesh* axrMeshCloneSubmeshes(const uint32_t submeshesCount, const AxrSubmesh* submeshes) {
    if (submeshes == nullptr) {
        axrLogErrorLocation("`submeshes` is null.");
        return {};
    }

    return AxrMeshRAII::cloneSubmeshes(submeshesCount, submeshes);
}

void axrMeshDestroySubmeshes(uint32_t* submeshesCount, AxrSubmesh** submeshes) {
    if (submeshesCount == nullptr) {
        axrLogErrorLocation("`submeshesCount` is null.");
        return;
    }

    if (submeshes == nullptr) {
        axrLogErrorLocation("`submeshes` is null.");
        return;
    }

    return AxrMeshRAII::destroySubmeshes(*submeshesCount, *submeshes);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrMeshRAII::AxrMeshRAII() = default;

AxrMeshRAII::AxrMeshRAII(const AxrMesh& mesh) {
    Submeshes = toVector(mesh.SubmeshCount, mesh.Submeshes);
}

AxrMeshRAII::AxrMeshRAII(const AxrMeshRAII& src) {
    Submeshes = src.Submeshes;
}

AxrMeshRAII::AxrMeshRAII(AxrMeshRAII&& src) noexcept {
    Submeshes = std::move(src.Submeshes);
}

AxrMeshRAII::~AxrMeshRAII() {
    cleanup();
}

AxrMeshRAII& AxrMeshRAII::operator=(const AxrMeshRAII& src) {
    if (this != &src) {
        cleanup();

        Submeshes = src.Submeshes;
    }

    return *this;
}

AxrMeshRAII& AxrMeshRAII::operator=(AxrMeshRAII&& src) noexcept {
    if (this != &src) {
        cleanup();

        Submeshes = std::move(src.Submeshes);
    }

    return *this;
}

// ---- Public Static Functions ----

AxrMesh AxrMeshRAII::cloneMesh(const AxrMesh& mesh) {
    return AxrMesh{
        .SubmeshCount = mesh.SubmeshCount,
        .Submeshes = cloneSubmeshes(mesh.SubmeshCount, mesh.Submeshes),
    };
}

void AxrMeshRAII::destroyMesh(AxrMesh& mesh) {
    destroySubmeshes(mesh.SubmeshCount, mesh.Submeshes);
}

AxrSubmesh* AxrMeshRAII::cloneSubmeshes(const uint32_t submeshesCount, const AxrSubmesh* submeshes) {
    if (submeshes == nullptr) return nullptr;

    AxrSubmesh* newSubmeshes = new AxrSubmesh[submeshesCount]{};
    for (uint32_t i = 0; i < submeshesCount; ++i) {
        newSubmeshes[i] = AxrSubmeshRAII::cloneSubmesh(submeshes[i]);
    }

    return newSubmeshes;
}

void AxrMeshRAII::destroySubmeshes(uint32_t& submeshesCount, AxrSubmesh*& submeshes) {
    if (submeshes == nullptr) return;

    for (uint32_t i = 0; i < submeshesCount; ++i) {
        AxrSubmeshRAII::destroySubmesh(submeshes[i]);
    }
    delete[] submeshes;
    submeshes = nullptr;
    submeshesCount = 0;
}

// ---- Private Functions ----

void AxrMeshRAII::cleanup() {
    Submeshes.clear();
}

// ---- Private Static Functions ----

std::vector<AxrSubmeshRAII> AxrMeshRAII::toVector(const uint32_t submeshesCount, const AxrSubmesh* submeshes) {
    if (submeshes == nullptr) return {};

    std::vector<AxrSubmeshRAII> submeshVector = std::vector<AxrSubmeshRAII>(submeshesCount);
    for (uint32_t i = 0; i < submeshesCount; ++i) {
        submeshVector[i] = AxrSubmeshRAII(submeshes[i]);
    }

    return submeshVector;
}
