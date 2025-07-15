// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "boolInputAction.hpp"
#include "actionUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrBoolInputActionConfig axrBoolInputActionConfigClone(const AxrBoolInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return {};
    }

    return AxrBoolInputAction::clone(*inputActionConfig);
}

void axrBoolInputActionConfigDestroy(AxrBoolInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return;
    }

    return AxrBoolInputAction::destroy(*inputActionConfig);
}

void axrBoolInputActionSetEnable(const AxrBoolInputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->enable();
}

void axrBoolInputActionSetDisable(const AxrBoolInputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->disable();
}

bool axrBoolInputActionIsEnabled(const AxrBoolInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->isEnabled();
}

bool axrBoolInputActionValueChanged(const AxrBoolInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->valueChanged();
}

bool axrBoolInputActionGetValue(const AxrBoolInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->getValue();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrBoolInputAction::AxrBoolInputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_XrVisibility(config.XrVisibility),
    m_IsEnabled(true),
    m_Value(false),
    m_ValueLastFrame(false),
    m_XrSystem(nullptr),
    m_XrAction(XR_NULL_HANDLE) {
    if (config.Bindings != nullptr) {
        for (uint32_t i = 0; i < config.BindingCount; ++i) {
            m_Bindings.insert(config.Bindings[i]);
        }
    }
}

AxrBoolInputAction::AxrBoolInputAction(AxrBoolInputAction&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_Bindings = std::move(src.m_Bindings);

    m_XrVisibility = src.m_XrVisibility;
    m_IsEnabled = src.m_IsEnabled;
    m_Value = src.m_Value;
    m_ValueLastFrame = src.m_ValueLastFrame;
    m_XrSystem = src.m_XrSystem;
    m_XrAction = src.m_XrAction;

    src.m_XrVisibility = {};
    src.m_IsEnabled = false;
    src.m_Value = false;
    src.m_ValueLastFrame = false;
    src.m_XrSystem = nullptr;
    src.m_XrAction = XR_NULL_HANDLE;
}

AxrBoolInputAction::~AxrBoolInputAction() {
    cleanup();
}

AxrBoolInputAction& AxrBoolInputAction::operator=(AxrBoolInputAction&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);
        m_Bindings = std::move(src.m_Bindings);

        m_XrVisibility = src.m_XrVisibility;
        m_IsEnabled = src.m_IsEnabled;
        m_Value = src.m_Value;
        m_ValueLastFrame = src.m_ValueLastFrame;
        m_XrSystem = src.m_XrSystem;
        m_XrAction = src.m_XrAction;

        src.m_XrVisibility = {};
        src.m_IsEnabled = false;
        src.m_Value = false;
        src.m_ValueLastFrame = false;
        src.m_XrSystem = nullptr;
        src.m_XrAction = XR_NULL_HANDLE;
    }
    return *this;
}

void AxrBoolInputAction::enable() {
    m_IsEnabled = true;
}

void AxrBoolInputAction::disable() {
    m_IsEnabled = false;
}

bool AxrBoolInputAction::isEnabled() const {
    return m_IsEnabled;
}

// ---- Public Functions ----

bool AxrBoolInputAction::valueChanged() const {
    return m_ValueLastFrame != m_Value;
}

bool AxrBoolInputAction::getValue() const {
    return m_Value;
}

AxrResult AxrBoolInputAction::setupXrActions(const AxrXrSystem_T xrSystem, const XrActionSet actionSet) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    m_XrSystem = xrSystem;

    const AxrResult axrResult = m_XrSystem->createAction(
        m_Name,
        m_LocalizedName,
        XR_ACTION_TYPE_BOOLEAN_INPUT,
        actionSet,
        m_XrAction
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrBoolInputAction::resetSetupXrActions() {
    if (m_XrSystem == nullptr) return;

    m_XrSystem->destroyAction(m_XrAction);
    m_XrSystem = nullptr;
}

void AxrBoolInputAction::newFrameStarted() {
    m_ValueLastFrame = m_Value;
}

XrAction AxrBoolInputAction::getXrAction() const {
    return m_XrAction;
}

const std::unordered_set<AxrBoolInputActionEnum>& AxrBoolInputAction::getBindings() const {
    return m_Bindings;
}

bool AxrBoolInputAction::containsBinding(const AxrBoolInputActionEnum biding) const {
    return m_Bindings.contains(biding);
}

void AxrBoolInputAction::trigger(const bool value) {
    m_Value = value;
}

void AxrBoolInputAction::reset() {
    m_Value = false;
}

bool AxrBoolInputAction::isVisibleToXrSession() const {
    switch (m_XrVisibility) {
        case AXR_ACTION_XR_VISIBILITY_ALWAYS: {
            return true;
        }
        case AXR_ACTION_XR_VISIBILITY_NEVER: {
            return false;
        }
        default: {
            axrLogErrorLocation(
                "Unknown AxrActionXrVisibilityEnum value: {0}.",
                static_cast<int32_t>(m_XrVisibility)
            );
            // Don't break. We intentionally fall through to the auto case
        }
        case AXR_ACTION_XR_VISIBILITY_AUTO: {
            for (const AxrBoolInputActionEnum binding : m_Bindings) {
                if (axrIsXrBoolInputAction(binding)) {
                    return true;
                }
            }

            break;
        }
    }

    return false;
}

void AxrBoolInputAction::updateXrActionValue() {
    if (m_XrSystem == nullptr || m_XrAction == XR_NULL_HANDLE) return;

    const XrActionStateBoolean actionState = m_XrSystem->getBoolActionState(m_XrAction);
    if (actionState.isActive && actionState.changedSinceLastSync) {
        trigger(actionState.currentState);
    }
}

// ---- Public Static Functions ----

AxrBoolInputActionConfig AxrBoolInputAction::clone(const AxrBoolInputActionConfig& inputActionConfig) {
    AxrBoolInputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .XrVisibility = inputActionConfig.XrVisibility,
        .BindingCount = inputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_ACTION_NAME_SIZE);

    // TODO: Add this check for any other similar cloning function. like assets
    if (inputActionConfig.BindingCount != 0 && inputActionConfig.Bindings != nullptr) {
        config.Bindings = new AxrBoolInputActionEnum[inputActionConfig.BindingCount]{};

        for (uint32_t i = 0; i < inputActionConfig.BindingCount; ++i) {
            config.Bindings[i] = inputActionConfig.Bindings[i];
        }
    }

    return config;
}

void AxrBoolInputAction::destroy(AxrBoolInputActionConfig& inputActionConfig) {
    memset(inputActionConfig.Name, 0, sizeof(inputActionConfig.Name));
    memset(inputActionConfig.LocalizedName, 0, sizeof(inputActionConfig.LocalizedName));
    inputActionConfig.XrVisibility = {};

    if (inputActionConfig.Bindings != nullptr) {
        delete[] inputActionConfig.Bindings;
        inputActionConfig.Bindings = nullptr;
    }
    inputActionConfig.BindingCount = 0;
}

// ---- Private Functions ----

void AxrBoolInputAction::cleanup() {
    resetSetupXrActions();

    m_Name.clear();
    m_LocalizedName.clear();
    m_XrVisibility = {};
    m_IsEnabled = false;
    m_Bindings.clear();
    m_Value = false;
    m_ValueLastFrame = false;
}
