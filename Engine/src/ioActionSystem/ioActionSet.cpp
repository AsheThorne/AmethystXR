// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "ioActionSet.hpp"

#include <ranges>

#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrIOActionSetConfig axrIOActionSetConfigClone(const AxrIOActionSetConfig* ioActionSetConfig) {
    if (ioActionSetConfig == nullptr) {
        axrLogErrorLocation("`ioActionSetConfig` is null");
        return {};
    }

    return AxrIOActionSet::clone(*ioActionSetConfig);
}

void axrIOActionSetConfigDestroy(AxrIOActionSetConfig* ioActionSetConfig) {
    if (ioActionSetConfig == nullptr) {
        axrLogErrorLocation("`ioActionSetConfig` is null");
        return;
    }

    return AxrIOActionSet::destroy(*ioActionSetConfig);
}

AxrBoolInputAction_T axrIOActionSetGetBoolInputAction(const AxrIOActionSet_T ioActionSet, const char* name) {
    if (ioActionSet == nullptr) {
        axrLogErrorLocation("`ioActionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return ioActionSet->getBoolInputAction(name);
}

AxrFloatInputAction_T axrIOActionSetGetFloatInputAction(const AxrIOActionSet_T ioActionSet, const char* name) {
    if (ioActionSet == nullptr) {
        axrLogErrorLocation("`ioActionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return ioActionSet->getFloatInputAction(name);
}

AxrVec2InputAction_T axrIOActionSetGetVec2InputAction(const AxrIOActionSet_T ioActionSet, const char* name) {
    if (ioActionSet == nullptr) {
        axrLogErrorLocation("`ioActionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return ioActionSet->getVec2InputAction(name);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrIOActionSet::AxrIOActionSet(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    // TODO: Make this config variable
    m_IsEnabled(true),
    // TODO: Make this config variable
    m_Priority(0),
    m_XrSystem(nullptr),
    m_XrActionSet(XR_NULL_HANDLE) {
    if (config.BoolInputActions != nullptr) {
        for (uint32_t i = 0; i < config.BoolInputActionCount; ++i) {
            m_BoolInputActions.insert(
                std::pair(
                    config.BoolInputActions[i].Name,
                    AxrBoolInputAction(
                        AxrBoolInputAction::Config{
                            .Name = config.BoolInputActions[i].Name,
                            .LocalizedName = config.BoolInputActions[i].LocalizedName,
                            .XrVisibility = config.BoolInputActions[i].XrVisibility,
                            .BindingCount = config.BoolInputActions[i].BindingCount,
                            .Bindings = config.BoolInputActions[i].Bindings,
                        }
                    )
                )
            );
        }
    }
    if (config.FloatInputActions != nullptr) {
        for (uint32_t i = 0; i < config.FloatInputActionCount; ++i) {
            m_FloatInputActions.insert(
                std::pair(
                    config.FloatInputActions[i].Name,
                    AxrFloatInputAction(
                        AxrFloatInputAction::Config{
                            .Name = config.FloatInputActions[i].Name,
                            .LocalizedName = config.FloatInputActions[i].LocalizedName,
                            .XrVisibility = config.FloatInputActions[i].XrVisibility,
                            .BindingCount = config.FloatInputActions[i].BindingCount,
                            .Bindings = config.FloatInputActions[i].Bindings,
                        }
                    )
                )
            );
        }
    }
    if (config.Vec2InputActions != nullptr) {
        for (uint32_t i = 0; i < config.Vec2InputActionCount; ++i) {
            m_Vec2InputActions.insert(
                std::pair(
                    config.Vec2InputActions[i].Name,
                    AxrVec2InputAction(
                        AxrVec2InputAction::Config{
                            .Name = config.Vec2InputActions[i].Name,
                            .LocalizedName = config.Vec2InputActions[i].LocalizedName,
                            .XrVisibility = config.Vec2InputActions[i].XrVisibility,
                            .BindingCount = config.Vec2InputActions[i].BindingCount,
                            .Bindings = config.Vec2InputActions[i].Bindings,
                        }
                    )
                )
            );
        }
    }
}

AxrIOActionSet::AxrIOActionSet(AxrIOActionSet&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_BoolInputActions = std::move(src.m_BoolInputActions);
    m_FloatInputActions = std::move(src.m_FloatInputActions);
    m_Vec2InputActions = std::move(src.m_Vec2InputActions);

    m_IsEnabled = src.m_IsEnabled;
    m_Priority = src.m_Priority;
    m_XrSystem = src.m_XrSystem;
    m_XrActionSet = src.m_XrActionSet;

    src.m_IsEnabled = false;
    src.m_Priority = 0;
    src.m_XrSystem = nullptr;
    src.m_XrActionSet = XR_NULL_HANDLE;
}

AxrIOActionSet::~AxrIOActionSet() {
    cleanup();
}

AxrIOActionSet& AxrIOActionSet::operator=(AxrIOActionSet&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);
        m_BoolInputActions = std::move(src.m_BoolInputActions);
        m_FloatInputActions = std::move(src.m_FloatInputActions);
        m_Vec2InputActions = std::move(src.m_Vec2InputActions);

        m_IsEnabled = src.m_IsEnabled;
        m_Priority = src.m_Priority;
        m_XrSystem = src.m_XrSystem;
        m_XrActionSet = src.m_XrActionSet;

        src.m_IsEnabled = false;
        src.m_Priority = 0;
        src.m_XrSystem = nullptr;
        src.m_XrActionSet = XR_NULL_HANDLE;
    }
    return *this;
}

// ---- Public Functions ----

AxrBoolInputAction_T AxrIOActionSet::getBoolInputAction(const std::string& name) {
    const auto foundInputAction = m_BoolInputActions.find(name);
    if (foundInputAction == m_BoolInputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrFloatInputAction_T AxrIOActionSet::getFloatInputAction(const std::string& name) {
    const auto foundInputAction = m_FloatInputActions.find(name);
    if (foundInputAction == m_FloatInputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrVec2InputAction_T AxrIOActionSet::getVec2InputAction(const std::string& name) {
    const auto foundInputAction = m_Vec2InputActions.find(name);
    if (foundInputAction == m_Vec2InputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrResult AxrIOActionSet::setupXrActions(const AxrXrSystem_T xrSystem) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    AxrResult axrResult = AXR_SUCCESS;
    m_XrSystem = xrSystem;

    axrResult = m_XrSystem->createActionSet(
        m_Name,
        m_LocalizedName,
        m_Priority,
        m_XrActionSet
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }

    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrActions(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            resetSetupXrActions();
            return axrResult;
        }
    }

    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrActions(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            resetSetupXrActions();
            return axrResult;
        }
    }

    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrActions(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            resetSetupXrActions();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrIOActionSet::resetSetupXrActions() {
    if (m_XrSystem == nullptr) return;

    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        inputAction.resetSetupXrActions();
    }

    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        inputAction.resetSetupXrActions();
    }

    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        inputAction.resetSetupXrActions();
    }

    m_XrSystem->destroyActionSet(m_XrActionSet);

    m_XrSystem = nullptr;
}

bool AxrIOActionSet::isEnabled() const {
    return m_IsEnabled;
}

uint32_t AxrIOActionSet::getPriority() const {
    return m_Priority;
}

void AxrIOActionSet::newFrameStarted() {
    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        inputAction.newFrameStarted();
    }
    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        inputAction.newFrameStarted();
    }
    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        inputAction.newFrameStarted();
    }
}

std::unordered_map<std::string, AxrBoolInputAction>& AxrIOActionSet::getBoolInputActions() {
    return m_BoolInputActions;
}

std::unordered_map<std::string, AxrFloatInputAction>& AxrIOActionSet::getFloatInputActions() {
    return m_FloatInputActions;
}

std::unordered_map<std::string, AxrVec2InputAction>& AxrIOActionSet::getVec2InputActions() {
    return m_Vec2InputActions;
}

XrActionSet AxrIOActionSet::getXrActionSet() const {
    return m_XrActionSet;
}

void AxrIOActionSet::updateXrActionValues() {
    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        inputAction.updateXrActionValue();
    }

    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        inputAction.updateXrActionValue();
    }
    
    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        inputAction.updateXrActionValue();
    }
}

// ---- Public Static Functions ----

AxrIOActionSetConfig AxrIOActionSet::clone(const AxrIOActionSetConfig& ioActionSetConfig) {
    AxrIOActionSetConfig config{
        .Name = {},
        .LocalizedName = {},
        .BoolInputActionCount = ioActionSetConfig.BoolInputActionCount,
        .BoolInputActions = nullptr,
        .FloatInputActionCount = ioActionSetConfig.FloatInputActionCount,
        .FloatInputActions = nullptr,
        .Vec2InputActionCount = ioActionSetConfig.Vec2InputActionCount,
        .Vec2InputActions = nullptr,
    };

    strncpy_s(config.Name, ioActionSetConfig.Name, AXR_MAX_IO_ACTION_SET_NAME_SIZE);
    strncpy_s(config.LocalizedName, ioActionSetConfig.LocalizedName, AXR_MAX_IO_ACTION_SET_NAME_SIZE);

    if (ioActionSetConfig.BoolInputActionCount != 0 && ioActionSetConfig.BoolInputActions != nullptr) {
        config.BoolInputActions = new AxrBoolInputActionConfig[ioActionSetConfig.BoolInputActionCount]{};

        for (uint32_t i = 0; i < ioActionSetConfig.BoolInputActionCount; ++i) {
            config.BoolInputActions[i] = AxrBoolInputAction::clone(ioActionSetConfig.BoolInputActions[i]);
        }
    }

    if (ioActionSetConfig.FloatInputActionCount != 0 && ioActionSetConfig.FloatInputActions != nullptr) {
        config.FloatInputActions = new AxrFloatInputActionConfig[ioActionSetConfig.FloatInputActionCount]{};

        for (uint32_t i = 0; i < ioActionSetConfig.FloatInputActionCount; ++i) {
            config.FloatInputActions[i] = AxrFloatInputAction::clone(ioActionSetConfig.FloatInputActions[i]);
        }
    }

    if (ioActionSetConfig.Vec2InputActionCount != 0 && ioActionSetConfig.Vec2InputActions != nullptr) {
        config.Vec2InputActions = new AxrVec2InputActionConfig[ioActionSetConfig.Vec2InputActionCount]{};

        for (uint32_t i = 0; i < ioActionSetConfig.Vec2InputActionCount; ++i) {
            config.Vec2InputActions[i] = AxrVec2InputAction::clone(ioActionSetConfig.Vec2InputActions[i]);
        }
    }

    return config;
}

void AxrIOActionSet::destroy(AxrIOActionSetConfig& ioActionSetConfig) {
    memset(ioActionSetConfig.Name, 0, sizeof(ioActionSetConfig.Name));
    memset(ioActionSetConfig.LocalizedName, 0, sizeof(ioActionSetConfig.LocalizedName));

    if (ioActionSetConfig.BoolInputActions != nullptr) {
        for (uint32_t i = 0; i < ioActionSetConfig.BoolInputActionCount; ++i) {
            AxrBoolInputAction::destroy(ioActionSetConfig.BoolInputActions[i]);
        }

        delete[] ioActionSetConfig.BoolInputActions;
        ioActionSetConfig.BoolInputActions = nullptr;
    }
    ioActionSetConfig.BoolInputActionCount = 0;

    if (ioActionSetConfig.FloatInputActions != nullptr) {
        for (uint32_t i = 0; i < ioActionSetConfig.FloatInputActionCount; ++i) {
            AxrFloatInputAction::destroy(ioActionSetConfig.FloatInputActions[i]);
        }

        delete[] ioActionSetConfig.FloatInputActions;
        ioActionSetConfig.FloatInputActions = nullptr;
    }
    ioActionSetConfig.FloatInputActionCount = 0;

    if (ioActionSetConfig.Vec2InputActions != nullptr) {
        for (uint32_t i = 0; i < ioActionSetConfig.Vec2InputActionCount; ++i) {
            AxrVec2InputAction::destroy(ioActionSetConfig.Vec2InputActions[i]);
        }

        delete[] ioActionSetConfig.Vec2InputActions;
        ioActionSetConfig.Vec2InputActions = nullptr;
    }
    ioActionSetConfig.Vec2InputActionCount = 0;
}

void AxrIOActionSet::cleanup() {
    resetSetupXrActions();

    m_Name.clear();
    m_LocalizedName.clear();
    m_BoolInputActions.clear();
    m_FloatInputActions.clear();
    m_Vec2InputActions.clear();
    m_IsEnabled = false;
    m_Priority = 0;
}

bool AxrIOActionSet::isVisibleToXrSession() const {
    // If any IO action is visible to the xr session, then the whole must be

    for (const AxrBoolInputAction& inputAction : m_BoolInputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrFloatInputAction& inputAction : m_FloatInputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrVec2InputAction& inputAction : m_Vec2InputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    return false;
}
