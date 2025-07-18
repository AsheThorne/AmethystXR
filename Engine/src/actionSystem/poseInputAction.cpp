// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "poseInputAction.hpp"
#include "actionUtils.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrPoseInputActionConfig axrPoseInputActionConfigClone(const AxrPoseInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return {};
    }

    return AxrPoseInputAction::clone(*inputActionConfig);
}

void axrPoseInputActionConfigDestroy(AxrPoseInputActionConfig* inputActionConfig) {
    if (inputActionConfig == nullptr) {
        axrLogErrorLocation("`inputActionConfig` is null");
        return;
    }

    return AxrPoseInputAction::destroy(*inputActionConfig);
}

void axrPoseInputActionSetEnable(const AxrPoseInputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->enable();
}

void axrPoseInputActionSetDisable(const AxrPoseInputAction_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return;
    }

    return inputAction->disable();
}

bool axrPoseInputActionIsEnabled(const AxrPoseInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->isEnabled();
}

AxrPose axrPoseInputActionGetValue(const AxrPoseInputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return AxrPose{
            .position = AxrVec3{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
            },
            .orientation = AxrQuaternion{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
                .w = 1.0f,
            }
        };
    }

    return inputAction->getValue();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrPoseInputAction::AxrPoseInputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_XrVisibility(config.XrVisibility),
    m_Binding(config.Binding),
    m_IsEnabled(true),
    m_Value(
        AxrPose{
            .position = AxrVec3{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
            },
            .orientation = AxrQuaternion{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
                .w = 1.0f,
            }
        }
    ),
    m_XrSystem(nullptr),
    m_XrAction(XR_NULL_HANDLE),
    m_XrSpace(XR_NULL_HANDLE) {
}

AxrPoseInputAction::AxrPoseInputAction(AxrPoseInputAction&& src) noexcept:
    m_Value({}) {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);

    m_Binding = src.m_Binding;
    m_XrVisibility = src.m_XrVisibility;
    m_IsEnabled = src.m_IsEnabled;
    m_Value = src.m_Value;
    m_XrSystem = src.m_XrSystem;
    m_XrAction = src.m_XrAction;
    m_XrSpace = src.m_XrSpace;

    src.m_Binding = AXR_POSE_INPUT_ACTION_UNDEFINED;
    src.m_XrVisibility = {};
    src.m_IsEnabled = false;
    src.m_Value = AxrPose{
        .position = AxrVec3{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
        },
        .orientation = AxrQuaternion{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .w = 1.0f,
        }
    };
    src.m_XrSystem = nullptr;
    src.m_XrAction = XR_NULL_HANDLE;
    src.m_XrSpace = XR_NULL_HANDLE;
}

AxrPoseInputAction::~AxrPoseInputAction() {
    cleanup();
}

AxrPoseInputAction& AxrPoseInputAction::operator=(AxrPoseInputAction&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_LocalizedName = std::move(src.m_LocalizedName);

        m_Binding = src.m_Binding;
        m_XrVisibility = src.m_XrVisibility;
        m_IsEnabled = src.m_IsEnabled;
        m_Value = src.m_Value;
        m_XrSystem = src.m_XrSystem;
        m_XrAction = src.m_XrAction;
        m_XrSpace = src.m_XrSpace;

        src.m_Binding = AXR_POSE_INPUT_ACTION_UNDEFINED;
        src.m_XrVisibility = {};
        src.m_IsEnabled = false;
        src.m_Value = AxrPose{
            .position = AxrVec3{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
            },
            .orientation = AxrQuaternion{
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
                .w = 1.0f,
            }
        };
        src.m_XrSystem = nullptr;
        src.m_XrAction = XR_NULL_HANDLE;
        src.m_XrSpace = XR_NULL_HANDLE;
    }
    return *this;
}

void AxrPoseInputAction::enable() {
    m_IsEnabled = true;
}

void AxrPoseInputAction::disable() {
    reset();
    m_IsEnabled = false;
}

bool AxrPoseInputAction::isEnabled() const {
    return m_IsEnabled;
}

// ---- Public Functions ----

AxrPose AxrPoseInputAction::getValue() const {
    return m_Value;
}

AxrResult AxrPoseInputAction::setupXrAction(const AxrXrSystem_T xrSystem, const XrActionSet actionSet) {
    if (!isVisibleToXrSession()) return AXR_SUCCESS;

    if (xrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    m_XrSystem = xrSystem;

    // AXR_POSE_INPUT_ACTION_XR_HMD doesn't use an action
    if (m_Binding != AXR_POSE_INPUT_ACTION_XR_HMD) {
        const AxrResult axrResult = m_XrSystem->createAction(
            m_Name,
            m_LocalizedName,
            XR_ACTION_TYPE_POSE_INPUT,
            actionSet,
            m_XrAction
        );
        if (AXR_FAILED(axrResult)) {
            resetSetupXrAction();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrPoseInputAction::resetSetupXrAction() {
    if (m_XrSystem == nullptr) return;

    destroyXrSpace();
    m_XrSystem->destroyAction(m_XrAction);
    m_XrSystem = nullptr;
}

AxrResult AxrPoseInputAction::createXrSpace() {
    if (m_XrSpace != XR_NULL_HANDLE) {
        axrLogErrorLocation("XrSpace already exists.");
        return AXR_ERROR;
    }

    if (m_XrSystem == nullptr) {
        axrLogErrorLocation("XrSystem is null");
        return AXR_ERROR;
    }

    if (m_Binding == AXR_POSE_INPUT_ACTION_XR_HMD) {
        const AxrResult axrResult = m_XrSystem->createViewSpace(
            m_XrSpace
        );
        if (AXR_FAILED(axrResult)) {
            destroyXrSpace();
            return axrResult;
        }
    } else {
        if (m_XrAction == XR_NULL_HANDLE) {
            axrLogErrorLocation("XrAction is null");
            return AXR_ERROR;
        }

        const AxrResult axrResult = m_XrSystem->createActionSpace(
            m_XrAction,
            m_XrSpace
        );
        if (AXR_FAILED(axrResult)) {
            destroyXrSpace();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrPoseInputAction::destroyXrSpace() {
    if (m_XrSystem == nullptr) return;

    m_XrSystem->destroySpace(m_XrSpace);
}

XrAction AxrPoseInputAction::getXrAction() const {
    return m_XrAction;
}

XrSpace AxrPoseInputAction::getXrSpace() const {
    return m_XrSpace;
}

AxrPoseInputActionEnum AxrPoseInputAction::getBinding() const {
    return m_Binding;
}

void AxrPoseInputAction::trigger(const AxrPose& value) {
    if (!isEnabled()) return;

    m_Value = value;
}

void AxrPoseInputAction::reset() {
    m_Value = AxrPose{
        .position = AxrVec3{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
        },
        .orientation = AxrQuaternion{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .w = 1.0f,
        }
    };
}

bool AxrPoseInputAction::isVisibleToXrSession() const {
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
            if (axrIsXrPoseInputAction(m_Binding)) {
                return true;
            }

            break;
        }
    }

    return false;
}

// ---- Public Static Functions ----

AxrPoseInputActionConfig AxrPoseInputAction::clone(const AxrPoseInputActionConfig& inputActionConfig) {
    AxrPoseInputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .XrVisibility = inputActionConfig.XrVisibility,
        .Binding = inputActionConfig.Binding,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_ACTION_NAME_SIZE);

    return config;
}

void AxrPoseInputAction::destroy(AxrPoseInputActionConfig& inputActionConfig) {
    memset(inputActionConfig.Name, 0, sizeof(inputActionConfig.Name));
    memset(inputActionConfig.LocalizedName, 0, sizeof(inputActionConfig.LocalizedName));
    inputActionConfig.XrVisibility = {};
    inputActionConfig.Binding = AXR_POSE_INPUT_ACTION_UNDEFINED;
}

void AxrPoseInputAction::cleanup() {
    resetSetupXrAction();

    m_Name.clear();
    m_LocalizedName.clear();
    m_XrVisibility = {};
    m_IsEnabled = false;
    m_Binding = AXR_POSE_INPUT_ACTION_UNDEFINED;
    m_Value = AxrPose{
        .position = AxrVec3{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
        },
        .orientation = AxrQuaternion{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .w = 1.0f,
        }
    };
}
