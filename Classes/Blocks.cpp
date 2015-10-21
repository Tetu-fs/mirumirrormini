#include "Blocks.h"
USING_NS_CC;


bool Blocks::init()
{
	if (!Sprite::initWithFile("graphics/mapchip_00.png"))
	{
		return false;
	}
	return true;
}

