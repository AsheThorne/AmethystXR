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
    inline void setLoggerLevel(const axr::LogLevelEnum level) {
        axrSetLoggerLevel(static_cast<AxrLogLevelEnum>(level));
    }

    /// Set up the logger for the current application
    inline void loggerSetup(const std::string& loggerName) {
        axrLoggerSetup(loggerName);
    }

    // ---- Util Functions ----

    /// Convert an axr::LogLevelEnum to a spdlog level enum
    /// @param level The axr::LogLevelEnum
    /// @returns The spdlog level enum
    inline spdlog::level::level_enum toSpdlogLevel(const axr::LogLevelEnum level) {
        return axrToSpdlogLevel(static_cast<AxrLogLevelEnum>(level));
    }

    // ---- Basic Logging Functions ----

    /// Log an info message to the default logger
    /// @param message Message
    inline void logInfo(const char* message) {
        axrLogInfo(message);
    }

    /// Log a warning message to the default logger
    /// @param message Message
    inline void logWarning(const char* message) {
        axrLogWarning(message);
    }

    /// Log an error message to the default logger
    /// @param message Message
    inline void logError(const char* message) {
        axrLogError(message);
    }

    /// Log a message to the default logger
    /// @param level Log level
    /// @param message Message
    inline void log(const axr::LogLevelEnum level, const char* message) {
        axrLog(static_cast<AxrLogLevelEnum>(level), message);
    }

    /// Log an info message to the named logger
    /// @param loggerName Name of the logger
    /// @param message Message
    inline void logInfoForLogger(const std::string& loggerName, const char* message) {
        axrLogInfoForLogger(loggerName, message);
    }

    /// Log a warning message to the named logger
    /// @param loggerName Name of the logger
    /// @param message Message
    inline void logWarningForLogger(const std::string& loggerName, const char* message) {
        axrLogWarningForLogger(loggerName, message);
    }

    /// Log an error message to the named logger
    /// @param loggerName Name of the logger
    /// @param message Message
    inline void logErrorForLogger(const std::string& loggerName, const char* message) {
        axrLogErrorForLogger(loggerName, message);
    }

    /// Log a message to the named logger
    /// @param loggerName Name of the logger
    /// @param level Log level
    /// @param message Message
    inline void logForLogger(const std::string& loggerName, const axr::LogLevelEnum level, const char* message) {
        axrLogForLogger(loggerName, static_cast<AxrLogLevelEnum>(level), message);
    }

    // ---- Formatted Logging Functions ----

    /// Log an info message to the default logger
    /// @tparam Args Message property types
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logInfo(spdlog::format_string_t<Args...> message, Args... args) {
        axrLogInfo(message, std::forward<Args>(args)...);
    }

    /// Log a warning message to the default logger
    /// @tparam Args Message property types
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logWarning(spdlog::format_string_t<Args...> message, Args... args) {
        axrLogWarning(message, std::forward<Args>(args)...);
    }

    /// Log an error message to the default logger
    /// @tparam Args Message property types
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logError(spdlog::format_string_t<Args...> message, Args... args) {
        axrLogError(message, std::forward<Args>(args)...);
    }

    /// Log a message to the default logger
    /// @tparam Args Message property types
    /// @param level Log level
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void log(const axr::LogLevelEnum level, spdlog::format_string_t<Args...> message, Args... args) {
        axrLog(static_cast<AxrLogLevelEnum>(level), message, std::forward<Args>(args)...);
    }

    /// Log an info message to the named logger
    /// @tparam Args Message property types
    /// @param loggerName Name of the logger
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logInfoForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
        axrLogInfoForLogger(loggerName, message, std::forward<Args>(args)...);
    }

    /// Log a warning message to the named logger
    /// @tparam Args Message property types
    /// @param loggerName Name of the logger
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logWarningForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
        axrLogWarningForLogger(loggerName, message, std::forward<Args>(args)...);
    }

    /// Log an error message to the named logger
    /// @tparam Args Message property types
    /// @param loggerName Name of the logger
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logErrorForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
        axrLogErrorForLogger(loggerName, message, std::forward<Args>(args)...);
    }

    /// Log a message to the named logger
    /// @tparam Args Message property types
    /// @param loggerName Name of the logger
    /// @param level Log level
    /// @param message Message
    /// @param args Message property values
    template <typename... Args>
    void logForLogger(
        const std::string& loggerName,
        const axr::LogLevelEnum level,
        spdlog::format_string_t<Args...> message,
        Args... args
    ) {
        axrLogForLogger(loggerName, static_cast<AxrLogLevelEnum>(level), message, std::forward<Args>(args)...);
    }
}
