#include "sample/sample_application.h"

#include "AudioEngine.h"
#include "sample/sample_scene.h"

void th_valley::SampleApplication::initGLContextAttrs() {
    // Set OpenGL context attributes:
    // red, green, blue, alpha, depth, stencil and multisamples count.
    GLContextAttrs gl_context_attrs{8, 8, 8, 8, 24, 8, 0};
    ax::GLView::setGLContextAttrs(gl_context_attrs);
}

bool th_valley::SampleApplication::applicationDidFinishLaunching() {
    // Initialize director.
    auto *director = ax::Director::getInstance();
    auto *glview = director->getGLView();
    if (!glview) {
        glview = ax::GLViewImpl::create("Sample Application");
        director->setGLView(glview);
    }

    // Set the window size.
    glview->setFrameSize(1920, 1080);

    // Set the design resolution size to improve resolution on high-res screens.
    glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);

    // Set FPS. the default value is 1.0/60 if you don't call this.
    director->setAnimationInterval(1.0 / 60);

    // Create a scene. it's an autorelease object.
    auto *scene = ax::utils::createInstance<SampleScene>();
    if (scene == nullptr) {
        return false;
    }

    // Run.
    director->runWithScene(scene);

    return true;
}

void th_valley::SampleApplication::applicationDidEnterBackground() {
    ax::Director::getInstance()->stopAnimation();
    // Pause the audio engine when the application enters the background.
    ax::AudioEngine::pauseAll();
}

void th_valley::SampleApplication::applicationWillEnterForeground() {
    ax::Director::getInstance()->startAnimation();
    // Resume the audio engine when the application enters the foreground.
    ax::AudioEngine::resumeAll();
}
