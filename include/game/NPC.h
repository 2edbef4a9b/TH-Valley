#ifndef NPC_H
#define NPC_H

#include "game/citizen.h"

class Sandy : public Citizen {
public:
    Sandy() {
        CitizenName = "Sandy";

        initSprite = cocos2d::Sprite::create("assets/avatar/sandy.png",
                                             cocos2d::Rect(0, 0, 16, 32));  
        // initSprite->setScale(0.6);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 16, i * 32, 16, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/avatar/sandy.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }
    }
};

class Abigail : public Citizen {
public:
    Abigail() {
        CitizenName = "Abigail";

        initSprite = cocos2d::Sprite::create("assets/avatar/Abigail.png",
                                             cocos2d::Rect(0, 0, 16, 32));
        // initSprite->setScale(0.6);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 16, i * 32, 16, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/avatar/Abigail.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }
    }
};

class Alex : public Citizen {
public:
    Alex() {
        CitizenName = "Alex";

        initSprite = cocos2d::Sprite::create("assets/avatar/Alex.png",
                                             cocos2d::Rect(0, 0, 16, 32));
        // initSprite->setScale(0.6);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 16, i * 32, 16, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/avatar/Alex.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }
    }
};

class Elliott : public Citizen {
public:
    Elliott() {
        CitizenName = "Elliott";

        initSprite = cocos2d::Sprite::create("assets/avatar/Elliott.png",
                                             cocos2d::Rect(0, 0, 16, 32));
        // initSprite->setScale(0.6);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 16, i * 32, 16, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/avatar/Elliott.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }
    }
};

class Evelyn : public Citizen {
public:
    Evelyn() {
        CitizenName = "Evelyn";

        initSprite = cocos2d::Sprite::create("assets/avatar/Evelyn.png",
                                             cocos2d::Rect(0, 0, 16, 32));
        // initSprite->setScale(0.6);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 16, i * 32, 16, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/avatar/Evelyn.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }
    }
};

#endif
