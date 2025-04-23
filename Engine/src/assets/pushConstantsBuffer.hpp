#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Push constants buffer
class AxrPushConstantsBuffer {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrPushConstantsBuffer();
    /// Constructor
    /// @param config Push constants buffer config
    AxrPushConstantsBuffer(const AxrPushConstantsBufferConfig& config);
    /// Copy Constructor
    /// @param src Source AxrPushConstantsBuffer to copy from
    AxrPushConstantsBuffer(const AxrPushConstantsBuffer& src);
    /// Move Constructor
    /// @param src Source AxrPushConstantsBuffer to move from
    AxrPushConstantsBuffer(AxrPushConstantsBuffer&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrPushConstantsBuffer();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPushConstantsBuffer to copy from
    AxrPushConstantsBuffer& operator=(const AxrPushConstantsBuffer& src);
    /// Move Assignment Operator
    /// @param src Source AxrPushConstantsBuffer to move from
    AxrPushConstantsBuffer& operator=(AxrPushConstantsBuffer&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the push constants buffer's name
    /// @returns The push constants buffer's name
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

    /// Cleanup the push constants buffer class
    void cleanup();
};

#endif
