#include "Magic.h"

USING_NS_CC;

bool Magic::init()
{
	if (!Sprite::initWithFile("graphics/white.png"))
	{
		return false;
	}


	auto white = Sprite::create("graphics/white.png");
	white->getTexture()->setAliasTexParameters();

	this->scheduleUpdate();
	return true;
}