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


	CREATE_FUNC(Blocks);

};
