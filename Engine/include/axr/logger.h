#pragma once

// ----------------------------------------- //
// Spdlog Headers
// ----------------------------------------- //
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <source_location>
#include <string>

// ----------------------------------------- //
// Macros
// ----------------------------------------- //

#define AXR_LOGGING_LOCATION_MESSAGE(message) \
fmt::runtime((message) + "\n{axrFileName}:{axrLine} {axrFunctionName}")

#define AXR_LOGGING_LOCATION_ARGS(location) \
fmt::arg("axrFileName", (location).file_name()), \
fmt::arg("axrLine", (location).line()), \
fmt::arg("axrFunctionName", (location).function_name())

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
// Structs
// ----------------------------------------- //

/// Log message with a location
struct AxrLogMessageWithLocation {
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    std::string Message;
    std::source_location Location;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    /// Constructor
    /// @param message Message
    /// @param location Source file location. You don't need to ever change this from the default
    template <typename String>
    AxrLogMessageWithLocation(
        const String& message,
        const std::source_location& location = std::source_location::current()
    ) :
        Message(message),
        Location(location) {
    }
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
/// Set the minimum log level for the default logger
/// @param level The minimum log level
void axrSetLoggerLevel(AxrLogLevelEnum level);
/// Set the minimum log level for the given logger
/// @param loggerName Logger name
/// @param level The minimum log level
void axrSetLoggerLevel(const std::string& loggerName, AxrLogLevelEnum level);
/// Set the pattern for the default logger
/// @param pattern Logger pattern
void axrSetLoggerPattern(const std::string& pattern);
/// Set the pattern for the given logger
/// @param loggerName Logger name
/// @param pattern Logger pattern
void axrSetLoggerPattern(const std::string& loggerName, const std::string& pattern);

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

// ---- Basic Logging Functions With Location ----

/// Log an info message to the default logger with the source location
/// @param message Message
void axrLogInfoLocation(const AxrLogMessageWithLocation& message);
/// Log a warning message to the default logger with the source location
/// @param message Message
void axrLogWarningLocation(const AxrLogMessageWithLocation& message);
/// Log an error message to the default logger with the source location
/// @param message Message
void axrLogErrorLocation(const AxrLogMessageWithLocation& message);
/// Log a message to the default logger with the source location
/// @param level Log level
/// @param message Message
void axrLogLocation(AxrLogLevelEnum level, const AxrLogMessageWithLocation& message);

/// Log an info message to the named logger with the source location
/// @param loggerName Name of the logger
/// @param message Message
void axrLogInfoLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// Log a warning message to the named logger with the source location
/// @param loggerName Name of the logger
/// @param message Message
void axrLogWarningLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// Log an error message to the named logger with the source location
/// @param loggerName Name of the logger
/// @param message Message
void axrLogErrorLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// Log a message to the named logger with the source location
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
void axrLogForLocationLogger(
    const std::string& loggerName,
    AxrLogLevelEnum level,
    const AxrLogMessageWithLocation& message
);

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

// ---- Formatted Logging Functions With Location ----

/// Log an info message to the default logger with the source location
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogInfoLocation(const AxrLogMessageWithLocation& message, Args... args);
/// Log a warning message to the default logger with the source location
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogWarningLocation(const AxrLogMessageWithLocation& message, Args... args);
/// Log an error message to the default logger with the source location
/// @tparam Args Message property types
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogErrorLocation(const AxrLogMessageWithLocation& message, Args... args);
/// Log a message to the default logger with the source location
/// @tparam Args Message property types
/// @param level Log level
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogLocation(AxrLogLevelEnum level, const AxrLogMessageWithLocation& message, Args... args);

/// Log an info message to the named logger with the source location
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogInfoLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args);
/// Log a warning message to the named logger with the source location
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogWarningLocationForLogger(
    const std::string& loggerName,
    const AxrLogMessageWithLocation& message,
    Args... args
);
/// Log an error message to the named logger with the source location
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogErrorLocationForLogger(
    const std::string& loggerName,
    const AxrLogMessageWithLocation& message,
    Args... args
);
/// Log a message to the named logger with the source location
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
/// @param args Message property values
template <typename... Args>
void axrLogLocationForLogger(
    const std::string& loggerName,
    AxrLogLevelEnum level,
    const AxrLogMessageWithLocation& message,
    Args... args
);

// ----------------------------------------- //
// Function Implementations
// ----------------------------------------- //

// ---- Logger ----

inline void axrCreateLogger(const std::string& loggerName) {
    // If the logger name already exists, return
    if (spdlog::get(loggerName) != nullptr) return;

    const std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(loggerName);
    logger->set_level(axrToSpdlogLevel(AXR_LOG_LEVEL_INFO));
    axrLogInfoForLogger(loggerName, "Logger Created.");
}

inline void axrSetDefaultLogger(const std::string& loggerName) {
    spdlog::set_default_logger(spdlog::get(loggerName));
}

inline void axrSetLoggerLevel(const AxrLogLevelEnum level) {
    spdlog::set_level(axrToSpdlogLevel(level));
}

inline void axrSetLoggerLevel(const std::string& loggerName, AxrLogLevelEnum level) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) return;

    logger->set_level(axrToSpdlogLevel(level));
}

inline void axrSetLoggerPattern(const std::string& pattern) {
    spdlog::logger* logger = spdlog::default_logger_raw();
    if (logger == nullptr) return;

    logger->set_pattern(pattern);
}

inline void axrSetLoggerPattern(const std::string& loggerName, const std::string& pattern) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) return;

    logger->set_pattern(pattern);
}


inline void axrLoggerSetup(const std::string& loggerName) {
    // Formatting flags: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
    spdlog::set_pattern("%^[%T] %l %n : %v%$");

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

// ---- Basic Logging Functions With Location ----

inline void axrLogInfoLocation(const AxrLogMessageWithLocation& message) {
    axrLogLocation(AXR_LOG_LEVEL_INFO, message);
}

inline void axrLogWarningLocation(const AxrLogMessageWithLocation& message) {
    axrLogLocation(AXR_LOG_LEVEL_WARNING, message);
}

inline void axrLogErrorLocation(const AxrLogMessageWithLocation& message) {
    axrLogLocation(AXR_LOG_LEVEL_ERROR, message);
}

inline void axrLogLocation(const AxrLogLevelEnum level, const AxrLogMessageWithLocation& message) {
    spdlog::log(
        axrToSpdlogLevel(level),
        AXR_LOGGING_LOCATION_MESSAGE(message.Message),
        AXR_LOGGING_LOCATION_ARGS(message.Location)
    );
}

inline void axrLogInfoLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_INFO, message);
}

inline void axrLogWarningLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message);
}

inline void axrLogErrorLocationForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message);
}

inline void axrLogLocationForLogger(
    const std::string& loggerName,
    const AxrLogLevelEnum level,
    const AxrLogMessageWithLocation& message
) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);

    if (logger == nullptr) {
        return;
    }

    logger->log(
        axrToSpdlogLevel(level),
        AXR_LOGGING_LOCATION_MESSAGE(message.Message),
        AXR_LOGGING_LOCATION_ARGS(message.Location)
    );
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

// ---- Formatted Logging Functions With Location ----

template <typename... Args>
void axrLogInfoLocation(const AxrLogMessageWithLocation& message, Args... args) {
    axrLogLocation(AXR_LOG_LEVEL_INFO, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogWarningLocation(const AxrLogMessageWithLocation& message, Args... args) {
    axrLogLocation(AXR_LOG_LEVEL_WARNING, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogErrorLocation(const AxrLogMessageWithLocation& message, Args... args) {
    axrLogLocation(AXR_LOG_LEVEL_ERROR, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogLocation(const AxrLogLevelEnum level, const AxrLogMessageWithLocation& message, Args... args) {
    spdlog::log(
        axrToSpdlogLevel(level),
        AXR_LOGGING_LOCATION_MESSAGE(message.Message),
        std::forward<Args>(args)...,
        AXR_LOGGING_LOCATION_ARGS(message.Location)
    );
}

template <typename... Args>
void axrLogInfoLocationForLogger(
    const std::string& loggerName,
    const AxrLogMessageWithLocation& message,
    Args... args
) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_INFO, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogWarningLocationForLogger(
    const std::string& loggerName,
    const AxrLogMessageWithLocation& message,
    Args... args
) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogErrorLocationForLogger(
    const std::string& loggerName,
    const AxrLogMessageWithLocation& message,
    Args... args
) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message, std::forward<Args>(args)...);
}

template <typename... Args>
void axrLogLocationForLogger(
    const std::string& loggerName,
    const AxrLogLevelEnum level,
    const AxrLogMessageWithLocation& message,
    Args... args
) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);

    if (logger == nullptr) {
        return;
    }

    logger->log(
        axrToSpdlogLevel(level),
        AXR_LOGGING_LOCATION_MESSAGE(message.Message),
        std::forward<Args>(args)...,
        AXR_LOGGING_LOCATION_ARGS(message.Location)
    );
}
