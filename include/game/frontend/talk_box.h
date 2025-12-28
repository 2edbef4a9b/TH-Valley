#ifndef TALK_BOX_H_
#define TALK_BOX_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "json/document.h"

#include "game/items/bag.h" // 修正引用

// 对话条目结构体
struct DialogueEntry {
    enum class EntryType { Dialogue, Question };
    EntryType type;
    std::string content;
    std::vector<std::string> options;
    std::unordered_map<std::string, std::vector<std::string>> conditions;
};

class TalkBox : public cocos2d::Layer {
public:
    // ... 保持原有构造/析构 ...
    static TalkBox* createWithEntries(const std::vector<DialogueEntry>& entries,
                                      const std::string& npcName,
                                      const std::string& npcAvatarPath);
    bool initWithEntries(const std::vector<DialogueEntry>& entries,
                         const std::string& npcName,
                         const std::string& npcAvatarPath);

    static std::vector<DialogueEntry> inputJson(const std::string& jsonPath);

    // 生命周期
    void onEnter() override;
    void onExit() override;

    // 显示控制
    void showDialog();
    void hideDialog();
    void showNextMessage();

    // 选项控制
    void showOptions(const std::vector<std::string>& options);
    void hideOptions();
    void moveSelection(int delta);
    void selectOption();

    // 数据注入
    void setBag(Bag* bag) { playerBag = bag; } // 重命名为标准 setter
    void setNPCName(std::string name) { CitizenName = name; }
    
    std::string getCurrentGameTime() const; // 需调用 WorldTime
    bool conditionsMet(const DialogueEntry& entry) const;

private:
    static constexpr std::string_view kFontPath = "assets/fonts/DFHannotateW5-A.ttf";

    // 监听器
    cocos2d::EventListenerTouchOneByOne* touchListener_;
    cocos2d::EventListenerKeyboard* keyboardListener_;
    
    // UI 组件
    cocos2d::Sprite* background_;
    cocos2d::Label* messageLabel_;
    cocos2d::Sprite* closeButton_;
    cocos2d::Label* npcNameLabel_;
    cocos2d::Sprite* npcAvatar_;
    
    // 数据
    std::string npcName_;
    std::string npcAvatarPath_;
    std::vector<DialogueEntry> dialogEntries_;
    int currentMessageIndex_;

    // 选项 UI
    std::vector<cocos2d::Label*> optionLabels_;
    std::vector<cocos2d::LayerColor*> optionBackgrounds_;
    int selectedOptionIndex_;
    bool waitingForChoice_;

    // 外部引用
    Bag* playerBag = nullptr;
    std::string CitizenName;
    std::unordered_map<std::string, bool> gameStates_;
};

#endif // TALK_BOX_H_