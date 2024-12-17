#include <gtest/gtest.h>

#include "sample/sample_application.h"

class SampleApplicationTests : public testing::Test {
protected:
    th_valley::SampleApplication app_;
};

TEST_F(SampleApplicationTests, Init) {
    // Test initializing the application.
    EXPECT_TRUE(app_.applicationDidFinishLaunching());
}

TEST_F(SampleApplicationTests, Run) {
    // Test running the application.
    EXPECT_EQ(th_valley::SampleApplication::getInstance()->run(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
