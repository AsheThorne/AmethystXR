#pragma once
#include "axr-hpp/application.hpp"

class TestScene {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    TestScene(axr::Application& app);
    /// Copy Constructor
    /// @param src Source TestScene to copy from
    TestScene(const TestScene& src) = delete;
    /// Move Constructor
    /// @param src Source TestScene to move from
    TestScene(TestScene&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~TestScene();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source TestScene to copy from
    TestScene& operator=(const TestScene& src) = delete;
    /// Move Assignment Operator
    /// @param src Source TestScene to move from
    TestScene& operator=(TestScene&& src) noexcept = delete;

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
    std::string m_SceneName = "TestScene";
    std::string m_ModelName = "Cube";
    std::string m_ImageName = "UvTesterImage";
    std::string m_MaterialName = "MyMaterial";

    axr::Application& m_Application;

    axr::Scene m_Scene;
    axr::Entity_T m_Entity;
    std::vector<AxrModelComponent::Mesh::Submesh> m_ComponentSubmeshes;
    std::vector<AxrModelComponent::Mesh> m_ComponentMeshes;
};
