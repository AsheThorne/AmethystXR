// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vec2InputAction.hpp"

#include "boolInputAction.hpp"
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

bool axrVec2InputActionWasValueSetThisFrame(const AxrVec2InputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return false;
    }

    return inputAction->wasValueSetThisFrame();
}

glm::vec2 axrVec2InputActionGetValue(const AxrVec2InputActionConst_T inputAction) {
    if (inputAction == nullptr) {
        axrLogErrorLocation("`inputAction` is null");
        return glm::vec2(0.0f);
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
    m_Value(glm::vec2(0.0f)),
    m_WasTriggeredThisFrame(false) {
    if (config.Bindings != nullptr) {
        for (uint32_t i = 0; i < config.BindingCount; ++i) {
            m_Bindings.insert(config.Bindings[i]);
        }
    }
}

AxrVec2InputAction::AxrVec2InputAction(AxrVec2InputAction&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_LocalizedName = std::move(src.m_LocalizedName);
    m_Bindings = std::move(src.m_Bindings);

    m_Value = src.m_Value;
    m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

    src.m_Value = glm::vec2(0.0f);
    src.m_WasTriggeredThisFrame = false;
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

        m_Value = src.m_Value;
        m_WasTriggeredThisFrame = src.m_WasTriggeredThisFrame;

        src.m_Value = glm::vec2(0.0f);
        src.m_WasTriggeredThisFrame = false;
    }
    return *this;
}

// ---- Public Functions ----

bool AxrVec2InputAction::wasValueSetThisFrame() const {
    return m_WasTriggeredThisFrame;
}

glm::vec2 AxrVec2InputAction::getValue() const {
    return m_Value;
}

void AxrVec2InputAction::newFrameStarted() {
    m_WasTriggeredThisFrame = false;
}

bool AxrVec2InputAction::containsBinding(const AxrVec2InputActionEnum biding) const {
    return m_Bindings.contains(biding);
}

void AxrVec2InputAction::trigger(const glm::vec2& value) {
    m_Value = value;
    m_WasTriggeredThisFrame = true;
}

// ---- Public Static Functions ----

AxrVec2InputActionConfig AxrVec2InputAction::clone(const AxrVec2InputActionConfig& inputActionConfig) {
    AxrVec2InputActionConfig config{
        .Name = {},
        .LocalizedName = {},
        .BindingCount = inputActionConfig.BindingCount,
        .Bindings = nullptr,
    };

    strncpy_s(config.Name, inputActionConfig.Name, AXR_MAX_IO_ACTION_NAME_SIZE);
    strncpy_s(config.LocalizedName, inputActionConfig.LocalizedName, AXR_MAX_IO_ACTION_NAME_SIZE);

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

    if (inputActionConfig.Bindings != nullptr) {
        delete[] inputActionConfig.Bindings;
        inputActionConfig.Bindings = nullptr;
    }
    inputActionConfig.BindingCount = 0;
}

void AxrVec2InputAction::cleanup() {
    m_Name.clear();
    m_LocalizedName.clear();
    m_Bindings.clear();

    m_Value = glm::vec2(0.0f);
    m_WasTriggeredThisFrame = false;
}
