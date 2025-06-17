// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "imageSampler.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrImageSamplerGetName(const AxrImageSampler_T imageSampler) {
    if (imageSampler == nullptr) {
        axrLogErrorLocation("`imageSampler` is null.");
        return "";
    }

    return imageSampler->getName().c_str();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrImageSampler::AxrImageSampler():
    m_Filter(AXR_IMAGE_SAMPLER_FILTER_UNDEFINED),
    m_Wrapping(AXR_IMAGE_SAMPLER_WRAPPING_UNDEFINED) {
}

AxrImageSampler::AxrImageSampler(const AxrImageSamplerConfig& config):
    m_Name(config.Name),
    m_Filter(config.Filter),
    m_Wrapping(config.Wrapping) {
}

AxrImageSampler::AxrImageSampler(const AxrImageSampler& src) {
    m_Name = src.m_Name;
    m_Filter = src.m_Filter;
    m_Wrapping = src.m_Wrapping;
}

AxrImageSampler::AxrImageSampler(AxrImageSampler&& src) noexcept {
    m_Name = std::move(src.m_Name);

    m_Filter = src.m_Filter;
    m_Wrapping = src.m_Wrapping;

    src.m_Filter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    src.m_Wrapping = AXR_IMAGE_SAMPLER_WRAPPING_UNDEFINED;
}

AxrImageSampler::~AxrImageSampler() {
    cleanup();
}

AxrImageSampler& AxrImageSampler::operator=(const AxrImageSampler& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_Filter = src.m_Filter;
        m_Wrapping = src.m_Wrapping;
    }

    return *this;
}

AxrImageSampler& AxrImageSampler::operator=(AxrImageSampler&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);

        m_Filter = src.m_Filter;
        m_Wrapping = src.m_Wrapping;

        src.m_Filter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
        src.m_Wrapping = AXR_IMAGE_SAMPLER_WRAPPING_UNDEFINED;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrImageSampler::getName() const {
    return m_Name;
}

AxrImageSamplerFilterEnum AxrImageSampler::getFilter() const {
    return m_Filter;
}

AxrImageSamplerWrappingEnum AxrImageSampler::getWrapping() const {
    return m_Wrapping;
}

// ---- Private Functions ----

void AxrImageSampler::cleanup() {
    m_Name.clear();
    m_Filter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    m_Wrapping = AXR_IMAGE_SAMPLER_WRAPPING_UNDEFINED;
}
