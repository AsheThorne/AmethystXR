#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

/// Axr Image Sampler
class AxrImageSampler {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrImageSampler();
    /// Constructor
    /// @param config Image sampler config
    AxrImageSampler(const AxrImageSamplerConfig& config);
    /// Copy Constructor
    /// @param src Source AxrImageSampler to copy from
    AxrImageSampler(const AxrImageSampler& src);
    /// Move Constructor
    /// @param src Source AxrImageSampler to move from
    AxrImageSampler(AxrImageSampler&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrImageSampler();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrImageSampler to copy from
    AxrImageSampler& operator=(const AxrImageSampler& src);
    /// Move Assignment Operator
    /// @param src Source AxrImageSampler to move from
    AxrImageSampler& operator=(AxrImageSampler&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the image sampler name
    /// @returns The name of the image sampler
    [[nodiscard]] const std::string& getName() const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Get the image sampler filter enum
    /// @returns The image sampler filter enum
    [[nodiscard]] AxrImageSamplerFilterEnum getFilter() const;
    /// Get the image sampler wrapping enum
    /// @returns The image sampler wrapping enum
    [[nodiscard]] AxrImageSamplerWrappingEnum getWrapping() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    AxrImageSamplerFilterEnum m_Filter;
    AxrImageSamplerWrappingEnum m_Wrapping;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
