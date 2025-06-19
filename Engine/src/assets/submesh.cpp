// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "submesh.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrVertex* axrSubmeshCloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) {
        axrLogErrorLocation("`vertices` is null.");
        return {};
    }

    return AxrSubmeshRAII::cloneVertices(verticesCount, vertices);
}

void axrSubmeshDestroyVertices(uint32_t* verticesCount, AxrVertex** vertices) {
    if (verticesCount == nullptr) {
        axrLogErrorLocation("`indicesCount` is null.");
        return;
    }
    
    if (vertices == nullptr) {
        axrLogErrorLocation("`vertices` is null.");
        return;
    }

    return AxrSubmeshRAII::destroyVertices(*verticesCount, *vertices);
}

uint32_t* axrSubmeshCloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) {
        axrLogErrorLocation("`indices` is null.");
        return {};
    }

    return AxrSubmeshRAII::cloneIndices(indicesCount, indices);
}

void axrSubmeshDestroyIndices(uint32_t* indicesCount, uint32_t** indices) {
    if (indicesCount == nullptr) {
        axrLogErrorLocation("`indicesCount` is null.");
        return;
    }
    
    if (indices == nullptr) {
        axrLogErrorLocation("`indices` is null.");
        return;
    }

    return AxrSubmeshRAII::destroyIndices(*indicesCount, *indices);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrSubmeshRAII::AxrSubmeshRAII() = default;

AxrSubmeshRAII::AxrSubmeshRAII(const AxrSubmesh& submesh) {
    Vertices = toVector(submesh.VerticesCount, submesh.Vertices);
    Indices = toVector(submesh.IndicesCount, submesh.Indices);
}

AxrSubmeshRAII::AxrSubmeshRAII(const AxrSubmeshRAII& src) {
    Vertices = src.Vertices;
    Indices = src.Indices;
}

AxrSubmeshRAII::AxrSubmeshRAII(AxrSubmeshRAII&& src) noexcept {
    Vertices = std::move(src.Vertices);
    Indices = std::move(src.Indices);
}

AxrSubmeshRAII::~AxrSubmeshRAII() {
    cleanup();
}

AxrSubmeshRAII& AxrSubmeshRAII::operator=(const AxrSubmeshRAII& src) {
    if (this != &src) {
        cleanup();

        Vertices = src.Vertices;
        Indices = src.Indices;
    }

    return *this;
}

AxrSubmeshRAII& AxrSubmeshRAII::operator=(AxrSubmeshRAII&& src) noexcept {
    if (this != &src) {
        cleanup();

        Vertices = std::move(src.Vertices);
        Indices = std::move(src.Indices);
    }

    return *this;
}

// ---- Public Static Functions ----

AxrSubmesh AxrSubmeshRAII::cloneSubmesh(const AxrSubmesh& submesh) {
    return AxrSubmesh{
        .VerticesCount = submesh.VerticesCount,
        .Vertices = cloneVertices(submesh.VerticesCount, submesh.Vertices),
        .IndicesCount = submesh.IndicesCount,
        .Indices = cloneIndices(submesh.IndicesCount, submesh.Indices),
    };
}

void AxrSubmeshRAII::destroySubmesh(AxrSubmesh& submesh) {
    destroyVertices(submesh.VerticesCount, submesh.Vertices);
    destroyIndices(submesh.IndicesCount, submesh.Indices);
}

AxrVertex* AxrSubmeshRAII::cloneVertices(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) return nullptr;

    AxrVertex* newVertices = new AxrVertex[verticesCount];
    for (uint32_t i = 0; i < verticesCount; ++i) {
        newVertices[i] = vertices[i];
    }

    return newVertices;
}

void AxrSubmeshRAII::destroyVertices(uint32_t& verticesCount, AxrVertex*& vertices) {
    verticesCount = 0;
    if (vertices == nullptr) return;

    delete[] vertices;
    vertices = nullptr;
}

uint32_t* AxrSubmeshRAII::cloneIndices(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) return nullptr;

    uint32_t* newIndices = new uint32_t[indicesCount];
    for (uint32_t i = 0; i < indicesCount; ++i) {
        newIndices[i] = indices[i];
    }

    return newIndices;
}

void AxrSubmeshRAII::destroyIndices(uint32_t& indicesCount, uint32_t*& indices) {
    indicesCount = 0;
    if (indices == nullptr) return;

    delete[] indices;
    indices = nullptr;
}

// ---- Private Functions ----

void AxrSubmeshRAII::cleanup() {
    Vertices.clear();
    Indices.clear();
}

// ---- Private Static Functions ----

std::vector<AxrVertex> AxrSubmeshRAII::toVector(const uint32_t verticesCount, const AxrVertex* vertices) {
    if (vertices == nullptr) return {};

    std::vector<AxrVertex> vertexVector = std::vector<AxrVertex>(verticesCount);
    for (uint32_t i = 0; i < verticesCount; ++i) {
        vertexVector[i] = vertices[i];
    }

    return vertexVector;
}

std::vector<uint32_t> AxrSubmeshRAII::toVector(const uint32_t indicesCount, const uint32_t* indices) {
    if (indices == nullptr) return {};

    std::vector<uint32_t> indexVector = std::vector<uint32_t>(indicesCount);
    for (uint32_t i = 0; i < indicesCount; ++i) {
        indexVector[i] = indices[i];
    }

    return indexVector;
}
