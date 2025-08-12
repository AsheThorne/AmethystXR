// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "actionSet.hpp"
#include "axr/logger.h"
#include "actionUtils.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <ranges>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrActionSetConfig axrActionSetConfigClone(const AxrActionSetConfig* actionSetConfig) {
    if (actionSetConfig == nullptr) {
        axrLogErrorLocation("`actionSetConfig` is null");
        return {};
    }

    return AxrActionSet::clone(*actionSetConfig);
}

void axrActionSetConfigDestroy(AxrActionSetConfig* actionSetConfig) {
    if (actionSetConfig == nullptr) {
        axrLogErrorLocation("`actionSetConfig` is null");
        return;
    }

    return AxrActionSet::destroy(*actionSetConfig);
}

void axrActionSetSetPriority(const AxrActionSet_T actionSet, const uint32_t priority) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return;
    }

    return actionSet->setPriority(priority);
}

uint32_t axrActionSetGetPriority(const AxrActionSetConst_T actionSet) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return 0;
    }

    return actionSet->getPriority();
}


void axrActionSetEnable(const AxrActionSet_T actionSet) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return;
    }

    return actionSet->enable();
}

void axrActionSetDisable(const AxrActionSet_T actionSet) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return;
    }

    return actionSet->disable();
}

bool axrActionSetIsEnabled(const AxrActionSetConst_T actionSet) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return false;
    }

    return actionSet->isEnabled();
}

AxrBoolInputAction_T axrActionSetGetBoolInputAction(const AxrActionSet_T actionSet, const char* name) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSet->getBoolInputAction(name);
}

AxrFloatInputAction_T axrActionSetGetFloatInputAction(const AxrActionSet_T actionSet, const char* name) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSet->getFloatInputAction(name);
}

AxrVec2InputAction_T axrActionSetGetVec2InputAction(const AxrActionSet_T actionSet, const char* name) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSet->getVec2InputAction(name);
}

AxrPoseInputAction_T axrActionSetGetPoseInputAction(const AxrActionSet_T actionSet, const char* name) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSet->getPoseInputAction(name);
}

AxrHapticOutputAction_T axrActionSetGetHapticOutputAction(const AxrActionSet_T actionSet, const char* name) {
    if (actionSet == nullptr) {
        axrLogErrorLocation("`actionSet` is null");
        return nullptr;
    }

    if (name == nullptr) {
        axrLogErrorLocation("`name` is null");
        return nullptr;
    }

    return actionSet->getHapticOutputAction(name);
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrActionSet::AxrActionSet(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_IsEnabled(true),
    m_Priority(0),
    m_XrSystem(nullptr),
    m_XrActionSet(XR_NULL_HANDLE) {
    if (config.BoolInputActions != nullptr) {
        for (uint32_t i = 0; i < config.BoolInputActionCount; ++i) {
            if (!axrIsActionNameValid(config.BoolInputActions[i].Name)) {
                axrLogErrorLocation("Bool Action name: {0} is invalid.", config.BoolInputActions[i].Name);
                continue;
            }

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
            if (!axrIsActionNameValid(config.FloatInputActions[i].Name)) {
                axrLogErrorLocation("Float Action name: {0} is invalid.", config.FloatInputActions[i].Name);
                continue;
            }

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
            if (!axrIsActionNameValid(config.Vec2InputActions[i].Name)) {
                axrLogErrorLocation("Vec2 Action name: {0} is invalid.", config.Vec2InputActions[i].Name);
                continue;
            }

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
    if (config.PoseInputActions != nullptr) {
        for (uint32_t i = 0; i < config.PoseInputActionCount; ++i) {
            if (!axrIsActionNameValid(config.PoseInputActions[i].Name)) {
                axrLogErrorLocation("Pose Action name: {0} is invalid.", config.PoseInputActions[i].Name);
                continue;
            }

            m_PoseInputActions.insert(
                std::pair(
                    config.PoseInputActions[i].Name,
                    AxrPoseInputAction(
                        AxrPoseInputAction::Config{
                            .Name = config.PoseInputActions[i].Name,
                            .LocalizedName = config.PoseInputActions[i].LocalizedName,
                            .XrVisibility = config.PoseInputActions[i].XrVisibility,
                            .Binding = config.PoseInputActions[i].Binding,
                        }
                    )
                )
            );
        }
    }
    if (config.HapticOutputActions != nullptr) {
        for (uint32_t i = 0; i < config.HapticOutputActionCount; ++i) {
            if (!axrIsActionNameValid(config.HapticOutputActions[i].Name)) {
                axrLogErrorLocation("Haptics Action name: {0} is invalid.", config.HapticOutputActions[i].Name);
                continue;
            }

            m_HapticOutputActions.insert(
                std::pair(
                    config.HapticOutputActions[i].Name,
                    AxrHapticOutputAction(
                        AxrHapticOutputAction::Config{
                            .Name = config.HapticOutputActions[i].Name,
                            .LocalizedName = config.HapticOutputActions[i].LocalizedName,
                            .XrVisibility = config.HapticOutputActions[i].XrVisibility,
                            .BindingCount = config.HapticOutputActions[i].BindingCount,
                            .Bindings = config.HapticOutputActions[i].Bindings,
                        }
                    )
                )
            );
        }
    }
}

AxrActionSet::AxrActionSet(AxrActionSet&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_BoolInputActions = std::move(src.m_BoolInputActions);
    m_FloatInputActions = std::move(src.m_FloatInputActions);
    m_Vec2InputActions = std::move(src.m_Vec2InputActions);
    m_PoseInputActions = std::move(src.m_PoseInputActions);
    m_HapticOutputActions = std::move(src.m_HapticOutputActions);

    m_IsEnabled = src.m_IsEnabled;
    m_Priority = src.m_Priority;
    m_XrSystem = src.m_XrSystem;
    m_XrActionSet = src.m_XrActionSet;

    src.m_IsEnabled = false;
    src.m_Priority = 0;
    src.m_XrSystem = nullptr;
    src.m_XrActionSet = XR_NULL_HANDLE;
}

AxrActionSet::~AxrActionSet() {
    cleanup();
}

AxrActionSet& AxrActionSet::operator=(AxrActionSet&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);
        m_BoolInputActions = std::move(src.m_BoolInputActions);
        m_FloatInputActions = std::move(src.m_FloatInputActions);
        m_Vec2InputActions = std::move(src.m_Vec2InputActions);
        m_PoseInputActions = std::move(src.m_PoseInputActions);
        m_HapticOutputActions = std::move(src.m_HapticOutputActions);

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

void AxrActionSet::setPriority(const uint32_t priority) {
    m_Priority = priority;
}

uint32_t AxrActionSet::getPriority() const {
    return m_Priority;
}

void AxrActionSet::enable() {
    m_IsEnabled = true;
}

void AxrActionSet::disable() {
    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::views::values) {
        inputAction.reset();
    }
    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::views::values) {
        inputAction.reset();
    }
    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::views::values) {
        inputAction.reset();
    }
    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::views::values) {
        inputAction.reset();
    }
    for (AxrHapticOutputAction& inputAction : m_HapticOutputActions | std::views::values) {
        inputAction.deactivate();
    }

    m_IsEnabled = false;
}

bool AxrActionSet::isEnabled() const {
    return m_IsEnabled;
}

AxrBoolInputAction_T AxrActionSet::getBoolInputAction(const std::string& name) {
    const auto foundInputAction = m_BoolInputActions.find(name);
    if (foundInputAction == m_BoolInputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrFloatInputAction_T AxrActionSet::getFloatInputAction(const std::string& name) {
    const auto foundInputAction = m_FloatInputActions.find(name);
    if (foundInputAction == m_FloatInputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrVec2InputAction_T AxrActionSet::getVec2InputAction(const std::string& name) {
    const auto foundInputAction = m_Vec2InputActions.find(name);
    if (foundInputAction == m_Vec2InputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrPoseInputAction_T AxrActionSet::getPoseInputAction(const std::string& name) {
    const auto foundInputAction = m_PoseInputActions.find(name);
    if (foundInputAction == m_PoseInputActions.end()) {
        return nullptr;
    }

    return &foundInputAction->second;
}

AxrHapticOutputAction_T AxrActionSet::getHapticOutputAction(const std::string& name) {
    const auto foundOutputAction = m_HapticOutputActions.find(name);
    if (foundOutputAction == m_HapticOutputActions.end()) {
        return nullptr;
    }

    return &foundOutputAction->second;
}

AxrResult AxrActionSet::setupXrActions(const AxrXrSystem_T xrSystem) {
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
        axrResult = inputAction.setupXrAction(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrAction(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrAction(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        axrResult = inputAction.setupXrAction(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    for (AxrHapticOutputAction& outputAction : m_HapticOutputActions | std::ranges::views::values) {
        axrResult = outputAction.setupXrAction(m_XrSystem, m_XrActionSet);
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    return AXR_SUCCESS;
}

void AxrActionSet::resetSetupXrActions() {
    if (m_XrSystem == nullptr) return;

    destroyXrSpaces();

    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        inputAction.resetSetupXrAction();
    }
    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        inputAction.resetSetupXrAction();
    }
    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        inputAction.resetSetupXrAction();
    }
    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        inputAction.resetSetupXrAction();
    }
    for (AxrHapticOutputAction& outputAction : m_HapticOutputActions | std::ranges::views::values) {
        outputAction.resetSetupXrAction();
    }
    m_XrSystem->destroyActionSet(m_XrActionSet);

    m_XrSystem = nullptr;
}

AxrResult AxrActionSet::createXrSpaces() {
    AxrResult axrResult = AXR_SUCCESS;

    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        axrResult = inputAction.createXrSpace();
        if (AXR_FAILED(axrResult)) {
            continue;
        }
    }

    return AXR_SUCCESS;
}

void AxrActionSet::destroyXrSpaces() {
    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        inputAction.destroyXrSpace();
    }
}

void AxrActionSet::newFrameStarted() {
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

std::unordered_map<std::string, AxrBoolInputAction>& AxrActionSet::getBoolInputActions() {
    return m_BoolInputActions;
}

std::unordered_map<std::string, AxrFloatInputAction>& AxrActionSet::getFloatInputActions() {
    return m_FloatInputActions;
}

std::unordered_map<std::string, AxrVec2InputAction>& AxrActionSet::getVec2InputActions() {
    return m_Vec2InputActions;
}

std::unordered_map<std::string, AxrPoseInputAction>& AxrActionSet::getPoseInputActions() {
    return m_PoseInputActions;
}

std::unordered_map<std::string, AxrHapticOutputAction>& AxrActionSet::getHapticOutputActions() {
    return m_HapticOutputActions;
}

bool AxrActionSet::containsBinding(const AxrBoolInputActionEnum binding) const {
    for (const AxrBoolInputAction& inputAction : m_BoolInputActions | std::views::values) {
        if (inputAction.containsBinding(binding)) {
            return true;
        }
    }

    return false;
}

bool AxrActionSet::containsBinding(const AxrFloatInputActionEnum binding) const {
    for (const AxrFloatInputAction& inputAction : m_FloatInputActions | std::views::values) {
        if (inputAction.containsBinding(binding)) {
            return true;
        }
    }

    return false;
}

bool AxrActionSet::containsBinding(const AxrVec2InputActionEnum binding) const {
    for (const AxrVec2InputAction& inputAction : m_Vec2InputActions | std::views::values) {
        if (inputAction.containsBinding(binding)) {
            return true;
        }
    }

    return false;
}

bool AxrActionSet::containsBinding(const AxrPoseInputActionEnum binding) const {
    for (const AxrPoseInputAction& inputAction : m_PoseInputActions | std::views::values) {
        if (inputAction.getBinding() == binding) {
            return true;
        }
    }

    return false;
}

bool AxrActionSet::containsBinding(const AxrHapticOutputActionEnum binding) const {
    for (const AxrHapticOutputAction& outputAction : m_HapticOutputActions | std::views::values) {
        if (outputAction.containsBinding(binding)) {
            return true;
        }
    }

    return false;
}

void AxrActionSet::triggerBoolInputAction(const AxrBoolInputActionEnum inputActionEnum, const bool value) {
    if (!isEnabled()) return;

    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.trigger(value);
        }
    }
}

void AxrActionSet::triggerFloatInputAction(const AxrFloatInputActionEnum inputActionEnum, const float value) {
    if (!isEnabled()) return;

    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.trigger(value);
        }
    }
}

void AxrActionSet::triggerVec2InputAction(const AxrVec2InputActionEnum inputActionEnum, const AxrVec2& value) {
    if (!isEnabled()) return;

    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.trigger(value);
        }
    }
}

void AxrActionSet::triggerPoseInputAction(const AxrPoseInputActionEnum inputActionEnum, const AxrPose& value) {
    if (!isEnabled()) return;

    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        if (inputAction.getBinding() == inputActionEnum) {
            inputAction.trigger(value);
        }
    }
}

void AxrActionSet::resetBoolInputAction(const AxrBoolInputActionEnum inputActionEnum) {
    for (AxrBoolInputAction& inputAction : m_BoolInputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.reset();
        }
    }
}

void AxrActionSet::resetFloatInputAction(const AxrFloatInputActionEnum inputActionEnum) {
    for (AxrFloatInputAction& inputAction : m_FloatInputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.reset();
        }
    }
}

void AxrActionSet::resetVec2InputAction(const AxrVec2InputActionEnum inputActionEnum) {
    for (AxrVec2InputAction& inputAction : m_Vec2InputActions | std::ranges::views::values) {
        if (inputAction.containsBinding(inputActionEnum)) {
            inputAction.reset();
        }
    }
}

void AxrActionSet::resetPoseInputAction(const AxrPoseInputActionEnum inputActionEnum) {
    for (AxrPoseInputAction& inputAction : m_PoseInputActions | std::ranges::views::values) {
        if (inputAction.getBinding() == inputActionEnum) {
            inputAction.reset();
        }
    }
}

XrActionSet AxrActionSet::getXrActionSet() const {
    return m_XrActionSet;
}

void AxrActionSet::updateXrActionValues() {
    if (!isEnabled()) return;

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

AxrActionSetConfig AxrActionSet::clone(const AxrActionSetConfig& actionSetConfig) {
    AxrActionSetConfig config{
        .Name = {},
        .LocalizedName = {},
        .BoolInputActionCount = actionSetConfig.BoolInputActionCount,
        .BoolInputActions = nullptr,
        .FloatInputActionCount = actionSetConfig.FloatInputActionCount,
        .FloatInputActions = nullptr,
        .Vec2InputActionCount = actionSetConfig.Vec2InputActionCount,
        .Vec2InputActions = nullptr,
        .PoseInputActionCount = actionSetConfig.PoseInputActionCount,
        .PoseInputActions = nullptr,
        .HapticOutputActionCount = actionSetConfig.HapticOutputActionCount,
        .HapticOutputActions = nullptr,
    };

    strncpy_s(config.Name, actionSetConfig.Name, AXR_MAX_ACTION_SET_NAME_SIZE);
    strncpy_s(config.LocalizedName, actionSetConfig.LocalizedName, AXR_MAX_ACTION_SET_NAME_SIZE);

    if (actionSetConfig.BoolInputActionCount != 0 && actionSetConfig.BoolInputActions != nullptr) {
        config.BoolInputActions = new AxrBoolInputActionConfig[actionSetConfig.BoolInputActionCount]{};

        for (uint32_t i = 0; i < actionSetConfig.BoolInputActionCount; ++i) {
            config.BoolInputActions[i] = AxrBoolInputAction::clone(actionSetConfig.BoolInputActions[i]);
        }
    }

    if (actionSetConfig.FloatInputActionCount != 0 && actionSetConfig.FloatInputActions != nullptr) {
        config.FloatInputActions = new AxrFloatInputActionConfig[actionSetConfig.FloatInputActionCount]{};

        for (uint32_t i = 0; i < actionSetConfig.FloatInputActionCount; ++i) {
            config.FloatInputActions[i] = AxrFloatInputAction::clone(actionSetConfig.FloatInputActions[i]);
        }
    }

    if (actionSetConfig.Vec2InputActionCount != 0 && actionSetConfig.Vec2InputActions != nullptr) {
        config.Vec2InputActions = new AxrVec2InputActionConfig[actionSetConfig.Vec2InputActionCount]{};

        for (uint32_t i = 0; i < actionSetConfig.Vec2InputActionCount; ++i) {
            config.Vec2InputActions[i] = AxrVec2InputAction::clone(actionSetConfig.Vec2InputActions[i]);
        }
    }

    if (actionSetConfig.PoseInputActionCount != 0 && actionSetConfig.PoseInputActions != nullptr) {
        config.PoseInputActions = new AxrPoseInputActionConfig[actionSetConfig.PoseInputActionCount]{};

        for (uint32_t i = 0; i < actionSetConfig.PoseInputActionCount; ++i) {
            config.PoseInputActions[i] = AxrPoseInputAction::clone(actionSetConfig.PoseInputActions[i]);
        }
    }

    if (actionSetConfig.HapticOutputActionCount != 0 && actionSetConfig.HapticOutputActions != nullptr) {
        config.HapticOutputActions = new AxrHapticOutputActionConfig[actionSetConfig.HapticOutputActionCount]{};

        for (uint32_t i = 0; i < actionSetConfig.HapticOutputActionCount; ++i) {
            config.HapticOutputActions[i] = AxrHapticOutputAction::clone(actionSetConfig.HapticOutputActions[i]);
        }
    }

    return config;
}

void AxrActionSet::destroy(AxrActionSetConfig& actionSetConfig) {
    memset(actionSetConfig.Name, 0, sizeof(actionSetConfig.Name));
    memset(actionSetConfig.LocalizedName, 0, sizeof(actionSetConfig.LocalizedName));

    if (actionSetConfig.BoolInputActions != nullptr) {
        for (uint32_t i = 0; i < actionSetConfig.BoolInputActionCount; ++i) {
            AxrBoolInputAction::destroy(actionSetConfig.BoolInputActions[i]);
        }

        delete[] actionSetConfig.BoolInputActions;
        actionSetConfig.BoolInputActions = nullptr;
    }
    actionSetConfig.BoolInputActionCount = 0;

    if (actionSetConfig.FloatInputActions != nullptr) {
        for (uint32_t i = 0; i < actionSetConfig.FloatInputActionCount; ++i) {
            AxrFloatInputAction::destroy(actionSetConfig.FloatInputActions[i]);
        }

        delete[] actionSetConfig.FloatInputActions;
        actionSetConfig.FloatInputActions = nullptr;
    }
    actionSetConfig.FloatInputActionCount = 0;

    if (actionSetConfig.Vec2InputActions != nullptr) {
        for (uint32_t i = 0; i < actionSetConfig.Vec2InputActionCount; ++i) {
            AxrVec2InputAction::destroy(actionSetConfig.Vec2InputActions[i]);
        }

        delete[] actionSetConfig.Vec2InputActions;
        actionSetConfig.Vec2InputActions = nullptr;
    }
    actionSetConfig.Vec2InputActionCount = 0;

    if (actionSetConfig.PoseInputActions != nullptr) {
        for (uint32_t i = 0; i < actionSetConfig.PoseInputActionCount; ++i) {
            AxrPoseInputAction::destroy(actionSetConfig.PoseInputActions[i]);
        }

        delete[] actionSetConfig.PoseInputActions;
        actionSetConfig.PoseInputActions = nullptr;
    }
    actionSetConfig.PoseInputActionCount = 0;

    if (actionSetConfig.HapticOutputActions != nullptr) {
        for (uint32_t i = 0; i < actionSetConfig.HapticOutputActionCount; ++i) {
            AxrHapticOutputAction::destroy(actionSetConfig.HapticOutputActions[i]);
        }

        delete[] actionSetConfig.HapticOutputActions;
        actionSetConfig.HapticOutputActions = nullptr;
    }
    actionSetConfig.HapticOutputActionCount = 0;
}

void AxrActionSet::cleanup() {
    resetSetupXrActions();

    m_Name.clear();
    m_LocalizedName.clear();
    m_BoolInputActions.clear();
    m_FloatInputActions.clear();
    m_Vec2InputActions.clear();
    m_PoseInputActions.clear();
    m_HapticOutputActions.clear();
    m_IsEnabled = false;
    m_Priority = 0;
}

bool AxrActionSet::isVisibleToXrSession() const {
    // If any action is visible to the xr session, then the whole must be

    for (const AxrBoolInputAction& inputAction : m_BoolInputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrFloatInputAction& inputAction : m_FloatInputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrVec2InputAction& inputAction : m_Vec2InputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrPoseInputAction& inputAction : m_PoseInputActions | std::views::values) {
        if (inputAction.isVisibleToXrSession()) return true;
    }

    for (const AxrHapticOutputAction& outputAction : m_HapticOutputActions | std::views::values) {
        if (outputAction.isVisibleToXrSession()) return true;
    }

    return false;
}
