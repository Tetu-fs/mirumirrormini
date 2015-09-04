#ifndef __STAGE_H__
#define __STAGE_H__
#include "cocos2d.h"
#include "Player.h"
class Stage : public cocos2d::Layer
{

protected:

	Stage();
	virtual ~Stage() override;
	bool init() override;
public:

	enum TileType
	{
		BLOCKS = 1 << 0,
		PLAYER = 1 << 1
	};

	bool leftFlag;
	bool rightFlag;
	
	cocos2d::Vec2 _velocity;

	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*, _tiledMap, TiledMap);

	CC_SYNTHESIZE_RETAIN(Player*, _player, Player);
	CREATE_FUNC(Stage);
	void update(float dt) override;
private:
	cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer*layer, cocos2d::Vec2&coodinate);
};
#endif //__STAGE_H__