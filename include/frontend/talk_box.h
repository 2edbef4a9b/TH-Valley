#ifndef TALK_BOX_H_
#define TALK_BOX_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "game/bag.h"

#include "cocos2d.h"
#include "json/document.h"

struct DialogueEntry {
    enum class EntryType { Dialogue, Question };
    EntryType type;
    std::string content;
    std::vector<std::string> options;
    std::unordered_map<std::string, std::vector<std::string>> conditions;
};

class TalkBox : public cocos2d::Layer {
public:
    TalkBox() = default;
    ~TalkBox() override = default;
    TalkBox(const TalkBox& other) = delete;
    TalkBox& operator=(const TalkBox& other) = delete;
    TalkBox(TalkBox&& other) = delete;
    TalkBox& operator=(TalkBox&& other) = delete;

    bool initWithEntries(const std::vector<DialogueEntry>& entries,
                         const std::string& npcName,
                         const std::string& npcAvatarPath);
    static TalkBox* createWithEntries(const std::vector<DialogueEntry>& entries,
                                      const std::string& npcName,
                                      const std::string& npcAvatarPath);

    static std::vector<DialogueEntry> inputJson(const std::string& jsonPath);

    void onEnter();
    void onExit();

    void showDialog();
    void hideDialog();

    void showNextMessage();
    void showOptions(const std::vector<std::string>& options);
    void hideOptions();
    void moveSelection(int delta);
    void selectOption();
    void getBag(Bag* bag_) { playerBag = bag_; }
    void getName(std::string name) { CitizenName = name; }
    // 获取当前游戏时间，格式为 "HHMM"
    std::string getCurrentGameTime() const;

    // 检查条件是否满足
    bool conditionsMet(const DialogueEntry& entry) const;

private:
    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    // 添加事件监听器
    cocos2d::EventListenerTouchOneByOne* touchListener_;
    cocos2d::EventListenerKeyboard* keyboardListener_;
    cocos2d::EventListenerMouse* mouseListener_;
    cocos2d::EventListenerTouchOneByOne* closeButtonListener_;

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    cocos2d::Sprite* background_;
    cocos2d::Label* messageLabel_;

    cocos2d::Sprite* closeButton_;
    cocos2d::LayerColor* closeButtonBackground_;

    cocos2d::Label* npcNameLabel_;
    cocos2d::Sprite* npcAvatar_;
    std::string npcName_;
    std::string npcAvatarPath_;

    std::vector<DialogueEntry> dialogEntries_;
    int currentMessageIndex_;

    std::vector<cocos2d::Label*> optionLabels_;
    std::vector<cocos2d::LayerColor*> optionBackgrounds_;
    int selectedOptionIndex_;
    bool waitingForChoice_;
    Bag* playerBag;
    std::string CitizenName;

    std::unordered_map<std::string, bool> gameStates_;
};

#endif  // TALK_BOX_H_
