#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

/// Submesh RAII wrapper
class AxrSubmeshRAII {
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
    AxrSubmeshRAII();
    /// Constructor
    /// @param submesh Raw submesh to copy from
    AxrSubmeshRAII(const AxrSubmesh& submesh);
    /// Copy Constructor
    /// @param src Source AxrSubmeshRAII to copy from
    AxrSubmeshRAII(const AxrSubmeshRAII& src);
    /// Move Constructor
    /// @param src Source AxrSubmeshRAII to move from
    AxrSubmeshRAII(AxrSubmeshRAII&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrSubmeshRAII();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrSubmeshRAII to copy from
    AxrSubmeshRAII& operator=(const AxrSubmeshRAII& src);
    /// Move Assignment Operator
    /// @param src Source AxrSubmeshRAII to move from
    AxrSubmeshRAII& operator=(AxrSubmeshRAII&& src) noexcept;
    
    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given submesh
    /// @param submesh Submesh to clone
    /// @returns A clone of the given submesh
    static AxrSubmesh cloneSubmesh(const AxrSubmesh& submesh);
    /// Destroy the given submesh
    /// @param submesh Submesh to destroy
    static void destroySubmesh(AxrSubmesh& submesh);

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
