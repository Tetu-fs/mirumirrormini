#include "Stage.h"
USING_NS_CC;

const Vec2 JUMP_IMPULSE = Vec2(0, 600);
const int MAPCHIP_SIZE = 16;
const int BLOCK_SIZE = 16;

Stage::Stage()
	:_player(nullptr)
	, _tiledMap(nullptr)
	, _blocks(nullptr)
	, leftPressFlag(false)
	, rightPressFlag(false)
	, upPressFlag(false)
	, _jumpFlag(false)
	, goalFlag(false)

	, _magic(nullptr)
	, testX(0)
	, testY(0)
	
	, blockX(0)
	, blockY(0)
	, mapX(0)
	, mapY(0)
	, tileID(0)
	, rectX(0)
	, rectY(0)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_magic);
	CC_SAFE_RELEASE_NULL(_player);
	CC_SAFE_RELEASE_NULL(_tiledMap);
	CC_SAFE_RELEASE_NULL(_blocks);

}



void Stage::jumpMethod()
{
	_player->getPhysicsBody()->applyImpulse(JUMP_IMPULSE);
	setJumpFlag(false);
}



void Stage::playerMove()
{

	//cocos2d::EventListenerKeyboard型のポインタ変数keyboardListenerを宣言し、EventListenerKeyboard::createを代入
	auto keyboardListener = EventListenerKeyboard::create();

	//キーボードが押された時のstopを書く関数？
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{

		//もし押されたキーが←だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			leftPressFlag = true;
	

			_player->rightFlag = false;
			if (rightPressFlag == true)
			{
				rightPressFlag = false;
			}
		}
		//そうではなくもし、キーが→だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			rightPressFlag = true;
			_player->rightFlag = true;
	
			if (leftPressFlag == true)
			{
				leftPressFlag = false;
			}
		}
		//もし押されたキーが↑だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			upPressFlag = true;

		}
		//もし押されたキーが↑だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == EventKeyboard::KeyCode::KEY_S)
		{


		}
		//そうではなくもし押されたキーがスペースだったら
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (getJumpFlag() == true)
			{
				_player->magicLRFlag = true;

				Magic* sideMagic = _player->MirrorMethod();
				this->setMagic(sideMagic);
				_magic->setPosition(_player->magicPosition);
				this->addChild(_magic);
			}
		}
	
	};
	//たぶんキーを離した時のstop　詳細わかんない
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {

		//もしも離されたキーが←、または→だったら
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

	//キーボードのstopを書いた後のおまじないみたいなもの。
	//(ぎぎさんのコメントより抜粋→)EventListenerってのにキーを押したときとか、話したときみたいなstopを紐付けておいて
	//最後にEventDispatcherっていうものに、今作ったEventListenerを登録して、「よしなにお願いします」って伝えるstop
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}



Blocks* Stage::BlockGen(int gid)
{
	Blocks* blockGen = Blocks::create();
	blockGen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto material = PhysicsMaterial();
	//摩擦
	material.friction = 99;
	material.restitution = 0.0;
	if (gid != 2 && gid != 3 && gid !=5 && gid != 10)
	{
		Point box[4]{Point(-8, 0), Point(-8, 8), Point(8, 8), Point(8, 0)};
		auto physicsBody = PhysicsBody::createEdgePolygon(box, 4, material);
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(1);
		physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
		physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));

		blockGen->setPhysicsBody(physicsBody);
	}

	if (gid==10)
	{
		Point goal[4]{Point(-7, -8), Point(-7, -8), Point(7, -7), Point(7, -7)};
		auto goalPhysics = PhysicsBody::createEdgePolygon(goal, 4, material);
		goalPhysics->setDynamic(false);
		goalPhysics->setCategoryBitmask(2);
		goalPhysics->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
		goalPhysics->setCollisionBitmask(static_cast<int>(TileType::PLAYER));

		blockGen->setPhysicsBody(goalPhysics);
	}
	auto tileSize = Size(5, 4);

	const int X_MAX = tileSize.width;

	rectX = ((gid - 1) % X_MAX + 1) - 1;
	rectY = (int)((gid - 1) / X_MAX);

	blockGen->setTextureRect(Rect(rectX * BLOCK_SIZE, rectY * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));

	return blockGen;
}

bool Stage::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//剛体の接触チェック
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact&contact){

		//プレイヤーではない方を抽出
		auto floor = contact.getShapeA()->getBody() == _player->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
		auto floorBody = floor->getBody();
		//カテゴリ抽出
		auto category = floorBody->getCategoryBitmask();

		Rect floorRect = floorBody->getNode()->getBoundingBox();
		float floorTopY = floorRect.origin.y + floorRect.size.height;

		float minX = floorRect.origin.x;
		float midX = (floorRect.origin.x + floorRect.size.width / 2);
		float maxX = floorRect.origin.x + floorRect.size.width - 1.0;

		Rect playerRect = _player->getBoundingBox();
		float playerBottomY = playerRect.origin.y;
		float playerX = _player->getPosition().x;


		bool isContains = minX <= playerX && playerX <= maxX;

		auto checkDot = Sprite::create("graphics/white.png");
		checkDot->setPosition(minX, floorTopY);
		checkDot->setScale(2.0f);

		if (category & static_cast<int>(Stage::TileType::BLOCKS))
		{
			this->addChild(checkDot);

			if (floorTopY <= playerBottomY)
			{
				setJumpFlag(true);
			}

			if (isContains == true)
			{
				if (_player->rightFlag == true)
				{
					log("check X=%d", (int)_player->getPosition().x+8);

						_player->magicPosition = Vec2(maxX, 0);
				}
				else if (_player->rightFlag == false)
				{
					log("check X=%d", (int)_player->getPosition().x + 8);
						_player->magicPosition = Vec2(minX, 0);
				}

			}
		}

		if (category & static_cast<int>(Stage::TileType::GOAL))
		{
			if (isContains == true)
			{
				goalFlag = true;
			}
		}
		else
		{
			goalFlag = false;
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//画面サイズ取得
	Size winSize = Director::getInstance()->getWinSize();
	/*
	//背景表示
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
	//雲動かす
	background2->runAction(
		RepeatForever::create(
		Sequence::create(
		MoveTo::create(32, (Vec2(-winSize.width, 0))),
		Place::create(Vec2::ZERO),
		NULL
		)));

	//背景空
	this->addChild(background1);
	//背景雲
	this->addChild(background2);
	*/
	//乗れる部分
	auto map = TMXTiledMap::create("graphics/ground2.tmx");
	//map->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	
	//this->addChild(map);
	//this->setTiledMap(map);
	

	//レイヤー抽出
	TMXLayer* layer = map->getLayer("ground");

	//マップの大きさ
	auto mapSize = map->getMapSize();
	auto category = 1;


	for (int x = 0; x < mapSize.width; x++)
	{
		for (int y = 0; y < mapSize.height; y++)
		{
			auto coordinate = Vec2(x, y);
			int tileID = layer->getTileGIDAt(coordinate);
			auto tileMap = layer->getTileAt(coordinate);
	
			if (tileID >= 1){

				Blocks* blockGen = BlockGen(tileID);
				blockGen->setPosition(tileMap->getPosition());
				this->addChild(blockGen);

				tileMap->removeFromParent();

			}			
		}
	}

	//キャラ配置
	auto luk = Player::create();
	luk->setPosition(Vec2(0, 91));
	luk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	luk->getTexture()->setAliasTexParameters();
	this->setPlayer(luk);
	this->addChild(luk);
	_prevPosition = _player->getPosition();

	playerMove();

	testBlock = Sprite::create("graphics/white.png");
	testBlock->setScale(16.0f);
	testBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	this->addChild(testBlock);

	// 上記の通りアニメーションを初期化
	_player->playAnimation(0);

	this->scheduleUpdate();

	return true;

}
void Stage::testMethod()
{
	//log("%d", (int)_player->getPosition().x);

	testX = ((int)_player->getPosition().x) / 16;
	testY = ((int)_player->getPosition().y) / 16;
}

void Stage::update(float dt)
{

	//Vec2型の_velocityという変数を整数値に直す？
	_velocity.normalize();

	const float  SPEED = 1.5;
	//自身の位置を、現在地＋ベクトル＊SPEEDの値にする
	_player->setPosition(_player->getPosition() + _velocity * SPEED);

	_playerPosition = _player->getPosition();

	Size winSize = Director::getInstance()->getWinSize();
	if (_playerPosition.x >= winSize.width)
	{
		_player->setPositionX(winSize.width);
		_velocity.x = 0;
	}

	else if (_playerPosition.x <= 0)
	{
		_player->setPositionX(0);
		_velocity.x = 0;
	}

	if (_playerPosition.y >= winSize.height)
	{
		_player->setPositionY(winSize.height);
		_velocity.y = 0;
	}

	else if (_playerPosition.y <= 0)
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

	if (goalFlag == true)
	{
		if (_playerPosition.y == _prevPosition.y)
		{
			log("goal");
		}
		_prevPosition = _playerPosition;

	}

	if (getJumpFlag() == false)
	{

		if (_playerPosition.y > _prevPosition.y)
		{
			_player->playAnimation(2);
		}
		else if (_playerPosition.y < _prevPosition.y)
		{
			_player->playAnimation(3);
		}
		_prevPosition = _playerPosition;
	}


	if (_player->magicLRFlag == false)
	{
		this->removeChild(_magic);
	}


	testMethod();
	testVec = Vec2(testX * 16, testY * 16);
	testBlock->setPosition(testVec);

}