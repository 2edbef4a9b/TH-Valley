#ifndef LOGGER_H_
#define LOGGER_H_

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

}  // namespace th_valley

#endif  // LOGGER_H_
