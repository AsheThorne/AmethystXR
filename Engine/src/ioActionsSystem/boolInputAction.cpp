// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "boolInputAction.hpp"
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

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrBoolInputAction::AxrBoolInputAction(const Config& config):
    m_Name(config.Name),
    m_LocalizedName(config.LocalizedName),
    m_Value(false),
    m_WasTriggeredThisFrame(false) {
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

    m_Value = src.m_Value;
    m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

    src.m_Value = false;
    src.m_WasTriggeredThisFrame = false;
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

        m_Value = src.m_Value;
        m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

        src.m_Value = false;
        src.m_WasTriggeredThisFrame = false;
    }
    return *this;
}

// ---- Public Functions ----

bool AxrBoolInputAction::containsBinding(const AxrBoolInputActionEnum biding) const {
    return m_Bindings.contains(biding);
}

void AxrBoolInputAction::trigger(const bool value) {
    m_Value = value;
    m_WasTriggeredThisFrame = true;
}

// ---- Public Static Functions ----

AxrBoolInputActionConfig AxrBoolInputAction::clone(const AxrBoolInputActionConfig& inputActionConfig) {
    AxrBoolInputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .BindingCount = inputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_IO_ACTION_NAME_SIZE);

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

    if (inputActionConfig.Bindings != nullptr) {
        delete[] inputActionConfig.Bindings;
        inputActionConfig.Bindings = nullptr;
    }
    inputActionConfig.BindingCount = 0;
}

// ---- Private Functions ----

void AxrBoolInputAction::cleanup() {
    m_Name.clear();
    m_LocalizedName.clear();
    m_Bindings.clear();
    m_Value = false;
    m_WasTriggeredThisFrame = false;
}
