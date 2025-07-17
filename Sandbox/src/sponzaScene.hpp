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
    axr::Application& m_Application;

    std::string m_SceneName = "SponzaScene";
    axr::Scene m_Scene;
    axr::Entity_T m_Entity;
    axr::Entity_T m_CameraEntity;
    axr::Entity_T m_XrHeadEntity;
    axr::Entity_T m_XrHandEntity;
    std::vector<AxrModelComponent::Mesh::Submesh> m_ComponentSubmeshes;
    std::vector<AxrModelComponent::Mesh> m_ComponentMeshes;
    std::vector<AxrModelComponent::Mesh::Submesh> m_HeadComponentSubmeshes;
    std::vector<AxrModelComponent::Mesh> m_HeadComponentMeshes;
    std::vector<AxrModelComponent::Mesh::Submesh> m_HandComponentSubmeshes;
    std::vector<AxrModelComponent::Mesh> m_HandComponentMeshes;
};
