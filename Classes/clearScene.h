//もしdefineされていなければ
#ifndef __MyGame__clearScene__
//コンパイル時に置き換える　インクルードガードというらしい。
//ifndefとendifの間に挟むとエラーが出なくなるおまじない。
#define __MyGame___clearScene__

#include "cocos2d.h"
#include "AudioEngine.h"

class clearScene :public cocos2d::Layer
{
protected:
	clearScene();
	virtual ~clearScene();
	bool init() override;
	//　void型の長い名前の関数の宣言
	void onEnterTransitionDidFinish() override;

public:
	static cocos2d::Scene* createScene();
	int clearJingle;
	void gameStart();

	CREATE_FUNC(clearScene);
};

#endif /*defined(__MyGame__clearScene__*/
