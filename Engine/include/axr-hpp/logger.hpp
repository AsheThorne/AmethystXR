#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"

namespace axr {
    // ----------------------------------------- //
    // LogLevel Enum
    // ----------------------------------------- //
    /// Log level enum type
    using LogLevel_T = int32_t;

    /// Log level enum
    enum class LogLevelEnum : LogLevel_T {
        Undefined = AXR_LOG_LEVEL_UNDEFINED,
        Warning = AXR_LOG_LEVEL_WARNING,
        Info = AXR_LOG_LEVEL_INFO,
        Error = AXR_LOG_LEVEL_ERROR
    };

    static_assert(
        sizeof(AxrLogLevelEnum) == sizeof(axr::LogLevelEnum),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Function Definitions
    // ----------------------------------------- //

    // ---- Logger ----

    /// Create a new logger
    /// @param loggerName Name of the logger
    inline void createLogger(const std::string& loggerName) {
        axrCreateLogger(loggerName);
    }

    /// Set the default logger for the application
    /// @param loggerName Name of the logger
    inline void setDefaultLogger(const std::string& loggerName) {
        axrSetDefaultLogger(loggerName);
    }

    /// Set the default logger minimum log level
    /// @param level The minimum log level
    inline void setLoggerLevel(const AxrLogLevelEnum level) {
        axrSetLoggerLevel(level);
    }

    /// Set up the logger for the current application
    inline void loggerSetup(const std::string& loggerName) {
        axrLoggerSetup(loggerName);
    }

    // ---- Util Functions ----

    /// Convert a AxrLogLevelEnum to a spdlog level enum
    /// @param level The AxrLogLevelEnum
    /// @returns The spdlog level enum
    inline spdlog::level::level_enum toSpdlogLevel(const AxrLogLevelEnum level) {
        return axrToSpdlogLevel(level);
    }

    // ---- Basic Logging Functions ----

    /// Log an info message
    /// @param message Message
    inline void logInfo(const char* message) {
        axrLogInfo(message);
    }

    /// Log a warning message
    /// @param message Message
    inline void logWarning(const char* message) {
        axrLogWarning(message);
    }

    /// Log an error message
    /// @param message Message
    inline void logError(const char* message) {
        axrLogError(message);
    }
}
