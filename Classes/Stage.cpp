#include "Stage.h"
USING_NS_CC;

const Vec2 JUMP_IMPULSE = Vec2(0, 600);

Stage::Stage()
	:_player(nullptr)
	, _tiledMap(nullptr)
	, leftPressFlag(false)
	, rightPressFlag(false)
	, upPressFlag(false)
	, _jumpFlag(false)
	, _magic(nullptr)
	, magicCount(0)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_magic);
	CC_SAFE_RELEASE_NULL(_player);
	CC_SAFE_RELEASE_NULL(_tiledMap);
}


Sprite* Stage::addPhysicsBody(TMXLayer *layer, Vec2 &coordinate)
{
	int checkSlope = layer->getTileGIDAt(coordinate);
	int checkflower = layer->getTileGIDAt(coordinate);
	int checkmirror = layer->getTileGIDAt(coordinate);
	//�X�v���C�g�𒊏o
	auto mapSprite = layer->getTileAt(coordinate);
	Point box[4]{Point(-8, 0), Point(-8, 8), Point(8, 8), Point(8, 0)};

	Point slope1[3]{Point(-8, -8), Point(8, -8), Point(8, 8)};
	Point slope2[3]{Point(-8, -8), Point(8, -8), Point(-8, 8)};

	if (mapSprite)
	{

		auto gid = layer->getTileGIDAt(coordinate);

		if (gid == 1 || gid == 4 || gid == 6 || gid == 7 || gid == 8 || gid == 9)
		{

			//���̃}�e���A���ݒ�
			auto material = PhysicsMaterial();
			//���C
			material.friction = 99;
			material.restitution = 0.0;

			//���̐ݒu

			auto category = 1;

			if (checkmirror != 4 && checkSlope != 6 && checkSlope != 9 && checkflower != 2 && checkflower != 3)
			{
				auto physicsBody = PhysicsBody::createEdgePolygon(box, 4, material);

				//���̌Œ�
				physicsBody->setDynamic(false);


				physicsBody->setCategoryBitmask(category);
				physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody);
			}
			if (checkmirror == 4)
			{
				auto physicsBody4 = PhysicsBody::createEdgePolygon(box, 4, material);

				_mirrorPosition = physicsBody4->getPosition();
				log("%d", _mirrorPosition.x);
				//���̌Œ�
				physicsBody4->setDynamic(false);

				physicsBody4->setCategoryBitmask(category);
				physicsBody4->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody4->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody4);

			}
			 if (checkSlope == 6)
			{
				auto physicsBody2 = PhysicsBody::createEdgePolygon(slope1, 3, material);

				//���̌Œ�
				physicsBody2->setDynamic(false);


				physicsBody2->setCategoryBitmask(category);
				physicsBody2->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
				physicsBody2->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
				mapSprite->setPhysicsBody(physicsBody2);

			}


			else if (checkSlope == 9)
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

void Stage::jumpMethod()
{
	_player->getPhysicsBody()->applyImpulse(JUMP_IMPULSE);
	setJumpFlag(false);
}



void Stage::playerMove()
{

	//cocos2d::EventListenerKeyboard�^�̃|�C���^�ϐ�keyboardListener��錾���AEventListenerKeyboard::create����
	auto keyboardListener = EventListenerKeyboard::create();

	//�L�[�{�[�h�������ꂽ����stop�������֐��H
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{

		//���������ꂽ�L�[������������
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
		{

			leftPressFlag = true;
			_player->rightFlag = false;
			if (rightPressFlag == true)
			{
				rightPressFlag = false;
			}
		}
		//�����ł͂Ȃ������A�L�[������������
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			rightPressFlag = true;
			_player->rightFlag = true;

			if (leftPressFlag == true)
			{
				leftPressFlag = false;
			}
		}
		//���������ꂽ�L�[������������
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			upPressFlag = true;

		}
		//�����ł͂Ȃ����������ꂽ�L�[���X�y�[�X��������
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			_player->magicLRFlag = true;

				Magic* sideMagic = _player->MirrorMethod();
				this->setMagic(sideMagic);
				this->addChild(_magic);

		}
	
	};
	//���Ԃ�L�[�𗣂�������stop�@�ڍׂ킩��Ȃ�
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {

		//�����������ꂽ�L�[�����A�܂��́���������
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			leftPressFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			rightPressFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			upPressFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			_player->magicLRFlag = false;
		}
	};

	//�L�[�{�[�h��stop����������̂��܂��Ȃ��݂����Ȃ��́B
	//(��������̃R�����g��蔲����)EventListener���Ă̂ɃL�[���������Ƃ��Ƃ��A�b�����Ƃ��݂�����stop��R�t���Ă�����
	//�Ō��EventDispatcher���Ă������̂ɁA�������EventListener��o�^���āA�u�悵�Ȃɂ��肢���܂��v���ē`����stop
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


}




void Stage::update(float dt)
{

	//Vec2�^��_velocity�Ƃ����ϐ��𐮐��l�ɒ����H
	_velocity.normalize();

	const float  SPEED = 1.5;
	//���g�̈ʒu���A���ݒn�{�x�N�g����SPEED�̒l�ɂ���
	_player->setPosition(_player->getPosition() + _velocity * SPEED);

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


	if (leftPressFlag == true)
	{
		if (getJumpFlag() == true)
		{
			_player->playAnimation(1);
		}

		_player->runAction(flip);
		_velocity.x = -2;
	}

	if (rightPressFlag == true)
	{
		if (getJumpFlag() == true)		{
			_player->playAnimation(1);
		}

		_player->runAction(flipback);
		_velocity.x = 2;
	}

	if (leftPressFlag == false && rightPressFlag == false)
	{
		if (getJumpFlag() == true)
		{
			_player->playAnimation(0);
		}

		_velocity.x = 0;
	}

	if (upPressFlag == true)
	{
		if (getJumpFlag() == true)
		{
			jumpMethod();
		}
	}
	
	if (getJumpFlag() == false)
	{

		if (playerPosition.y > _prevPosition.y)
		{
			_player->playAnimation(2);
		}
		else if (playerPosition.y < _prevPosition.y)
		{
			_player->playAnimation(3);
		}
		_prevPosition = playerPosition;
	}


	if (_player->magicLRFlag == false)
	{
		this->removeChild(_magic);
	}
}

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
	luk->setPosition(Vec2(64, winSize.height / 2));
	luk->getTexture()->setAliasTexParameters();
	this->setPlayer(luk);
	this->addChild(luk);
	_prevPosition = _player->getPosition();

	playerMove();

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


	// ��L�̒ʂ�A�j���[�V������������
	_player->playAnimation(0);


	this->scheduleUpdate();

	return true;

}