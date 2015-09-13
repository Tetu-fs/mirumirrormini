#ifndef __STAGE_H__
#define __STAGE_H__
#include "cocos2d.h"
#include "Player.h"
#include "Blocks.h"
class Stage : public cocos2d::Layer
{

protected:

	Stage();
	virtual ~Stage() override;
	bool init() override;

	bool leftPressFlag;
	bool rightPressFlag;

	bool upPressFlag;

	bool goalFlag;

	void playerMove();
	void jumpMethod();
	float testX;
	float testY;
	
	//BlockGen
	int blockX;
	int blockY;
	int mapX;
	int mapY;
	int rectX;
	int rectY;
	int tileID;

	cocos2d::Vec2 testVec;

	cocos2d::Sprite* testBlock;

public:

	enum TileType
	{
		BLOCKS = 1 << 0,
		GOAL =1<<1,
		PLAYER = 2 << 0
	};

	cocos2d::Vec2 _velocity;
	cocos2d::Vec2 _prevPosition;

	cocos2d::Vec2 _playerPosition;
	cocos2d::Vec2 _mirrorPosition;

	void update(float dt) override;

	Magic* MirrorMethod();

	void testMethod();

	Blocks* BlockGen(int gid);

	CC_SYNTHESIZE(bool, _jumpFlag, JumpFlag);

	CC_SYNTHESIZE_RETAIN(Magic*, _magic, Magic);
	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*, _tiledMap, TiledMap);
	CC_SYNTHESIZE_RETAIN(Player*, _player, Player);
	CC_SYNTHESIZE_RETAIN(Blocks*, _blocks, Blocks);

	CREATE_FUNC(Stage);


private:
	//cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer*layer, cocos2d::Vec2&coodinate);

};

#endif //__STAGE_H__