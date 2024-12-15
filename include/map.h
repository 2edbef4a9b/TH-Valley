#ifndef MAP_H_
#define MAP_H_

#include "cocos2d.h"
#include <vector>

struct Tile {
    int x, y;
    int gid;
    Tile(int x, int y, int gid) : x(x), y(y), gid(gid) {};
};

class Map : public cocos2d::TMXTiledMap {
private:
    cocos2d::TMXTiledMap* tileMap;
    std::vector<cocos2d::TMXLayer*> mapLayer;
    cocos2d::TMXObjectGroup* objectGroup;   // ��Ʒ��

public:
    Map() = default;
    ~Map() override = default;
    Map(const Map& other) = default;
    Map& operator=(const Map& other) = default;
    Map(Map&& other) = default;
    Map& operator=(Map&& other) = default;

    bool initWithTMXFile(const std::string& tmxFile);

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

    /**
     * @brief �ж��Ƿ��д�����
     * @param cocos2d::Vec2 pos ���ص�����  string ObjectLayerName ������
     * @return true: �д����ţ�ִ�д��Ͳ���
     false: ��
     */
    bool isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName = "Objects");

    /**
     * @brief �����ӽ����ģ�����λ�ø���ʱ����
     * @param cocos2d::Vec2 pos
     */
    void setViewpointCenter(cocos2d::Vec2 pos);

    /**
     * @brief ��ȡ�����ʼλ��
     * @return cocos2d::Vec2
     */
    cocos2d::Vec2 getStartPos();

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
     * @param cocos2d::Vec2 tileCoord  int newGID
     */
    void updateTileAt(cocos2d::Vec2 tileCoord, int newGID, std::string LayerName);

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

    //void update(float dt);
    static void SetResourcePath(const std::string& path);
    static Map* create(const std::string& tmxFile);
};


#endif  // MAP_H_
