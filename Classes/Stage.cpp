#include "Stage.h"
#include "MainScene.h"
#include "TitleScene.h"
#include"AudioManager.h"

USING_NS_CC;
//using namespace experimental;

const Vec2 JUMP_IMPULSE = Vec2(0, 260);
const int MAPCHIP_SIZE = 16;
const float MAP_HEIGHT = 14;
const int MAX_LEVEL = 9;

const char* STAGE_FILE = "graphics/stage%d.tmx";

Stage::Stage()
	: _tiledMap(nullptr)
	, _player(nullptr)
	, _blocks(nullptr)
	, leftPressFlag(false)
	, rightPressFlag(false)
	, upPressFlag(false)
	, _jumpFlag(false)
	, goalFlag(false)
	, magicUse(true)
	, moveFlag(false)
	, wallFlag(false)

	, _state(GameState::PLAYING)

	, _sideMagic(nullptr)
	, _upDownMagic(nullptr)

	, _lastKeyCode(EventKeyboard::KeyCode::KEY_NONE)

	, playerX(0)
	, playerY(0)

	, _level(0)

	, blockX(0)
	, blockY(0)
	, mapX(0)
	, mapY(0)
	, tileID(0)
	, rectX(0)
	, rectY(0)
	, nextLevel(0)
	, mirrorMove(0)
	, mainBgmID(0)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_sideMagic);
	CC_SAFE_RELEASE_NULL(_upDownMagic);

	CC_SAFE_RELEASE_NULL(_player);
	CC_SAFE_RELEASE_NULL(_tiledMap);
	CC_SAFE_RELEASE_NULL(_blocks);

	_mirrorAblePositions.clear();
}

Stage* Stage::createWithLevel(int level)
{
	Stage* ret = new Stage();
	if (ret->initWithLevel(level))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}


//ジャンプメソッド
void Stage::jumpMethod()
{
	_player->getPhysicsBody()->applyImpulse(JUMP_IMPULSE);
	_player->getPhysicsBody()->setGravityEnable(true);

	setJumpFlag(false);
}

//横反射の挙動
void Stage::moveBlockX(Blocks* mirrorBlock, Vec2 mirrorPosition)
{
	std::vector<float> playerDiffPositions;
	float diffPosition = mirrorPosition.x - _standBlockPosition.x;
	playerDiffPositions.push_back(diffPosition);

	mirrorMove = StageVecConvertX(_standBlockPosition.x - diffPosition);
	MoveTo* moveAction = MoveTo::create(0.2, Vec2(mirrorMove, mirrorBlock->getPosition().y));
	
	if (_player->rightFlag == true && _standBlockPosition.x < mirrorPosition.x)
	{
		mirrorBlock->runAction(moveAction);
		//_neighborBlocks.eraseObject(mirrorBlock);

		//mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::AIR));
	}
	else if (_player->rightFlag == false && _standBlockPosition.x > mirrorPosition.x)
	{
		mirrorBlock->runAction(moveAction);
		//_neighborBlocks.eraseObject(mirrorBlock);

		//mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::AIR));
	}


	log("size = %d", _neighborBlocks.size());

	this->scheduleOnce([this](float dt)
	{
		for (Blocks* mirrorBlock : _mirrorAbleBlocks)
		{
			if (mirrorBlock->getPhysicsBody()->getCategoryBitmask() != static_cast<int>(TileType::BLOCKS)){
				mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::BLOCKS));
			}
		}
	}, 0.2, "key");
	playerDiffPositions.clear();
}

//縦反射の挙動
void Stage::moveBlockY(Blocks* mirrorBlock, Vec2 mirrorPosition)
{
	std::vector<float> playerDiffPositions;
	float diffPosition = mirrorPosition.y - _standBlockPosition.y;
	playerDiffPositions.push_back(diffPosition);

	mirrorMove = StageVecConvertY(_standBlockPosition.y - diffPosition);
	MoveTo* moveAction = MoveTo::create(0.2, Vec2(mirrorBlock->getPosition().x, mirrorMove));

	if (_player->upFlag == true)
	{
		if (_standBlockPosition.y > mirrorPosition.y)
		{
			mirrorBlock->runAction(moveAction);
			//_neighborBlocks.eraseObject(mirrorBlock);
			//mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::AIR));
		}
	}
	else if(_player->downFlag == true)
	{
		if (_standBlockPosition.y < mirrorPosition.y)
		{
			mirrorBlock->runAction(moveAction);
			//_neighborBlocks.eraseObject(mirrorBlock);

			//mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::AIR));
		}
	}

	this->scheduleOnce([this](float dt)
	{
		for (Blocks* mirrorBlock : _mirrorAbleBlocks)
		{
			mirrorBlock->getPhysicsBody()->setCategoryBitmask(static_cast<int>(TileType::BLOCKS));
		}
	}, 0.2, "key");
	playerDiffPositions.clear();
}

Magic* Stage::upDownMirrorEffect()
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();

	magic = Magic::create();
	magic->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	magic->setScaleX(0.25);
	ScaleTo* whiteUpScale = ScaleTo::create(0.2, 1, 1);
	ScaleTo* whiteDownScale = ScaleTo::create(0.2, 1, 1);
	MoveTo* goUp = MoveTo::create(0.2, Vec2(winSize.width/2, winSize.height));
	MoveTo* goDown = MoveTo::create(0.2, Vec2(winSize.width / 2, -winSize.height));
	Sequence* upMagic = Sequence::create(whiteUpScale, goUp, RemoveSelf::create(), NULL);
	Sequence* downMagic = Sequence::create(whiteDownScale, goDown, RemoveSelf::create(), NULL);
	

	if (_player->upFlag == true)
	{
		magic->setRotation(-90);
		magic->runAction(upMagic);

	}

	else
	{
		magic->setRotation(90);

		magic->runAction(downMagic);

	}

	return magic;
}

Magic* Stage::sideMirrorEffect()
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	magic = Magic::create();

	auto flip = FlipX::create(true);
	auto flipback = FlipX::create(false);
	ScaleTo* whiteScale = ScaleTo::create(0.2, 2, 1);
	MoveTo* goRight = MoveTo::create(0.2, Vec2(winSize.width, 0));
	MoveTo* goLeft = MoveTo::create(0.2, Vec2(-winSize.width, 0));
	Sequence* RightMagic = Sequence::create(flipback, whiteScale, goRight, RemoveSelf::create(), NULL);
	Sequence* LeftMagic = Sequence::create(flip, whiteScale, goLeft, RemoveSelf::create(), NULL);

	if (_player->rightFlag == true)
	{
		magic->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		magic->runAction(RightMagic);

	}

	else
	{
		magic->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

		magic->runAction(LeftMagic);

	}

	return magic;
}

//プレイヤーの操作
void Stage::playerMove()
{
	/*
	*スペースキーを押すと　向いている方向に横反射
	*Aを押すと左を向いて横反射
	*Dを押すと右を向いて横反射
	*Wを押すと上反射
	*Sを押すと下反射
	*/
	//cocos2d::EventListenerKeyboard型のポインタ変数keyboardListenerを宣言し、EventListenerKeyboard::createを代入
	auto keyboardListener = EventListenerKeyboard::create();

	//キーボードが押された時のstopを書く関数？
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{

		if (keyCode == EventKeyboard::KeyCode::KEY_R)
		{
			auto mainScene = MainScene::createSceneWithLevel(_level);
			auto mainTransition = TransitionFade::create(1.0f, mainScene);
			Director::getInstance()->replaceScene(mainTransition);
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			experimental::AudioEngine::stop(mainBgmID);

			auto titleScene = TitleScene::createScene();
			auto titleTransition = TransitionFade::create(1.0f, titleScene);
			Director::getInstance()->replaceScene(titleTransition);
		}

		auto flip = FlipX::create(true);
		auto flipback = FlipX::create(false);

		//もし押されたキーがスペースだったら
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (_state == GameState::PLAYING)
			{
				if (magicUse == true && moveFlag == false)
				{
					if (getJumpFlag() == true)
					{
						_player->magicFlag = true;
						Magic* sideMagic = sideMirrorEffect();
						this->setSideMagic(sideMagic);
						_sideMagic->setPosition(_player->LRMagicPosition);
						this->addChild(_sideMagic);
					}

					for (Blocks* mirrorBlock : _mirrorAbleBlocks)
					{
						_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));

						if (_player->magicFlag == true)
						{
							for (Vec2 mirrorPosition : _mirrorAblePositions)
							{
								moveBlockX(mirrorBlock, mirrorPosition);
							}
							//auto it = std::remove_if(_neighborBlocks.begin(), _neighborBlocks.end(), [mirrorBlock](Blocks* blocks){
							//	return blocks == mirrorBlock;
							//});
							//_neighborBlocks.erase(it, _neighborBlocks.end());

						}
						_mirrorAblePositions.clear();
					}
				}
			}
			else if (_state == GameState::RESULT)
			{
				if (_level < MAX_LEVEL)
				{
					nextLevel = _level + 1;
					auto mainScene = MainScene::createSceneWithLevel(nextLevel);
					auto mainTransition = TransitionFade::create(1.0f, mainScene);
					Director::getInstance()->replaceScene(mainTransition);
				}

				else
				{
					experimental::AudioEngine::stop(mainBgmID);

					auto titleScene = TitleScene::createScene();
					auto titleTransition = TransitionFade::create(1.0f, titleScene);
					Director::getInstance()->replaceScene(titleTransition);
				}
			}
		}

		if (_state == GameState::PLAYING)
		{
			//もし押されたキーが←だったら
			if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				leftPressFlag = true;
				_player->rightFlag = false;
				moveFlag = true;
				_lastKeyCode = keyCode;
			}
			//そうではなくもし、キーが→だったら
			if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				rightPressFlag = true;
				_player->rightFlag = true;
				moveFlag = true;
				_lastKeyCode = keyCode;
			}
			//もし押されたキーが↑だったら
			if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
			{
				upPressFlag = true;
				moveFlag = true;
			}
			if (magicUse == false)
			{
				this->scheduleOnce([this](float dt)
				{
					magicUse = true;
				}, 0.2, "key");


			}
			if (magicUse == true && moveFlag == false)
			{
				//上反射
				if (keyCode == EventKeyboard::KeyCode::KEY_W)
				{
					_player->upFlag = true;
					if (_player->downFlag == true){ _player->downFlag = false; }
					if (getJumpFlag() == true)
					{
						_player->magicFlag = true;
						Magic* upMagic = upDownMirrorEffect();
						this->setUpDownMagic(upMagic);
						_upDownMagic->setPosition(_player->UDMagicPosition);
						this->addChild(_upDownMagic);
					}

					for (Blocks* mirrorBlock : _mirrorAbleBlocks)
					{
						_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));

						if (_player->magicFlag == true)
						{
							for (Vec2 mirrorPosition : _mirrorAblePositions)
							{
								moveBlockY(mirrorBlock, mirrorPosition);
							}
							//auto it = std::remove_if(_neighborBlocks.begin(), _neighborBlocks.end(), [mirrorBlock](Blocks* blocks){
							//	return blocks == mirrorBlock;
							//});
							//_neighborBlocks.erase(it, _neighborBlocks.end());

						}
						_mirrorAblePositions.clear();
					}
				}

				//下反射
				if (keyCode == EventKeyboard::KeyCode::KEY_S)
				{
					_player->downFlag = true;
					if (_player->upFlag == true){ _player->upFlag = false; }

					if (getJumpFlag() == true)
					{
						_player->magicFlag = true;
						Magic* upMagic = upDownMirrorEffect();
						this->setUpDownMagic(upMagic);
						_upDownMagic->setPosition(_player->UDMagicPosition);
						this->addChild(_upDownMagic);
					}

					for (Blocks* mirrorBlock : _mirrorAbleBlocks)
					{
						_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));

						if (_player->magicFlag == true)
						{
							for (Vec2 mirrorPosition : _mirrorAblePositions)
							{
								if (_standBlockPosition.y < mirrorPosition.y)
								{
									moveBlockY(mirrorBlock, mirrorPosition);
								}
								//auto it = std::remove_if(_neighborBlocks.begin(), _neighborBlocks.end(), [mirrorBlock](Blocks* blocks){
								//	return blocks == mirrorBlock;
								//});
								//_neighborBlocks.erase(it, _neighborBlocks.end());

							}

						}
						_mirrorAblePositions.clear();
					}
				}

				//左反射
				if (keyCode == EventKeyboard::KeyCode::KEY_A)
				{
					_player->rightFlag = false;

					if (getJumpFlag() == true)
					{
						_player->magicFlag = true;

						Magic* sideMagic = sideMirrorEffect();
						this->setSideMagic(sideMagic);
						_sideMagic->setPosition(_player->LRMagicPosition);

						this->addChild(_sideMagic);
					}

					for (Blocks* mirrorBlock : _mirrorAbleBlocks)
					{
						_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));

						if (_player->magicFlag == true)
						{
							for (Vec2 mirrorPosition : _mirrorAblePositions)
							{
								moveBlockX(mirrorBlock, mirrorPosition);
							}
							//auto it = std::remove_if(_neighborBlocks.begin(), _neighborBlocks.end(), [mirrorBlock](Blocks* blocks){
							//	return blocks == mirrorBlock;
							//});
							//_neighborBlocks.erase(it, _neighborBlocks.end());

						}
						_mirrorAblePositions.clear();
					}
				}


				//右反射
				if (keyCode == EventKeyboard::KeyCode::KEY_D)
				{
					_player->rightFlag = true;

					if (getJumpFlag() == true)
					{
						_player->magicFlag = true;

						Magic* sideMagic = sideMirrorEffect();
						this->setSideMagic(sideMagic);
						_sideMagic->setPosition(_player->LRMagicPosition);

						this->addChild(_sideMagic);

					}

					for (Blocks* mirrorBlock : _mirrorAbleBlocks)
					{
						_mirrorAblePositions.push_back(BlockVecConvert(mirrorBlock->getPosition()));

						if (_player->magicFlag == true)
						{
							for (Vec2 mirrorPosition : _mirrorAblePositions)
							{
								moveBlockX(mirrorBlock, mirrorPosition);
							}

						}
						_mirrorAblePositions.clear();
					}
				}
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

		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE || keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D)
		{
			_player->magicFlag = false;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			_player->magicFlag = false;
			_player->upFlag = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			_player->magicFlag = false;
			_player->downFlag = false;
		}
	};

	//キーボードのstopを書いた後のおまじないみたいなもの。
	//(ぎぎさんのコメントより抜粋→)EventListenerってのにキーを押したときとか、話したときみたいなstopを紐付けておいて
	//最後にEventDispatcherっていうものに、今作ったEventListenerを登録して、「よしなにお願いします」って伝えるstop
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

//ブロックの生成メソッド
Blocks* Stage::BlockGen(int gid)
{
	Blocks* blockGen = Blocks::create();
	blockGen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//剛体マテリアル設定
	auto material = PhysicsMaterial();
	//摩擦
	material.friction = 1.0f;
	material.restitution = 0.0f;

	//剛体設置
	if (gid == 1 || gid == 4 || gid == 6 || gid == 7 || gid == 8 || gid == 9 || gid == 12 || gid == 13){
	auto category = 1;
	cocos2d::Point box[4]{cocos2d::Point(-8, -8), cocos2d::Point(-8, 8), cocos2d::Point(8, 8), cocos2d::Point(8, -8)};
	auto physicsBody = PhysicsBody::createBox(Size(16,16));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(category);
	physicsBody->setCollisionBitmask(static_cast<int>(TileType::NONE));
	physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));

	blockGen->setPhysicsBody(physicsBody);
	_allBlocks.pushBack(blockGen);
	}
	auto tileSize = cocos2d::Size(5, 4);
	const int X_MAX = tileSize.width;

	rectX = ((gid - 1) % X_MAX + 1) - 1;
	rectY = (int)((gid - 1) / X_MAX);

	blockGen->setTextureRect(cocos2d::Rect(rectX * MAPCHIP_SIZE, rectY * MAPCHIP_SIZE, MAPCHIP_SIZE, MAPCHIP_SIZE));

	auto blockRect = blockGen->getBoundingBox();

	return blockGen;
}

//ステージ座標をマップ座標に変換
Vec2 Stage::BlockVecConvert(Vec2 blockAncorVecs)
{
	Vec2 Position = blockAncorVecs;
	float x = floor((Position.x) / MAPCHIP_SIZE);
	float y = MAP_HEIGHT - floor((Position.y) / MAPCHIP_SIZE) - 1;
	return  Vec2(x, y);
}

//マップ座標をステージ座標に変換
Vec2 Stage::StageVecConvert(Vec2 blockAncorVecs)
{
	Vec2 Position = blockAncorVecs;
	float x = floor((Position.x) * MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	float y = MAP_HEIGHT * MAPCHIP_SIZE - floor((Position.y + 1)* MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
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
	float y = MAP_HEIGHT * MAPCHIP_SIZE - floor((Position + 1)* MAPCHIP_SIZE) + MAPCHIP_SIZE / 2;
	return  y;
}

//BGM再生とロード
void Stage::onEnterTransitionDidFinish()
{
	if (Music::mainMusicID < 2)
	{
		mainBgmID = experimental::AudioEngine::play2d("sounds/main_bgm.mp3", true, 0.8f);
		Music::mainMusicID = mainBgmID;
		//log("playBgm %d", mainBgmID);

	}
	if (Music::mainMusicID != mainBgmID)
	{
		mainBgmID = Music::mainMusicID;
		//log("noBgm %d", mainBgmID);
	}
	else
	{
		Music::mainMusicID = mainBgmID;

	}
	Layer::onEnterTransitionDidFinish();
	experimental::AudioEngine::setVolume(Music::mainMusicID, 0.8f);


	this->scheduleUpdate();
}

void Stage::checkStop()
{

}

//毎フレーム更新
void Stage::update(float dt)
{
	//Vec2型の_velocityという変数を整数値に直す？
	_velocity.normalize();

	const float  SPEED = 1.5;
	//自身の位置を、現在地＋ベクトル＊SPEEDの値にする
	_player->setPosition(_player->getPosition() + _velocity * SPEED);

	_playerPosition = _player->getPosition();

	cocos2d::Size winSize = Director::getInstance()->getWinSize();


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
	}

	else if (_playerPosition.y <= -16)
	{
		_player->setPositionY(0);
		_state = GameState::GAMEOVER;
	}
	if (_state == GameState::PLAYING)
	{
		auto flip = FlipX::create(true);
		auto flipback = FlipX::create(false);
		//左右の向き
		if (_player->rightFlag == true)
		{
			_player->runAction(flipback);
		}
		else
		{
			_player->runAction(flip);
		}

		//左右移動
		if (rightPressFlag == true && leftPressFlag == true)
		{
			if (_lastKeyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				_player->rightFlag = false;
				if (getJumpFlag() == true)
				{
					_player->playAnimation(1);
				}
				_velocity.x = -2;
			}
			else
			{
				_player->rightFlag = true;

				if (getJumpFlag() == true)
				{
					_player->playAnimation(1);
				}
				_velocity.x = 2;
			}
		}
		else if (leftPressFlag == true)
		{
			_player->rightFlag = false;

			if (getJumpFlag() == true)
			{
				_player->playAnimation(1);
			}
			_velocity.x = -2;
		}
		else if (rightPressFlag == true)
		{
			_player->rightFlag = true;

			if (getJumpFlag() == true)
			{
				_player->playAnimation(1);
			}
			_velocity.x = 2;
		}

		//反射魔法や待機モーションの切り替え
		if (leftPressFlag == false && rightPressFlag == false && upPressFlag == false)
		{
			moveFlag = false;
			if (getJumpFlag() == true)
			{
				if (magicUse == true)
				{
					if (_player->magicFlag == true)
					{
						experimental::AudioEngine::play2d("sounds/se_mirror.mp3", false, 0.8f);
						if (_player->upFlag == true)
						{
							_player->playAnimation(5);
						}
						else if (_player->downFlag == true)
						{
							_player->playAnimation(6);
						}
						else
						{
							_player->playAnimation(4);
						}
						magicUse = false;
					}

				}
				_player->playAnimation(0);
			}
		}
		//左右移動していない時に動きを止める
		if (leftPressFlag == false && rightPressFlag == false)
		{
			_velocity.x = 0;
		}


		playerX = BlockVecConvert(_player->getPosition()).x;
		playerY = BlockVecConvert(_player->getPosition()).y;
		playerMapVec = Vec2(playerX, playerY);
		cocos2d::Rect playerRect = Rect(_player->getPosition().x - 4, _player->getPosition().y - 12, 8, 12);

		//着地時に落下を止める
		for (Blocks* block : _allBlocks)
		{
			cocos2d::Rect blockRect = Rect(block->getPosition().x - MAPCHIP_SIZE / 2, block->getPosition().y - MAPCHIP_SIZE / 2, MAPCHIP_SIZE, MAPCHIP_SIZE);
			Vec2 convertBlockMapVec = BlockVecConvert(block->getPosition());


			//if (playerMapVec.x == convertBlockMapVec.x){
			if ((int)blockRect.getMinX() - (int)playerRect.getMaxX() <= 0 && (int)blockRect.getMaxX() - (int)playerRect.getMinX() >= 0){

				if (playerMapVec.y + 1 == convertBlockMapVec.y){

					if ((int)playerRect.getMinY() - (int)blockRect.getMaxY() <= 0
						&& (int)playerRect.getMinY() - (int)blockRect.getMaxY() >= -1){
						if (_prevPosition.y > _player->getPositionY()){


							_standBlockPosition = BlockVecConvert(block->getPosition());
							setJumpFlag(true);
							_player->getPhysicsBody()->setGravityEnable(false);
							_player->getPhysicsBody()->setVelocity(Vec2(0, 0));
							_player->setPositionY(block->getPositionY() + 20);
							break;
						}
					}
				}
			}

			else{
				//地面がなければ落とす
				setJumpFlag(false);
				_player->getPhysicsBody()->setGravityEnable(true);

			}

		}

		//横にぶつかったら止める
		for (Blocks* block : _allBlocks)
		{
			cocos2d::Rect blockRect = block->getBoundingBox();
			Vec2 convertBlockMapVec = BlockVecConvert(block->getPosition());
			if (playerMapVec.y == convertBlockMapVec.y){
				//右
				if (playerMapVec.x + 1 == convertBlockMapVec.x){
					if ((int)playerRect.getMaxX() - (int)blockRect.getMinX() >= 0
						&& (int)playerRect.getMaxX() - (int)blockRect.getMinX() <= 1)
					{
						Sprite* sprite = Sprite::create();
						sprite->setTextureRect(Rect(0, 0, 10, 12));
						sprite->setColor(Color3B::WHITE);
						sprite->setPosition(Vec2(_player->getPosition().x, _player->getPosition().y));
						this->addChild(sprite);
						testBlock->setPosition(block->getPosition());
						_player->stopR = true;
					}

					break;
				}
				//左
				else if (playerMapVec.x - 1 == convertBlockMapVec.x){
					if ((int)playerRect.getMinX() - (int)blockRect.getMaxX() <= 0
						&& (int)playerRect.getMinX() - (int)blockRect.getMaxX() >= -1)
					{
						_player->stopL = true;
					}

					break;
				}
				//どちらでもない
				else{
					if (_velocity.x < 0)
					{
						_player->stopR = false;
					}
					else if (_velocity.x > 0)
					{
						_player->stopL = false;
					}
				}
			}
			else{
				_player->stopR = false;
				_player->stopL = false;
			}
		}
		//上昇落下中の左右停止処理
		for (Blocks* block : _allBlocks)
		{
			cocos2d::Rect blockRect = block->getBoundingBox();
			Vec2 convertBlockMapVec = BlockVecConvert(block->getPosition());
			if (getJumpFlag() == false){
				if (playerMapVec.y - 1 == convertBlockMapVec.y
					|| playerMapVec.y + 1 == convertBlockMapVec.y){
					//右
					if (playerMapVec.x + 1 == convertBlockMapVec.x){
						if ((int)playerRect.getMaxX() - (int)blockRect.getMinX() >= 0
							&& (int)playerRect.getMaxX() - (int)blockRect.getMinX() <= 1){
							//testBlock->setPosition(block->getPosition());
							_player->stopR = true;
						}

						break;
					}
					//左
					if (playerMapVec.x - 1 == convertBlockMapVec.x){
						if ((int)playerRect.getMinX() - (int)blockRect.getMaxX() <= 0
							&& (int)playerRect.getMinX() - (int)blockRect.getMaxX() >= -1){
							_player->stopL = true;
						}

						break;
					}
					//どちらでもない
					else{
						if (_velocity.x < 0)
						{
							_player->stopR = false;
						}
						else if (_velocity.x > 0)
						{
							_player->stopL = false;
						}
					}
				}
			}
		}
		//壁に衝突した際に動きを止める
		if (_player->stopL == true)
		{
			if (_velocity.x < 0)
			{
				_velocity.x = 0;
			}
		}
		else if (_player->stopR == true)
		{

			if (_velocity.x > 0)
			{
				_velocity.x = 0;
			}
		}
		//ジャンプ
		if (getJumpFlag() == true)
		{
			if (upPressFlag == true)
			{
				jumpMethod();
			}
		}
		//反射魔法のエフェクト範囲
		if (getJumpFlag() == true && wallFlag == false)
		{


			for (Blocks* point : _neighborBlocks)
			{
				cocos2d::Rect playerRect = _player->getBoundingBox();
				cocos2d::Rect blockRect = Rect(point->getPosition().x - MAPCHIP_SIZE / 2, point->getPosition().y - MAPCHIP_SIZE / 2, MAPCHIP_SIZE, MAPCHIP_SIZE);


				if (_player->getPosition().x - blockRect.getMinX() > 0 && blockRect.getMaxX() - _player->getPosition().x > 0)
				{
					_standBlockPosition = BlockVecConvert(point->getPosition());
										
					if (_player->rightFlag == true)
					{
						_player->LRMagicPosition = Vec2(blockRect.getMaxX(), 0);
					}
					else
					{
						_player->LRMagicPosition = Vec2(blockRect.getMinX(), 0);
					}

					if (_player->upFlag == true)
					{
						_player->UDMagicPosition = Vec2(winSize.width/2, blockRect.getMaxY());
					}
					else
					{
						_player->UDMagicPosition = Vec2(winSize.width / 2, blockRect.getMinY());
					}
				}
				else if (_neighborBlocks.size()==1 && wallFlag==false)
				{
					_standBlockPosition = BlockVecConvert(point->getPosition());

					if (_player->rightFlag == true)
					{
						_player->LRMagicPosition = Vec2(blockRect.getMaxX(), 0);
					}
					else
					{
						_player->LRMagicPosition = Vec2(blockRect.getMinX(), 0);
					}

					if (_player->upFlag == true)
					{
						_player->UDMagicPosition = Vec2(winSize.width / 2, blockRect.getMaxY());
					}
					else
					{
						_player->UDMagicPosition = Vec2(winSize.width / 2, blockRect.getMinY());
					}
				}
			}			
		}
		//ゴールフラグ
		for (Blocks* block : _allBlocks)
		{

			Vec2 neighborBlockMapVec = BlockVecConvert(block->getPosition());
			if (Vec2(playerMapVec.x, playerMapVec.y + 1) == neighborBlockMapVec
				&& Vec2(playerMapVec) == goalVec)
			{
				goalFlag = true;
				log("goalFlag true");
				break;
			}
			else
			{
				goalFlag = false;
			}
		}
		//ゴール判定
		if (goalFlag == true)
		{
			if (getJumpFlag() == true)
			{
		
				_state = GameState::RESULT;
				if (_state == GameState::RESULT)
				{
					_velocity.x = 0;
					_player->playAnimation(7);
					experimental::AudioEngine::play2d("sounds/se_clear.mp3", false, 1.0f);

					onResult();
				}
				_prevPosition = _playerPosition;

			}
		}

		//上昇落下でのアニメーション切り替え
		if (getJumpFlag() == false)
		{
			if (_playerPosition.y > _prevPosition.y)
			{
				_player->playAnimation(2);
			}

			if (_playerPosition.y < _prevPosition.y)
			{
				setJumpFlag(false);
				_player->playAnimation(3);
			}

			//毎フレーム位置を更新
			_prevPosition = _playerPosition;
		}
	}

	//ゲームオーバー
	if (_state == GameState::GAMEOVER && _player->getParent() != nullptr)
	{
		experimental::AudioEngine::play2d("sounds/se_cancel.mp3", false, 0.8f);
		auto mainScene = MainScene::createSceneWithLevel(_level);
		auto mainTransition = TransitionFade::create(1.0f, mainScene);
		Director::getInstance()->replaceScene(mainTransition);
		_player->removeFromParent();
	}
}

//クリア時の表示
void Stage::onResult()
{
	experimental::AudioEngine::setVolume(mainBgmID, 0.4);

	if (_level < MAX_LEVEL)
	{
		clearNext->setVisible(true);
	}
	else
	{
		clearTitle->setVisible(true);
	}

}

//メイン
bool Stage::initWithLevel(int level)
{
	if (!Layer::init())
	{
		return false;
	}
	//現ステージ数を格納
	_level = level;
	/*
	//剛体の接触チェック
	auto contactListener = EventListenerPhysicsContact::create();

	contactListener->onContactBegin = [this](PhysicsContact&contact){

		//プレイヤーではない方を抽出
		PhysicsShape* ground = contact.getShapeA()->getBody() == _player->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
		
		Node* groundNode = ground->getBody()->getNode();
		
		//NodeをBlocks*にダウンキャスト
		Blocks *_neighborBlock = dynamic_cast<Blocks *>(groundNode);
		if (_neighborBlock != nullptr) 
		{
			//なんもしない
		}

		nowBlockPosition = _neighborBlock->getPosition();

		auto nowBlockVec = BlockVecConvert(nowBlockPosition);

		//カテゴリ抽出
		int category = groundNode->getPhysicsBody()->getCategoryBitmask();

		cocos2d::Rect groundRect = groundNode->getBoundingBox();
		float groundTopY = groundRect.getMaxY();

		float minX = groundRect.origin.x;
		float maxX = groundRect.getMaxX() ;
	
		cocos2d::Rect playerRect = _player->getBoundingBox();
		float playerBottomY = playerRect.origin.y;


		if (category & static_cast<int>(Stage::TileType::BLOCKS))
		{
			bool isExist = _neighborBlocks.contains(_neighborBlock);
			if (!isExist){
				_neighborBlocks.pushBack(_neighborBlock);
			}


			if (groundTopY <= playerBottomY)
			{
				//壁乗りを封じる
				for (Blocks* block : _allBlocks)
				{

					for (Blocks* neighborBlocks : _neighborBlocks)
					{

						Vec2 neighborBlockMapVec = BlockVecConvert(neighborBlocks->getPosition());
						Vec2 convertBlockMapVec = BlockVecConvert(block->getPosition());

						//もしプレイヤーの真下にブロックがない
						if (getJumpFlag() == true && Vec2(playerMapVec.x, playerMapVec.y + 1) != neighborBlockMapVec)
						{

							if (_neighborBlocks.back()->getPosition() == StageVecConvert(Vec2(playerMapVec.x - 1, playerMapVec.y + 1))
								&& _player->stopL == true)
							{
								if (_neighborBlocks.back()->getPosition() == block->getPosition())
								{
									_player->getPhysicsBody()->setVelocity(Vec2(0, -18));
									_player->getPhysicsBody()->setGravityEnable(true);
									wallFlag = true;
								}
							}
							else if (_neighborBlocks.back()->getPosition() == StageVecConvert(Vec2(playerMapVec.x + 1, playerMapVec.y + 1))
								&& _player->stopR == true)
							{
								if (_neighborBlocks.back()->getPosition() == block->getPosition())
								{
									_player->getPhysicsBody()->setVelocity(Vec2(0, -18));
									_player->getPhysicsBody()->setGravityEnable(true);
									wallFlag = true;
								}
							}

						}
						else
						{
							setJumpFlag(true);
							_player->getPhysicsBody()->setGravityEnable(false);
							_player->getPhysicsBody()->setVelocity(Vec2(0, 0));
							wallFlag = false;
						}
					}
				}
			}
			//左右の衝突時に止める
			else if (nowBlockVec.y >= playerMapVec.y)
			{
				if (maxX >= playerRect.getMinX() && nowBlockPosition.x < _player->getPositionX())
				{
					_player->stopL = true;
				}
				else if (minX <= playerRect.getMaxX() && nowBlockPosition.x > _player->getPositionX())
				{
					_player->stopR = true;
				}
			}
		}

		return true;
	}; 

	*/
	//画面サイズ取得
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	
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
	auto stageFile = StringUtils::format(STAGE_FILE, level);
	auto map = TMXTiledMap::create(stageFile);

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
				_blockGen->setPosition(tileMap->getPosition().x + MAPCHIP_SIZE / 2, tileMap->getPosition().y + MAPCHIP_SIZE / 2);
				this->addChild(_blockGen);

				if (tileID == 4)
				{
					_blockGen->setLocalZOrder(5);
					_mirrorAbleBlocks.pushBack(_blockGen);
				}
				if (tileID == 10)
				{
					goalVec = BlockVecConvert(_blockGen->getPosition());
				}

				tileMap->removeFromParent();
			}			
		}
	}

	//キャラ配置
	auto luk = Player::create();
	this->setPlayer(luk);
	luk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	luk->setPosition(Vec2(56, 92));
	luk->getTexture()->setAliasTexParameters();
	luk->setLocalZOrder(10);
	_player->playAnimation(0);
	this->addChild(luk);
	_prevPosition = luk->getPosition();

	playerMove(); 

	Sprite* guide = Sprite::create("graphics/mainguide.png");
	guide->setPosition(Vec2(5, 210));
	guide->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	guide->getTexture()->setAliasTexParameters();
	this->addChild(guide);



	
	testBlock = Sprite::create("graphics/white.png");
	testBlock->setPosition(Vec2(0,0));
	testBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	testBlock->setScale(16.0f);
	testBlock->getTexture()->setAliasTexParameters();
	testBlock->setZOrder(99);
	this->addChild(testBlock);
	
	if (_level < MAX_LEVEL)
	{
		clearNext = Sprite::create("graphics/clear_next.png");
		clearNext->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		clearNext->setVisible(false);
		clearNext->getTexture()->setAliasTexParameters();
		this->addChild(clearNext);
	}
	else
	{
		clearTitle = Sprite::create("graphics/clear_title.png");
		clearTitle->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		clearTitle->setVisible(false);
		clearTitle->getTexture()->setAliasTexParameters();
		this->addChild(clearTitle);
	}

	this->scheduleUpdate();

	return true;
}
