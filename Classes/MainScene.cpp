#include "MainScene.h"
USING_NS_CC;

const Vec2 GRAVITY_ACCELERATION = Vec2(0, -18);

MainScene::MainScene()
	:_stage(nullptr)
{

}

MainScene::~MainScene()
{
	CC_SAFE_RELEASE_NULL(_stage);
}

Scene* MainScene::createSceneWithLevel(int level)
{
	auto scene = Scene::createWithPhysics();

	auto world = scene->getPhysicsWorld();
	
	//world->setUpdateRate(2);

	world->setGravity(GRAVITY_ACCELERATION);

#if COCOS2D_DEBUG > 0
	
	world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

#endif

	world->setSpeed(4.0);

	auto layer = new MainScene();
	if (layer && layer->initWithLevel(level))
	{
		layer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(layer);
	}
		scene->addChild(layer);

	return scene;
}


bool MainScene::initWithLevel(int level)
{
	if (!Layer::init())
	{
		return false;
	}

	auto stage = Stage::createWithLevel(level);
	this->setStage(stage);
	this->addChild(stage);
	/*
	//�J�e�S���쐬
	const int PhysicsCategoryBlocks = 1 << 0;
	const int PhysicsCategoryChara = 2 << 0;

	//���̍쐬
	auto blocks = PhysicsBody::create();
	//���̂ɃJ�e�S�����Z�b�g
	blocks->setCategoryBitmask(PhysicsCategoryBlocks);
	blocks->setContactTestBitmask(PhysicsCategoryChara);

	//�v���C���[�̍��̍쐬
	auto player = PhysicsBody::create();
	player->setCategoryBitmask(PhysicsCategoryChara);

	//�Փ˂�L����
	//player->setCollisionBitmask(PhysicsCategoryBlocks);
	player->setContactTestBitmask(PhysicsCategoryBlocks);
	*/
	return true;

}