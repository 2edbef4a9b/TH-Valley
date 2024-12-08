#include <gtest/gtest.h>

#include <string>

#include "sample/sample_application.h"

class SampleApplicationTests : public testing::Test {
protected:
    SampleApplication app_;
};

TEST_F(SampleApplicationTests, Init) {
    // Test initializing the application.
    EXPECT_TRUE(app_.applicationDidFinishLaunching());

    // Test the modification of the resource root path.
    const std::string& resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
    EXPECT_EQ(resource_root_path.substr(resource_root_path.size() - 7),
              "assets/");
}

TEST_F(SampleApplicationTests, Run) {
    // Test running the application.
    EXPECT_EQ(SampleApplication::getInstance()->run(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
