#pragma once

#include <VREngine/Core/Includes.hpp>

#ifndef LOG_LEVEL
#define LOG_LEVEL(level, fmt, ...) vre::log(level, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef LOG_FATAL
#define LOG_FATAL(fmt, ...) vre::log(vre::LogLevel::eFatal, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef LOG_ERROR
#define LOG_ERROR(fmt, ...) vre::log(vre::LogLevel::eError, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef LOG_WARN
#define LOG_WARN(fmt, ...) vre::log(vre::LogLevel::eWarn, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef LOG_DEBUG
#define LOG_DEBUG(fmt, ...) vre::log(vre::LogLevel::eDebug, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef LOG_INFO
#define LOG_INFO(fmt, ...) vre::log(vre::LogLevel::eInfo, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif

#ifndef VRE_LOG
#define VRE_LOG(level, fmt, ...) vre::Logger::Log(level, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef VRE_FATAL
#define VRE_FATAL(fmt, ...) vre::Logger::Log(vre::LogLevel::eFatal, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef VRE_ERROR
#define VRE_ERROR(fmt, ...) vre::Logger::Log(vre::LogLevel::eError, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef VRE_WARN
#define VRE_WARN(fmt, ...) vre::Logger::Log(vre::LogLevel::eWarn, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef VRE_DEBUG
#define VRE_DEBUG(fmt, ...) vre::Logger::Log(vre::LogLevel::eDebug, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef VRE_INFO
#define VRE_INFO(fmt, ...) vre::Logger::Log(vre::LogLevel::eInfo, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif

#ifdef VRE_BUILD_TYPE_DEBUG
#ifndef DLOG_LEVEL
#define DLOG_LEVEL(level, fmt, ...) vre::log(level, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DLOG_FATAL
#define DLOG_FATAL(fmt, ...) vre::log(vre::LogLevel::eFatal, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DLOG_ERROR
#define DLOG_ERROR(fmt, ...) vre::log(vre::LogLevel::eError, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DLOG_WARN
#define DLOG_WARN(fmt, ...) vre::log(vre::LogLevel::eWarn, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DLOG_DEBUG
#define DLOG_DEBUG(fmt, ...) vre::log(vre::LogLevel::eDebug, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DLOG_INFO
#define DLOG_INFO(fmt, ...) vre::log(vre::LogLevel::eInfo, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif

#ifndef DVRE_LOG
#define DVRE_LOG(level, fmt, ...) vre::Logger::Log(level, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DVRE_FATAL
#define DVRE_FATAL(fmt, ...) vre::Logger::Log(vre::LogLevel::eFatal, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DVRE_ERROR
#define DVRE_ERROR(fmt, ...) vre::Logger::Log(vre::LogLevel::eError, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DVRE_WARN
#define DVRE_WARN(fmt, ...) vre::Logger::Log(vre::LogLevel::eWarn, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DVRE_DEBUG
#define DVRE_DEBUG(fmt, ...) vre::Logger::Log(vre::LogLevel::eDebug, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#ifndef DVRE_INFO
#define DVRE_INFO(fmt, ...) vre::Logger::Log(vre::LogLevel::eInfo, __FUNCTION__, std::source_location::current(), fmt, ##__VA_ARGS__)
#endif
#else
#ifndef DLOG_LEVEL
#define DLOG_LEVEL(level, fmt, ...)
#endif
#ifndef DLOG_FATAL
#define DLOG_FATAL(fmt, ...)
#endif
#ifndef DLOG_ERROR
#define DLOG_ERROR(fmt, ...)
#endif
#ifndef DLOG_WARN
#define DLOG_WARN(fmt, ...)
#endif
#ifndef DLOG_DEBUG
#define DLOG_DEBUG(fmt, ...)
#endif
#ifndef DLOG_INFO
#define DLOG_INFO(fmt, ...)
#endif

#ifndef DVRE_LOG
#define DVRE_LOG(level, fmt, ...)
#endif
#ifndef DVRE_FATAL
#define DVRE_FATAL(fmt, ...)
#endif
#ifndef DVRE_ERROR
#define DVRE_ERROR(fmt, ...)
#endif
#ifndef DVRE_WARN
#define DVRE_WARN(fmt, ...)
#endif
#ifndef DVRE_DEBUG
#define DVRE_DEBUG(fmt, ...)
#endif
#ifndef DVRE_INFO
#define DVRE_INFO(fmt, ...)
#endif
#endif

#ifndef VRE_ASSERT
#define VRE_ASSERT(condition, ...) vre::assertImpl((condition), #condition, __FUNCTION__, std::source_location::current(), ##__VA_ARGS__)
#endif

#ifndef VRE_VK_CHECK
#define VRE_VK_CHECK(result)                                               \
    do {                                                                   \
        vk::Result _VRE_VK_RESULT_ = result;                               \
        if (_VRE_VK_RESULT_ != vk::Result::eSuccess) {                     \
            LOG_ERROR("Vulkan Error: {}", vk::to_string(_VRE_VK_RESULT_)); \
            std::abort();                                                  \
        }                                                                  \
    } while (false)
#endif

#ifdef VRE_BUILD_TYPE_DEBUG
#ifndef DVRE_ASSERT
#define DVRE_ASSERT(condition, ...) vre::assertImpl((condition), #condition, __FUNCTION__, std::source_location::current(), ##__VA_ARGS__)
#endif
#ifndef DVRE_VK_CHECK
#define DVRE_VK_CHECK(result)                                              \
    do {                                                                   \
        vk::Result _VRE_VK_RESULT_ = result;                               \
        if (_VRE_VK_RESULT_ != vk::Result::eSuccess) {                     \
            LOG_ERROR("Vulkan Error: {}", vk::to_string(_VRE_VK_RESULT_)); \
            std::abort();                                                  \
        }                                                                  \
    } while (false)
#endif
#else
#ifndef DVRE_ASSERT
#define DVRE_ASSERT(condition, ...) \
    do {                            \
        bool result = condition;    \
    } while (false)
#endif
#ifndef DVRE_VK_CHECK
#define DVRE_VK_CHECK(result)  \
    do {                       \
        vk::Result _ = result; \
    } while (false)
#endif
#endif

namespace vre {
    enum class LogLevel {
        eFatal = 0,
        eError = 1,
        eWarn  = 2,
        eDebug = 3,
        eInfo  = 4,
    };

    void        printLog(LogLevel level, const std::string &function, const std::source_location &location, const std::string &log);
    std::string makeLogStr(LogLevel level, const std::string &function, const std::source_location &location, const std::string &log);

    template <typename... Args>
    std::string formatStr(const std::string &fmt, const Args &...args) {
        return std::vformat(fmt, std::make_format_args(args...));
    }

    template <typename... Args>
    void log(LogLevel level, const std::string &function, const std::source_location &location, const std::string &fmt, const Args &...args) {
        std::string message = std::vformat(fmt, std::make_format_args(args...));
        printLog(level, function, location, message);
    }

    template <typename... Args>
    void assertImpl(bool condition, const std::string &conditionStr, const std::string &function, const std::source_location &location, const Args &...args) {
        if (condition) return;

        if constexpr (sizeof...(args) > 0) {
            std::string message = formatStr(std::forward<const Args &>(args)...);
            vre::log(LogLevel::eFatal, function, location, "Assertion failed for: '{}' with message: '{}'", conditionStr, message);
        } else {
            vre::log(LogLevel::eFatal, function, location, "Assertion failed for: '{}'", conditionStr);
        }

        std::abort();
    }

    class Logger {
       public:
        static void Initialize();
        static void Shutdown();

        static bool IsInitialized();
        static bool IsVerbose(LogLevel level);

        static void SetLogLevel(LogLevel level);
        static void SetTraceEnable(bool trace);
        static void SetTraceFile(const std::string &file);
        static void SetVerbose(LogLevel level, bool verbose);
        static void SetAllVerbose(bool verbose);

        template <typename... Args>
        static void Log(LogLevel level, const std::string &function, const std::source_location &location, const std::string &fmt, Args &&...args) {
            DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
            if (g_Level < level) return;

            std::string message = std::vformat(fmt, std::make_format_args(args...));
            printLog(level, function, location, message);

            if (!g_Trace) return;

            g_TraceContent << makeLogStr(level, function, location, message) << std::endl;
        }

       private:
        static LogLevel          g_Level;
        static bool              g_Trace;
        static std::string       g_TracePath;
        static std::stringstream g_TraceContent;
        static bool              g_IsInitialized;
        static Logger            g_State;

       private:
        Logger() = default;
        ~Logger();
    };
}  // namespace vre