#ifndef FARM_TOOL_MAP_H
#define FARM_TOOL_MAP_H

#include <string>
#include <unordered_map>

#include "map.h"

// ������ GID ��ӳ���
//static std::unordered_map<std::string, int> toolGidMapping = {
//    {"hoe", 3},           // ��ͷ��Ӧ�ĸ�����Ƭ GID
//    {"watering_can", 4},  // ˮ����Ӧ��ʪ�������Ƭ GID
//    {"axe", 5},           // ��ͷ��Ӧ�Ŀ��� GID
//    {"pickaxe", 6},       // ���Ӷ�Ӧ���û�ʯͷ GID
//    {"seed", 7}           // ���Ӷ�Ӧ�Ĳ��� GID
//};

class FarmToolMap : public Map {
//private:
//    struct CropData {
//        int growthStage;  // ���������׶�
//        int maxGrowthStage;  // ��������׶�
//        float timeSinceLastUpdate;  // �ϴθ���ʱ��
//        float growthTime;           // ����ʱ��
//        CropData(int maxGrowth, float growthInterval)
//            : growthStage(0),
//              maxGrowthStage(maxGrowth),
//              timeSinceLastUpdate(0.0f),
//              growthTime(growthInterval) {}
//    };
//
//    std::unordered_map<cocos2d::Vec2, CropData>
//        crops;  // ���ڴ洢���������״̬
//    
//    void growCrops(float delta);
//
//public:
//    FarmToolMap() = default;
//    ~FarmToolMap() override = default;
//
//    void addSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 tileCoord,
//                   const std::string& layerName);  // ��Ӿ���
//
//    void useToolOnTile(cocos2d::Vec2 tileCoord,
//                       const std::string& toolName);  // ʹ�ù���
//    void plantSeed(cocos2d::Vec2 tileCoord, int seedGID);  // ��ֲ����
//    void update(float delta) override;                     // ���µ�ͼ
//
//    CREATE_FUNC(FarmToolMap);
};

#endif  // FARM_TOOL_MAP_H
