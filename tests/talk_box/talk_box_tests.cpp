#include <gtest/gtest.h>

#include "sample/sample_application.h"
#include "sample/sample_scene.h"
#include "talk_box.h"
#include "tool_bar.h"

std::vector<DialogueEntry> inputJson(std::string fileName) {
    // 加载 JSON 文件
    std::vector<DialogueEntry> dialogEntries;

    std::string fullPath =
        cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string jsonData =
        cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);

    CCLOG("JSON file path: %s", fullPath.c_str());
    CCLOG("JSON data: %s", jsonData.c_str());

    rapidjson::Document doc;
    doc.Parse(jsonData.c_str());
    if (doc.IsArray()) {
        for (auto& v : doc.GetArray()) {
            DialogueEntry entry;

            // 解析 type
            if (v.HasMember("type") && v["type"].IsString()) {
                std::string typeStr = v["type"].GetString();
                if (typeStr == "dialogue") {
                    entry.type = DialogueEntry::EntryType::Dialogue;
                } else if (typeStr == "question") {
                    entry.type = DialogueEntry::EntryType::Question;
                }
            }

            // 解析 content
            if (v.HasMember("content") && v["content"].IsString()) {
                entry.content = v["content"].GetString();
            }

            // 解析 options
            if (entry.type == DialogueEntry::EntryType::Question &&
                v.HasMember("options") && v["options"].IsArray()) {
                for (auto& opt : v["options"].GetArray()) {
                    if (opt.IsString()) {
                        entry.options.push_back(opt.GetString());
                    }
                }
            }

            // 解析 conditions
            if (v.HasMember("conditions") && v["conditions"].IsObject()) {
                const auto& conditions = v["conditions"];
                // 时间条件
                if (conditions.HasMember("time") &&
                    conditions["time"].IsArray()) {
                    std::vector<std::string> timeRange;
                    for (auto& timeValue : conditions["time"].GetArray()) {
                        if (timeValue.IsString()) {
                            timeRange.push_back(timeValue.GetString());
                        }
                    }
                    entry.conditions["time"] = timeRange;
                }
                // 事件条件
                if (conditions.HasMember("event") &&
                    conditions["event"].IsArray()) {
                    std::vector<std::string> events;
                    for (auto& eventValue : conditions["event"].GetArray()) {
                        if (eventValue.IsString()) {
                            events.push_back(eventValue.GetString());
                        }
                    }
                    entry.conditions["event"] = events;
                }
            }

            dialogEntries.push_back(entry);
        }
    }
    // 输出调试信息
    CCLOG("Parsed %zu dialogue entries", dialogEntries.size());
    for (const auto& entry : dialogEntries) {
        CCLOG("Type: %s, Content: %s",
              entry.type == DialogueEntry::EntryType::Dialogue ? "Dialogue"
                                                               : "Question",
              entry.content.c_str());
        if (entry.type == DialogueEntry::EntryType::Question) {
            for (const auto& option : entry.options) {
                CCLOG("Option: %s", option.c_str());
            }
        }
        for (const auto& condition : entry.conditions) {
            CCLOG("Condition: %s", condition.first.c_str());
            for (const auto& value : condition.second) {
                CCLOG("Value: %s", value.c_str());
            }
        }
    }
    return dialogEntries;
}

class TalkBoxTestsApp : public th_valley::SampleApplication {
public:
    bool applicationDidFinishLaunching() override {
        // Initialize the director.
        auto *director = cocos2d::Director::getInstance();
        auto *glview = director->getOpenGLView();
        if (!glview) {
            glview = cocos2d::GLViewImpl::create("Talk Box Tests");
            director->setOpenGLView(glview);
        }

        // Set the window size.
        // glview->setFrameSize(kWindowSize.width, kWindowSize.height);
        glview->setFrameSize(1920, 1080);
        // Improve resolution on high-res screens.
        // glview->setDesignResolutionSize(kDesignResolutionSize.width,
        //                                kDesignResolutionSize.height,
        //                                ResolutionPolicy::NO_BORDER);
        glview->setDesignResolutionSize(192 * 4, 108 * 4,
                                        ResolutionPolicy::NO_BORDER);

        // Set FPS. The default value is 1.0/60 if you don't call this.
        director->setAnimationInterval(kAnimationInterval);

        // Remove the last directory name from the resource root path.
        UpdateResourcePath();
        const std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
        CCLOG("Resource root path: %s", resource_root_path.c_str());

        // Create a scene. It's an autorelease object.
        //auto *scene = th_valley::SampleScene::create();
        auto *scene = th_valley::SampleScene::create();
        if (scene == nullptr) {
            return false;
        } 


        //scene->addChild(
        //    TalkBox::createWithEntries(inputJson("assets/dialogue/npc.json"),
        //                               "npc", "assets/Portraits/Abigail.png"),
        //    100);
        scene->addChild(
            TalkBox::createWithEntries(inputJson("assets/dialogue/Sebastian.json"),
                                       "Sebastian", "assets/Portraits/Sebastian.png"),
            100);

        scene->addChild(ToolBar::create(), 200);


        // Run.
        director->runWithScene(scene);

        return true;
    }
};

class TalkBoxTests : public ::testing::Test {
protected:
    TalkBoxTestsApp app_;
};

TEST_F(TalkBoxTests, Run) {
    // Test running the application.
    EXPECT_EQ(TalkBoxTestsApp::getInstance()->run(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
