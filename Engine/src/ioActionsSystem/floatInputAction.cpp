// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "floatInputAction.hpp"
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

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrFloatInputAction::AxrFloatInputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_Value(0.0f),
    m_WasTriggeredThisFrame(false) {
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

    m_Value = src.m_Value;
    m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

    src.m_Value = 0.0f;
    src.m_WasTriggeredThisFrame = false;
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

        m_Value = src.m_Value;
        m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

        src.m_Value = 0.0f;
        src.m_WasTriggeredThisFrame = false;
    }
    return *this;
}

bool AxrFloatInputAction::containsBinding(const AxrFloatInputActionEnum biding) const {
    return m_Bindings.contains(biding);
}

void AxrFloatInputAction::trigger(const float value) {
    m_Value = value;
    m_WasTriggeredThisFrame = true;
}

// ---- Public Functions ----

// ---- Public Static Functions ----

AxrFloatInputActionConfig AxrFloatInputAction::clone(const AxrFloatInputActionConfig& inputActionConfig) {
    AxrFloatInputActionConfig config{
        .Name = {},
        .LocalizedName = {},
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

    if (inputActionConfig.Bindings != nullptr) {
        delete[] inputActionConfig.Bindings;
        inputActionConfig.Bindings = nullptr;
    }
    inputActionConfig.BindingCount = 0;
}

// ---- Private Functions ----

void AxrFloatInputAction::cleanup() {
    m_Name.clear();
    m_LocalizedName.clear();
    m_Bindings.clear();
    m_Value = 0.0f;
    m_WasTriggeredThisFrame = false;
}
