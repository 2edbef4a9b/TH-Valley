#ifndef MAP_H_
#define MAP_H_

#include "cocos2d.h"
#include <vector>
#include <unordered_map>
#include "avatar/avatar_scene.h"

struct Tile {
    int x, y;
    int gid;
    Tile(int x, int y, int gid) : x(x), y(y), gid(gid) {};
};



class Map : public cocos2d::TMXTiledMap {
protected:
    cocos2d::TMXTiledMap* tileMap;
    std::unordered_map<std::string, cocos2d::TMXLayer*> mapLayer;
    cocos2d::TMXObjectGroup* objectGroup;   // 物品组
    cocos2d::Vec2 playerPos;          // 玩家位置
    cocos2d::Sprite* playerSprite;
    cocos2d::ValueMap playerObject;

    // 按键状态
    bool isKeyPressedW;
    bool isKeyPressedA;
    bool isKeyPressedS;
    bool isKeyPressedD;

    direction dir;
public:
    Map() = default;
    ~Map() override = default;
    Map(const Map& other) = default;
    Map& operator=(const Map& other) = default;
    Map(Map&& other) = default;
    Map& operator=(Map&& other) = default;

    /**
     * @brief 像素点坐标转换为瓦片坐标
     * @param string tmxFile
     * @return true: 成功加载 false: 加载失败
     */
    bool initWithTMXFile(const std::string& tmxFile)
        ;
    /**
     * @brief 设置玩家位置
     * @param cocos2d::Vec2 pos
     */
    void setPlayerPos(cocos2d::Vec2 pos);

    /**
     * @brief 像素点坐标转换为瓦片坐标
     * @param cocos2d::Vec2 pos
     * @return cocos2d::Vec2
     */
    cocos2d::Vec2 tileCoordFromPos(cocos2d::Vec2 pos);

    /**
     * @brief 判断是否有碰撞
     * @param cocos2d::Vec2 pos 像素点坐标  string LayerName 层名称
     * @return true: 有碰撞，不可以移动
     false: 无碰撞，可以移动
     */
    bool isCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool isCollisionAtAnyLayer(cocos2d::Vec2 pos);

    /**
     * @brief 判断是否有传送门
     * @param cocos2d::Vec2 pos 像素点坐标  string ObjectLayerName 层名称
     * @return true: 有传送门，执行传送操作
     false: 无
     */
    bool isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName = "Objects");

    /**
     * @brief 切图
     * @param string portalName 
     false: 无
     */
    void triggerPortalEvent(const std::string& portalName);

    /**
     * @brief 设置视角中心，主角位置更新时调用
     * @param cocos2d::Vec2 pos
     */
    void setViewpointCenter(cocos2d::Vec2 pos);

    /**
     * @brief 获取玩家坐标
     * @return cocos2d::Vec2
     */
    cocos2d::Vec2 getPos();

    /**
     * @brief 检测活动并触发
     * @param cocos2d::Vec2 tileCoord
     */
    void checkEventsAndTrigger(cocos2d::Vec2 tileCoord);

    /**
     * @brief 更新贴图
     * @param cocos2d::Vec2 tileCoord; int newGID; string LayerName
     */
    void updateTileAt(cocos2d::Vec2 tileCoord, int newGID, std::string LayerName);

    /**
     * @brief 监视用户操作
     */
    void onEnter();

    /**
     * @brief 创建小地图
     */
    void createMiniMap();

    /**
     * @brief 保存地图
     */
    void save();

    /**
     * @brief 加载地图
     */
    void load();

    void update(float delta) override;

    static Map* create(const std::string& tmxFile);

};


#endif  // MAP_H_
