#pragma once

// ----------------------------------------- //
// Spdlog Headers
// ----------------------------------------- //
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Log level enum
enum AxrLogLevelEnum {
    AXR_LOG_LEVEL_UNDEFINED = 0,
    AXR_LOG_LEVEL_INFO = 1,
    AXR_LOG_LEVEL_WARNING = 2,
    AXR_LOG_LEVEL_ERROR = 3,
};

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- Logger ----

/// Create a new logger
/// @param loggerName Name of the logger
void axrCreateLogger(const std::string& loggerName);
/// Set the default logger for the application
/// @param loggerName Name of the logger
void axrSetDefaultLogger(const std::string& loggerName);
/// Set the default logger minimum log level
/// @param level The minimum log level
void axrSetLoggerLevel(AxrLogLevelEnum level);

/// Set up the logger for the current application
void axrLoggerSetup(const std::string& loggerName);

// ---- Util Functions ----

/// Convert a AxrLogLevelEnum to a spdlog level enum
/// @param level The AxrLogLevelEnum
/// @returns The spdlog level enum
spdlog::level::level_enum axrToSpdlogLevel(AxrLogLevelEnum level);

// ---- Basic Logging Functions ----

/// Log an info message to the default logger
/// @param message Message
void axrLogInfo(const char* message);
/// Log a warning message to the default logger
/// @param message Message
void axrLogWarning(const char* message);
/// Log an error message to the default logger
/// @param message Message
void axrLogError(const char* message);
/// Log a message to the default logger
/// @param level Log level
/// @param message Message
void axrLog(AxrLogLevelEnum level, const char* message);

/// Log an info message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogInfoForLogger(const std::string& loggerName, const char* message);
/// Log a warning message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogWarningForLogger(const std::string& loggerName, const char* message);
/// Log an error message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogErrorForLogger(const std::string& loggerName, const char* message);
/// Log a message to the named logger
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
void axrLogForLogger(const std::string& loggerName, AxrLogLevelEnum level, const char* message);

// ---- Formatted Logging Functions ----

/// Log an info message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogInfo(spdlog::format_string_t<Args...> message, Args... args);
/// Log a warning message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogWarning(spdlog::format_string_t<Args...> message, Args... args);
/// Log an error message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogError(spdlog::format_string_t<Args...> message, Args... args);
/// Log a message to the default logger
/// @tparam Args Message property types
/// @param level Log level
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLog(AxrLogLevelEnum level, spdlog::format_string_t<Args...> message, Args... args);

/// Log an info message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogInfoForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args);
/// Log a warning message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogWarningForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args);
/// Log an error message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogErrorForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args);
/// Log a message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogForLogger(
    const std::string& loggerName,
    AxrLogLevelEnum level,
    spdlog::format_string_t<Args...> message,
    Args... args
);

// ----------------------------------------- //
// Function Implementations
// ----------------------------------------- //

// ---- Logger ----

inline void axrCreateLogger(const std::string& loggerName) {
    // Formatting flags: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
    spdlog::set_pattern("%^[%T] %l %n : %v%$");

    const std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(loggerName);
    logger->set_level(axrToSpdlogLevel(AXR_LOG_LEVEL_INFO));
    logger->info("Logger Created.");
}

inline void axrSetDefaultLogger(const std::string& loggerName) {
    spdlog::set_default_logger(spdlog::get(loggerName));
}

inline void axrSetLoggerLevel(const AxrLogLevelEnum level) {
    spdlog::set_level(axrToSpdlogLevel(level));
}


inline void axrLoggerSetup(const std::string& loggerName) {
    if (spdlog::get(loggerName) != nullptr) return;

    axrCreateLogger(loggerName);
    axrSetDefaultLogger(loggerName);
}


// ---- Util Functions ----

inline spdlog::level::level_enum axrToSpdlogLevel(const AxrLogLevelEnum level) {
    switch (level) {
        case AXR_LOG_LEVEL_INFO: {
            return spdlog::level::level_enum::info;
        }
        case AXR_LOG_LEVEL_WARNING: {
            return spdlog::level::level_enum::warn;
        }
        case AXR_LOG_LEVEL_ERROR: {
            return spdlog::level::level_enum::err;
        }
        case AXR_LOG_LEVEL_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            return spdlog::level::level_enum::n_levels;
        }
    }
}

// ---- Basic Logging Functions ----

inline void axrLogInfo(const char* message) {
    spdlog::info(message);
}

inline void axrLogWarning(const char* message) {
    spdlog::warn(message);
}

inline void axrLogError(const char* message) {
    spdlog::error(message);
}

inline void axrLog(const AxrLogLevelEnum level, const char* message) {
    spdlog::log(axrToSpdlogLevel(level), message);
}

inline void axrLogInfoForLogger(const std::string& loggerName, const char* message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_INFO, message);
}

inline void axrLogWarningForLogger(const std::string& loggerName, const char* message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message);
}

inline void axrLogErrorForLogger(const std::string& loggerName, const char* message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message);
}

inline void axrLogForLogger(const std::string& loggerName, const AxrLogLevelEnum level, const char* message) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);

    if (logger == nullptr) {
        return;
    }

    logger->log(axrToSpdlogLevel(level), message);
}

// ---- Formatted Logging Functions ----

template <typename... Args>
void axrLogInfo(spdlog::format_string_t<Args...> message, Args... args) {
    spdlog::info(message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogWarning(spdlog::format_string_t<Args...> message, Args... args) {
    spdlog::warn(message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogError(spdlog::format_string_t<Args...> message, Args... args) {
    spdlog::error(message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLog(const AxrLogLevelEnum level, spdlog::format_string_t<Args...> message, Args... args) {
    spdlog::log(axrToSpdlogLevel(level), message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogInfoForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_INFO, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogWarningForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogErrorForLogger(const std::string& loggerName, spdlog::format_string_t<Args...> message, Args... args) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogForLogger(
    const std::string& loggerName,
    const AxrLogLevelEnum level,
    spdlog::format_string_t<Args...> message,
    Args... args
) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);

    if (logger == nullptr) {
        return;
    }

    logger->log(axrToSpdlogLevel(level), message, std::forward<Args>(args)...);
}
