// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "imageSampler.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrImageSamplerGetName(const AxrImageSamplerConst_T imageSampler) {
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
    m_MinFilter(AXR_IMAGE_SAMPLER_FILTER_UNDEFINED),
    m_MagFilter(AXR_IMAGE_SAMPLER_FILTER_UNDEFINED),
    m_MipmapFilter(AXR_IMAGE_SAMPLER_FILTER_UNDEFINED),
    m_WrapU(AXR_IMAGE_SAMPLER_WRAP_UNDEFINED),
    m_WrapV(AXR_IMAGE_SAMPLER_WRAP_UNDEFINED) {
}

AxrImageSampler::AxrImageSampler(const AxrImageSamplerConfig& config):
    m_Name(config.Name),
    m_MinFilter(config.MinFilter),
    m_MagFilter(config.MagFilter),
    m_MipmapFilter(config.MipmapFilter),
    m_WrapU(config.WrapU),
    m_WrapV(config.WrapV) {
}

AxrImageSampler::AxrImageSampler(const AxrImageSampler& src) {
    m_Name = src.m_Name;
    m_MinFilter = src.m_MinFilter;
    m_MagFilter = src.m_MagFilter;
    m_MipmapFilter = src.m_MipmapFilter;
    m_WrapU = src.m_WrapU;
    m_WrapV = src.m_WrapV;
}

AxrImageSampler::AxrImageSampler(AxrImageSampler&& src) noexcept {
    m_Name = std::move(src.m_Name);

    m_MinFilter = src.m_MinFilter;
    m_MagFilter = src.m_MagFilter;
    m_MipmapFilter = src.m_MipmapFilter;
    m_WrapU = src.m_WrapU;
    m_WrapV = src.m_WrapV;

    src.m_MinFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    src.m_MagFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    src.m_MipmapFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    src.m_WrapU = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
    src.m_WrapV = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
}

AxrImageSampler::~AxrImageSampler() {
    cleanup();
}

AxrImageSampler& AxrImageSampler::operator=(const AxrImageSampler& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_MinFilter = src.m_MinFilter;
        m_MagFilter = src.m_MagFilter;
        m_MipmapFilter = src.m_MipmapFilter;
        m_WrapU = src.m_WrapU;
        m_WrapV = src.m_WrapV;
    }

    return *this;
}

AxrImageSampler& AxrImageSampler::operator=(AxrImageSampler&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);

        m_MinFilter = src.m_MinFilter;
        m_MagFilter = src.m_MagFilter;
        m_MipmapFilter = src.m_MipmapFilter;
        m_WrapU = src.m_WrapU;
        m_WrapV = src.m_WrapV;

        src.m_MinFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
        src.m_MagFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
        src.m_MipmapFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
        src.m_WrapU = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
        src.m_WrapV = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrImageSampler::getName() const {
    return m_Name;
}

AxrImageSamplerFilterEnum AxrImageSampler::getMinFilter() const {
    return m_MinFilter;
}

AxrImageSamplerFilterEnum AxrImageSampler::getMagFilter() const {
    return m_MagFilter;
}

AxrImageSamplerFilterEnum AxrImageSampler::getMipmapFilter() const {
    return m_MipmapFilter;
}

AxrImageSamplerWrapEnum AxrImageSampler::getWrapU() const {
    return m_WrapU;
}

AxrImageSamplerWrapEnum AxrImageSampler::getWrapV() const {
    return m_WrapV;
}

// ---- Private Functions ----

void AxrImageSampler::cleanup() {
    m_Name.clear();

    m_MinFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    m_MagFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    m_MipmapFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    m_WrapU = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
    m_WrapV = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
}
