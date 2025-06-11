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

    return model->getName().c_str();
}

AxrResult axrModelSetData(const AxrModel_T model, const uint32_t meshesCount, const AxrMesh* meshes) {
    if (model == nullptr) {
        axrLogErrorLocation("`model` is null.");
        return AXR_ERROR;
    }

    return model->setData(meshesCount, meshes);
}


// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrModel::AxrModel() = default;

AxrModel::AxrModel(const AxrModelConfig& config):
    m_Name(config.Name),
    m_FilePath(config.FilePath) {
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
    src.m_FilePath = "";
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
        src.m_FilePath = "";
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrModel::getName() const {
    return m_Name;
}

AxrResult AxrModel::setData(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) {
        axrLogErrorLocation("Meshes are null.");
        return AXR_ERROR;
    }

    std::vector<AxrMeshRAII> meshVector = std::vector<AxrMeshRAII>(meshesCount);
    for (uint32_t i = 0; i < meshesCount; ++i) {
        meshVector[i] = AxrMeshRAII(meshes[i]);
    }

    m_Meshes = meshVector;
    return AXR_SUCCESS;
}

bool AxrModel::isLoaded() const {
    return !m_Meshes.empty();
}

AxrResult AxrModel::loadFile() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (isLoaded()) {
        return AXR_SUCCESS;
    }

    if (m_FilePath.empty()) {
        axrLogErrorLocation("File path is empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // TODO: implement this
    // return axrLoadModel(m_FilePath, m_Meshes);
    return AXR_ERROR;
}

void AxrModel::unloadFile() const {
    // Don't clear the meshes if there is no original data file to be loaded again
    if (m_FilePath.empty()) return;

    m_Meshes.clear();
}

const std::vector<AxrMeshRAII>& AxrModel::getMeshes() const {
    return m_Meshes;
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
    m_FilePath = "";
    m_Meshes.clear();
}
