#pragma once
#include "axr-hpp/application.hpp"

class SponzaScene {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    SponzaScene(axr::Application& app);
    /// Copy Constructor
    /// @param src Source SponzaScene to copy from
    SponzaScene(const SponzaScene& src) = delete;
    /// Move Constructor
    /// @param src Source SponzaScene to move from
    SponzaScene(SponzaScene&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~SponzaScene();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source SponzaScene to copy from
    SponzaScene& operator=(const SponzaScene& src) = delete;
    /// Move Assignment Operator
    /// @param src Source SponzaScene to move from
    SponzaScene& operator=(SponzaScene&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    axr::Result setup();
    axr::Result loadScene() const;
    axr::Result setAsActiveScene() const;

    void update();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::string m_SceneName = "SponzaScene";
    std::string m_ModelName = "SponzaModel";
    std::string m_DefaultMaterialName = "SponzaDefaultMaterial";

    axr::Application& m_Application;

    axr::Scene m_Scene;
    axr::Entity_T m_Entity;
    std::vector<std::string> m_ImageSamplerNames;
    std::vector<std::string> m_ImageNames;
    std::vector<std::string> m_MaterialNames;
    std::vector<AxrModelComponent::Mesh::Submesh> m_ComponentSubmeshes;
    std::vector<AxrModelComponent::Mesh> m_ComponentMeshes;
};
