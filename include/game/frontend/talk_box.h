#ifndef TALK_BOX_H_
#define TALK_BOX_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "json/document.h"

#include "game/items/bag.h" // Correct reference - Refactored with Observer Pattern

// Dialogue entry struct - Refactored with Observer Pattern
struct DialogueEntry {
    enum class EntryType { Dialogue, Question };
    EntryType type;
    std::string content;
    std::vector<std::string> options;
    std::unordered_map<std::string, std::vector<std::string>> conditions;
};

class TalkBox : public cocos2d::Layer {
public:
    // ... Keep original constructor/destructor ... - Refactored with Observer Pattern
    static TalkBox* createWithEntries(const std::vector<DialogueEntry>& entries,
                                      const std::string& npcName,
                                      const std::string& npcAvatarPath);
    bool initWithEntries(const std::vector<DialogueEntry>& entries,
                         const std::string& npcName,
                         const std::string& npcAvatarPath);

    static std::vector<DialogueEntry> inputJson(const std::string& jsonPath);

    // Lifecycle - Refactored with Observer Pattern
    void onEnter() override;
    void onExit() override;

    // Display control - Refactored with Observer Pattern
    void showDialog();
    void hideDialog();
    void showNextMessage();

    // Option control - Refactored with Observer Pattern
    void showOptions(const std::vector<std::string>& options);
    void hideOptions();
    void moveSelection(int delta);
    void selectOption();

    // Data injection - Refactored with Observer Pattern
    void setBag(Bag* bag) { playerBag = bag; } // Renamed to standard setter - Refactored with Observer Pattern
    void setNPCName(std::string name) { CitizenName = name; }
    
    std::string getCurrentGameTime() const; // Need to call WorldTime - Refactored with Observer Pattern
    bool conditionsMet(const DialogueEntry& entry) const;

private:
    static constexpr std::string_view kFontPath = "assets/fonts/DFHannotateW5-A.ttf";

    // Listeners - Refactored with Observer Pattern
    cocos2d::EventListenerTouchOneByOne* touchListener_;
    cocos2d::EventListenerKeyboard* keyboardListener_;
    
    // UI components - Refactored with Observer Pattern
    cocos2d::Sprite* background_;
    cocos2d::Label* messageLabel_;
    cocos2d::Sprite* closeButton_;
    cocos2d::Label* npcNameLabel_;
    cocos2d::Sprite* npcAvatar_;
    
    // Data - Refactored with Observer Pattern
    std::string npcName_;
    std::string npcAvatarPath_;
    std::vector<DialogueEntry> dialogEntries_;
    int currentMessageIndex_;

    // Option UI - Refactored with Observer Pattern
    std::vector<cocos2d::Label*> optionLabels_;
    std::vector<cocos2d::LayerColor*> optionBackgrounds_;
    int selectedOptionIndex_;
    bool waitingForChoice_;

    // External references - Refactored with Observer Pattern
    Bag* playerBag = nullptr;
    std::string CitizenName;
    std::unordered_map<std::string, bool> gameStates_;
};

#endif // TALK_BOX_H_