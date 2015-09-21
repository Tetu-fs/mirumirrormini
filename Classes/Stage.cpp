#include "Stage.h"
USING_NS_CC;

const Vec2 JUMP_IMPULSE = Vec2(0, 600);
const int MAPCHIP_SIZE = 16;
const float MAP_HEIGHT = 14;
Stage::Stage()
	:_player(nullptr)
	, _tiledMap(nullptr)
	, _blocks(nullptr)
	, leftPressFlag(false)
	, rightPressFlag(false)
	, upPressFlag(false)
	, _jumpFlag(false)
	, goalFlag(false)

	, _sideMagic(nullptr)
	, _upDownMagic(nullptr)
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
	CC_SAFE_RELEASE_NULL(_sideMagic);
	CC_SAFE_RELEASE_NULL(_upDownMagic);

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
	/*
	スペースキーを押すと　向いている方向に横反射
	Aを押すと左を向いて横反射
	Dを押すと右を向いて横反射
	Wを押すと上反射
	Sを押すと下反射
	*/
	//cocos2d::EventListenerKeyboard型のポインタ変数keyboardListenerを宣言し、EventListenerKeyboard::createを代入
	auto keyboardListener = EventListenerKeyboard::create();

	//キーボードが押された時のstopを書く関数？
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{

		auto flip = FlipX::create(true);
		auto flipback = FlipX::create(false);

		//もし押されたキーが←だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			leftPressFlag = true;

			_player->rightFlag = false;
			if (rightPressFlag == true)
			{
				rightPressFlag = false;
			}
		}
		//そうではなくもし、キーが→だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			rightPressFlag = true;
			_player->rightFlag = true;

			if (leftPressFlag == true)
			{
				leftPressFlag = false;
			}
		}
		//もし押されたキーが↑だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			upPressFlag = true;

		}
		//もし押されたキーが↑だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_S)
		{


		}
		//そうではなくもし押されたキーがスペースだったら
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (getJumpFlag() == true)
			{
				_player->magicFlag = true;

				Magic* sideMagic = _player->sideMirrorEffect();
				this->setSideMagic(sideMagic);
				_sideMagic->setPosition(_player->LRMagicPosition);
				this->addChild(_sideMagic);
			}

			for (Blocks* mirrorBlock : _mirrorAbleBlocks)
			{
				_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));


				std::vector<float> playerDiffPositions;
				float diffPosition;
				if (_player->magicFlag == true)
				{
					for (Vec2 mirrorPosition : _mirrorAblePositions)
					{
						if (_player->rightFlag == true)
						{

							if (_standBlockPosition.x < mirrorPosition.x)
							{
								diffPosition = mirrorPosition.x - _standBlockPosition.x;
								playerDiffPositions.push_back(diffPosition);
								float rightMirrorMove = StageVecConvertX(_standBlockPosition.x - diffPosition);
								MoveTo* rightMoveAction = MoveTo::create(0.2, Vec2(rightMirrorMove, mirrorBlock->getPosition().y));
								mirrorBlock->runAction(rightMoveAction);
								//log("Rcount = %f", _standBlockPosition.x - diffPosition * MAPCHIP_SIZE);
							}
						}
						else
						{
							if (_standBlockPosition.x > mirrorPosition.x)
							{
								diffPosition = _standBlockPosition.x - mirrorPosition.x;
								playerDiffPositions.push_back(diffPosition);
								float leftMirrorMove = StageVecConvertX(_standBlockPosition.x + diffPosition);
								MoveTo* leftMoveAction = MoveTo::create(0.2, Vec2(leftMirrorMove, mirrorBlock->getPosition().y));
								mirrorBlock->runAction(leftMoveAction);
								//log("Lcount = %f", diffPosition);
							}
						}
					}
				}
				_mirrorAblePositions.clear();
				playerDiffPositions.clear();
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			_player->upFlag = true;
			if (getJumpFlag() == true)
			{
				_player->magicFlag = true;
				Magic* upMagic = _player->upDownMirrorEffect();
				this->setUpDownMagic(upMagic);
				_upDownMagic->setPosition(_player->UDMagicPosition);
				this->addChild(_upDownMagic);
			}

			for (Blocks* mirrorBlock : _mirrorAbleBlocks)
			{
				_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));


				std::vector<float> playerDiffPositions;
				float diffPosition;
				if (_player->magicFlag == true)
				{
					for (Vec2 mirrorPosition : _mirrorAblePositions)
					{
							if (_standBlockPosition.y > mirrorPosition.y)
							{
								diffPosition = _standBlockPosition.y - mirrorPosition.y;
								//playerDiffPositions.push_back(diffPosition);
								float upMirrorMove = StageVecConvertY(_standBlockPosition.y + diffPosition);
								MoveTo* upMoveAction = MoveTo::create(0.2, Vec2(mirrorBlock->getPosition().x, upMirrorMove));
								mirrorBlock->runAction(upMoveAction);
								//log("Rcount = %f", upMirrorMove);
							}
						}

				}
				_mirrorAblePositions.clear();
				playerDiffPositions.clear();
			}
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			_player->upFlag = false;

			if (getJumpFlag() == true)
			{
				_player->magicFlag = true;
				Magic* upMagic = _player->upDownMirrorEffect();
				this->setUpDownMagic(upMagic);
				_upDownMagic->setPosition(_player->UDMagicPosition);
				this->addChild(_upDownMagic);
			}

			for (Blocks* mirrorBlock : _mirrorAbleBlocks)
			{
				_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));


				std::vector<float> playerDiffPositions;
				float diffPosition;
				if (_player->magicFlag == true)
				{
					for (Vec2 mirrorPosition : _mirrorAblePositions)
					{
						if (_standBlockPosition.y < mirrorPosition.y)
						{
							diffPosition = mirrorPosition.y - _standBlockPosition.y;
							playerDiffPositions.push_back(diffPosition);
							float downMirrorMove = StageVecConvertY(_standBlockPosition.y - diffPosition);
							MoveTo* downMoveAction = MoveTo::create(0.2, Vec2(mirrorBlock->getPosition().x, downMirrorMove));
							mirrorBlock->runAction(downMoveAction);
							//log("Rcount = %f", downMirrorMove);
						}
					}

				}
				_mirrorAblePositions.clear();
				playerDiffPositions.clear();
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			_player->rightFlag = false;
			_player->runAction(flip);

			if (getJumpFlag() == true)
			{
				_player->magicFlag = true;

				Magic* sideMagic = _player->sideMirrorEffect();
				this->setSideMagic(sideMagic);
				_sideMagic->setPosition(_player->LRMagicPosition);
				this->addChild(_sideMagic);
			}

			for (Blocks* mirrorBlock : _mirrorAbleBlocks)
			{
				_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));


				std::vector<float> playerDiffPositions;
				float diffPosition;
				if (_player->magicFlag == true)
				{
					for (Vec2 mirrorPosition : _mirrorAblePositions)
					{
						if (_player->rightFlag == false)
						{
							if (_standBlockPosition.x > mirrorPosition.x)
							{
								diffPosition = _standBlockPosition.x - mirrorPosition.x;
								playerDiffPositions.push_back(diffPosition);
								float leftMirrorMove = StageVecConvertX(_standBlockPosition.x + diffPosition);
								MoveTo* leftMoveAction = MoveTo::create(0.2, Vec2(leftMirrorMove, mirrorBlock->getPosition().y));
								mirrorBlock->runAction(leftMoveAction);
							}
						}

					}
				}
				_mirrorAblePositions.clear();
				playerDiffPositions.clear();
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			_player->rightFlag = true;
			_player->runAction(flipback);

			if (getJumpFlag() == true)
			{
				_player->magicFlag = true;

				Magic* sideMagic = _player->sideMirrorEffect();
				this->setSideMagic(sideMagic);
				_sideMagic->setPosition(_player->LRMagicPosition);
				this->addChild(_sideMagic);
			}

			for (Blocks* mirrorBlock : _mirrorAbleBlocks)
			{
				_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));


				std::vector<float> playerDiffPositions;
				float diffPosition;
				if (_player->magicFlag == true)
				{
					for (Vec2 mirrorPosition : _mirrorAblePositions)
					{
						if (_player->rightFlag == true)
						{
							if (_standBlockPosition.x < mirrorPosition.x)
							{
								diffPosition = mirrorPosition.x - _standBlockPosition.x;
								playerDiffPositions.push_back(diffPosition);
								float rightMirrorMove = StageVecConvertX(_standBlockPosition.x - diffPosition);
								MoveTo* rightMoveAction = MoveTo::create(0.2, Vec2(rightMirrorMove, mirrorBlock->getPosition().y));
								mirrorBlock->runAction(rightMoveAction);
								//log("Rcount = %f", _standBlockPosition.x - diffPosition * MAPCHIP_SIZE);
							}
						}

					}
				}
				_mirrorAblePositions.clear();
				playerDiffPositions.clear();
			}
		}

		
	};

	//たぶんキーを離した時のstop　詳細わかんない
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {

		//もしも離されたキーが←、または→だったら
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			leftPressFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			rightPressFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			upPressFlag = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			_player->magicFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			_player->magicFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			_player->magicFlag = false;
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
	//剛体マテリアル設定
	auto material = PhysicsMaterial();
	//摩擦
	material.friction = 99;
	material.restitution = 0.0;

	//剛体設置
	if (gid == 1 || gid == 4 || gid == 6 || gid == 7 || gid == 8 || gid == 9){
	auto category = 1;
	Point box[4]{Point(-8, 0), Point(-8, 8), Point(8, 8), Point(8, 0)};
	auto physicsBody = PhysicsBody::createEdgePolygon(box,4, material);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(category);
	physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
	physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
	blockGen->setPhysicsBody(physicsBody);
	}
	if (gid == 10)
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

	blockGen->setTextureRect(Rect(rectX * MAPCHIP_SIZE, rectY * MAPCHIP_SIZE, MAPCHIP_SIZE, MAPCHIP_SIZE));


	auto blockRect = blockGen->getBoundingBox();

	return blockGen;
}
Vec2 Stage::BlockVecConvert(Vec2 blockAncorVecs)
{
	Vec2 Position = blockAncorVecs;
	float x = floor((Position.x) / MAPCHIP_SIZE);
	float y = MAP_HEIGHT - floor((Position.y) / MAPCHIP_SIZE) - 1;
	//log("%f", Position.x);
	return  Vec2(x, y);
}
Vec2 Stage::StageVecConvert(Vec2 blockAncorVecs)
{
	Vec2 Position = blockAncorVecs;
	float x = floor((Position.x) * MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	float y = MAP_HEIGHT * MAPCHIP_SIZE - floor((Position.y + 1)* MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	//log("%f", Position.x);
	return  Vec2(x, y);
}
float Stage::StageVecConvertX(float blockAncorVecs)
{
	float Position = blockAncorVecs;
	float x = floor((Position) * MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	return  x;
}
float Stage::StageVecConvertY(float blockAncorVecs)
{
	float Position = blockAncorVecs;
	//Positionが9のとき
	//14 * 16 = 244
	//(9 + 1) * 16 + 8 = 168
	//244 - 168 = 76
	float y = MAP_HEIGHT * MAPCHIP_SIZE - floor((Position + 1)* MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	//log("check = %f", floor((Position + 1)* MAPCHIP_SIZE) + MAPCHIP_SIZE / 2);
	return  y;
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


	if (_player->magicFlag == false)
	{
		this->removeChild(_sideMagic);
		this->removeChild(_upDownMagic);

	}

	for (Vec2 point : _neighborBlockPositions)
	{
		Rect blockRect = Rect(point.x - 8, point.y - 8, MAPCHIP_SIZE, MAPCHIP_SIZE);
		if (_player->getPosition().x - blockRect.getMinX() > 0 && blockRect.getMaxX() - _player->getPosition().x > 0)
		{
			_standBlockPosition = BlockVecConvert(point);
			//log("BlockVec = %f", _standBlockPosition.x);
			testVec = Vec2(point);
			testBlock->setPosition(testVec);
			if (_player->rightFlag == true)
			{
				_player->LRMagicPosition = Vec2(blockRect.getMaxX(), -224);
			}
			else
			{
				_player->LRMagicPosition = Vec2(blockRect.getMinX(), -224);
			}

			if (_player->upFlag == true)
			{
				_player->UDMagicPosition = Vec2(-384, blockRect.getMinY());
			}
			else
			{
				_player->UDMagicPosition = Vec2(-384,blockRect.getMaxY());
			}
		}
	}
}
bool Stage::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//auto backGroundLayer = Stage::create();


	//剛体の接触チェック
	auto contactListener = EventListenerPhysicsContact::create();

	contactListener->onContactBegin = [this](PhysicsContact&contact){

		//プレイヤーではない方を抽出
		PhysicsShape* ground = contact.getShapeA()->getBody() == _player->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
		Node* groundNode = ground->getBody()->getNode();
		nowBlockPosition = groundNode->getPosition();
		Vec2 worldPos = groundNode->getParent()->convertToWorldSpace(groundNode->getPosition());
		//log("%f", worldPos.x);
		_neighborBlockPositions.push_back(nowBlockPosition);

		//カテゴリ抽出
		int category = groundNode->getPhysicsBody()->getCategoryBitmask();

		Rect groundRect = groundNode->getBoundingBox();
		float groundTopY = groundRect.origin.y + groundRect.size.height;

		float minX = groundRect.origin.x;
		float midX = (groundRect.origin.x + groundRect.size.width / 2);
		float maxX = groundRect.origin.x + groundRect.size.width - 1.0;
		Rect playerRect = _player->getBoundingBox();
		float playerBottomY = playerRect.origin.y;
		float playerX = _player->getPosition().x;

		bool isContains = minX <= playerX && playerX <= maxX;

		if (category & static_cast<int>(Stage::TileType::BLOCKS))
		{


			if (groundTopY <= playerBottomY)
			{
				setJumpFlag(true);
			}

		}

		if (category & static_cast<int>(Stage::TileType::GOAL))
		{
			goalFlag = true;
		}
		else
		{
			goalFlag = false;
		}
		return true;
	};
	contactListener->onContactSeperate = [this](PhysicsContact&contact){
		_neighborBlockPositions.erase(_neighborBlockPositions.begin());
		//log("std::_neighborBlockPositions.size = %d", _neighborBlockPositions.size());

	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//画面サイズ取得
	Size winSize = Director::getInstance()->getWinSize();
	
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
	
			if (tileID >= 1)
			{
				_blockGen = BlockGen(tileID);
				_blockGen->setPosition(tileMap->getPosition().x + 8, tileMap->getPosition().y + 8);
				this->addChild(_blockGen);
				if (tileID == 4)
				{
					_mirrorAbleBlocks.push_back(_blockGen);
				}

				tileMap->removeFromParent();
			}			
		}
	}

	//キャラ配置
	auto luk = Player::create();
	this->setPlayer(luk);
	luk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	luk->setPosition(Vec2(0, 64));
	luk->getTexture()->setAliasTexParameters();

	this->addChild(luk);
	_prevPosition = luk->getPosition();

	playerMove();



	testBlock = Sprite::create("graphics/white.png");
	testBlock->setScale((float)MAPCHIP_SIZE);
	testBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(testBlock);

	// 上記の通りアニメーションを初期化
	_player->playAnimation(0);

	this->scheduleUpdate();

	return true;

}

