//もしdefineされていなければ
#ifndef __MyGame__TitleScene__
//コンパイル時に置き換える　インクルードガードというらしい。
//ifndefとendifの間に挟むとエラーが出なくなるおまじない。
#define __MyGame___TitleScene__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class TitleScene :public cocos2d::Layer
{
protected:
	TitleScene();
	virtual ~TitleScene();
	bool init() override;
	//　void型の長い名前の関数の宣言
	void onEnterTransitionDidFinish() override;

public:
	static cocos2d::Scene* createScene();
	cocos2d::Sprite* Start;
	cocos2d::Sprite* logo;

	void gameStart();

	CREATE_FUNC(TitleScene);
};

#endif /*defined(__MyGame__TitleScene__*/