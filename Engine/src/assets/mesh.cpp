// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "mesh.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrVertex* axrMeshCloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) {
        axrLogErrorLocation("`vertices` is null.");
        return {};
    }

    return AxrMeshRAII::cloneVertices(verticesCount, vertices);
}

void axrMeshDestroyVertices(uint32_t* verticesCount, AxrVertex** vertices) {
    if (verticesCount == nullptr) {
        axrLogErrorLocation("`indicesCount` is null.");
        return;
    }
    
    if (vertices == nullptr) {
        axrLogErrorLocation("`vertices` is null.");
        return;
    }

    return AxrMeshRAII::destroyVertices(*verticesCount, *vertices);
}

uint32_t* axrMeshCloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) {
        axrLogErrorLocation("`indices` is null.");
        return {};
    }

    return AxrMeshRAII::cloneIndices(indicesCount, indices);
}

void axrMeshDestroyIndices(uint32_t* indicesCount, uint32_t** indices) {
    if (indicesCount == nullptr) {
        axrLogErrorLocation("`indicesCount` is null.");
        return;
    }
    
    if (indices == nullptr) {
        axrLogErrorLocation("`indices` is null.");
        return;
    }

    return AxrMeshRAII::destroyIndices(*indicesCount, *indices);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrMeshRAII::AxrMeshRAII() = default;

AxrMeshRAII::AxrMeshRAII(const AxrMesh& mesh) {
    Vertices = toVector(mesh.VerticesCount, mesh.Vertices);
    Indices = toVector(mesh.IndicesCount, mesh.Indices);
}

AxrMeshRAII::AxrMeshRAII(const AxrMeshRAII& src) {
    Vertices = src.Vertices;
    Indices = src.Indices;
}

AxrMeshRAII::AxrMeshRAII(AxrMeshRAII&& src) noexcept {
    Vertices = std::move(src.Vertices);
    Indices = std::move(src.Indices);
}

AxrMeshRAII::~AxrMeshRAII() {
    cleanup();
}

AxrMeshRAII& AxrMeshRAII::operator=(const AxrMeshRAII& src) {
    if (this != &src) {
        cleanup();

        Vertices = src.Vertices;
        Indices = src.Indices;
    }

    return *this;
}

AxrMeshRAII& AxrMeshRAII::operator=(AxrMeshRAII&& src) noexcept {
    if (this != &src) {
        cleanup();

        Vertices = std::move(src.Vertices);
        Indices = std::move(src.Indices);
    }

    return *this;
}

// ---- Public Static Functions ----

AxrMesh AxrMeshRAII::cloneMesh(const AxrMesh& mesh) {
    return AxrMesh{
        .VerticesCount = mesh.VerticesCount,
        .Vertices = cloneVertices(mesh.VerticesCount, mesh.Vertices),
        .IndicesCount = mesh.IndicesCount,
        .Indices = cloneIndices(mesh.IndicesCount, mesh.Indices),
    };
}

void AxrMeshRAII::destroyMesh(AxrMesh& mesh) {
    destroyVertices(mesh.VerticesCount, mesh.Vertices);
    destroyIndices(mesh.IndicesCount, mesh.Indices);
}

AxrVertex* AxrMeshRAII::cloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) return nullptr;

    AxrVertex* newVertices = new AxrVertex[verticesCount];
    for (uint32_t i = 0; i < verticesCount; ++i) {
        newVertices[i] = vertices[i];
    }

    return newVertices;
}

void AxrMeshRAII::destroyVertices(uint32_t& verticesCount, AxrVertex*& vertices) {
    verticesCount = 0;
    if (vertices == nullptr) return;

    delete[] vertices;
    vertices = nullptr;
}

uint32_t* AxrMeshRAII::cloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) return nullptr;

    uint32_t* newIndices = new uint32_t[indicesCount];
    for (uint32_t i = 0; i < indicesCount; ++i) {
        newIndices[i] = indices[i];
    }

    return newIndices;
}

void AxrMeshRAII::destroyIndices(uint32_t& indicesCount, uint32_t*& indices) {
    indicesCount = 0;
    if (indices == nullptr) return;

    delete[] indices;
    indices = nullptr;
}

// ---- Private Functions ----

void AxrMeshRAII::cleanup() {
    Vertices.clear();
    Indices.clear();
}

// ---- Private Static Functions ----

std::vector<AxrVertex> AxrMeshRAII::toVector(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) return {};

    std::vector<AxrVertex> vertexVector = std::vector<AxrVertex>(verticesCount);
    for (uint32_t i = 0; i < verticesCount; ++i) {
        vertexVector[i] = vertices[i];
    }

    return vertexVector;
}

std::vector<uint32_t> AxrMeshRAII::toVector(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) return {};

    std::vector<uint32_t> indexVector = std::vector<uint32_t>(indicesCount);
    for (uint32_t i = 0; i < indicesCount; ++i) {
        indexVector[i] = indices[i];
    }

    return indexVector;
}
