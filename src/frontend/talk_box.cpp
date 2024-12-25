#include "frontend/talk_box.h"

USING_NS_CC;

TalkBox* TalkBox::createWithEntries(const std::vector<DialogueEntry>& entries,
                                    const std::string& npcName,
                                    const std::string& npcAvatarPath) {
    TalkBox* ret = new (std::nothrow) TalkBox();
    if (ret && ret->initWithEntries(entries, npcName, npcAvatarPath)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TalkBox::initWithEntries(const std::vector<DialogueEntry>& entries,
                              const std::string& npcName,
                              const std::string& npcAvatarPath) {
    if (!Layer::init()) {
        return false;
    }

    visible_size_ = Director::getInstance()->getVisibleSize();
    visible_origin_ = Director::getInstance()->getVisibleOrigin();

    dialogEntries_ = entries;
    currentMessageIndex_ = 0;
    npcName_ = npcName;
    npcAvatarPath_ = npcAvatarPath;

    // ���ضԻ��򱳾�
    background_ = Sprite::create("assets/talk_box/textBox.png");
    background_->setAnchorPoint(Vec2(0.5f, 0));
    background_->setScale(1.1f);
    background_->setPosition(visible_origin_.x + visible_size_.width / 2,
                             visible_origin_.y);
    this->addChild(background_, -5);

    // �����ı���ǩ
    messageLabel_ = Label::createWithTTF("", std::string(kFontPath), 12);
    messageLabel_->setTextColor(Color4B::WHITE);
    messageLabel_->setAnchorPoint(Vec2(0, 1));
    messageLabel_->setPosition(
        background_->getPosition() +
        1.1f * Vec2(-background_->getContentSize().width / 2 + 10,
                    background_->getContentSize().height -
                        10));  // ����λ��Ϊ�Ի����ڲ�����벿��
    messageLabel_->setWidth((2 * background_->getContentSize().width) /
                            3);  // ���ÿ��Ϊ�Ի����ȵ�һ��
    messageLabel_->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    this->addChild(messageLabel_, 1);

    // ��ӹرհ�ť
    closeButton_ = Sprite::create("assets/talk_box/close_button.png");
    closeButton_->setScale(0.01f);
    closeButton_->setAnchorPoint(Vec2(1.0f, 1.0f));
    closeButton_->setPosition(
        background_->getPosition() +
        1.1f * Vec2(background_->getContentSize().width / 2 - 5,
                    background_->getContentSize().height - 5));
    this->addChild(closeButton_, 2);

    // ��ӹرհ�ť�ı�����
    closeButtonBackground_ =
        LayerColor::create(Color4B(168, 168, 168, 168),
                           closeButton_->getContentSize().width * 0.01f,
                           closeButton_->getContentSize().height * 0.01f);
    closeButtonBackground_->setAnchorPoint(Vec2(1, 1));
    closeButtonBackground_->setPosition(
        closeButton_->getPosition() -
        Vec2(closeButton_->getContentSize().width * 0.01f,
             closeButton_->getContentSize().height * 0.01f));
    closeButtonBackground_->setVisible(false);
    this->addChild(closeButtonBackground_, 1);

    // ���NPC���Ʊ�ǩ
    npcNameLabel_ = Label::createWithTTF(npcName_, std::string(kFontPath), 12);
    npcNameLabel_->setTextColor(Color4B::BLACK);
    npcNameLabel_->setAnchorPoint(Vec2(0.5f, 0));
    npcNameLabel_->setPosition(
        background_->getPosition() +
        1.1f * Vec2(background_->getContentSize().width / 2 - 58, 1));
    this->addChild(npcNameLabel_, 5);

    // ���NPCͷ��
    npcAvatar_ = Sprite::create(npcAvatarPath_, Rect(0, 0, 60, 60));
    npcAvatar_->setAnchorPoint(Vec2(0.5f, 0));
    npcAvatar_->setScale(1.1f);
    npcAvatar_->setPosition(
        npcNameLabel_->getPosition() +
        Vec2(0, 10 + npcNameLabel_->getContentSize().height));
    this->addChild(npcAvatar_, 5);

    gameStates_["hasReceivedGift"] = true;

    return true;
}

std::vector<DialogueEntry> TalkBox::inputJson(const std::string& fileName) {
    // ���� JSON �ļ�
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

            // ���� type
            if (v.HasMember("type") && v["type"].IsString()) {
                std::string typeStr = v["type"].GetString();
                if (typeStr == "dialogue") {
                    entry.type = DialogueEntry::EntryType::Dialogue;
                } else if (typeStr == "question") {
                    entry.type = DialogueEntry::EntryType::Question;
                }
            }

            // ���� content
            if (v.HasMember("content") && v["content"].IsString()) {
                entry.content = v["content"].GetString();
            }

            // ���� options
            if (entry.type == DialogueEntry::EntryType::Question &&
                v.HasMember("options") && v["options"].IsArray()) {
                for (auto& opt : v["options"].GetArray()) {
                    if (opt.IsString()) {
                        entry.options.push_back(opt.GetString());
                    }
                }
            }

            // ���� conditions
            if (v.HasMember("conditions") && v["conditions"].IsObject()) {
                const auto& conditions = v["conditions"];
                // ʱ������
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
                // �¼�����
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
    // ���������Ϣ
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

void TalkBox::onEnter() {
    Layer::onEnter();

    // ��Ӵ����¼�������
    touchListener_ = EventListenerTouchOneByOne::create();
    touchListener_->setSwallowTouches(true);
    touchListener_->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // ����Ƿ����˹رհ�ť
        if (closeButton_ &&
            closeButton_->getBoundingBox().containsPoint(touchLocation)) {
            return false;  // �ùرհ�ť���¼�����������
        }

        if (waitingForChoice_) {
            for (size_t i = 0; i < optionLabels_.size(); ++i) {
                if (optionLabels_[i]->getBoundingBox().containsPoint(
                        touchLocation)) {
                    // ����ѡ��״̬
                    moveSelection(static_cast<int>(i) - selectedOptionIndex_);
                    selectOption();
                    break;
                }
            }
        } else {
            showNextMessage();
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener_,
                                                             this);

    // ��ӹرհ�ť�Ĵ����¼�������
    closeButtonListener_ = EventListenerTouchOneByOne::create();
    closeButtonListener_->setSwallowTouches(true);
    closeButtonListener_->onTouchBegan = [this](Touch* touch, Event* event) {
        if (closeButton_->getBoundingBox().containsPoint(
                touch->getLocation())) {
            return true;
        }
        return false;
    };
    closeButtonListener_->onTouchEnded = [this](Touch* touch, Event* event) {
        if (closeButton_->getBoundingBox().containsPoint(
                touch->getLocation())) {
            hideDialog();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        closeButtonListener_, closeButton_);

    // ��Ӽ����¼�������
    keyboardListener_ = EventListenerKeyboard::create();
    keyboardListener_->onKeyPressed = [this](EventKeyboard::KeyCode keyCode,
                                             Event* event) {
        if (waitingForChoice_) {
            if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
                moveSelection(-1);
            } else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
                moveSelection(1);
            } else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER ||
                       keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                selectOption();
            }
        } else {
            if (keyCode == EventKeyboard::KeyCode::KEY_SPACE ||
                keyCode == EventKeyboard::KeyCode::KEY_ENTER ||
                keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                showNextMessage();
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener_,
                                                             this);

    // �������¼�������
    mouseListener_ = EventListenerMouse::create();
    mouseListener_->onMouseMove = [this](Event* event) {
        EventMouse* e = static_cast<EventMouse*>(event);
        Vec2 loc = Vec2(e->getCursorX(), e->getCursorY());
        if (closeButton_->getBoundingBox().containsPoint(loc)) {
            closeButtonBackground_->setVisible(true);  // ��ʾ��ɫ����
        } else {
            closeButtonBackground_->setVisible(false);  // ���ػ�ɫ����
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener_,
                                                             this);

    showDialog();
}

void TalkBox::onExit() {
    Layer::onExit();
    _eventDispatcher->removeEventListener(touchListener_);
    _eventDispatcher->removeEventListener(keyboardListener_);
    _eventDispatcher->removeEventListener(mouseListener_);
    _eventDispatcher->removeEventListener(closeButtonListener_);
}

void TalkBox::showDialog() {
    while (currentMessageIndex_ < dialogEntries_.size()) {
        const auto& entry = dialogEntries_[currentMessageIndex_];

        // ��������Ƿ�����
        if (conditionsMet(entry)) {
            if (entry.type == DialogueEntry::EntryType::Dialogue) {
                // ��ʾ�Ի�
                messageLabel_->setString(entry.content);
                hideOptions();
                waitingForChoice_ = false;
            } else if (entry.type == DialogueEntry::EntryType::Question) {
                // ��ʾ���⼰ѡ��
                messageLabel_->setString(entry.content);
                showOptions(entry.options);
                waitingForChoice_ = true;
            }
            currentMessageIndex_++;  // ���� currentMessageIndex_
            return;  // �˳�ѭ������ʾ��ǰ�Ի�������
        } else {
            // ���������㣬������ǰ�Ի�
            currentMessageIndex_++;
        }
    }

    if (currentMessageIndex_ >= dialogEntries_.size()) {
        // û�и���ĶԻ����رնԻ���
        hideDialog();
    }
}

void TalkBox::showNextMessage() {
    if (currentMessageIndex_ < dialogEntries_.size()) {
        // currentMessageIndex_++;
        showDialog();
    } else {
        hideDialog();
    }
}

void TalkBox::showOptions(const std::vector<std::string>& options) {
    // ���֮ǰ��ѡ��
    for (auto label : optionLabels_) {
        this->removeChild(label);
    }
    optionLabels_.clear();
    for (auto bg : optionBackgrounds_) {
        this->removeChild(bg);
    }
    optionBackgrounds_.clear();

    selectedOptionIndex_ = 0;

    float startY = messageLabel_->getPositionY() -
                   messageLabel_->getContentSize().height / 2 - 10;
    for (size_t i = 0; i < options.size(); ++i) {
        // ����ѡ�����
        auto optionBg = LayerColor::create(
            (i == selectedOptionIndex_) ? Color4B(50, 50, 50, 200)
                                        : Color4B(0, 0, 0, 0),
            (1.6 * background_->getContentSize().width - 40) / 2,
            10);                               // ���ø߶�Ϊ20
        optionBg->setAnchorPoint(Vec2(0, 1));  // ����ê��Ϊ���Ͻ�
        optionBg->setPosition(
            Vec2(visible_origin_.x + visible_size_.width / 2 -
                     1.2 * background_->getContentSize().width / 2 + 20,
                 startY - (i + 1) * 20));  // ����λ��Ϊ�Ի����ڲ�����벿��
        this->addChild(optionBg, 1);
        optionBackgrounds_.push_back(optionBg);

        // ����ѡ���ǩ
        auto optionLabel =
            Label::createWithTTF(options[i], std::string(kFontPath), 12);
        optionLabel->setAnchorPoint(Vec2(0, 0));  // ����ê��Ϊ���Ͻ�
        optionLabel->setPosition(optionBg->getPosition() -
                                 Vec2(0, 0));  // ����λ���뱳��һ��
        optionLabel->setTextColor(Color4B::WHITE);
        this->addChild(optionLabel, 2);
        optionLabels_.push_back(optionLabel);
    }
}

void TalkBox::hideOptions() {
    for (auto label : optionLabels_) {
        this->removeChild(label);
    }
    optionLabels_.clear();
    for (auto bg : optionBackgrounds_) {
        this->removeChild(bg);
    }
    optionBackgrounds_.clear();
}

void TalkBox::moveSelection(int delta) {
    if (optionLabels_.empty()) return;

    // ����֮ǰ��ѡ���
    optionBackgrounds_[selectedOptionIndex_]->setColor(Color3B::BLACK);
    optionBackgrounds_[selectedOptionIndex_]->setOpacity(0);

    selectedOptionIndex_ += delta;
    if (selectedOptionIndex_ < 0) {
        selectedOptionIndex_ = optionLabels_.size() - 1;
    } else if (selectedOptionIndex_ >= optionLabels_.size()) {
        selectedOptionIndex_ = 0;
    }

    // �����µ�ѡ���
    optionBackgrounds_[selectedOptionIndex_]->setColor(Color3B(50, 50, 50));
    optionBackgrounds_[selectedOptionIndex_]->setOpacity(200);
}

void TalkBox::selectOption() {
    // ��ȡ���ѡ���ѡ��
    int choice = selectedOptionIndex_;
    CCLOG("Select choice %d", choice);

    if (CitizenName == "Sebastian") {
        if (choice == 0 && playerBag->getMoney() >= 50) {
            ItemSprite* CarrotSeed = new ItemSprite(
                "CarrotSeed", 10, "Used for planting carrots",
                "assets/Crops/crops.png", cocos2d::Rect(2, 785, 11, 13));
            playerBag->add(CarrotSeed);
            playerBag->SpendMoney(50);
        } else if (choice == 1 && playerBag->getMoney() >= 40) {
            ItemSprite* PotatoSeed = new ItemSprite(
                "PotatoSeed", 10, "Used for planting potatoes",
                "assets/Crops/crops.png", cocos2d::Rect(130, 54, 10, 7));
            playerBag->add(PotatoSeed);
            playerBag->SpendMoney(40);
        } else if (choice == 2 && playerBag->getMoney() >= 60) {
            ItemSprite* StrawberrySeed = new ItemSprite(
                "StrawberrySeed", 10, "Used for planting strawberries",
                "assets/Crops/crops.png", cocos2d::Rect(1, 593, 13, 14));
            playerBag->add(StrawberrySeed);
            playerBag->SpendMoney(60);
        }
    } 
    // ����ѡ�������һ���Ի�
    hideOptions();
    waitingForChoice_ = false;
    if (currentMessageIndex_ < dialogEntries_.size()) {
        currentMessageIndex_++;  // ���� currentMessageIndex_
    }
    showNextMessage();
}

void TalkBox::hideDialog() { this->removeFromParentAndCleanup(true); }

bool TalkBox::conditionsMet(const DialogueEntry& entry) const {
    // ���ʱ������
    auto it = entry.conditions.find("time");
    if (it != entry.conditions.end()) {
        std::string currentTime = getCurrentGameTime();  // ��ʽΪ "HHMM"
        std::string startTime = it->second[0];
        std::string endTime = it->second[1];
        if (currentTime < startTime || currentTime > endTime) {
            return false;
        }
    }

    // ����¼�����
    auto eventIt = entry.conditions.find("event");
    if (eventIt != entry.conditions.end()) {
        for (const auto& eventName : eventIt->second) {
            bool negate = eventName[0] == '!';
            std::string actualEventName =
                negate ? eventName.substr(1) : eventName;
            auto stateIt = gameStates_.find(actualEventName);
            if (stateIt == gameStates_.end() || stateIt->second == negate) {
                return false;
            }
        }
    }

    return true;
}

std::string TalkBox::getCurrentGameTime() const {
    // ʾ������ȡ��Ϸ�ڵ�ʱ��
    return "1400";
}
