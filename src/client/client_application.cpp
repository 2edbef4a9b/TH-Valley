#include "client/client_application.h"

#include "AudioEngine.h"
#include "client/client_controller.h"
#include "utility/logger.h"

namespace th_valley {

ClientApplication::~ClientApplication() {
    // Release the shared instance of the audio engine.
    cocos2d::AudioEngine::end();
}

bool ClientApplication::applicationDidFinishLaunching() {
    // Initialize director.
    auto *director = cocos2d::Director::getInstance();
    auto *glview = director->getOpenGLView();
    if (!glview) {
        glview = cocos2d::GLViewImpl::create("Sample Application");
        director->setOpenGLView(glview);
    }

    // Set the window size.
    glview->setFrameSize(kWindowSize.width, kWindowSize.height);

    // Set the design resolution size to improve resolution on high-res screens.
    glview->setDesignResolutionSize(kDesignResolutionSize.width,
                                    kDesignResolutionSize.height,
                                    ResolutionPolicy::NO_BORDER);

    // Set FPS. the default value is 1.0/60 if you don't call this.
    director->setAnimationInterval(kInterval);

    // Remove the last directory name from the resource root path.
    UpdateResourcePath();
    const std::string resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
    Logger::GetInstance().LogInfo("Resource root path: {}", resource_root_path);

    // Create a blank scene to start the application.
    auto *blank_scene = cocos2d::Scene::create();
    director->runWithScene(blank_scene);

    // TODO(2edbef4a9b): Initialize the audio engine (optional).
    // Set the client state to start up.
    ClientController::GetInstance().SetClientState(
        ClientController::ClientState::kStartUp);

    Logger::GetInstance().LogInfo("Application started successfully.");
    return true;
}

void ClientApplication::applicationDidEnterBackground() {
    cocos2d::Director::getInstance()->stopAnimation();
    // Pause the audio engine when the application enters the background.
    cocos2d::AudioEngine::pauseAll();
}

void ClientApplication::applicationWillEnterForeground() {
    cocos2d::Director::getInstance()->startAnimation();
    // Resume the audio engine when the application enters the foreground.
    cocos2d::AudioEngine::resumeAll();
}

void ClientApplication::initGLContextAttrs() {
    // Set OpenGL context attributes:
    // red, green, blue, alpha, depth, stencil and multisamples count.
    GLContextAttrs gl_context_attrs{8, 8, 8, 8, 24, 8, 0};
    cocos2d::GLView::setGLContextAttrs(gl_context_attrs);
}

void ClientApplication::UpdateResourcePath() {
    static bool is_called = false;
    // Ensure that this method is called only once.
    if (is_called) {
        return;
    }
    is_called = true;

    // Set the resource path.
    auto *file_utils = cocos2d::FileUtils::getInstance();

    // Get the current resource root absolute path.
    std::string resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

    // Remove the last directory name from the resource root path.
    const size_t pos =
        resource_root_path.find_last_of('/', resource_root_path.size() - 2);
    if (pos != std::string::npos) {
        resource_root_path = resource_root_path.substr(0, pos + 1);
    } else {
        resource_root_path = "/";
    }

    // Update the default resource root path.
    file_utils->setDefaultResourceRootPath(resource_root_path);
}

}  // namespace th_valley
