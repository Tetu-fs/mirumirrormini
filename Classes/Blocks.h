#include "cocos2d.h"

class Blocks : public cocos2d::Sprite
{
protected:
	bool init() override;

	Blocks::Blocks();
	//virtual Blocks::~Blocks() override;

	/*
	int blockLife;
	*/

public:

	
	cocos2d::Vec2 BlockVec;
	CREATE_FUNC(Blocks);

};
