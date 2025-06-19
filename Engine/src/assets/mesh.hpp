#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "submesh.hpp"

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
    std::vector<AxrSubmeshRAII> Submeshes;

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

    /// Clone the given submeshes
    /// @param submeshesCount Number of submeshes in the given array
    /// @param submeshes Submesh array to clone
    /// @returns A cloned array of the given submeshes
    static AxrSubmesh* cloneSubmeshes(uint32_t submeshesCount, const AxrSubmesh* submeshes);
    /// Destroy the given submeshes
    /// @param submeshesCount Number of submeshes in the given array
    /// @param submeshes Submesh array to destroy
    static void destroySubmeshes(uint32_t& submeshesCount, AxrSubmesh*& submeshes);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //
    
    /// Convert the given submeshes to a c++ vector
    /// @param submeshesCount Number of submeshes in the given array
    /// @param submeshes Submesh array to convert
    /// @returns The given submeshes converted to a c++ vector
    static std::vector<AxrSubmeshRAII> toVector(uint32_t submeshesCount, const AxrSubmesh* submeshes);
};
