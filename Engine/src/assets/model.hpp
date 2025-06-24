#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr/assets.h>
#include "mesh.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

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
    [[nodiscard]] const std::string& getName() const;

    /// Set the mesh data for the model
    /// @param meshCount Number of meshes in the array
    /// @param meshes Meshes array
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setData(uint32_t meshCount, const AxrMesh* meshes);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Check if this model is loaded
    /// @returns True if this model is loaded
    [[nodiscard]] bool isLoaded() const;
    /// Load this model file
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadFile() const;
    /// Unload this model file
    void unloadFile() const;

    /// Get the model meshes
    /// @returns The model meshes
    [[nodiscard]] const std::vector<AxrMeshRAII>& getMeshes() const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Clone the given meshes
    /// @param meshCount Number of meshes in the given array
    /// @param meshes Mesh array to clone
    /// @returns A cloned array of the given meshes
    [[nodiscard]] static AxrMesh* cloneMeshes(uint32_t meshCount, const AxrMesh* meshes);
    /// Destroy the given meshes
    /// @param meshCount Number of meshes in the given array
    /// @param meshes Mesh array to destroy
    static void destroyMeshes(uint32_t& meshCount, AxrMesh*& meshes);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_FilePath;
    mutable std::vector<AxrMeshRAII> m_Meshes;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
