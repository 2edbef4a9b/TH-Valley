#include "game/items/tool_strategies.h"
#include "game/entities/avatar.h"
#include "cocos2d.h"

namespace th_valley {

// --- Object Pool Implementation - Refactored with Object Pool Pattern ---
std::list<cocos2d::Sprite*> IToolStrategy::effect_pool_;

void IToolStrategy::CleanupPool() {
    for (auto* sprite : effect_pool_) {
        sprite->release();
    }
    effect_pool_.clear();
}

cocos2d::Sprite* IToolStrategy::GetEffectSprite(const std::string& textureName, const cocos2d::Rect& rect) {
    if (!effect_pool_.empty()) {
        auto* sprite = effect_pool_.front();
        effect_pool_.pop_front();
        
        sprite->setTexture(textureName);
        sprite->setTextureRect(rect);
        sprite->setVisible(true);
        sprite->setOpacity(255);
        sprite->setScale(1.0f);
        sprite->stopAllActions();
        sprite->setRotation(0);
        sprite->setFlippedX(false);
        sprite->setFlippedY(false);
        
        sprite->autorelease();
        return sprite;
    }
    return cocos2d::Sprite::create(textureName, rect);
}

void IToolStrategy::ReturnEffectSprite(cocos2d::Sprite* sprite) {
    if (sprite) {
        sprite->retain();
        sprite->removeFromParent();
        sprite->setVisible(false);
        effect_pool_.push_back(sprite);
    }
}

// --- Strategy Pattern Implementation - Refactored with Strategy Pattern ---

void WateringCanStrategy::Use(Avatar* user, cocos2d::Vec2 tarpos) {
    constexpr double kScale = 5.0;
    auto direction = user->GetDirection();
    using Direction = Entity::Direction;

    int pro = 2;
    if (direction == Direction::kUp) {
        tarpos.y += 8;
        pro = 1;
    } else if (direction == Direction::kDown) {
        tarpos.y -= 8;
    } else if (direction == Direction::kRight) {
        tarpos.x += 8;
        tarpos.y -= 4;
    } else if (direction == Direction::kLeft) {
        tarpos.x -= 8;
        tarpos.y -= 4;
        pro = 1;
    }

    cocos2d::Rect rect(241, 217, 15, 23);
    bool flippedX = false;

    if (direction == Direction::kUp) {
        rect = cocos2d::Rect(239 - 16, 239 - 18, 16, 18);
    } else if (direction == Direction::kLeft) {
        rect = cocos2d::Rect(289 - 20, 240 - 19, 20, 19);
        flippedX = true;
    } else if (direction == Direction::kRight) {
        rect = cocos2d::Rect(289 - 20, 240 - 19, 20, 19);
    }

    cocos2d::Sprite* tempavatar = GetEffectSprite("assets/tool/tools.png", rect);
    if (flippedX) tempavatar->setFlippedX(true);

    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));
    tempavatar->setOpacity(0);
    tempavatar->setScale(kScale);
    user->addChild(tempavatar, pro);

    auto fadeIn = cocos2d::FadeIn::create(0.05f);
    auto delay = cocos2d::DelayTime::create(1.0f);
    auto fadeOut = cocos2d::FadeOut::create(0.05f);

    auto lambda = [tempavatar]() { ReturnEffectSprite(tempavatar); };
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create(lambda), nullptr);
    tempavatar->runAction(sequence);
}

void FishingRodStrategy::Use(Avatar* user, cocos2d::Vec2 tarpos) {
    constexpr double kScale = 5.0;
    cocos2d::Sprite* Scene[6];
    tarpos.x += 4;
    tarpos.y -= 4;
    
    Scene[0] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(0, 295, 35, 31));
    Scene[1] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(55, 295, 35, 31));
    Scene[2] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(102, 295, 35, 31));
    Scene[3] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(160, 295, 35, 31));
    Scene[4] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(210, 295, 35, 31));
    Scene[5] = GetEffectSprite("assets/tool/tools.png", cocos2d::Rect(256, 295, 35, 31));

    for (int i = 0; i < 5; ++i) {
        Scene[i]->setPosition(cocos2d::Vec2(tarpos.x - 2, tarpos.y));
        Scene[i]->setOpacity(0);
        Scene[i]->setScale(kScale);
        user->addChild(Scene[i]);

        auto fadeIn = cocos2d::FadeIn::create(0.1f);
        auto delay = cocos2d::DelayTime::create(0.8f);
        auto fadeOut = cocos2d::FadeOut::create(0.1f);

        auto lambda = [scenePtr = Scene[i]]() {
            if (scenePtr) {
                ReturnEffectSprite(scenePtr);
            }
        };
        auto sequence = cocos2d::Sequence::create(
            cocos2d::DelayTime::create(i * 0.8f), fadeIn, delay, fadeOut,
            cocos2d::CallFunc::create(lambda), nullptr);

        Scene[i]->runAction(sequence);
    }
    Scene[5]->setPosition(cocos2d::Vec2(tarpos.x - 5, tarpos.y + 3.5));
    Scene[5]->setOpacity(0);
    Scene[5]->setScale(kScale);
    user->addChild(Scene[5]);

    auto fadeIn = cocos2d::FadeIn::create(0.1f);
    auto delay = cocos2d::DelayTime::create(2.0f);
    auto fadeOut = cocos2d::FadeOut::create(0.1f);

    auto lambda = [scenePtr = Scene[5]]() {
        if (scenePtr) {
            ReturnEffectSprite(scenePtr);
        }
    };
    auto sequence = cocos2d::Sequence::create(
        cocos2d::DelayTime::create(5 * 0.8f), fadeIn, delay, fadeOut,
        cocos2d::CallFunc::create(lambda), nullptr);

    Scene[5]->runAction(sequence);
}

void HoeStrategy::Use(Avatar* user, cocos2d::Vec2 tarpos) {
    constexpr double kScale = 5.0;
    auto direction = user->GetDirection();
    using Direction = Entity::Direction;
    
    int y = 156 - 125; // Hoe specific
    int pro = 2;

    if (direction == Direction::kUp) {
        tarpos.y += 8;
        pro = 1;
    } else if (direction == Direction::kDown) {
        tarpos.y -= 8;
    } else if (direction == Direction::kRight) {
        tarpos.x += 4;
    } else if (direction == Direction::kLeft) {
        tarpos.x -= 4;
        pro = 1;
    }

    cocos2d::Rect rect(242, y, 11, 22);
    bool flippedX = false;

    if (direction == Direction::kUp) {
        rect = cocos2d::Rect(291, y, 12, 25);
    } else if (direction == Direction::kLeft) {
        rect = cocos2d::Rect(302, y, 19, 19);
        flippedX = true;
    } else if (direction == Direction::kRight) {
        rect = cocos2d::Rect(302, y, 19, 19);
    }

    cocos2d::Sprite* tempavatar = GetEffectSprite("assets/tool/tools.png", rect);
    if (flippedX) tempavatar->setFlippedX(true);

    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));
    tempavatar->setOpacity(0);
    tempavatar->setScale(kScale);
    user->addChild(tempavatar, pro);

    auto fadeIn = cocos2d::FadeIn::create(0.05f);
    auto delay = cocos2d::DelayTime::create(0.08f);
    auto fadeOut = cocos2d::FadeOut::create(0.05f);

    auto lambda = [tempavatar]() { ReturnEffectSprite(tempavatar); };
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create(lambda), nullptr);
    tempavatar->runAction(sequence);
}

void WeaponStrategy::Use(Avatar* user, cocos2d::Vec2 tarpos) {
    constexpr double kScale = 5.0;
    auto direction = user->GetDirection();
    using Direction = Entity::Direction;
    
    int y = 0;
    if (weaponName_ == "Axe") {
        y = 156;
    } else if (weaponName_ == "Draft") {
        y = 156 - 67;
    } else {
        return;
    }
    
    int pro = 2;
    if (direction == Direction::kUp) {
        tarpos.y += 8;
        pro = 1;
    } else if (direction == Direction::kDown) {
        tarpos.y -= 8;
    } else if (direction == Direction::kRight) {
        tarpos.x += 4;
    } else if (direction == Direction::kLeft) {
        tarpos.x -= 4;
        pro = 1;
    }

    cocos2d::Rect rect(242, y, 11, 22);
    bool flippedX = false;

    if (direction == Direction::kUp) {
        rect = cocos2d::Rect(291, y, 12, 25);
    } else if (direction == Direction::kLeft) {
        rect = cocos2d::Rect(302, y, 19, 19);
        flippedX = true;
    } else if (direction == Direction::kRight) {
        rect = cocos2d::Rect(302, y, 19, 19);
    }

    cocos2d::Sprite* tempavatar = GetEffectSprite("assets/tool/tools.png", rect);
    if (flippedX) tempavatar->setFlippedX(true);

    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));
    tempavatar->setOpacity(0);
    tempavatar->setScale(kScale);
    user->addChild(tempavatar, pro);

    auto fadeIn = cocos2d::FadeIn::create(0.05f);
    auto delay = cocos2d::DelayTime::create(0.08f);
    auto fadeOut = cocos2d::FadeOut::create(0.05f);

    auto lambda = [tempavatar]() { ReturnEffectSprite(tempavatar); };
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create(lambda), nullptr);
    tempavatar->runAction(sequence);
}

} // namespace th_valley