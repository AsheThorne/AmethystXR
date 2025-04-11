// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "model.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrVertex* axrMeshCloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) {
        axrLogErrorLocation("`vertices` is null.");
        return {};
    }

    return AxrModel::cloneVertices(verticesCount, vertices);
}

uint32_t* axrMeshCloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) {
        axrLogErrorLocation("`indices` is null.");
        return {};
    }

    return AxrModel::cloneIndices(indicesCount, indices);
}

AxrMesh* axrModelCloneMeshes(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) {
        axrLogErrorLocation("`meshes` is null.");
        return nullptr;
    }

    return AxrModel::cloneMeshes(meshesCount, meshes);
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
    m_FilePath(nullptr),
    m_MeshesCount(0),
    m_Meshes(nullptr) {
}

AxrModel::AxrModel(const AxrModelConfig& config):
    m_Name(config.Name),
    m_FilePath(config.FilePath),
    m_MeshesCount(0),
    m_Meshes(nullptr) {
    m_MeshesCount = config.MeshesCount;
    m_Meshes = cloneMeshes(config.MeshesCount, config.Meshes);
}

AxrModel::AxrModel(const AxrModel& src) {
    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;
    m_MeshesCount = src.m_MeshesCount;
    m_Meshes = cloneMeshes(src.m_MeshesCount, src.m_Meshes);
}

AxrModel::AxrModel(AxrModel&& src) noexcept {
    m_Name = src.m_Name;
    m_FilePath = src.m_FilePath;
    m_MeshesCount = src.m_MeshesCount;
    m_Meshes = src.m_Meshes;

    src.m_Name = "";
    src.m_FilePath = nullptr;
    src.m_MeshesCount = 0;
    src.m_Meshes = nullptr;
}

AxrModel::~AxrModel() {
    cleanup();
}

AxrModel& AxrModel::operator=(const AxrModel& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;
        m_MeshesCount = src.m_MeshesCount;
        m_Meshes = cloneMeshes(src.m_MeshesCount, src.m_Meshes);
    }

    return *this;
}

AxrModel& AxrModel::operator=(AxrModel&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_FilePath = src.m_FilePath;
        m_MeshesCount = src.m_MeshesCount;
        m_Meshes = src.m_Meshes;

        src.m_Name = "";
        src.m_FilePath = nullptr;
        src.m_MeshesCount = 0;
        src.m_Meshes = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

const char* AxrModel::getName() const {
    return m_Name;
}

AxrMesh* AxrModel::cloneMeshes(const uint32_t meshesCount, const AxrMesh* meshes) {
    if (meshes == nullptr) return nullptr;

    AxrMesh* newMeshes = new AxrMesh[meshesCount];
    for (uint32_t i = 0; i < meshesCount; ++i) {
        newMeshes[i] = cloneMesh(meshes[i]);
    }

    return newMeshes;
}

AxrMesh AxrModel::cloneMesh(const AxrMesh& mesh) {
    return AxrMesh{
        .VerticesCount = mesh.VerticesCount,
        .Vertices = cloneVertices(mesh.VerticesCount, mesh.Vertices),
        .IndicesCount = mesh.IndicesCount,
        .Indices = cloneIndices(mesh.IndicesCount, mesh.Indices),
    };
}

AxrVertex* AxrModel::cloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) return nullptr;

    AxrVertex* newVertices = new AxrVertex[verticesCount];
    for (uint32_t i = 0; i < verticesCount; ++i) {
        newVertices[i] = vertices[i];
    }

    return newVertices;
}

uint32_t* AxrModel::cloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) return nullptr;

    uint32_t* newIndices = new uint32_t[indicesCount];
    for (uint32_t i = 0; i < indicesCount; ++i) {
        newIndices[i] = indices[i];
    }

    return newIndices;
}

void AxrModel::cleanup() {
    destroyMeshes();

    m_Name = "";
    m_FilePath = nullptr;
}

void AxrModel::destroyMeshes() {
    for (uint32_t i = 0; i < m_MeshesCount; ++i) {
        destroyMesh(m_Meshes[i]);
    }

    delete[] m_Meshes;
    m_Meshes = nullptr;
    m_MeshesCount = 0;
}

void AxrModel::destroyMesh(AxrMesh& mesh) const {
    delete[] mesh.Vertices;
    mesh.Vertices = nullptr;
    mesh.VerticesCount = 0;

    delete[] mesh.Indices;
    mesh.Indices = nullptr;
    mesh.IndicesCount = 0;
}
