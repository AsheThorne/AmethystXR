#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Push constant buffer
class AxrPushConstantBuffer {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrPushConstantBuffer();
    /// Constructor
    /// @param config Push constant buffer config
    AxrPushConstantBuffer(const AxrPushConstantBufferConfig& config);
    /// Copy Constructor
    /// @param src Source AxrPushConstantBuffer to copy from
    AxrPushConstantBuffer(const AxrPushConstantBuffer& src);
    /// Move Constructor
    /// @param src Source AxrPushConstantBuffer to move from
    AxrPushConstantBuffer(AxrPushConstantBuffer&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrPushConstantBuffer();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPushConstantBuffer to copy from
    AxrPushConstantBuffer& operator=(const AxrPushConstantBuffer& src);
    /// Move Assignment Operator
    /// @param src Source AxrPushConstantBuffer to move from
    AxrPushConstantBuffer& operator=(AxrPushConstantBuffer&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the push constant buffer's name
    /// @returns The push constant buffer's name
    [[nodiscard]] const char* getName() const;

    /// Clone the given data
    /// @param size Size of the data
    /// @param data Data to clone
    /// @returns The cloned data
    [[nodiscard]] static void* cloneData(uint32_t size, const void* data);
    /// Create new data of the given size
    /// @param size Size of the data
    /// @returns The data
    [[nodiscard]] static void* createData(uint32_t size);
    /// Destroy the given data
    /// @param size Size of the data
    /// @param data Data to destroy
    static void destroyData(uint32_t& size, void*& data);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Get the buffer size
    /// @returns The buffer size
    [[nodiscard]] const uint32_t& getSize() const;
    /// Get the buffer data
    /// @returns The buffer data
    [[nodiscard]] const void* getData() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_Name;
    uint32_t m_DataSize;
    void* m_Data;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup the push constant buffer class
    void cleanup();
};

#endif
