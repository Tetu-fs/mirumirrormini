#include "TitleScene.h"
#include "MainScene.h"

USING_NS_CC;

const int INITIAL_LEVEL = 1;

Scene *TitleScene::createScene()
{

	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);

	return scene;
}

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}
void TitleScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/title_bgm.wav", true);
}

bool TitleScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	//”wŒi•\Ž¦
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
	//‰_“®‚©‚·
	background2->runAction(
		RepeatForever::create(
		Sequence::create(
		MoveTo::create(32, (Vec2(-winSize.width, 0))),
		Place::create(Vec2::ZERO),
		NULL
		)));

	//”wŒi‹ó
	this->addChild(background1);
	//”wŒi‰_
	this->addChild(background2);


	logo = Sprite::create("graphics/mmm_logo.png");
	logo->setPosition(Vec2(194, 300));
	logo->runAction(MoveTo::create(1.0, Vec2(194, 160)));
	logo->getTexture()->setAliasTexParameters();
	logo->setScale(2.0f);
	this->addChild(logo);


	Sequence* textShow = Sequence::create(Hide::create(), DelayTime::create(1), Show::create(), NULL);

	Start = Sprite::create("graphics/titleguide.png");
	Start->setPosition(Vec2(194, 60));
	Start->getTexture()->setAliasTexParameters();
	Start->runAction(textShow);
	this->addChild(Start);

	auto keyboardListener = EventListenerKeyboard::create();

	//ƒL[ƒ{[ƒh‚ª‰Ÿ‚³‚ê‚½Žž‚Ìstop‚ð‘‚­ŠÖ”H
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event *event)
	{


		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			Start->stopAllActions();
			logo->stopAllActions();

			Start->runAction(Blink::create(1,4));
			logo->setPosition(Vec2(194, 160));
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/se_ok.wav", false);
			
			auto scene = MainScene::createSceneWithLevel(INITIAL_LEVEL);
			auto transition = TransitionFade::create(1.0f, scene);
			Director::getInstance()->replaceScene(transition);
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

