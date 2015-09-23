#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "Stage.h"
class MainScene : public cocos2d::Layer
{

protected:
	
	bool init() override;

	MainScene::MainScene();

	virtual ~MainScene() override;


public:

	static cocos2d::Scene* createScene();

	CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);

	CREATE_FUNC(MainScene);

};

#endif //__MAINSCENE_H__
