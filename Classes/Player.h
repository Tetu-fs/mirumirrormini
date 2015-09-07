#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Magic.h"
class Player :public cocos2d::Sprite
{
protected:
	
	Player();
	bool init() override;
	void update(float dt) override;

	int indexCheck;

public:


	bool magicLRFlag;
	bool rightFlag;

	void jump();
	void playAnimation(int index);
	Magic* MirrorMethod();


	CREATE_FUNC(Player);

};

#endif //__PLAYER_H__