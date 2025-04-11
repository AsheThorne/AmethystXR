// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "model.hpp"
#include "../utils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrMesh* axrModelCloneMeshes(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) {
        axrLogErrorLocation("`meshes` is null.");
        return nullptr;
    }

    return AxrModel::cloneMeshes(meshesCount, meshes);
}

void axrModelDestroyMeshes(uint32_t* meshesCount, AxrMesh** meshes) {
    if (meshesCount == nullptr) {
        axrLogErrorLocation("`meshesCount` is null.");
        return;
    }

    if (meshes == nullptr) {
        axrLogErrorLocation("`meshes` is null.");
        return;
    }

    return AxrModel::destroyMeshes(*meshesCount, *meshes);
}

const char* axrModelGetName(const AxrModel_T model) {
    if (model == nullptr) {
        axrLogErrorLocation("`model` is null.");
        return "";
    }

    return model->getName();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrModel::AxrModel():
    m_Name(""),
    m_FilePath(nullptr) {
}

AxrModel::AxrModel(const AxrModelConfig& config):
    m_Name(config.Name),
    m_FilePath(config.FilePath) {
    m_Meshes = toVector(config.MeshesCount, config.Meshes);
}

AxrModel::AxrModel(const AxrModel& src) {
    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;
    m_Meshes = src.m_Meshes;
}

AxrModel::AxrModel(AxrModel&& src) noexcept {
    m_Meshes = std::move(src.m_Meshes);

    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;

    src.m_Name = "";
    src.m_FilePath = nullptr;
}

AxrModel::~AxrModel() {
    cleanup();
}

AxrModel& AxrModel::operator=(const AxrModel& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;
        m_Meshes = src.m_Meshes;
    }

    return *this;
}

AxrModel& AxrModel::operator=(AxrModel&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Meshes = std::move(src.m_Meshes);

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;

        src.m_Name = "";
        src.m_FilePath = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

const char* AxrModel::getName() const {
    return m_Name;
}

bool AxrModel::isLoaded() const {
    return !m_Meshes.empty();
}

AxrResult AxrModel::loadFile() const {
    if (axrStringIsEmpty(m_FilePath)) {
        if (m_Meshes.empty()) {
            axrLogErrorLocation("There is no file path for the model and no meshes were assigned.");
            return AXR_ERROR;
        }

        // File path isn't required if mesh data has been provided manually
        return AXR_SUCCESS;
    }

    // TODO: implement this
    // return axrLoadModel(m_FilePath, m_Meshes);
    return AXR_ERROR;
}

void AxrModel::unloadFile() const {
    // Don't clear the meshes if there is no original data file to be loaded again
    if (axrStringIsEmpty(m_FilePath)) {
        return;
    }

    m_Meshes.clear();
}

// ---- Public Static Functions ----

AxrMesh* AxrModel::cloneMeshes(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) return nullptr;

    AxrMesh* newMeshes = new AxrMesh[meshesCount];
    for (uint32_t i = 0; i < meshesCount; ++i) {
        newMeshes[i] = AxrMeshRAII::cloneMesh(meshes[i]);
    }

    return newMeshes;
}

void AxrModel::destroyMeshes(uint32_t& meshesCount, AxrMesh*& meshes) {
    if (meshes != nullptr) {
        for (uint32_t i = 0; i < meshesCount; ++i) {
            AxrMeshRAII::destroyMesh(meshes[i]);
        }

        delete[] meshes;
        meshes = nullptr;
    }

    meshesCount = 0;
}

// ---- Private Functions ----

void AxrModel::cleanup() {
    m_Name = "";
    m_FilePath = nullptr;
    m_Meshes.clear();
}

// ---- Private Static Functions ----

std::vector<AxrMeshRAII> AxrModel::toVector(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) return {};

    std::vector<AxrMeshRAII> meshVector = std::vector<AxrMeshRAII>(meshesCount);
    for (uint32_t i = 0; i < meshesCount; ++i) {
        meshVector[i] = AxrMeshRAII(meshes[i]);
    }

    return meshVector;
}
