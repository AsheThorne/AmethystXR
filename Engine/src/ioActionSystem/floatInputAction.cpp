// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "floatInputAction.hpp"
#include "ioActionUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrFloatInputActionConfig axrFloatInputActionConfigClone(const AxrFloatInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return {};
    }

    return AxrFloatInputAction::clone(*inputActionConfig);
}

void axrFloatInputActionConfigDestroy(AxrFloatInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return;
    }

    return AxrFloatInputAction::destroy(*inputActionConfig);
}

bool axrFloatInputActionWasValueSetThisFrame(const AxrFloatInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->wasValueSetThisFrame();
}

float axrFloatInputActionGetValue(const AxrFloatInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return 0.0f;
    }

    return inputAction->getValue();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrFloatInputAction::AxrFloatInputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_XrVisibility(config.XrVisibility),
    m_Value(0.0f),
    m_WasTriggeredThisFrame(false),
    m_XrSystem(nullptr),
    m_XrAction(XR_NULL_HANDLE) {
    if (config.Bindings != nullptr) {
        for (uint32_t i = 0; i < config.BindingCount; ++i) {
            m_Bindings.insert(config.Bindings[i]);
        }
    }
}

AxrFloatInputAction::AxrFloatInputAction(AxrFloatInputAction&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_Bindings = std::move(src.m_Bindings);

    m_XrVisibility = src.m_XrVisibility;
    m_Value = src.m_Value;
    m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;
    m_XrSystem = src.m_XrSystem;
    m_XrAction = src.m_XrAction;

    src.m_XrVisibility = {};
    src.m_Value = 0.0f;
    src.m_WasTriggeredThisFrame = false;
    src.m_XrSystem = nullptr;
    src.m_XrAction = XR_NULL_HANDLE;
}

AxrFloatInputAction::~AxrFloatInputAction() {
    cleanup();
}

AxrFloatInputAction& AxrFloatInputAction::operator=(AxrFloatInputAction&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);
        m_Bindings = std::move(src.m_Bindings);

        m_XrVisibility = src.m_XrVisibility;
        m_Value = src.m_Value;
        m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;
        m_XrSystem = src.m_XrSystem;
        m_XrAction = src.m_XrAction;

        src.m_XrVisibility = {};
        src.m_Value = 0.0f;
        src.m_WasTriggeredThisFrame = false;
        src.m_XrSystem = nullptr;
        src.m_XrAction = XR_NULL_HANDLE;
    }
    return *this;
}

// ---- Public Functions ----

bool AxrFloatInputAction::wasValueSetThisFrame() const {
    return m_WasTriggeredThisFrame;
}

float AxrFloatInputAction::getValue() const {
    return m_Value;
}

AxrResult AxrFloatInputAction::setupXrActions(const AxrXrSystem_T xrSystem, const XrActionSet actionSet) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    m_XrSystem = xrSystem;

    const AxrResult axrResult = m_XrSystem->createAction(
        m_Name,
        m_LocalizedName,
        XR_ACTION_TYPE_FLOAT_INPUT,
        actionSet,
        m_XrAction
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrActions();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrFloatInputAction::resetSetupXrActions() {
    if (m_XrSystem == nullptr) return;

    m_XrSystem->destroyAction(m_XrAction);
    m_XrSystem = nullptr;
}

void AxrFloatInputAction::newFrameStarted() {
    m_WasTriggeredThisFrame = false;
}

XrAction AxrFloatInputAction::getXrAction() const {
    return m_XrAction;
}

const std::unordered_set<AxrFloatInputActionEnum>& AxrFloatInputAction::getBindings() const {
    return m_Bindings;
}

bool AxrFloatInputAction::containsBinding(const AxrFloatInputActionEnum biding) const {
    return m_Bindings.contains(biding);
}

void AxrFloatInputAction::trigger(const float value) {
    m_Value = value;
    m_WasTriggeredThisFrame = true;
}

bool AxrFloatInputAction::isVisibleToXrSession() const {
    switch (m_XrVisibility) {
        case AXR_IO_ACTION_XR_VISIBILITY_ALWAYS: {
            return true;
        }
        case AXR_IO_ACTION_XR_VISIBILITY_NEVER: {
            return false;
        }
        default: {
            axrLogErrorLocation(
                "Unknown AxrIOActionXrVisibilityEnum value: {0}.",
                static_cast<int32_t>(m_XrVisibility)
            );
            // Don't break. We intentionally fall through to the auto case
        }
        case AXR_IO_ACTION_XR_VISIBILITY_AUTO: {
            for (const AxrFloatInputActionEnum binding : m_Bindings) {
                if (axrIsXrFloatInputAction(binding)) {
                    return true;
                }
            }

            break;
        }
    }

    return false;
}

void AxrFloatInputAction::updateXrActionValue() {
    if (m_XrSystem == nullptr || m_XrAction == XR_NULL_HANDLE) return;

    const XrActionStateFloat actionState = m_XrSystem->getFloatActionState(m_XrAction);
    if (actionState.isActive && actionState.changedSinceLastSync) {
        trigger(actionState.currentState);
    }
}

// ---- Public Static Functions ----

AxrFloatInputActionConfig AxrFloatInputAction::clone(const AxrFloatInputActionConfig& inputActionConfig) {
    AxrFloatInputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .XrVisibility = inputActionConfig.XrVisibility,
        .BindingCount = inputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_IO_ACTION_NAME_SIZE);

    if (inputActionConfig.BindingCount != 0 && inputActionConfig.Bindings != nullptr) {
        config.Bindings = new AxrFloatInputActionEnum[inputActionConfig.BindingCount]{};

        for (uint32_t i = 0; i < inputActionConfig.BindingCount; ++i) {
            config.Bindings[i] = inputActionConfig.Bindings[i];
        }
    }

    return config;
}

void AxrFloatInputAction::destroy(AxrFloatInputActionConfig& inputActionConfig) {
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

void AxrFloatInputAction::cleanup() {
    resetSetupXrActions();

    m_Name.clear();
    m_LocalizedName.clear();
    m_XrVisibility = {};
    m_Bindings.clear();
    m_Value = 0.0f;
    m_WasTriggeredThisFrame = false;
}
