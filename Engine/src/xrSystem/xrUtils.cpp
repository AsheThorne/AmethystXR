﻿// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "xrUtils.hpp"
#include "axr/logger.h"

// Vulkan headers are required for <openxr/openxr_platform.h>
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>
#endif

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr_platform.h>

void axrLogXrResult(
    const XrResult xrResult,
    const char* functionName,
    const std::source_location& location
) {
    if (XR_FAILED(xrResult)) {
        axrLogErrorLocation(
            {"{0} failed with a result of {1}.", location},
            functionName,
            axrToString(xrResult)
        );
    }
}

const char* axrToString(const XrResult xrResult) {
    switch (xrResult) {
        case XR_SUCCESS:
            return "XR_SUCCESS";
        case XR_TIMEOUT_EXPIRED:
            return "XR_TIMEOUT_EXPIRED";
        case XR_SESSION_LOSS_PENDING:
            return "XR_SESSION_LOSS_PENDING";
        case XR_EVENT_UNAVAILABLE:
            return "XR_EVENT_UNAVAILABLE";
        case XR_SPACE_BOUNDS_UNAVAILABLE:
            return "XR_SPACE_BOUNDS_UNAVAILABLE";
        case XR_SESSION_NOT_FOCUSED:
            return "XR_SESSION_NOT_FOCUSED";
        case XR_FRAME_DISCARDED:
            return "XR_FRAME_DISCARDED";
        case XR_ERROR_VALIDATION_FAILURE:
            return "XR_ERROR_VALIDATION_FAILURE";
        case XR_ERROR_RUNTIME_FAILURE:
            return "XR_ERROR_RUNTIME_FAILURE";
        case XR_ERROR_OUT_OF_MEMORY:
            return "XR_ERROR_OUT_OF_MEMORY";
        case XR_ERROR_API_VERSION_UNSUPPORTED:
            return "XR_ERROR_API_VERSION_UNSUPPORTED";
        case XR_ERROR_INITIALIZATION_FAILED:
            return "XR_ERROR_INITIALIZATION_FAILED";
        case XR_ERROR_FUNCTION_UNSUPPORTED:
            return "XR_ERROR_FUNCTION_UNSUPPORTED";
        case XR_ERROR_FEATURE_UNSUPPORTED:
            return "XR_ERROR_FEATURE_UNSUPPORTED";
        case XR_ERROR_EXTENSION_NOT_PRESENT:
            return "XR_ERROR_EXTENSION_NOT_PRESENT";
        case XR_ERROR_LIMIT_REACHED:
            return "XR_ERROR_LIMIT_REACHED";
        case XR_ERROR_SIZE_INSUFFICIENT:
            return "XR_ERROR_SIZE_INSUFFICIENT";
        case XR_ERROR_HANDLE_INVALID:
            return "XR_ERROR_HANDLE_INVALID";
        case XR_ERROR_INSTANCE_LOST:
            return "XR_ERROR_INSTANCE_LOST";
        case XR_ERROR_SESSION_RUNNING:
            return "XR_ERROR_SESSION_RUNNING";
        case XR_ERROR_SESSION_NOT_RUNNING:
            return "XR_ERROR_SESSION_NOT_RUNNING";
        case XR_ERROR_SESSION_LOST:
            return "XR_ERROR_SESSION_LOST";
        case XR_ERROR_SYSTEM_INVALID:
            return "XR_ERROR_SYSTEM_INVALID";
        case XR_ERROR_PATH_INVALID:
            return "XR_ERROR_PATH_INVALID";
        case XR_ERROR_PATH_COUNT_EXCEEDED:
            return "XR_ERROR_PATH_COUNT_EXCEEDED";
        case XR_ERROR_PATH_FORMAT_INVALID:
            return "XR_ERROR_PATH_FORMAT_INVALID";
        case XR_ERROR_PATH_UNSUPPORTED:
            return "XR_ERROR_PATH_UNSUPPORTED";
        case XR_ERROR_LAYER_INVALID:
            return "XR_ERROR_LAYER_INVALID";
        case XR_ERROR_LAYER_LIMIT_EXCEEDED:
            return "XR_ERROR_LAYER_LIMIT_EXCEEDED";
        case XR_ERROR_SWAPCHAIN_RECT_INVALID:
            return "XR_ERROR_SWAPCHAIN_RECT_INVALID";
        case XR_ERROR_SWAPCHAIN_FORMAT_UNSUPPORTED:
            return "XR_ERROR_SWAPCHAIN_FORMAT_UNSUPPORTED";
        case XR_ERROR_ACTION_TYPE_MISMATCH:
            return "XR_ERROR_ACTION_TYPE_MISMATCH";
        case XR_ERROR_SESSION_NOT_READY:
            return "XR_ERROR_SESSION_NOT_READY";
        case XR_ERROR_SESSION_NOT_STOPPING:
            return "XR_ERROR_SESSION_NOT_STOPPING";
        case XR_ERROR_TIME_INVALID:
            return "XR_ERROR_TIME_INVALID";
        case XR_ERROR_REFERENCE_SPACE_UNSUPPORTED:
            return "XR_ERROR_REFERENCE_SPACE_UNSUPPORTED";
        case XR_ERROR_FILE_ACCESS_ERROR:
            return "XR_ERROR_FILE_ACCESS_ERROR";
        case XR_ERROR_FILE_CONTENTS_INVALID:
            return "XR_ERROR_FILE_CONTENTS_INVALID";
        case XR_ERROR_FORM_FACTOR_UNSUPPORTED:
            return "XR_ERROR_FORM_FACTOR_UNSUPPORTED";
        case XR_ERROR_FORM_FACTOR_UNAVAILABLE:
            return "XR_ERROR_FORM_FACTOR_UNAVAILABLE";
        case XR_ERROR_API_LAYER_NOT_PRESENT:
            return "XR_ERROR_API_LAYER_NOT_PRESENT";
        case XR_ERROR_CALL_ORDER_INVALID:
            return "XR_ERROR_CALL_ORDER_INVALID";
        case XR_ERROR_GRAPHICS_DEVICE_INVALID:
            return "XR_ERROR_GRAPHICS_DEVICE_INVALID";
        case XR_ERROR_POSE_INVALID:
            return "XR_ERROR_POSE_INVALID";
        case XR_ERROR_INDEX_OUT_OF_RANGE:
            return "XR_ERROR_INDEX_OUT_OF_RANGE";
        case XR_ERROR_VIEW_CONFIGURATION_TYPE_UNSUPPORTED:
            return "XR_ERROR_VIEW_CONFIGURATION_TYPE_UNSUPPORTED";
        case XR_ERROR_ENVIRONMENT_BLEND_MODE_UNSUPPORTED:
            return "XR_ERROR_ENVIRONMENT_BLEND_MODE_UNSUPPORTED";
        case XR_ERROR_NAME_DUPLICATED:
            return "XR_ERROR_NAME_DUPLICATED";
        case XR_ERROR_NAME_INVALID:
            return "XR_ERROR_NAME_INVALID";
        case XR_ERROR_ACTIONSET_NOT_ATTACHED:
            return "XR_ERROR_ACTIONSET_NOT_ATTACHED";
        case XR_ERROR_ACTIONSETS_ALREADY_ATTACHED:
            return "XR_ERROR_ACTIONSETS_ALREADY_ATTACHED";
        case XR_ERROR_LOCALIZED_NAME_DUPLICATED:
            return "XR_ERROR_LOCALIZED_NAME_DUPLICATED";
        case XR_ERROR_LOCALIZED_NAME_INVALID:
            return "XR_ERROR_LOCALIZED_NAME_INVALID";
        case XR_ERROR_GRAPHICS_REQUIREMENTS_CALL_MISSING:
            return "XR_ERROR_GRAPHICS_REQUIREMENTS_CALL_MISSING";
        case XR_ERROR_RUNTIME_UNAVAILABLE:
            return "XR_ERROR_RUNTIME_UNAVAILABLE";
        case XR_ERROR_ANDROID_THREAD_SETTINGS_ID_INVALID_KHR:
            return "XR_ERROR_ANDROID_THREAD_SETTINGS_ID_INVALID_KHR";
        case XR_ERROR_ANDROID_THREAD_SETTINGS_FAILURE_KHR:
            return "XR_ERROR_ANDROID_THREAD_SETTINGS_FAILURE_KHR";
        case XR_ERROR_CREATE_SPATIAL_ANCHOR_FAILED_MSFT:
            return "XR_ERROR_CREATE_SPATIAL_ANCHOR_FAILED_MSFT";
        case XR_ERROR_SECONDARY_VIEW_CONFIGURATION_TYPE_NOT_ENABLED_MSFT:
            return "XR_ERROR_SECONDARY_VIEW_CONFIGURATION_TYPE_NOT_ENABLED_MSFT";
        case XR_ERROR_CONTROLLER_MODEL_KEY_INVALID_MSFT:
            return "XR_ERROR_CONTROLLER_MODEL_KEY_INVALID_MSFT";
        case XR_ERROR_REPROJECTION_MODE_UNSUPPORTED_MSFT:
            return "XR_ERROR_REPROJECTION_MODE_UNSUPPORTED_MSFT";
        case XR_ERROR_COMPUTE_NEW_SCENE_NOT_COMPLETED_MSFT:
            return "XR_ERROR_COMPUTE_NEW_SCENE_NOT_COMPLETED_MSFT";
        case XR_ERROR_SCENE_COMPONENT_ID_INVALID_MSFT:
            return "XR_ERROR_SCENE_COMPONENT_ID_INVALID_MSFT";
        case XR_ERROR_SCENE_COMPONENT_TYPE_MISMATCH_MSFT:
            return "XR_ERROR_SCENE_COMPONENT_TYPE_MISMATCH_MSFT";
        case XR_ERROR_SCENE_MESH_BUFFER_ID_INVALID_MSFT:
            return "XR_ERROR_SCENE_MESH_BUFFER_ID_INVALID_MSFT";
        case XR_ERROR_SCENE_COMPUTE_FEATURE_INCOMPATIBLE_MSFT:
            return "XR_ERROR_SCENE_COMPUTE_FEATURE_INCOMPATIBLE_MSFT";
        case XR_ERROR_SCENE_COMPUTE_CONSISTENCY_MISMATCH_MSFT:
            return "XR_ERROR_SCENE_COMPUTE_CONSISTENCY_MISMATCH_MSFT";
        case XR_ERROR_DISPLAY_REFRESH_RATE_UNSUPPORTED_FB:
            return "XR_ERROR_DISPLAY_REFRESH_RATE_UNSUPPORTED_FB";
        case XR_ERROR_COLOR_SPACE_UNSUPPORTED_FB:
            return "XR_ERROR_COLOR_SPACE_UNSUPPORTED_FB";
        case XR_ERROR_SPACE_COMPONENT_NOT_SUPPORTED_FB:
            return "XR_ERROR_SPACE_COMPONENT_NOT_SUPPORTED_FB";
        case XR_ERROR_SPACE_COMPONENT_NOT_ENABLED_FB:
            return "XR_ERROR_SPACE_COMPONENT_NOT_ENABLED_FB";
        case XR_ERROR_SPACE_COMPONENT_STATUS_PENDING_FB:
            return "XR_ERROR_SPACE_COMPONENT_STATUS_PENDING_FB";
        case XR_ERROR_SPACE_COMPONENT_STATUS_ALREADY_SET_FB:
            return "XR_ERROR_SPACE_COMPONENT_STATUS_ALREADY_SET_FB";
        case XR_ERROR_UNEXPECTED_STATE_PASSTHROUGH_FB:
            return "XR_ERROR_UNEXPECTED_STATE_PASSTHROUGH_FB";
        case XR_ERROR_FEATURE_ALREADY_CREATED_PASSTHROUGH_FB:
            return "XR_ERROR_FEATURE_ALREADY_CREATED_PASSTHROUGH_FB";
        case XR_ERROR_FEATURE_REQUIRED_PASSTHROUGH_FB:
            return "XR_ERROR_FEATURE_REQUIRED_PASSTHROUGH_FB";
        case XR_ERROR_NOT_PERMITTED_PASSTHROUGH_FB:
            return "XR_ERROR_NOT_PERMITTED_PASSTHROUGH_FB";
        case XR_ERROR_INSUFFICIENT_RESOURCES_PASSTHROUGH_FB:
            return "XR_ERROR_INSUFFICIENT_RESOURCES_PASSTHROUGH_FB";
        case XR_ERROR_UNKNOWN_PASSTHROUGH_FB:
            return "XR_ERROR_UNKNOWN_PASSTHROUGH_FB";
        case XR_ERROR_RENDER_MODEL_KEY_INVALID_FB:
            return "XR_ERROR_RENDER_MODEL_KEY_INVALID_FB";
        case XR_RENDER_MODEL_UNAVAILABLE_FB:
            return "XR_RENDER_MODEL_UNAVAILABLE_FB";
        case XR_ERROR_MARKER_NOT_TRACKED_VARJO:
            return "XR_ERROR_MARKER_NOT_TRACKED_VARJO";
        case XR_ERROR_MARKER_ID_INVALID_VARJO:
            return "XR_ERROR_MARKER_ID_INVALID_VARJO";
        case XR_ERROR_MARKER_DETECTOR_PERMISSION_DENIED_ML:
            return "XR_ERROR_MARKER_DETECTOR_PERMISSION_DENIED_ML";
        case XR_ERROR_MARKER_DETECTOR_LOCATE_FAILED_ML:
            return "XR_ERROR_MARKER_DETECTOR_LOCATE_FAILED_ML";
        case XR_ERROR_MARKER_DETECTOR_INVALID_DATA_QUERY_ML:
            return "XR_ERROR_MARKER_DETECTOR_INVALID_DATA_QUERY_ML";
        case XR_ERROR_MARKER_DETECTOR_INVALID_CREATE_INFO_ML:
            return "XR_ERROR_MARKER_DETECTOR_INVALID_CREATE_INFO_ML";
        case XR_ERROR_MARKER_INVALID_ML:
            return "XR_ERROR_MARKER_INVALID_ML";
        case XR_ERROR_LOCALIZATION_MAP_INCOMPATIBLE_ML:
            return "XR_ERROR_LOCALIZATION_MAP_INCOMPATIBLE_ML";
        case XR_ERROR_LOCALIZATION_MAP_UNAVAILABLE_ML:
            return "XR_ERROR_LOCALIZATION_MAP_UNAVAILABLE_ML";
        case XR_ERROR_LOCALIZATION_MAP_FAIL_ML:
            return "XR_ERROR_LOCALIZATION_MAP_FAIL_ML";
        case XR_ERROR_LOCALIZATION_MAP_IMPORT_EXPORT_PERMISSION_DENIED_ML:
            return "XR_ERROR_LOCALIZATION_MAP_IMPORT_EXPORT_PERMISSION_DENIED_ML";
        case XR_ERROR_LOCALIZATION_MAP_PERMISSION_DENIED_ML:
            return "XR_ERROR_LOCALIZATION_MAP_PERMISSION_DENIED_ML";
        case XR_ERROR_LOCALIZATION_MAP_ALREADY_EXISTS_ML:
            return "XR_ERROR_LOCALIZATION_MAP_ALREADY_EXISTS_ML";
        case XR_ERROR_LOCALIZATION_MAP_CANNOT_EXPORT_CLOUD_MAP_ML:
            return "XR_ERROR_LOCALIZATION_MAP_CANNOT_EXPORT_CLOUD_MAP_ML";
        case XR_ERROR_SPATIAL_ANCHOR_NAME_NOT_FOUND_MSFT:
            return "XR_ERROR_SPATIAL_ANCHOR_NAME_NOT_FOUND_MSFT";
        case XR_ERROR_SPATIAL_ANCHOR_NAME_INVALID_MSFT:
            return "XR_ERROR_SPATIAL_ANCHOR_NAME_INVALID_MSFT";
        case XR_SCENE_MARKER_DATA_NOT_STRING_MSFT:
            return "XR_SCENE_MARKER_DATA_NOT_STRING_MSFT";
        case XR_ERROR_SPACE_MAPPING_INSUFFICIENT_FB:
            return "XR_ERROR_SPACE_MAPPING_INSUFFICIENT_FB";
        case XR_ERROR_SPACE_LOCALIZATION_FAILED_FB:
            return "XR_ERROR_SPACE_LOCALIZATION_FAILED_FB";
        case XR_ERROR_SPACE_NETWORK_TIMEOUT_FB:
            return "XR_ERROR_SPACE_NETWORK_TIMEOUT_FB";
        case XR_ERROR_SPACE_NETWORK_REQUEST_FAILED_FB:
            return "XR_ERROR_SPACE_NETWORK_REQUEST_FAILED_FB";
        case XR_ERROR_SPACE_CLOUD_STORAGE_DISABLED_FB:
            return "XR_ERROR_SPACE_CLOUD_STORAGE_DISABLED_FB";
        case XR_ERROR_PASSTHROUGH_COLOR_LUT_BUFFER_SIZE_MISMATCH_META:
            return "XR_ERROR_PASSTHROUGH_COLOR_LUT_BUFFER_SIZE_MISMATCH_META";
        case XR_ERROR_HINT_ALREADY_SET_QCOM:
            return "XR_ERROR_HINT_ALREADY_SET_QCOM";
        case XR_ERROR_NOT_AN_ANCHOR_HTC:
            return "XR_ERROR_NOT_AN_ANCHOR_HTC";
        case XR_ERROR_SPACE_NOT_LOCATABLE_EXT:
            return "XR_ERROR_SPACE_NOT_LOCATABLE_EXT";
        case XR_ERROR_PLANE_DETECTION_PERMISSION_DENIED_EXT:
            return "XR_ERROR_PLANE_DETECTION_PERMISSION_DENIED_EXT";
        default:
            return "Unhandled XrResult";
    }
}

const char* axrGetXrApiLayerName(const AxrXrApiLayerTypeEnum apiLayerType) {
    switch (apiLayerType) {
        case AXR_XR_API_LAYER_TYPE_CORE_VALIDATION: {
            return "XR_APILAYER_LUNARG_core_validation";
        }
        case AXR_XR_API_LAYER_TYPE_END:
        case AXR_XR_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(apiLayerType));
            return "";
        }
    }
}

const char* axrGetXrExtensionName(const AxrXrExtensionTypeEnum extensionType) {
    switch (extensionType) {
        case AXR_XR_EXTENSION_TYPE_DEBUG_UTILS: {
            return XR_EXT_DEBUG_UTILS_EXTENSION_NAME;
        }
        case AXR_XR_EXTENSION_TYPE_VULKAN_ENABLE: {
            return XR_KHR_VULKAN_ENABLE2_EXTENSION_NAME;
        }
        case AXR_XR_EXTENSION_TYPE_END:
        case AXR_XR_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extensionType));
            return "";
        }
    }
}