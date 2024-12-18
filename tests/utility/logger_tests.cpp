#include <gtest/gtest.h>

#include "utility/logger.h"

TEST(LoggerTests, LogMessages) {
    // Test logging with different types.
    th_valley::Logger::GetInstance().LogDebug("Debug message");
    th_valley::Logger::GetInstance().LogInfo("Info message");
    th_valley::Logger::GetInstance().LogWarning("Warning message");
    th_valley::Logger::GetInstance().LogError("Error message");
    th_valley::Logger::GetInstance().LogFatal("Fatal message");
}

TEST(LoggerTests, LogFormatted) {
    // Test formatted logging with different types.
    th_valley::Logger::GetInstance().LogDebug("Debug message: {}", 42);
    th_valley::Logger::GetInstance().LogInfo("Info message: {}", 3.14);
    th_valley::Logger::GetInstance().LogWarning("Warning message: {}", "test");
    th_valley::Logger::GetInstance().LogError("Error message: {}", true);
    th_valley::Logger::GetInstance().LogFatal("Fatal message: {}", 'A');
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
