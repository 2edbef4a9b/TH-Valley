#include <gtest/gtest.h>

#include "tool_bar.h"
#include "sample/sample_application.h"

class ToolBarTestsApp : public th_valley::SampleApplication {
public:
    bool applicationDidFinishLaunching() override {
        // Initialize the director.
        auto *director = cocos2d::Director::getInstance();
        auto *glview = director->getOpenGLView();
        if (!glview) {
            glview = cocos2d::GLViewImpl::create("Tool Bar Tests");
            director->setOpenGLView(glview);
        }

        // Set the window size.
        //glview->setFrameSize(kWindowSize.width, kWindowSize.height);
        glview->setFrameSize(1920, 1080);
        // Improve resolution on high-res screens.
        //glview->setDesignResolutionSize(kDesignResolutionSize.width,
        //                                kDesignResolutionSize.height,
        //                                ResolutionPolicy::NO_BORDER);
        glview->setDesignResolutionSize(192 * 3,
                                        108 * 3,
                                        ResolutionPolicy::NO_BORDER);

        // Set FPS. The default value is 1.0/60 if you don't call this.
        director->setAnimationInterval(kAnimationInterval);

        // Remove the last directory name from the resource root path.
        UpdateResourcePath();
        const std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
        CCLOG("Resource root path: %s", resource_root_path.c_str());

        // Create a scene. It's an autorelease object.
        auto *scene = ToolBar::create();
        if (scene == nullptr) {
            return false;
        }

        // Run.
        director->runWithScene(scene);

        return true;
    }
};

class ToolBarTests : public ::testing::Test {
protected:
    ToolBarTestsApp app_;
};

TEST_F(ToolBarTests, Run) {
    // Test running the application.
    EXPECT_EQ(ToolBarTestsApp::getInstance()->run(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
