#include "map.h"
#include "random"
#include "ui/CocosGUI.h"

void Map::initAnimalPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> MaxHeight(
        getMapSize().height * getTileSize().height / 4,
        getMapSize().height * getTileSize().height * 3 / 4);
    std::uniform_int_distribution<> MaxWidth(
        getMapSize().width * getTileSize().height / 4,
        getMapSize().width * getTileSize().height * 3 / 4);
    for (int i = 0; i < MapAnimals.size(); i++) {
        int RandomHeight = MaxHeight(gen);
        int RandomWidth = MaxWidth(gen);
        while (
            isCollisionAtAnyLayer(cocos2d::Vec2(RandomWidth, RandomHeight))) {
            RandomHeight = MaxHeight(gen);
            RandomWidth = MaxWidth(gen);
        }

        SpritetoAnimal[MapAnimals[i]->initSprite] = MapAnimals[i];
        AnimalSprite.push_back(MapAnimals[i]->initSprite);
        MapAnimals[i]->initSprite->setPosition(
            cocos2d::Vec2(RandomWidth, RandomHeight));
        MapAnimals[i]->initSprite->setScale(MapAnimals[i]->scale[MapAnimals[i]->CurrentGrowthStage]);
        CCLOG("Animal at: %d %d\n", RandomWidth, RandomHeight);
        this->addChild(MapAnimals[i]->initSprite);
    }

    this->schedule(CC_SCHEDULE_SELECTOR(Map::updateAnimalSprites), 5.0f);
}

void Map::updateAnimalSprites(float dt) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> MaxHeight(
        getMapSize().height * getTileSize().height / 4,
        getMapSize().height * getTileSize().height * 3 / 4);
    std::uniform_int_distribution<> MaxWidth(
        getMapSize().width * getTileSize().height / 4,
        getMapSize().width * getTileSize().height * 3 / 4);
    std::uniform_int_distribution<> direction(0, 3);
    std::uniform_int_distribution<> Step(20, 100);
    for (int i = 0; i < AnimalSprite.size(); i++) {
        bool shouldMove = CCRANDOM_0_1() > 0.7f;
        CCLOG("shouldMove: %d moved: %d\n", shouldMove,
              SpritetoAnimal[AnimalSprite[i]]->moved);

        if (shouldMove && !SpritetoAnimal[AnimalSprite[i]]->moved) {
            SpritetoAnimal[AnimalSprite[i]]->moved = true;
            int dir = direction(gen);
            float targetX, targetY;
            cocos2d::Vec2 position = AnimalSprite[i]->getPosition();
            targetX = position.x;
            targetY = position.y;
            int deltaX, deltaY;
            if (dir == 0) {
                deltaX = 0;
                deltaY = -1;
            }
            if (dir == 1) {
                deltaX = 1;
                deltaY = 0;
            }
            if (dir == 2) {
                deltaX = 0;
                deltaY = 1;
            }
            if (dir == 3) {
                deltaX = -1;
                deltaY = 0;
            }
            int step = Step(gen);
            for (int i = 1; i <= step; i++) {
                float nextX = targetX + deltaX;
                float nextY = targetY + deltaY;
                if (nextY < getMapSize().height * getTileSize().height / 4 ||
                    nextY > getMapSize().height * getTileSize().height * 3 / 4)
                    break;
                if (nextX < getMapSize().width * getTileSize().width / 4 ||
                    nextX > getMapSize().width * getTileSize().width * 3 / 4)
                    break;
                if (isCollisionAtAnyLayer(cocos2d::Vec2(nextX, nextY))) break;

                targetX = nextX;
                targetY = nextY;
            }

            CCLOG("Direction: %d\n %f %f\n %d\n", dir, targetX, targetY,
                  SpritetoAnimal[AnimalSprite[i]]->moved);


            if (SpritetoAnimal[AnimalSprite[i]]->animFrames[dir].empty()) {
                CCLOG("Animation frames are empty for direction: %d", dir);
            }

            auto animation = cocos2d::Animation::createWithSpriteFrames(
                SpritetoAnimal[AnimalSprite[i]]->animFrames[dir], 0.36);
            auto animate = cocos2d::Animate::create(animation);

            /*float elapsedTime = 0.0f;
            this->schedule(
                [&](float dt) {
                    elapsedTime += dt;
                    float nextX = targetX + deltaX;
                    float nextY = targetY + deltaY;
                    bool check = 1;
                    if (nextY <
                            getMapSize().height * getTileSize().height / 4 ||
                        nextY >
                            getMapSize().height * getTileSize().height * 3 / 4)
                        check = 0;
                    if (nextX < getMapSize().width * getTileSize().width / 4 ||
                        nextX >
                            getMapSize().width * getTileSize().width * 3 / 4)
                        check = 0;
                    if (isCollisionAtAnyLayer(cocos2d::Vec2(nextX, nextY)))
                        check = 0;

                    targetX = nextX;
                    targetY = nextY;
                    AnimalSprite[i]->setPosition(
                        cocos2d::Vec2(targetX, targetY));
                },
                0.1f, "change Target Position");*/
            //AnimalSprite[i]->runAction(cocos2d::RepeatForever::create(animate));

            auto repeatAnimate = cocos2d::RepeatForever::create(animate);
            auto moveAction =
                cocos2d::MoveTo::create(step * 0.1f, cocos2d::Vec2(targetX, targetY));
            auto stopAnimate = cocos2d::CallFunc::create([=]() {
                AnimalSprite[i]->stopAction(repeatAnimate);  // stop animation
                AnimalSprite[i]->setTextureRect(
                    cocos2d::Rect(0, dir * 32, 32, 32));
                SpritetoAnimal[AnimalSprite[i]]->moved = false;
                CCLOG("RepeatForever animation stopped after moveAction.");
            });
            auto sequence =
                cocos2d::Sequence::create(moveAction, stopAnimate, nullptr);

            AnimalSprite[i]->runAction(repeatAnimate);
            AnimalSprite[i]->runAction(sequence);
        }
    }
}

void Map::ShowAnimalInfomation(cocos2d::Sprite *Animal,
                               const cocos2d::Vec2 &InfoPosition,
                               int &priority) {
    CCLOG("Has opend animal infobox\n");
    auto middleNode = cocos2d::Node::create();
    middleNode->setPosition(cocos2d::Vec2(0, 0));
    Animal->addChild(middleNode, priority++);

    // infobox
    auto infoBox =
        cocos2d::Sprite::create("assets/testpng/boardgameborder.png");
    infoBox->setScaleX(0.3 /
                       SpritetoAnimal[Animal]
                           ->scale[SpritetoAnimal[Animal]->CurrentGrowthStage]);
    infoBox->setScaleY(
        0.5 / SpritetoAnimal[Animal]
                  ->scale[SpritetoAnimal[Animal]
                              ->CurrentGrowthStage]);  // Make the infobox the
                                                       // same scale
    // infoBox->setScale(1.0 /
    // SpritetoAnimal[Animal]->scale[SpritetoAnimal[Animal]->CurrentGrowthStage]);
    middleNode->addChild(infoBox, priority++);
    infoBox->setPosition(cocos2d::Vec2(15, 40));

    // Title Define
    size_t kFontSize = 50;
    std::string output = SpritetoAnimal[Animal]->AnimalName;
    if (SpritetoAnimal[Animal]
            ->GrowthStage[SpritetoAnimal[Animal]->CurrentGrowthStage] == "Cub")
        output = output + " cub";
    auto *InfoTitle = cocos2d::Label::createWithTTF(
        output, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    InfoTitle->setScale(0.13);
    InfoTitle->setPosition(cocos2d::Vec2(15, 55));
    middleNode->addChild(InfoTitle, priority);

    // Stage Define
    kFontSize = 50;
    std::string outputStage;
    if (SpritetoAnimal[Animal]->isUnhappy) outputStage += "Unhappy\n";
    if (SpritetoAnimal[Animal]->isHungry) outputStage += "Hungry";
    if (!outputStage.empty()) {
        auto *Stage = cocos2d::Label::createWithTTF(
            outputStage, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
        Stage->setScale(0.13);
        Stage->setPosition(cocos2d::Vec2(0, 40));
        Stage->setTextColor(cocos2d::Color4B::ORANGE);
        middleNode->addChild(Stage, priority);
    }

    // Button Define
    // Stroke Button
    auto StrokeButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    StrokeButton->setScaleX(0.13);
    StrokeButton->setScaleY(0.15);
    StrokeButton->setPosition(cocos2d::Vec2(30, 45));
    StrokeButton->setTitleText("Stroke");
    StrokeButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    StrokeButton->setTitleColor(cocos2d::Color3B::BLACK);
    StrokeButton->setColor(cocos2d::Color3B::WHITE);
    StrokeButton->setTitleFontSize(40);
    StrokeButton->addClickEventListener([this, Animal](cocos2d::Ref *sender) {
        CCLOG("Stroke button clicked");
        SpritetoAnimal[Animal]->Stroke();
    });
    //StrokeButton->setTouchEnabled(true);
    //StrokeButton->setSwallowTouches(true);
    middleNode->addChild(StrokeButton, priority);

    // Confirm Button
    auto ConfirmButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    ConfirmButton->setScaleX(0.13);
    ConfirmButton->setScaleY(0.15);
    ConfirmButton->setPosition(cocos2d::Vec2(30, 35));
    ConfirmButton->setTitleText("Confirm");
    ConfirmButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    ConfirmButton->setTitleColor(cocos2d::Color3B::BLACK);
    ConfirmButton->setColor(cocos2d::Color3B::WHITE);
    ConfirmButton->setTitleFontSize(40);
    ConfirmButton->addClickEventListener([this, middleNode, Animal](cocos2d::Ref *sender) {
            CCLOG("Confirm button clicked");
            middleNode->setVisible(false);
            SpritetoAnimal[Animal]->InfoOpen = false;
            CCLOG("middleNode visibility: %d", middleNode->isVisible());
    });
    //ConfirmButton->setTouchEnabled(true);
    //ConfirmButton->setSwallowTouches(true);
    middleNode->addChild(ConfirmButton, priority);
}
