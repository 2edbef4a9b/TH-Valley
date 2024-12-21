#include <gtest/gtest.h>

#include "sample/sample_application.h"
#include "sample/sample_scene.h"
#include "tool_bar.h"
#include "bag_gui.h"

class BagGUITestsApp : public th_valley::SampleApplication {
public:
    bool applicationDidFinishLaunching() override {
        // Initialize the director.
        auto *director = cocos2d::Director::getInstance();
        auto *glview = director->getOpenGLView();
        if (!glview) {
            glview = cocos2d::GLViewImpl::create("Bag GUI Tests");
            director->setOpenGLView(glview);
        }

        // Set the window size.
        // glview->setFrameSize(kWindowSize.width, kWindowSize.height);
        glview->setFrameSize(1920, 1080);
        // Improve resolution on high-res screens.
        // glview->setDesignResolutionSize(kDesignResolutionSize.width,
        //                                kDesignResolutionSize.height,
        //                                ResolutionPolicy::NO_BORDER);
        glview->setDesignResolutionSize(192 * 2, 108 * 2,
                                        ResolutionPolicy::NO_BORDER);

        // Set FPS. The default value is 1.0/60 if you don't call this.
        director->setAnimationInterval(kAnimationInterval);

        // Remove the last directory name from the resource root path.
        UpdateResourcePath();
        const std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
        CCLOG("Resource root path: %s", resource_root_path.c_str());

        // Create a scene. It's an autorelease object.
        auto *scene = th_valley::SampleScene::create();
        if (scene == nullptr) {
            return false;
        }

        scene->addChild(ToolBar::create(), 20);
        scene->addChild(BagGUI::create(), 21);
        // Run.
        director->runWithScene(scene);

        return true;
    }
};

class BagGUITests : public ::testing::Test {
protected:
    BagGUITestsApp app_;
};

TEST_F(BagGUITests, Run) {
    // Test running the application.
    EXPECT_EQ(BagGUITestsApp::getInstance()->run(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
