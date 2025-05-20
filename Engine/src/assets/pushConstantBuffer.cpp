#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "pushConstantBuffer.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

void* axrPushConstantBufferCloneData(const uint32_t size, const void* data) {
    return AxrPushConstantBuffer::cloneData(size, data);
}

void axrPushConstantBufferDestroyData(uint32_t* size, void** data) {
    return AxrPushConstantBuffer::destroyData(*size, *data);
}

const char* axrPushConstantBufferGetName(const AxrPushConstantBuffer_T pushConstantBuffer) {
    if (pushConstantBuffer == nullptr) {
        axrLogErrorLocation("`pushConstantBuffer` is null.");
        return "";
    }

    return pushConstantBuffer->getName().c_str();
}


// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrPushConstantBuffer::AxrPushConstantBuffer():
    m_Name(""),
    m_DataSize(0),
    m_Data(nullptr) {
}

AxrPushConstantBuffer::AxrPushConstantBuffer(const AxrPushConstantBufferConfig& config):
    m_Name(config.Name),
    m_DataSize(config.DataSize),
    m_Data(nullptr) {
    if (config.Data == nullptr) {
        m_Data = createData(config.DataSize);
    } else {
        m_Data = cloneData(m_DataSize, config.Data);
    }
}

AxrPushConstantBuffer::AxrPushConstantBuffer(const AxrPushConstantBuffer& src) {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = cloneData(src.m_DataSize, src.m_Data);
}

AxrPushConstantBuffer::AxrPushConstantBuffer(AxrPushConstantBuffer&& src) noexcept {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = src.m_Data;

    src.m_Name = "";
    src.m_DataSize = 0;
    src.m_Data = nullptr;
}

AxrPushConstantBuffer::~AxrPushConstantBuffer() {
    cleanup();
}

AxrPushConstantBuffer& AxrPushConstantBuffer::operator=(const AxrPushConstantBuffer& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_DataSize = src.m_DataSize;
        m_Data = cloneData(src.m_DataSize, src.m_Data);
    }
    return *this;
}

AxrPushConstantBuffer& AxrPushConstantBuffer::operator=(AxrPushConstantBuffer&& src) noexcept {
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

const std::string& AxrPushConstantBuffer::getName() const {
    return m_Name;
}

void* AxrPushConstantBuffer::cloneData(const uint32_t size, const void* data) {
    if (data == nullptr) return nullptr;

    void* newData = malloc(size);
    memcpy_s(newData, size, data, size);
    return newData;
}

void* AxrPushConstantBuffer::createData(const uint32_t size) {
    return calloc(1, size);
}

void AxrPushConstantBuffer::destroyData(uint32_t& size, void*& data) {
    if (data == nullptr) return;

    size = 0;
    free(data);
    data = nullptr;
}

const uint32_t& AxrPushConstantBuffer::getSize() const {
    return m_DataSize;
}

const void* AxrPushConstantBuffer::getData() const {
    return m_Data;
}

// ---- Private Functions ----

void AxrPushConstantBuffer::cleanup() {
    destroyData(m_DataSize, m_Data);

    m_Name = "";
}

#endif
