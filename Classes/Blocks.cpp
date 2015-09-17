#include "Blocks.h"
USING_NS_CC;

const float MAP_HEIGHT = 14;
Blocks::Blocks()

{

}

bool Blocks::init()
{
	if (!Sprite::initWithFile("graphics/mapchip_00.png"))
	{
		return false;
	}
	BlockVec = Vec2(BlockVecConvert());

	return true;
}

Vec2 Blocks::BlockVecConvert()
{
	auto Position = this->getPosition();
	float x = floor((Position.x + 8) / 16);
	float y = MAP_HEIGHT - floor((Position.y + 8) / 16) - 1;
	//log("%f");
	return  Vec2(x, y);
}