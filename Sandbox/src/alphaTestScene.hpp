#pragma once
#include "axr-hpp/application.hpp"

class AlphaTestScene {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AlphaTestScene(axr::Application& app);
    /// Copy Constructor
    /// @param src Source AlphaTestScene to copy from
    AlphaTestScene(const AlphaTestScene& src) = delete;
    /// Move Constructor
    /// @param src Source AlphaTestScene to move from
    AlphaTestScene(AlphaTestScene&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AlphaTestScene();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AlphaTestScene to copy from
    AlphaTestScene& operator=(const AlphaTestScene& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AlphaTestScene to move from
    AlphaTestScene& operator=(AlphaTestScene&& src) noexcept = delete;

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

    std::string m_SceneName = "AlphaTestScene";
    axr::Scene m_Scene;
    axr::Entity_T m_Entity;
    axr::Entity_T m_CameraEntity;
    axr::Entity_T m_CyanGlassEntity;
    axr::Entity_T m_MagentaGlassEntity;
    axr::Entity_T m_YellowGlassEntity;
};
