// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "scene.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrSceneGetName(const AxrSceneConst_T scene) {
    if (scene == nullptr) {
        axrLogErrorLocation("`scene` is null");
        return "";
    }

    return scene->getName().c_str();
}

AxrAssetCollection_T axrSceneGetAssetCollection(const AxrScene_T scene) {
    if (scene == nullptr) {
        axrLogErrorLocation("`scene` is null");
        return nullptr;
    }

    return scene->getAssetCollection();
}

entt::registry* axrSceneGetEcsRegistry(const AxrScene_T scene) {
    if (scene == nullptr) {
        axrLogErrorLocation("`scene` is null");
        return nullptr;
    }

    return scene->getEcsRegistry();
}

entt::entity axrSceneCreateEntity(const AxrScene_T scene) {
    if (scene == nullptr) {
        axrLogErrorLocation("`scene` is null");
        return {};
    }

    return scene->createEntity();
}

void axrSceneSetMainCamera(const AxrScene_T scene, const AxrEntityConst_T entity) {
    if (scene == nullptr) {
        axrLogErrorLocation("`scene` is null");
        return;
    }

    return scene->setMainCamera(entity);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrScene::AxrScene():
    m_Name(""),
    m_MainCamera({m_Registry, entt::null}) {
}

AxrScene::AxrScene(const char* name):
    m_Name(name),
    m_MainCamera(m_Registry, entt::null) {
}

AxrScene::AxrScene(AxrScene&& src) noexcept {
    m_AssetCollection = std::move(src.m_AssetCollection);
    m_Registry = std::move(src.m_Registry);

    m_Name = src.m_Name;
    m_MainCamera = src.m_MainCamera;

    src.m_Name = "";
    src.m_MainCamera = AxrEntity_T{m_Registry, entt::null};
}

AxrScene::~AxrScene() {
    cleanup();
}

AxrScene& AxrScene::operator=(AxrScene&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_AssetCollection = std::move(src.m_AssetCollection);
        m_Registry = std::move(src.m_Registry);

        m_Name = src.m_Name;
        m_MainCamera = src.m_MainCamera;

        src.m_Name = "";
        src.m_MainCamera = AxrEntity_T{m_Registry, entt::null};
    }

    return *this;
}

const std::string& AxrScene::getName() const {
    return m_Name;
}

// ---- Public Functions ----

AxrAssetCollection_T AxrScene::getAssetCollection() {
    return &m_AssetCollection;
}

entt::registry* AxrScene::getEcsRegistry() {
    return &m_Registry;
}

entt::entity AxrScene::createEntity() {
    return m_Registry.create();
}

void AxrScene::setMainCamera(const AxrEntityConst_T entity) {
    auto [cameraComponent, transformComponent] = entity.try_get<AxrCameraComponent, AxrTransformComponent>();
    if (cameraComponent == nullptr) {
        axrLogErrorLocation("Entity doesn't have a camera component.");
        return;
    }

    if (transformComponent == nullptr) {
        axrLogErrorLocation("Entity doesn't have a transform component.");
        return;
    }

    m_MainCamera = entity;
}

bool AxrScene::isMainCameraValid() const {
    if (m_MainCamera == entt::null) {
        return false;
    }

    auto [cameraComponent, transformComponent] = m_MainCamera.try_get<AxrCameraComponent, AxrTransformComponent>();
    if (cameraComponent == nullptr || transformComponent == nullptr) {
        return false;
    }

    return true;
}

AxrEntityConst_T AxrScene::getMainCamera() const {
    return m_MainCamera;
}

// ---- Private Functions ----

void AxrScene::cleanup() {
    m_Registry.clear();
    m_AssetCollection.cleanup();
    m_Name = "";
    m_MainCamera = {m_Registry, entt::null};
}
