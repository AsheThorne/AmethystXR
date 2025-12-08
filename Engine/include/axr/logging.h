#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "common/enums.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// ----------------------------------------- //
// Classes
// ----------------------------------------- //

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

        msg.color_range_start = dest.size();

        // Append level name
        dest.push_back(' ');
        const auto levelStringView = spdlog::level::to_string_view(msg.level);
        // Capitalize the first character
        dest.push_back(static_cast<const char>(std::toupper(levelStringView.data()[0])));
        dest.append(levelStringView.begin() + 1, levelStringView.end());

        // Append logger name
        dest.push_back(' ');
        dest.push_back('[');
        spdlog::details::fmt_helper::append_string_view(msg.logger_name, dest);
        dest.push_back(']');

        msg.color_range_end = dest.size();

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
/// @return AXR_SUCCESS on success. AXR_DUPLICATE if the logger name already exists.
AxrResult axrLoggerCreate(const std::string& loggerName);
/// @brief Set the default logger to the named logger
/// @param loggerName The logger's name
/// @return AXR_SUCCESS on success. AXR_ERROR_NOT_FOUND if the logger doesn't exist.
AxrResult axrLoggerSetDefault(const std::string& loggerName);

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- Logger ----

inline void axrLoggerSetup(const std::string& loggerName) {
    axrLoggerCreate(loggerName);
    axrLoggerSetDefault(loggerName);
}

inline AxrResult axrLoggerCreate(const std::string& loggerName) {
    if (spdlog::get(loggerName) != nullptr) {
        return AXR_DUPLICATE;
    }

    const std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(loggerName, spdlog::color_mode::always);
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<AxrFlagFormatter>('.').set_pattern("%.");
    logger->set_formatter(std::move(formatter));
    logger->set_level(spdlog::level::trace);

    logger->log(spdlog::level::info, "Logger Created.");

    return AXR_SUCCESS;
}

inline AxrResult axrLoggerSetDefault(const std::string& loggerName) {
    const std::shared_ptr<spdlog::logger> logger = spdlog::get(loggerName);
    if (logger == nullptr) {
        return AXR_ERROR_NOT_FOUND;
    }

    spdlog::set_default_logger(logger);
    return AXR_SUCCESS;
}
