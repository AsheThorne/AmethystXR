#pragma once


/// AmethystXr Application
class AxrApplication {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Application config
    AxrApplication(const AxrApplicationConfig& config);
    /// Copy Constructor
    /// @param src Source AxrApplication to copy from
    AxrApplication(const AxrApplication& src) = delete;
    /// Move Constructor
    /// @param src Source AxrApplication to move from
    AxrApplication(AxrApplication&& src)  = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrApplication();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrApplication to copy from
    AxrApplication& operator=(const AxrApplication& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrApplication to move from
    AxrApplication& operator=(AxrApplication&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the AxrApplication class
    void setup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrApplicationConfig m_Config;
};
