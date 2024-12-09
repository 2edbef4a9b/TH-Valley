#include <gtest/gtest.h>

#include "frontend/title_screen.h"
#include "sample/sample_application.h"

class TitleScreenTestsApp : public SampleApplication {
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
        glview->setFrameSize(1920, 1080);

        // Improve resolution on high-res screens.
        glview->setDesignResolutionSize(1920, 1080,
                                        ResolutionPolicy::NO_BORDER);

        // Set FPS. The default value is 1.0/60 if you don't call this.
        director->setAnimationInterval(1.0 / 60);

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
public:
    static void SetResourcePath(const std::string &path) {
        auto *file_utils = cocos2d::FileUtils::getInstance();

        // Get the current resource root absolute path.
        std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

        // Replace the last directory name with the new path.
        const size_t pos =
            resource_root_path.find_last_of('/', resource_root_path.size() - 2);
        if (pos != std::string::npos) {
            resource_root_path.replace(pos + 1, std::string::npos, path + "/");
        } else {
            resource_root_path = path + "/";
        }

        // Update the default resource root path.
        file_utils->setDefaultResourceRootPath(resource_root_path);
    }

    void SetUp() override {
        // Change the default resource root path from "Resources/" to "assets/".
        SetResourcePath("assets");
    }

protected:
    TitleScreenTestsApp app_;
};

TEST_F(TitleScreenTests, TitleScreen) {
    // The test is successful if the title screen is displayed.
    EXPECT_TRUE(app_.applicationDidFinishLaunching());
}

TEST_F(TitleScreenTests, Run) {
    // Test running the application.
    EXPECT_EQ(TitleScreenTestsApp::getInstance()->run(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
