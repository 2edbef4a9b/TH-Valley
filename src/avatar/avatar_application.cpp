#include "AudioEngine.h"
#include "avatar/avatar_application.h"
#include "avatar/avatar_scene.h"
#include "sample/sample_application.h"
#include "sample/sample_scene.h"
#include "map.h"

avatarApplication::~avatarApplication() {
    // Release the shared instance of the audio engine.
    cocos2d::AudioEngine::end();
}

void avatarApplication::initGLContextAttrs() {
    // Set OpenGL context attributes:
    // red, green, blue, alpha, depth, stencil and multisamples count.
    GLContextAttrs gl_context_attrs{8, 8, 8, 8, 24, 8, 0};
    cocos2d::GLView::setGLContextAttrs(gl_context_attrs);
}

bool avatarApplication::applicationDidFinishLaunching() {
    // Initialize director.
    auto *director = cocos2d::Director::getInstance();
    auto *glview = director->getOpenGLView();
    if (!glview) {
        glview = cocos2d::GLViewImpl::create("avatar Application");
        director->setOpenGLView(glview);
    }

    // Set the window size.
    glview->setFrameSize(1980, 1080);

    // Set the design resolution size to improve resolution on high-res screens.
    glview->setDesignResolutionSize(192*4, 108*4, ResolutionPolicy::NO_BORDER);

    // Set FPS. the default value is 1.0/60 if you don't call this.
    director->setAnimationInterval(1.0 / 60);

    // Create a scene. it's an autorelease object.
    auto *scene = SampleScene::create();
    if (scene == nullptr) {
        return false;
    }
    scene->addChild(Map::create("maps/Farm.tmx"));
    //scene->addChild(avatarScene::create());

    // Run.
    director->runWithScene(scene);

    return true;
}

void avatarApplication::applicationDidEnterBackground() {
    // Pause the audio engine when the application enters the background.
    cocos2d::AudioEngine::pauseAll();
}

void avatarApplication::applicationWillEnterForeground() {
    // Resume the audio engine when the application enters the foreground.
    cocos2d::AudioEngine::resumeAll();
}
