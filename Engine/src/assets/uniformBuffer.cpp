// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "uniformBuffer.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

void* axrUniformBufferCloneData(const uint64_t size, const void* data) {
    return AxrUniformBuffer::cloneData(size, data);
}

void axrUniformBufferDestroyData(uint64_t* size, void** data) {
    return AxrUniformBuffer::destroyData(*size, *data);
}

const char* axrUniformBufferGetName(const AxrUniformBuffer_T uniformBuffer) {
    if (uniformBuffer == nullptr) {
        axrLogErrorLocation("`uniformBuffer` is null.");
        return "";
    }

    return uniformBuffer->getName().c_str();
}


// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrUniformBuffer::AxrUniformBuffer():
    m_Name(""),
    m_DataSize(0),
    m_Data(nullptr) {
}

AxrUniformBuffer::AxrUniformBuffer(const AxrUniformBufferConfig& config):
    m_Name(config.Name),
    m_DataSize(config.DataSize),
    m_Data(nullptr) {
    if (config.Data == nullptr) {
        m_Data = createData(config.DataSize);
    } else {
        m_Data = cloneData(m_DataSize, config.Data);
    }}

AxrUniformBuffer::AxrUniformBuffer(const AxrUniformBuffer& src) {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = cloneData(src.m_DataSize, src.m_Data);
}

AxrUniformBuffer::AxrUniformBuffer(AxrUniformBuffer&& src) noexcept {
    m_Name = src.m_Name;
    m_DataSize = src.m_DataSize;
    m_Data = src.m_Data;

    src.m_Name = "";
    src.m_DataSize = 0;
    src.m_Data = nullptr;
}

AxrUniformBuffer::~AxrUniformBuffer() {
    cleanup();
}

AxrUniformBuffer& AxrUniformBuffer::operator=(const AxrUniformBuffer& src) {
    if (this != &src) {
        cleanup();

        m_Name = src.m_Name;
        m_DataSize = src.m_DataSize;
        m_Data = cloneData(src.m_DataSize, src.m_Data);
    }
    return *this;
}

AxrUniformBuffer& AxrUniformBuffer::operator=(AxrUniformBuffer&& src) noexcept {
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

const std::string& AxrUniformBuffer::getName() const {
    return m_Name;
}

void* AxrUniformBuffer::cloneData(const uint64_t size, const void* data) {
    void* newData = createData(size);
    memcpy_s(newData, size, data, size);
    return newData;
}

void AxrUniformBuffer::destroyData(uint64_t& size, void*& data) {
    if (data != nullptr) return;

    free(data);
    data = nullptr;
    size = 0;
}

void* AxrUniformBuffer::createData(const uint64_t size) {
    return malloc(size);
}

const uint64_t& AxrUniformBuffer::getSize() const {
    return m_DataSize;
}

const void* AxrUniformBuffer::getData() const {
    return m_Data;
}

// ---- Private Functions ----

void AxrUniformBuffer::cleanup() {
    destroyData(m_DataSize, m_Data);

    m_Name = "";
}
