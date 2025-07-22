// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vec2InputAction.hpp"
#include "actionUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrVec2InputActionConfig axrVec2InputActionConfigClone(const AxrVec2InputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return {};
    }

    return AxrVec2InputAction::clone(*inputActionConfig);
}

void axrVec2InputActionConfigDestroy(AxrVec2InputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return;
    }

    return AxrVec2InputAction::destroy(*inputActionConfig);
}

void axrVec2InputActionEnable(const AxrVec2InputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->enable();
}

void axrVec2InputActionDisable(const AxrVec2InputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->disable();
}

bool axrVec2InputActionIsEnabled(const AxrVec2InputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->isEnabled();
}

bool axrVec2InputActionValueChanged(const AxrVec2InputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->valueChanged();
}

AxrVec2 axrVec2InputActionGetValue(const AxrVec2InputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return {.x = 0.0f, .y = 0.0f};
    }

    return inputAction->getValue();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVec2InputAction::AxrVec2InputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_XrVisibility(config.XrVisibility),
    m_IsEnabled(true),
    m_Value(
        AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        }
    ),
    m_ValueLastFrame(
        AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        }
    ),
    m_XrSystem(nullptr),
    m_XrAction(XR_NULL_HANDLE) {
    if (config.Bindings != nullptr) {
        for (uint32_t i = 0; i < config.BindingCount; ++i) {
            m_Bindings.insert(config.Bindings[i]);
        }
    }
}

AxrVec2InputAction::AxrVec2InputAction(AxrVec2InputAction&& src) noexcept:
    m_Value({}),
    m_ValueLastFrame({}) {
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
    src.m_Value = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };
    src.m_ValueLastFrame = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };
    src.m_XrSystem = nullptr;
    src.m_XrAction = XR_NULL_HANDLE;
}

AxrVec2InputAction::~AxrVec2InputAction() {
    cleanup();
}

AxrVec2InputAction& AxrVec2InputAction::operator=(AxrVec2InputAction&& src) noexcept {
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
        src.m_Value = AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        };
        src.m_ValueLastFrame = AxrVec2{
            .x = 0.0f,
            .y = 0.0f,
        };
        src.m_XrSystem = nullptr;
        src.m_XrAction = XR_NULL_HANDLE;
    }
    return *this;
}

void AxrVec2InputAction::enable() {
    m_IsEnabled = true;
}

void AxrVec2InputAction::disable() {
    reset();
    m_IsEnabled = false;
}

bool AxrVec2InputAction::isEnabled() const {
    return m_IsEnabled;
}

// ---- Public Functions ----

bool AxrVec2InputAction::valueChanged() const {
    return m_ValueLastFrame.x != m_Value.x ||
        m_ValueLastFrame.y != m_Value.y;
}

AxrVec2 AxrVec2InputAction::getValue() const {
    return m_Value;
}

AxrResult AxrVec2InputAction::setupXrAction(const AxrXrSystem_T xrSystem, const XrActionSet actionSet) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    m_XrSystem = xrSystem;

    const AxrResult axrResult = m_XrSystem->createAction(
        m_Name,
        m_LocalizedName,
        XR_ACTION_TYPE_VECTOR2F_INPUT,
        actionSet,
        m_XrAction
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrAction();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVec2InputAction::resetSetupXrAction() {
    if (m_XrSystem == nullptr) return;

    m_XrSystem->destroyAction(m_XrAction);
    m_XrSystem = nullptr;
}

void AxrVec2InputAction::newFrameStarted() {
    m_ValueLastFrame = m_Value;
}

XrAction AxrVec2InputAction::getXrAction() const {
    return m_XrAction;
}

const std::unordered_set<AxrVec2InputActionEnum>& AxrVec2InputAction::getBindings() const {
    return m_Bindings;
}

bool AxrVec2InputAction::containsBinding(const AxrVec2InputActionEnum binding) const {
    return m_Bindings.contains(binding);
}

void AxrVec2InputAction::trigger(const AxrVec2& value) {
    if (!isEnabled()) return;

    m_Value = value;
}

void AxrVec2InputAction::reset() {
    m_Value = AxrVec2{
        .x = 0.0f,
        .y = 0.0f
    };
}

bool AxrVec2InputAction::isVisibleToXrSession() const {
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
            for (const AxrVec2InputActionEnum binding : m_Bindings) {
                if (axrIsXrVec2InputAction(binding)) {
                    return true;
                }
            }

            break;
        }
    }

    return false;
}

void AxrVec2InputAction::updateXrActionValue() {
    if (m_XrSystem == nullptr || m_XrAction == XR_NULL_HANDLE) return;

    const XrActionStateVector2f actionState = m_XrSystem->getVec2ActionState(m_XrAction);
    if (actionState.isActive && actionState.changedSinceLastSync) {
        trigger(
            AxrVec2{
                .x = actionState.currentState.x,
                .y = actionState.currentState.y,
            }
        );
    }
}

// ---- Public Static Functions ----

AxrVec2InputActionConfig AxrVec2InputAction::clone(const AxrVec2InputActionConfig& inputActionConfig) {
    AxrVec2InputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .XrVisibility = inputActionConfig.XrVisibility,
        .BindingCount = inputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_ACTION_NAME_SIZE);

    if (inputActionConfig.BindingCount != 0 && inputActionConfig.Bindings != nullptr) {
        config.Bindings = new AxrVec2InputActionEnum[inputActionConfig.BindingCount]{};

        for (uint32_t i = 0; i < inputActionConfig.BindingCount; ++i) {
            config.Bindings[i] = inputActionConfig.Bindings[i];
        }
    }

    return config;
}

void AxrVec2InputAction::destroy(AxrVec2InputActionConfig& inputActionConfig) {
    memset(inputActionConfig.Name, 0, sizeof(inputActionConfig.Name));
    memset(inputActionConfig.LocalizedName, 0, sizeof(inputActionConfig.LocalizedName));
    inputActionConfig.XrVisibility = {};

    if (inputActionConfig.Bindings != nullptr) {
        delete[] inputActionConfig.Bindings;
        inputActionConfig.Bindings = nullptr;
    }
    inputActionConfig.BindingCount = 0;
}

void AxrVec2InputAction::cleanup() {
    resetSetupXrAction();

    m_Name.clear();
    m_LocalizedName.clear();
    m_XrVisibility = {};
    m_IsEnabled = false;
    m_Bindings.clear();
    m_Value = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };
    m_ValueLastFrame = AxrVec2{
        .x = 0.0f,
        .y = 0.0f,
    };
}
