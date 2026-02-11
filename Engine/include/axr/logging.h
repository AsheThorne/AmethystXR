#pragma once

// ----------------------------------------- //
//
// NOTE (Ashe):
//  The logger does not use AxrAllocator.
//  It is safe to use at any stage of the engine's lifecycle.
//
// ----------------------------------------- //


// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <source_location>
#include "common/enums.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// ----------------------------------------- //
// Classes
// ----------------------------------------- //

/// Log message with a location
class AxrLogMessageWithLocation {
public:
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    std::string Message;
    std::source_location Location;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    /// @brief Constructor
    /// @param message Message
    /// @param location Source file location. You don't need to ever change this from the default
    template<typename String>
    // NOLINTNEXTLINE(google-explicit-constructor)
    AxrLogMessageWithLocation(const String& message,
                              const std::source_location& location = std::source_location::current()) :
        Message(message),
        Location(location) {
    }
};

// Reference: https://deepwiki.com/gabime/spdlog/7.2-custom-types-and-formatters
// Based on spdlog::details::full_formatter (the default spdlog formatter)
/// Custom spdlog flag formatter for AXR
class AxrFlagFormatter final : public spdlog::custom_flag_formatter {
public:
    explicit AxrFlagFormatter() = default;

    void format(const spdlog::details::log_msg& msg, const std::tm& tm_time, spdlog::memory_buf_t& dest) override {
        const auto duration = msg.time.time_since_epoch();
        const auto secs = duration_cast<std::chrono::seconds>(duration);

        // cache the date/time part for the next second.
        if (cache_timestamp_ != secs || cached_datetime_.size() == 0) {
            cached_datetime_.clear();
            cached_datetime_.push_back('[');
            spdlog::details::fmt_helper::append_int(tm_time.tm_year + 1'900, cached_datetime_);
            cached_datetime_.push_back('-');

            spdlog::details::fmt_helper::pad2(tm_time.tm_mon + 1, cached_datetime_);
            cached_datetime_.push_back('-');

            spdlog::details::fmt_helper::pad2(tm_time.tm_mday, cached_datetime_);
            cached_datetime_.push_back(' ');

            spdlog::details::fmt_helper::pad2(tm_time.tm_hour, cached_datetime_);
            cached_datetime_.push_back(':');

            spdlog::details::fmt_helper::pad2(tm_time.tm_min, cached_datetime_);
            cached_datetime_.push_back(':');

            spdlog::details::fmt_helper::pad2(tm_time.tm_sec, cached_datetime_);
            cached_datetime_.push_back(']');

            cache_timestamp_ = secs;
        }

        dest.append(cached_datetime_.begin(), cached_datetime_.end());

        // Append level name
        dest.push_back(' ');
        dest.push_back('[');
        msg.color_range_start = dest.size();
        const auto levelStringView = spdlog::level::to_string_view(msg.level);
        // Capitalize the first character
        dest.push_back(static_cast<const char>(std::toupper(levelStringView.data()[0])));
        dest.append(levelStringView.begin() + 1, levelStringView.end());
        msg.color_range_end = dest.size();
        dest.push_back(']');

        // Append logger name
        dest.push_back(' ');
        dest.push_back('[');
        spdlog::details::fmt_helper::append_string_view(msg.logger_name, dest);
        dest.push_back(']');

        // Append source location if it exists
        if (!msg.source.empty()) {
            dest.push_back(' ');
            dest.push_back('[');
            const char* filename =
                spdlog::details::short_filename_formatter<spdlog::details::null_scoped_padder>::basename(
                    msg.source.filename);
            spdlog::details::fmt_helper::append_string_view(filename, dest);
            dest.push_back(':');
            spdlog::details::fmt_helper::append_int(msg.source.line, dest);
            dest.push_back(']');
        }

        dest.push_back(' ');
        dest.push_back(':');

        // Append message
        dest.push_back(' ');
        spdlog::details::fmt_helper::append_string_view(msg.payload, dest);
    }

    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<AxrFlagFormatter>();
    }

private:
    std::chrono::seconds cache_timestamp_{0};
    spdlog::memory_buf_t cached_datetime_;
};

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Log level enum
enum AxrLogLevelEnum {
    AXR_LOG_LEVEL_UNDEFINED = 0,
    AXR_LOG_LEVEL_INFO,
    AXR_LOG_LEVEL_WARNING,
    AXR_LOG_LEVEL_ERROR,
};

// ----------------------------------------- //
// Function Declarations
// ----------------------------------------- //

// ---- Logger ----

/// @brief Set up a new default logger
/// @param loggerName The logger's name
void axrLoggerSetup(const std::string& loggerName);
/// @brief Create a new logger
/// @param loggerName The logger's name
/// @return AXR_SUCCESS on success. AXR_ERROR_DUPLICATE if the logger name already exists.
[[nodiscard]] AxrResult axrLoggerCreate(const std::string& loggerName);
/// @brief Set the default logger to the named logger
/// @param loggerName The logger's name
/// @return AXR_SUCCESS on success. AXR_ERROR_NOT_FOUND if the logger doesn't exist.
[[nodiscard]] AxrResult axrLoggerSetDefault(const std::string& loggerName);

// ---- Util Functions ----

/// @brief Convert a AxrLogLevelEnum to a spdlog level enum
/// @param level The AxrLogLevelEnum
/// @return The spdlog level enum
[[nodiscard]] spdlog::level::level_enum axrToSpdlogLevel(AxrLogLevelEnum level);

// ---- Basic Logging Functions ----

/// @brief Log an info message to the default logger
/// @param message Message
void axrLogInfo(const AxrLogMessageWithLocation& message);
/// @brief Log a warning message to the default logger
/// @param message Message
void axrLogWarning(const AxrLogMessageWithLocation& message);
/// @brief Log an error message to the default logger
/// @param message Message
void axrLogError(const AxrLogMessageWithLocation& message);
/// @brief Log a message to the default logger
/// @param level Log level
/// @param message Message
void axrLog(AxrLogLevelEnum level, const AxrLogMessageWithLocation& message);

/// @brief Log an info message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogInfoForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// @brief Log a warning message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogWarningForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// @brief Log an error message to the named logger
/// @param loggerName Name of the logger
/// @param message Message
void axrLogErrorForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message);
/// @brief Log a message to the named logger
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
void axrLogForLogger(const std::string& loggerName, AxrLogLevelEnum level, const AxrLogMessageWithLocation& message);

// ---- Formatted Logging Functions ----

/// @brief Log an info message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogInfo(const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log a warning message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogWarning(const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log an error message to the default logger
/// @tparam Args Message property types
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogError(const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log a message to the default logger
/// @tparam Args Message property types
/// @param level Log level
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLog(AxrLogLevelEnum level, const AxrLogMessageWithLocation& message, Args... args);

/// @brief Log an info message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogInfoForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log a warning message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogWarningForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log an error message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogErrorForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args);
/// @brief Log a message to the named logger
/// @tparam Args Message property types
/// @param loggerName Name of the logger
/// @param level Log level
/// @param message Message
/// @param args Message properties
template<typename... Args>
void axrLogForLogger(const std::string& loggerName,
                     AxrLogLevelEnum level,
                     const AxrLogMessageWithLocation& message,
                     Args... args);

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- Logger ----

inline void axrLoggerSetup(const std::string& loggerName) {
    (void)axrLoggerCreate(loggerName);
    (void)axrLoggerSetDefault(loggerName);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create logger. "
inline AxrResult axrLoggerCreate(const std::string& loggerName) {
    if (spdlog::get(loggerName) != nullptr) {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Logger named \"{}\" already exists.", loggerName.c_str());
        return AXR_ERROR_DUPLICATE;
    }

    const std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(loggerName);
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<AxrFlagFormatter>('.').set_pattern("%.");
    formatter->need_localtime(true);
    logger->set_formatter(std::move(formatter));
    logger->set_level(axrToSpdlogLevel(AXR_LOG_LEVEL_INFO));

    axrLogInfoForLogger(loggerName, "Logger created.");

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to set default logger. "
inline AxrResult axrLoggerSetDefault(const std::string& loggerName) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Logger named \"{}\" could not be found.", loggerName.c_str());
        return AXR_ERROR_NOT_FOUND;
    }

    spdlog::set_default_logger(logger);
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

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
        default: {
            return spdlog::level::level_enum::n_levels;
        }
    }
}

// ---- Basic Logging Functions ----

inline void axrLogInfo(const AxrLogMessageWithLocation& message) {
    axrLog(AXR_LOG_LEVEL_INFO, message);
}

inline void axrLogWarning(const AxrLogMessageWithLocation& message) {
    axrLog(AXR_LOG_LEVEL_WARNING, message);
}

inline void axrLogError(const AxrLogMessageWithLocation& message) {
    axrLog(AXR_LOG_LEVEL_ERROR, message);
}

inline void axrLog(const AxrLogLevelEnum level, const AxrLogMessageWithLocation& message) {
#ifdef AXR_LOG_SOURCE_LOCATION
    spdlog::log(spdlog::source_loc(message.Location.file_name(),
                                   static_cast<int>(message.Location.line()),
                                   message.Location.function_name()),
                axrToSpdlogLevel(level),
                message.Message);
#else
    spdlog::log(axrToSpdlogLevel(level), message.Message);
#endif
}

inline void axrLogInfoForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_INFO, message);
}

inline void axrLogWarningForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message);
}

inline void axrLogErrorForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message);
}

inline void axrLogForLogger(const std::string& loggerName,
                            const AxrLogLevelEnum level,
                            const AxrLogMessageWithLocation& message) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) {
        return;
    }

#ifdef AXR_LOG_SOURCE_LOCATION
    logger->log(spdlog::source_loc(message.Location.file_name(),
                                   static_cast<int>(message.Location.line()),
                                   message.Location.function_name()),
                axrToSpdlogLevel(level),
                message.Message);
#else
    logger->log(axrToSpdlogLevel(level), message.Message);
#endif
}

// ---- Formatted Logging Functions ----

template<typename... Args>
void axrLogInfo(const AxrLogMessageWithLocation& message, Args... args) {
    axrLog(AXR_LOG_LEVEL_INFO, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLogWarning(const AxrLogMessageWithLocation& message, Args... args) {
    axrLog(AXR_LOG_LEVEL_WARNING, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLogError(const AxrLogMessageWithLocation& message, Args... args) {
    axrLog(AXR_LOG_LEVEL_ERROR, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLog(const AxrLogLevelEnum level, const AxrLogMessageWithLocation& message, Args... args) {
#ifdef AXR_LOG_SOURCE_LOCATION
    spdlog::log(spdlog::source_loc(message.Location.file_name(),
                                   static_cast<int>(message.Location.line()),
                                   message.Location.function_name()),
                axrToSpdlogLevel(level),
                fmt::runtime(message.Message),
                std::forward<Args>(args)...);
#else
    spdlog::log(axrToSpdlogLevel(level), fmt::runtime(message.Message), std::forward<Args>(args)...);
#endif
}

template<typename... Args>
void axrLogInfoForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_INFO, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLogWarningForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args) {
    axrLogLocationForLogger(loggerName, AXR_LOG_LEVEL_WARNING, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLogErrorForLogger(const std::string& loggerName, const AxrLogMessageWithLocation& message, Args... args) {
    axrLogForLogger(loggerName, AXR_LOG_LEVEL_ERROR, message, std::forward<Args>(args)...);
}

template<typename... Args>
void axrLogForLogger(const std::string& loggerName,
                     const AxrLogLevelEnum level,
                     const AxrLogMessageWithLocation& message,
                     Args... args) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) {
        return;
    }

#ifdef AXR_LOG_SOURCE_LOCATION
    logger->log(spdlog::source_loc(message.Location.file_name(),
                                   static_cast<int>(message.Location.line()),
                                   message.Location.function_name()),
                axrToSpdlogLevel(level),
                fmt::runtime(message.Message),
                std::forward<Args>(args)...);
#else
    logger->log(axrToSpdlogLevel(level), fmt::runtime(message.Message), std::forward<Args>(args)...);
#endif
}
