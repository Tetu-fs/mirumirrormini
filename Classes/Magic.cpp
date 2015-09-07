#include "Magic.h"

USING_NS_CC;

bool Magic::init()
{
	if (!Sprite::initWithFile("graphics/white.png"))
	{
		return false;
	}


	auto white = Sprite::create("graphics/white.png");
	white->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);


	/*
	auto playerPosition = _player->getPosition();
	auto PtoM = (_mirrorPosition.x - playerPosition.x) / 16;
	log("%d", PtoM);
	PtoM *= -1;
	log("%d", PtoM);
	*/
	this->scheduleUpdate();
	return true;
}