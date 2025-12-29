#ifndef LOGGER_H_
#define LOGGER_H_

#include <format>
#include <string_view>
#include <string>

namespace th_valley {

class Logger {
public:
    static Logger& GetInstance();

    // Basic print interface - Refactored with Singleton Pattern
    void LogDebug(std::string_view message);
    void LogInfo(std::string_view message);
    void LogWarning(std::string_view message);
    void LogError(std::string_view message);
    void LogFatal(std::string_view message);

    // Template interface with formatted parameters (C++20 std::format) - Refactored with Singleton Pattern
    template <typename... Args>
    void LogDebug(std::string_view format_str, Args&&... args);

    template <typename... Args>
    void LogInfo(std::string_view format_str, Args&&... args);

    template <typename... Args>
    void LogWarning(std::string_view format_str, Args&&... args);

    template <typename... Args>
    void LogError(std::string_view format_str, Args&&... args);

    template <typename... Args>
    void LogFatal(std::string_view format_str, Args&&... args);

    // Disable copy/move - Refactored with Singleton Pattern
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    Logger();
    ~Logger() = default;

    void InitLogger();
};

template <typename... Args>
void Logger::LogDebug(std::string_view format_str, Args&&... args) {
    LogDebug(std::vformat(format_str, std::make_format_args(args...)));
}

template <typename... Args>
void Logger::LogInfo(std::string_view format_str, Args&&... args) {
    LogInfo(std::vformat(format_str, std::make_format_args(args...)));
}

template <typename... Args>
void Logger::LogWarning(std::string_view format_str, Args&&... args) {
    LogWarning(std::vformat(format_str, std::make_format_args(args...)));
}

template <typename... Args>
void Logger::LogError(std::string_view format_str, Args&&... args) {
    LogError(std::vformat(format_str, std::make_format_args(args...)));
}

template <typename... Args>
void Logger::LogFatal(std::string_view format_str, Args&&... args) {
    LogFatal(std::vformat(format_str, std::make_format_args(args...)));
}

}  // namespace th_valley

#define LOG_DEBUG(...) th_valley::Logger::GetInstance().LogDebug(__VA_ARGS__)
#define LOG_INFO(...)  th_valley::Logger::GetInstance().LogInfo(__VA_ARGS__)
#define LOG_WARN(...)  th_valley::Logger::GetInstance().LogWarning(__VA_ARGS__)
#define LOG_ERROR(...) th_valley::Logger::GetInstance().LogError(__VA_ARGS__)
#define LOG_FATAL(...) th_valley::Logger::GetInstance().LogFatal(__VA_ARGS__)

#endif  // LOGGER_H_