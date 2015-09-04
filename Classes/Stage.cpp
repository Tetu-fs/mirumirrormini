#include "Stage.h"
USING_NS_CC;

const Vec2 JUMP_IMPULSE = Vec2(0, 1200);

Stage::Stage()
	:_player(nullptr)
	, _tiledMap(nullptr)
	, leftFlag(false)
	, rightFlag(false)
	, _jumpFlag(false)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_player);
	CC_SAFE_RELEASE_NULL(_tiledMap);
}

void Stage::update(float dt)
{

	//Vec2�^��_velocity�Ƃ����ϐ��𐮐��l�ɒ����H
	_velocity.normalize();
	//���g�̈ʒu���A���ݒn�{�x�N�g����SPEED�̒l�ɂ���
	_player->setPosition(_player->getPosition() + _velocity);

	auto playerPosition = _player->getPosition();

	Size winSize = Director::getInstance()->getWinSize();
	if (playerPosition.x >= winSize.width)
	{
		_player->setPositionX(winSize.width);
		_velocity.x = 0;
	}
	
	else if (playerPosition.x <= 0)
	{
		_player->setPositionX(0);
		_velocity.x = 0;
	}
	
	if (playerPosition.y >= winSize.height)
	{
		_player->setPositionY(winSize.height);
		_velocity.y = 0;
	}
	
	else if (playerPosition.y <= 0)
	{
		_player->setPositionY(0);
		_velocity.y = 0;
	}


	auto flip = FlipX::create(true);
	auto flipback = FlipX::create(false);

	
	if (leftFlag == true)
	{
		_player->playAnimation(1);
		_player->runAction(flip);
		_velocity.x = -2;


	}

	if (rightFlag == true)
	{
		_player->playAnimation(1);
		_player->runAction(flipback);
		_velocity.x = 2;
	}

	if (leftFlag == false && rightFlag == false)
	{
		_player->playAnimation(0);
		_velocity.x = 0;
	}

}
Sprite* Stage::addPhysicsBody(TMXLayer *layer, Vec2 &coordinate)
{
	int checkSlope = layer->getTileGIDAt(coordinate);
	int checkflower = layer->getTileGIDAt(coordinate);
	//�X�v���C�g�𒊏o
	auto mapSprite = layer->getTileAt(coordinate);
	Point box[4]{Point(-8, 0), Point(-8, 8), Point(8, 8), Point(8, 0)};
	Point slope1[3]{Point(-8, -8), Point(8, -8), Point(8, 8)};
	Point slope2[3]{Point(-8, -8), Point(8, -8), Point(-8, 8)};

	if (mapSprite)
	{

		auto gid = layer->getTileGIDAt(coordinate);

		if (gid == 1 || gid == 5 || gid == 6 || gid == 7 || gid == 8)
		{

			//���̃}�e���A���ݒ�
			auto material = PhysicsMaterial();
			//���C
			material.friction = 99;
			material.restitution = 0.0;
			//���̐ݒu

			auto category = 1;

			if (checkSlope != 5 && checkSlope != 8 && checkflower != 3)
			{
				auto physicsBody = PhysicsBody::createPolygon(box, 4, material);

				//���̌Œ�
				physicsBody->setDynamic(false);


				physicsBody->setCategoryBitmask(category);
				physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody);
			}




			else if (checkSlope == 5)
			{
				auto physicsBody2 = PhysicsBody::createEdgePolygon(slope1, 3, material);

				//���̌Œ�
				physicsBody2->setDynamic(false);


				physicsBody2->setCategoryBitmask(category);
				physicsBody2->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody2->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody2);

			}


			else if (checkSlope == 8)
			{
				auto physicsBody3 = PhysicsBody::createEdgePolygon(slope2, 3, material);

				//���̌Œ�
				physicsBody3->setDynamic(false);


				physicsBody3->setCategoryBitmask(category);
				physicsBody3->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody3->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody3);

			}



		}
		mapSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		return mapSprite;

	}

	return nullptr;

};

bool Stage::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//��ʃT�C�Y�擾
	Size winSize = Director::getInstance()->getWinSize();

	//�w�i�\��
	auto background1 = Sprite::create("graphics/sky.png");
	background1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background1->setPosition(Vec2(0, 0));
	background1->getTexture()->setAliasTexParameters();

	auto background2 = Node::create();
	background2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto sky1 = Sprite::create("graphics/cloud.png");
	sky1->setPosition(Vec2(0, 0));
	sky1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sky1->getTexture()->setAliasTexParameters();

	auto sky2 = Sprite::create("graphics/cloud.png");
	sky2->setPosition(winSize.width, 0);
	sky2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sky2->getTexture()->setAliasTexParameters();

	background2->addChild(sky1);
	background2->addChild(sky2);
	//�_������
	background2->runAction(
		RepeatForever::create(
		Sequence::create(
		MoveTo::create(32, (Vec2(-winSize.width, 0))),
		Place::create(Vec2::ZERO),
		NULL
		)));

	//�w�i��
	this->addChild(background1);
	//�w�i�_
	this->addChild(background2);

	//�L�����z�u
	auto luk = Player::create();
	luk->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	luk->setPosition(Vec2(144, winSize.height / 2));
	luk->getTexture()->setAliasTexParameters();
	this->setPlayer(luk);
	this->addChild(luk);

	//���镔��
	auto map = TMXTiledMap::create("graphics/ground2.tmx");
	map->setPositionX(8);
	this->addChild(map);
	this->setTiledMap(map);

	//���C���[���o
	auto ground = map->getLayer("ground");

	//�}�b�v�̑傫��
	auto mapSize = map->getMapSize();
	for (int x = 0; x < mapSize.width; ++x)
	{
		for (int y = 0; y < mapSize.height; ++y)
		{
			auto coordinate = Vec2(x, y);

 			this->addPhysicsBody(ground, coordinate);
		}
	}


	//cocos2d::EventListenerKeyboard�^�̃|�C���^�ϐ�keyboardListener��錾���AEventListenerKeyboard::create����
	auto keyboardListener = EventListenerKeyboard::create();

	//�L�[�{�[�h�������ꂽ����stop�������֐��H
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{
			//���������ꂽ�L�[������������
			if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
			{
				leftFlag = true;

				if (rightFlag == true)
				{
					rightFlag = false;
				}
			}
			//�����ł͂Ȃ������A�L�[������������
			if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
			{
				rightFlag = true;
				if (leftFlag == true)
				{
					leftFlag = false;
				}
			}
			//���������ꂽ�L�[������������
			if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W) 
			{
				log("%d", getJumpFlag());
				if (getJumpFlag() == true)
				{
					_player->playAnimation(2);
					_player->getPhysicsBody()->applyImpulse(JUMP_IMPULSE);
					setJumpFlag(false);
				}
			}
			//�����ł͂Ȃ����������ꂽ�L�[������������
			else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == EventKeyboard::KeyCode::KEY_S)
			{
				//playAnimation(int index)�̕ϐ�index��2����
				//�������ւ̃A�j���[�V�������Đ�
				//_player->playAnimation(2);
			}
		};
	//���Ԃ�L�[�𗣂�������stop�@�ڍׂ킩��Ȃ�
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
		//�����������ꂽ�L�[�����A�܂��́���������
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			leftFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			rightFlag = false;
		}
		//�����������ꂽ�L�[�����A�܂��́���������
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			//playAnimation(int index)�̕ϐ�index��0����
			//�A�j���[�V������ҋ@�A���ړ��̂��֖̂߂�
			//_player->playAnimation(0);
			//_velocity.y = 0;

		}

	};

	//�L�[�{�[�h��stop����������̂��܂��Ȃ��݂����Ȃ��́B
	//(��������̃R�����g��蔲����)EventListener���Ă̂ɃL�[���������Ƃ��Ƃ��A�b�����Ƃ��݂�����stop��R�t���Ă�����
	//�Ō��EventDispatcher���Ă������̂ɁA�������EventListener��o�^���āA�u�悵�Ȃɂ��肢���܂��v���ē`����stop
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);



	// ��L�̒ʂ�A�j���[�V������������
	_player->playAnimation(0);

	this->scheduleUpdate();

	return true;

}