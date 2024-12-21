#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "frontend/bag_gui.h"
#include "frontend/tool_bar.h"
#include "game/agriculture.h"
#include "utility/position.h"

struct Tile {
    int x, y;
    int gid;
    Tile(int x, int y, int gid) : x(x), y(y), gid(gid) {};
};
class Crops;

class Animals;
class Map : public cocos2d::TMXTiledMap {
protected:
    cocos2d::TMXTiledMap* tileMap;
    std::unordered_map<std::string, cocos2d::TMXLayer*> mapLayer;
    cocos2d::TMXObjectGroup* objectGroup;
    cocos2d::Vec2 playerPos;
    cocos2d::ValueMap playerObject;

    bool isKeyPressedW;
    bool isKeyPressedA;
    bool isKeyPressedS;
    bool isKeyPressedD;

public:
    cocos2d::Sprite* playerSprite;
    std::map<Position, Crops*> CropPosition;
    std::map<Position, cocos2d::Sprite*> SpritePosition;
    std::map<cocos2d::Sprite*, Animals*> SpritetoAnimal;
    std::vector<Crops*> MapCrops;
    std::vector<Animals*> MapAnimals;
    std::vector<cocos2d::Sprite*> AnimalSprite;
    ToolBar* MapToolBar;
    BagGUI* MapBag;
    int priority = 255;
    Map() = default;
    ~Map() override = default;
    Map(const Map& other) = default;
    Map& operator=(const Map& other) = default;
    Map(Map&& other) = default;
    Map& operator=(Map&& other) = default;

    /**
     * @brief ���ص�����ת��Ϊ��Ƭ����
     * @param string tmxFile
     * @return true: �ɹ����� false: ����ʧ��
     */
    bool initWithTMXFile(const std::string& tmxFile, cocos2d::Sprite* Haley,
                         ToolBar* CurrentToolBar, BagGUI* CurrentBag);
    /**
     * @brief �������λ��
     * @param cocos2d::Vec2 pos
     */
    void setPlayerPos(cocos2d::Vec2 pos);

    /**
     * @brief ���ص�����ת��Ϊ��Ƭ����
     * @param cocos2d::Vec2 pos
     * @return cocos2d::Vec2
     */
    cocos2d::Vec2 tileCoordFromPos(cocos2d::Vec2 pos);
    /**
     * @brief �ж��Ƿ�����ײ
     * @param cocos2d::Vec2 pos ���ص�����  string LayerName ������
     * @return true: ����ײ���������ƶ�
     false: ����ײ�������ƶ�
     */
    bool isCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool isCollisionAtAnyLayer(cocos2d::Vec2 pos);

    /**
     * @brief �ж��Ƿ��д�����
     * @param cocos2d::Vec2 pos ���ص�����  string ObjectLayerName ������
     * @return true: �д����ţ�ִ�д��Ͳ���
     false: ��
     */
    bool isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName = "Objects");

    /**
     * @brief ��ͼ
     * @param string portalName
     false: ��
     */
    void triggerPortalEvent(const std::string& portalName);

    /**
     * @brief �����ӽ����ģ�����λ�ø���ʱ����
     * @param cocos2d::Vec2 pos
     */
    void setViewpointCenter(cocos2d::Vec2 pos);

    /**
     * @brief ��ȡ�������
     * @return cocos2d::Vec2
     */
    cocos2d::Vec2 getPos();

    /**
     * @brief ���������
     * @param cocos2d::Vec2 tileCoord
     */
    void checkEventsAndTrigger(cocos2d::Vec2 tileCoord);

    /**
     * @brief ������ͼ
     * @param cocos2d::Vec2 tileCoord; int newGID; string LayerName
     */
    void updateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                      std::string LayerName);
    int getTileID(cocos2d::Vec2 tileCoord, std::string LayerName);
    bool PropertyCheck(int gid, std::string property);
    int getTileID(std::string Properties);

    /**
     * @brief �����û�����
     */
    void onEnter();

    /**
     * @brief ����С��ͼ
     */
    void createMiniMap();

    /**
     * @brief �����ͼ
     */
    void save();

    /**
     * @brief ���ص�ͼ
     */
    void load();

    void update(float delta) override;

    static Map* create(const std::string& tmxFile,
                       cocos2d::Sprite* PlayerSprite, ToolBar* CurrentToolBar,
                       BagGUI* CurrentBag);
};

#endif  // MAP_H_
