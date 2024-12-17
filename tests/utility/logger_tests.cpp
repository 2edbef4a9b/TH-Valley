#include <gtest/gtest.h>

#include "utility/logger.h"

TEST(LoggerTests, LogType) {
    // Test logging with different types.
    th_valley::Logger::GetInstance().LogDebug("Debug message");
    th_valley::Logger::GetInstance().LogInfo("Info message");
    th_valley::Logger::GetInstance().LogWarning("Warning message");
    th_valley::Logger::GetInstance().LogError("Error message");
    th_valley::Logger::GetInstance().LogFatal("Fatal message");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
