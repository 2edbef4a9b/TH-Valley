#include "utility/logger.h"

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <cstddef>
#include <iostream>

namespace th_valley {

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

void Logger::LogDebug(std::string_view message) {
    BOOST_LOG_TRIVIAL(debug) << message;
}

void Logger::LogInfo(std::string_view message) {
    BOOST_LOG_TRIVIAL(info) << message;
}

void Logger::LogWarning(std::string_view message) {
    BOOST_LOG_TRIVIAL(warning) << message;
}

void Logger::LogError(std::string_view message) {
    BOOST_LOG_TRIVIAL(error) << message;
}

void Logger::LogFatal(std::string_view message) {
    BOOST_LOG_TRIVIAL(fatal) << message;
}

Logger::Logger() { InitLogger(); }

void Logger::InitLogger() {
    constexpr auto kRotationSize = static_cast<const size_t>(5 * 1024 * 1024);

    // Add console sink for log output.
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%]: %Message%");

    // Add file sink for log output.
    boost::log::add_file_log(
        boost::log::keywords::file_name =
            "th_valley_%N.log",  // Log file with rotation.
        boost::log::keywords::rotation_size =
            kRotationSize,  // Rotate log file when it reaches 5 MB.
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%]: %Message%");

    // Add common attributes like timestamps.
    boost::log::add_common_attributes();
}

}  // namespace th_valley
