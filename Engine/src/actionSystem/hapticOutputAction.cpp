// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "hapticOutputAction.hpp"
#include "actionUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrHapticOutputActionConfig axrHapticOutputActionConfigClone(const AxrHapticOutputActionConfig* outputActionConfig) {
    if (outputActionConfig == nullptr) {
        axrLogErrorLocation("`outputActionConfig` is null");
        return {};
    }

    return AxrHapticOutputAction::clone(*outputActionConfig);
}

void axrHapticOutputActionConfigDestroy(AxrHapticOutputActionConfig* outputActionConfig) {
    if (outputActionConfig == nullptr) {
        axrLogErrorLocation("`outputActionConfig` is null");
        return;
    }

    return AxrHapticOutputAction::destroy(*outputActionConfig);
}

void axrHapticOutputActionEnable(const AxrHapticOutputAction_T outputAction) {
    if (outputAction == nullptr) {
        axrLogErrorLocation("`outputAction` is null");
        return;
    }

    return outputAction->enable();
}

void axrHapticOutputActionDisable(const AxrHapticOutputAction_T outputAction) {
    if (outputAction == nullptr) {
        axrLogErrorLocation("`outputAction` is null");
        return;
    }

    return outputAction->disable();
}

bool axrHapticOutputActionIsEnabled(const AxrHapticOutputActionConst_T outputAction) {
    if (outputAction == nullptr) {
        axrLogErrorLocation("`outputAction` is null");
        return false;
    }

    return outputAction->isEnabled();
}

void axrHapticOutputActionActivate(
    const AxrHapticOutputAction_T outputAction,
    const int64_t duration,
    const float frequency,
    const float amplitude
) {
    if (outputAction == nullptr) {
        axrLogErrorLocation("`outputAction` is null");
        return;
    }

    return outputAction->activate(duration, frequency, amplitude);
}

void axrHapticOutputActionDeactivate(const AxrHapticOutputAction_T outputAction) {
    if (outputAction == nullptr) {
        axrLogErrorLocation("`outputAction` is null");
        return;
    }

    return outputAction->deactivate();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrHapticOutputAction::AxrHapticOutputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_XrVisibility(config.XrVisibility),
    m_IsEnabled(true),
    m_XrSystem(nullptr),
    m_XrAction(XR_NULL_HANDLE) {
    if (config.Bindings != nullptr) {
        for (uint32_t i = 0; i < config.BindingCount; ++i) {
            m_Bindings.insert(config.Bindings[i]);
        }
    }
}

AxrHapticOutputAction::AxrHapticOutputAction(AxrHapticOutputAction&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_Bindings = std::move(src.m_Bindings);

    m_XrVisibility = src.m_XrVisibility;
    m_IsEnabled = src.m_IsEnabled;
    m_XrSystem = src.m_XrSystem;
    m_XrAction = src.m_XrAction;

    src.m_XrVisibility = {};
    src.m_IsEnabled = false;
    src.m_XrSystem = nullptr;
    src.m_XrAction = XR_NULL_HANDLE;
}

AxrHapticOutputAction::~AxrHapticOutputAction() {
    cleanup();
}

AxrHapticOutputAction& AxrHapticOutputAction::operator=(AxrHapticOutputAction&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);
        m_Bindings = std::move(src.m_Bindings);

        m_XrVisibility = src.m_XrVisibility;
        m_IsEnabled = src.m_IsEnabled;
        m_XrSystem = src.m_XrSystem;
        m_XrAction = src.m_XrAction;

        src.m_XrVisibility = {};
        src.m_IsEnabled = false;
        src.m_XrSystem = nullptr;
        src.m_XrAction = XR_NULL_HANDLE;
    }
    return *this;
}

void AxrHapticOutputAction::enable() {
    m_IsEnabled = true;
}

void AxrHapticOutputAction::disable() {
    deactivate();
    m_IsEnabled = false;
}

bool AxrHapticOutputAction::isEnabled() const {
    return m_IsEnabled;
}

void AxrHapticOutputAction::activate(const int64_t duration, const float frequency, const float amplitude) {
    if (m_XrSystem == nullptr || m_XrAction == XR_NULL_HANDLE) return;

    const AxrResult axrResult = m_XrSystem->applyHapticFeedback(m_XrAction, duration, frequency, amplitude);
    if (AXR_FAILED(axrResult)) return;
}

void AxrHapticOutputAction::deactivate() {
    if (m_XrSystem == nullptr || m_XrAction == XR_NULL_HANDLE) return;

    const AxrResult axrResult = m_XrSystem->stopHapticFeedback(m_XrAction);
    if (AXR_FAILED(axrResult)) return;
}

// ---- Public Functions ----

AxrResult AxrHapticOutputAction::setupXrAction(const AxrXrSystem_T xrSystem, const XrActionSet actionSet) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    m_XrSystem = xrSystem;

    const AxrResult axrResult = m_XrSystem->createAction(
        m_Name,
        m_LocalizedName,
        XR_ACTION_TYPE_VIBRATION_OUTPUT,
        actionSet,
        m_XrAction
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrAction();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrHapticOutputAction::resetSetupXrAction() {
    if (m_XrSystem == nullptr) return;

    m_XrSystem->destroyAction(m_XrAction);
    m_XrSystem = nullptr;
}

XrAction AxrHapticOutputAction::getXrAction() const {
    return m_XrAction;
}

const std::unordered_set<AxrHapticOutputActionEnum>& AxrHapticOutputAction::getBindings() const {
    return m_Bindings;
}

bool AxrHapticOutputAction::containsBinding(const AxrHapticOutputActionEnum binding) const {
    return m_Bindings.contains(binding);
}

bool AxrHapticOutputAction::isVisibleToXrSession() const {
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
            for (const AxrHapticOutputActionEnum binding : m_Bindings) {
                if (axrIsXrHapticOutputAction(binding)) {
                    return true;
                }
            }

            break;
        }
    }

    return false;
}

// ---- Public Static Functions ----

AxrHapticOutputActionConfig AxrHapticOutputAction::clone(const AxrHapticOutputActionConfig& outputActionConfig) {
    AxrHapticOutputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .XrVisibility = outputActionConfig.XrVisibility,
        .BindingCount = outputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, outputActionConfig.Name, AXR_MAX_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, outputActionConfig.LocalizedName, AXR_MAX_ACTION_NAME_SIZE);

    if (outputActionConfig.BindingCount != 0 && outputActionConfig.Bindings != nullptr) {
        config.Bindings = new AxrHapticOutputActionEnum[outputActionConfig.BindingCount]{};

        for (uint32_t i = 0; i < outputActionConfig.BindingCount; ++i) {
            config.Bindings[i] = outputActionConfig.Bindings[i];
        }
    }

    return config;
}

void AxrHapticOutputAction::destroy(AxrHapticOutputActionConfig& outputActionConfig) {
    memset(outputActionConfig.Name, 0, sizeof(outputActionConfig.Name));
    memset(outputActionConfig.LocalizedName, 0, sizeof(outputActionConfig.LocalizedName));
    outputActionConfig.XrVisibility = {};

    if (outputActionConfig.Bindings != nullptr) {
        delete[] outputActionConfig.Bindings;
        outputActionConfig.Bindings = nullptr;
    }
    outputActionConfig.BindingCount = 0;
}

void AxrHapticOutputAction::cleanup() {
    resetSetupXrAction();

    m_Name.clear();
    m_LocalizedName.clear();
    m_XrVisibility = {};
    m_IsEnabled = false;
    m_Bindings.clear();
}
