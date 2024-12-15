#include <gtest/gtest.h>

#include "frontend/title_screen.h"
#include "sample/sample_application.h"

class TitleScreenTestsApp : public th_valley::SampleApplication {
public:
    bool applicationDidFinishLaunching() override {
        // Initialize the director.
        auto *director = cocos2d::Director::getInstance();
        auto *glview = director->getOpenGLView();
        if (!glview) {
            glview = cocos2d::GLViewImpl::create("Title Screen Tests");
            director->setOpenGLView(glview);
        }

        // Set the window size.
        glview->setFrameSize(kWindowSize.width, kWindowSize.height);

        // Improve resolution on high-res screens.
        glview->setDesignResolutionSize(kDesignResolutionSize.width, kDesignResolutionSize.height,
                                        ResolutionPolicy::NO_BORDER);

        // Set FPS. The default value is 1.0/60 if you don't call this.
        director->setAnimationInterval(kAnimationInterval);

        // Remove the last directory name from the resource root path.
        UpdateResourcePath();
        const std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
        CCLOG("Resource root path: %s", resource_root_path.c_str());

        // Create a scene. It's an autorelease object.
        auto *scene = th_valley::TitleScreen::create();
        if (scene == nullptr) {
            return false;
        }

        // Run.
        director->runWithScene(scene);

        return true;
    }
};

class TitleScreenTests : public ::testing::Test {
protected:
    TitleScreenTestsApp app_;
};

TEST_F(TitleScreenTests, Run) {
    // Test running the application.
    EXPECT_EQ(TitleScreenTestsApp::getInstance()->run(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
