#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

/// Mesh RAII wrapper
class AxrMeshRAII {
public:
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    std::vector<AxrVertex> Vertices;
    std::vector<uint32_t> Indices;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrMeshRAII();
    /// Constructor
    /// @param mesh Raw mesh to copy from
    AxrMeshRAII(const AxrMesh& mesh);
    /// Copy Constructor
    /// @param src Source AxrMeshRAII to copy from
    AxrMeshRAII(const AxrMeshRAII& src);
    /// Move Constructor
    /// @param src Source AxrMeshRAII to move from
    AxrMeshRAII(AxrMeshRAII&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrMeshRAII();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrMeshRAII to copy from
    AxrMeshRAII& operator=(const AxrMeshRAII& src);
    /// Move Assignment Operator
    /// @param src Source AxrMeshRAII to move from
    AxrMeshRAII& operator=(AxrMeshRAII&& src) noexcept;
    
    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given mesh
    /// @param mesh Mesh to clone
    /// @returns A clone of the given mesh
    static AxrMesh cloneMesh(const AxrMesh& mesh);
    /// Destroy the given mesh
    /// @param mesh Mesh to destroy
    static void destroyMesh(AxrMesh& mesh);

    /// Clone the given vertices
    /// @param verticesCount Number of vertices in the given array
    /// @param vertices Vertex array to clone
    /// @returns A cloned array of the given vertices
    static AxrVertex* cloneVertices(uint32_t verticesCount, const AxrVertex* vertices);
    /// Destroy the given vertices
    /// @param verticesCount Number of vertices in the given array
    /// @param vertices Vertex array to destroy
    static void destroyVertices(uint32_t& verticesCount, AxrVertex*& vertices);

    /// Clone the given indices
    /// @param indicesCount Number of indices in the given array
    /// @param indices Index array to clone
    /// @returns A cloned array of the given indices
    static uint32_t* cloneIndices(uint32_t indicesCount, const uint32_t* indices);
    /// Destroy the given indices
    /// @param indicesCount Number of indices in the given array
    /// @param indices Index array to destroy
    static void destroyIndices(uint32_t& indicesCount, uint32_t*& indices);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //
    
    /// Convert the given vertices to a c++ vector
    /// @param verticesCount Number of vertices in the given array
    /// @param vertices Vertex array to convert
    /// @returns The given vertices converted to a c++ vector
    static std::vector<AxrVertex> toVector(uint32_t verticesCount, const AxrVertex* vertices);
    /// Convert the given indices to a c++ vector
    /// @param indicesCount Number of indices in the given array
    /// @param indices Index array to convert
    /// @returns The given indices converted to a c++ vector
    static std::vector<uint32_t> toVector(uint32_t indicesCount, const uint32_t* indices);
};
