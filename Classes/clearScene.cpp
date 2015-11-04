#include "clearScene.h"

#include "TitleScene.h"
#include"AudioManager.h"

USING_NS_CC;

const int INITIAL_LEVEL = 1;

Scene *clearScene::createScene()
{

	auto scene = Scene::create();
	auto layer = clearScene::create();
	scene->addChild(layer);

	return scene;
}

clearScene::clearScene()
	:clearJingle(0)
{
	Music::mainMusicID = -1;
}

clearScene::~clearScene()
{

}
void clearScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	clearJingle = experimental::AudioEngine::play2d("sounds/allclear_jingle.mp3",false,1.0f);
}

bool clearScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	cocos2d::Size winSize = Director::getInstance()->getWinSize();

	//背景表示
	auto background1 = Sprite::create("graphics/gameclear.png");
	background1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background1->setPosition(Vec2(0, 0));
	background1->getTexture()->setAliasTexParameters();

	this->addChild(background1);

	auto background2 = Sprite::create("graphics/gameclear_2.png");
	background2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background2->setPosition(Vec2(0, 0));
	background2->getTexture()->setAliasTexParameters();

	this->addChild(background2);

	background2->runAction(RepeatForever::create(Blink::create(1, 4)));

	Sprite* clearTitle = Sprite::create("graphics/clear_title.png");
	clearTitle->setPosition(Vec2(80, 32));
	clearTitle->getTexture()->setAliasTexParameters();
	this->addChild(clearTitle);
 
	auto keyboardListener = EventListenerKeyboard::create();
	//キーボードが押された時のstopを書く関数？
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{


		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			experimental::AudioEngine::stop(clearJingle);
			experimental::AudioEngine::play2d("sounds/se_ok.mp3", false, 0.8f);
			auto clearscene = TitleScene::createScene();
			auto Trans = TransitionFade::create(2.0f, clearscene);
			Director::getInstance()->replaceScene(Trans);
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			Director::getInstance()->end();
		}


	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	this->scheduleUpdate();
	return true;

}

