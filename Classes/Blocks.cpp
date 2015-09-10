#include "Blocks.h"
USING_NS_CC;


Blocks::Blocks()

{

}

bool Blocks::init()
{
	if (!Sprite::initWithFile("graphics/mapchip_00.png"))
	{
		return false;
	}
	//auto blockSize = Size(BLOCK_SIZE, BLOCK_SIZE);

	return true;

}
