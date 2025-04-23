#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "pushConstantsBuffer.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AXR_API void* axrPushConstantsCloneData(const uint32_t size, const void* data) {
    return AxrPushConstantsBuffer::cloneData(size, data);
}

AXR_API void axrPushConstantsDestroyData(uint32_t* size, void** data) {
    return AxrPushConstantsBuffer::destroyData(*size, *data);
}

const char* axrPushConstantsBufferGetName(const AxrPushConstantsBuffer_T pushConstantsBuffer) {
    if (pushConstantsBuffer == nullptr) {
        axrLogErrorLocation("`pushConstantsBuffer` is null.");
        return "";
    }

    return pushConstantsBuffer->getName();
}


// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrPushConstantsBuffer::AxrPushConstantsBuffer():
    m_Name(""),
    m_DataSize(0),
    m_Data(nullptr) {
}

AxrPushConstantsBuffer::AxrPushConstantsBuffer(const AxrPushConstantsBufferConfig& config):
    m_Name(config.Name),
    m_DataSize(config.BufferSize),
    m_Data(nullptr) {
    if (config.Data == nullptr) {
        m_Data = createData(config.BufferSize);
    } else {
        m_Data = cloneData(m_DataSize, config.Data);
    }
}

AxrPushConstantsBuffer::AxrPushConstantsBuffer(const AxrPushConstantsBuffer& src) {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = cloneData(src.m_DataSize, src.m_Data);
}

AxrPushConstantsBuffer::AxrPushConstantsBuffer(AxrPushConstantsBuffer&& src) noexcept {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = src.m_Data;

    src.m_Name = "";
    src.m_DataSize = 0;
    src.m_Data = nullptr;
}

AxrPushConstantsBuffer::~AxrPushConstantsBuffer() {
    cleanup();
}

AxrPushConstantsBuffer& AxrPushConstantsBuffer::operator=(const AxrPushConstantsBuffer& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_DataSize = src.m_DataSize;
        m_Data = cloneData(src.m_DataSize, src.m_Data);
    }
    return *this;
}

AxrPushConstantsBuffer& AxrPushConstantsBuffer::operator=(AxrPushConstantsBuffer&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_DataSize = src.m_DataSize;
        m_Data = src.m_Data;

        src.m_Name = "";
        src.m_DataSize = 0;
        src.m_Data = nullptr;
    }
    return *this;
}

// ---- Public Functions ----

const char* AxrPushConstantsBuffer::getName() const {
    return m_Name;
}

void* AxrPushConstantsBuffer::cloneData(const uint32_t size, const void* data) {
    if (data == nullptr) return nullptr;

    void* newData = malloc(size);
    memcpy_s(newData, size, data, size);
    return newData;
}

void* AxrPushConstantsBuffer::createData(const uint32_t size) {
    return calloc(1, size);
}

void AxrPushConstantsBuffer::destroyData(uint32_t& size, void*& data) {
    if (data == nullptr) return;

    size = 0;
    free(data);
    data = nullptr;
}

const uint32_t& AxrPushConstantsBuffer::getSize() const {
    return m_DataSize;
}

const void* AxrPushConstantsBuffer::getData() const {
    return m_Data;
}

// ---- Private Functions ----

void AxrPushConstantsBuffer::cleanup() {
    destroyData(m_DataSize, m_Data);

    m_Name = "";
}

#endif
