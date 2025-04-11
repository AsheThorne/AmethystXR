#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr/assets.h>

/// Model
class AxrModel {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrModel();
    /// Constructor
    /// @param config Model config
    AxrModel(const AxrModelConfig& config);
    /// Copy Constructor
    /// @param src Source AxrModel to copy from
    AxrModel(const AxrModel& src);
    /// Move Constructor
    /// @param src Source AxrModel to move from
    AxrModel(AxrModel&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrModel();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrModel to copy from
    AxrModel& operator=(const AxrModel& src);
    /// Move Assignment Operator
    /// @param src Source AxrModel to move from
    AxrModel& operator=(AxrModel&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the model name
    /// @returns The name of the model
    const char* getName() const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given meshes
    /// @param meshesCount Number of meshes in the given array
    /// @param meshes Mesh array to clone
    /// @returns A cloned array of the given meshes
    static AxrMesh* cloneMeshes(uint32_t meshesCount, const AxrMesh* meshes);
    /// Clone the given mesh
    /// @param mesh Mesh to clone
    /// @returns A clone of the given mesh
    static AxrMesh cloneMesh(const AxrMesh& mesh);
    /// Clone the given vertices
    /// @param verticesCount Number of vertices in the given array
    /// @param vertices Vertex array to clone
    /// @returns A cloned array of the given vertices
    static AxrVertex* cloneVertices(uint32_t verticesCount, const AxrVertex* vertices);
    /// Clone the given indices
    /// @param indicesCount Number of indices in the given array
    /// @param indices Index array to clone
    /// @returns A cloned array of the given indices
    static uint32_t* cloneIndices(uint32_t indicesCount, const uint32_t* indices);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_Name;
    const char* m_FilePath;
    uint32_t m_MeshesCount;
    AxrMesh* m_Meshes;

    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
    /// Destroy the meshes
    void destroyMeshes();
    /// Destroy the given mesh
    void destroyMesh(AxrMesh& mesh) const;
};
