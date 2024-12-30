#include <VREngine/Core/Logger.hpp>

#ifdef VRE_PLATFORM_WINDOWS
#pragma warning(disable : 5082)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
const int32_t VRE_TEXT_COLOR_FATAL_RED = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED;
const int32_t VRE_TEXT_COLOR_RED       = FOREGROUND_RED | FOREGROUND_INTENSITY;
const int32_t VRE_TEXT_COLOR_YELLOW    = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const int32_t VRE_TEXT_COLOR_BLUE      = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const int32_t VRE_TEXT_COLOR_GREEN     = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const int32_t VRE_TEXT_COLOR_RESET     = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
#elif VRE_PLATFORM_UNIX
#pragma GCC diagnostic ignored "-Wno-5082"
const std::string VRE_TEXT_COLOR_FATAL_RED = "\033[97;41m";
const std::string VRE_TEXT_COLOR_RED       = "\033[91m";
const std::string VRE_TEXT_COLOR_YELLOW    = "\033[93m";
const std::string VRE_TEXT_COLOR_BLUE      = "\033[94m";
const std::string VRE_TEXT_COLOR_GREEN     = "\033[92m";
const std::string VRE_TEXT_COLOR_RESET     = "\033[0m";
#else
#error Not supported platform
#endif

namespace vre {
    LogLevel          Logger::g_Level{};
    bool              Logger::g_Trace{};
    std::string       Logger::g_TracePath{};
    std::stringstream Logger::g_TraceContent{};
    bool              Logger::g_IsInitialized{false};
    Logger            Logger::g_State{};

    bool g_LogLevelVerbosity[5]{true, true, true, false, false};

    const char *LogLevelStrs[]{
        "FATAL",
        "ERROR",
        "WARN",
        "DEBUG",
        "INFO",
    };

    void printLog(LogLevel level, const std::string &function, const std::source_location &location, const std::string &message) {
#if defined(VRE_PLATFORM_WINDOWS)
        std::int32_t color = 0;
#elif defined(VRE_PLATFORM_UNIX)
        const char *color = nullptr;
#endif
        switch (level) {
            case LogLevel::eFatal:
                color = VRE_TEXT_COLOR_FATAL_RED;
                break;
            case LogLevel::eError:
                color = VRE_TEXT_COLOR_RED;
                break;
            case LogLevel::eWarn:
                color = VRE_TEXT_COLOR_YELLOW;
                break;
            case LogLevel::eDebug:
                color = VRE_TEXT_COLOR_GREEN;
                break;
            case LogLevel::eInfo:
                color = VRE_TEXT_COLOR_BLUE;
                break;
            default:
                throw std::runtime_error("Unknown log level!");
        }

        std::string log = makeLogStr(level, function, location, message);

#if defined(VRE_PLATFORM_WINDOWS)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
#elif defined(VRE_PLATFORM_UNIX)
        std::clog << color;
#endif
        std::clog << log;
#if defined(VRE_PLATFORM_WINDOWS)
        SetConsoleTextAttribute(hConsole, VRE_TEXT_COLOR_RESET);
        std::clog << '\n';
#elif defined(VRE_PLATFORM_UNIX)
        std::clog << VRE_TEXT_COLOR_RESET << '\n';
#endif
    }

    std::string makeLogStr(LogLevel level, const std::string &function, const std::source_location &location, const std::string &message) {
        using namespace std::chrono;

        time_point<system_clock> now  = system_clock::now();
        std::time_t              time = system_clock::to_time_t(now);

        std::tm tm{};
        localtime_s(&tm, &time);

        tm.tm_year += 1900;
        tm.tm_mon += 1;

        if (Logger::IsVerbose(level)) {
            const std::uint32_t line   = location.line();
            const std::uint32_t column = location.column();
            std::string         file   = location.file_name();
            // std::string function_name = location.function_name();

            return std::vformat(
                "[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}]::[{}]::[{}({}:{})]::[{}]: {}",
                std::make_format_args(tm.tm_year,
                                      tm.tm_mon,
                                      tm.tm_mday,
                                      tm.tm_hour,
                                      tm.tm_min,
                                      tm.tm_sec,
                                      LogLevelStrs[std::size_t(level)],
                                      file,
                                      line,
                                      column,
                                      function,
                                      message));
        }

        return std::vformat(
            "[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}]::[{}]: {}",
            std::make_format_args(tm.tm_year,
                                  tm.tm_mon,
                                  tm.tm_mday,
                                  tm.tm_hour,
                                  tm.tm_min,
                                  tm.tm_sec,
                                  LogLevelStrs[std::size_t(level)],
                                  message));
    }

    void Logger::Initialize() {
        DVRE_ASSERT(!g_IsInitialized, "vre::Logger must be shut down before initializing");
        DLOG_INFO("Initializing vre::Logger");

        g_Level     = LogLevel::eWarn;
        g_Trace     = false;
        g_TracePath = "VulkanRenderEngine.txt";
        g_TraceContent.clear();
        g_IsInitialized = true;
    }

    void Logger::Shutdown() {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized before shutting down");
        DLOG_INFO("Shutting vre::Logger down");

        if (g_Trace && !g_TracePath.empty()) {
            std::ofstream file{g_TracePath};
            file << g_TraceContent.str();
            g_TraceContent.clear();
        }

        g_IsInitialized = false;
    }

    bool Logger::IsInitialized() {
        return g_IsInitialized;
    }

    bool Logger::IsVerbose(LogLevel level) {
        return g_LogLevelVerbosity[std::size_t(level)];
    }

    void Logger::SetLogLevel(LogLevel level) {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
        g_Level = level;
    }

    void Logger::SetTraceEnable(bool trace) {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
        g_Trace = trace;
    }

    void Logger::SetTraceFile(const std::string &file) {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
        g_TracePath = file;
    }

    void Logger::SetVerbose(LogLevel level, bool verbose) {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
        g_LogLevelVerbosity[std::size_t(level)] = verbose;
    }

    void Logger::SetAllVerbose(bool verbose) {
        DVRE_ASSERT(g_IsInitialized, "vre::Logger must be initialized");
        g_LogLevelVerbosity[std::size_t(LogLevel::eFatal)] = verbose;
        g_LogLevelVerbosity[std::size_t(LogLevel::eError)] = verbose;
        g_LogLevelVerbosity[std::size_t(LogLevel::eWarn)]  = verbose;
        g_LogLevelVerbosity[std::size_t(LogLevel::eDebug)] = verbose;
        g_LogLevelVerbosity[std::size_t(LogLevel::eInfo)]  = verbose;
    }

    Logger::~Logger() {
        VRE_ASSERT(!g_IsInitialized, "vre::Logger must be shut down before closing!");
    }
}  // namespace vre