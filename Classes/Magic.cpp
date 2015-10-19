#include "Magic.h"

USING_NS_CC;

bool Magic::init()
{
	if (!Sprite::initWithFile("graphics/wave.png"))
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	auto white = Sprite::create("graphics/wave.png");
	white->getTexture()->setAliasTexParameters();


	this->scheduleUpdate();
	return true;
}