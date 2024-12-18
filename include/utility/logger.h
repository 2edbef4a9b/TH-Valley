#ifndef LOGGER_H_
#define LOGGER_H_

#include <format>
#include <string_view>

namespace th_valley {

class Logger {
public:
    static Logger& GetInstance();

    void LogDebug(std::string_view message);
    void LogInfo(std::string_view message);
    void LogWarning(std::string_view message);
    void LogError(std::string_view message);
    void LogFatal(std::string_view message);

    template <typename... Args>
    void LogDebug(std::string_view format, Args&&... args);

    template <typename... Args>
    void LogInfo(std::string_view format, Args&&... args);

    template <typename... Args>
    void LogWarning(std::string_view format, Args&&... args);

    template <typename... Args>
    void LogError(std::string_view format, Args&&... args);

    template <typename... Args>
    void LogFatal(std::string_view format, Args&&... args);

    // Delete member functions should be public.
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(Logger&& other) = delete;

private:
    Logger();
    ~Logger() = default;

    void InitLogger();
};

template <typename... Args>
void Logger::LogDebug(std::string_view format, Args&&... args) {
    LogDebug(std::vformat(
        format, std::make_format_args(std::forward<const Args>(args)...)));
}

template <typename... Args>
void Logger::LogInfo(std::string_view format, Args&&... args) {
    LogInfo(std::vformat(
        format, std::make_format_args(std::forward<const Args>(args)...)));
}

template <typename... Args>
void Logger::LogWarning(std::string_view format, Args&&... args) {
    LogWarning(std::vformat(
        format, std::make_format_args(std::forward<const Args>(args)...)));
}

template <typename... Args>
void Logger::LogError(std::string_view format, Args&&... args) {
    LogError(std::vformat(
        format, std::make_format_args(std::forward<const Args>(args)...)));
}

template <typename... Args>
void Logger::LogFatal(std::string_view format, Args&&... args) {
    LogFatal(std::vformat(
        format, std::make_format_args(std::forward<const Args>(args)...)));
}

}  // namespace th_valley

#endif  // LOGGER_H_
