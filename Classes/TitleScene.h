//����define����Ă��Ȃ����
#ifndef __MyGame__TitleScene__
//�R���p�C�����ɒu��������@�C���N���[�h�K�[�h�Ƃ����炵���B
//ifndef��endif�̊Ԃɋ��ނƃG���[���o�Ȃ��Ȃ邨�܂��Ȃ��B
#define __MyGame___TitleScene__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class TitleScene :public cocos2d::Layer
{
protected:
	TitleScene();
	virtual ~TitleScene();
	bool init() override;
	//�@void�^�̒������O�̊֐��̐錾
	void onEnterTransitionDidFinish() override;

public:
	static cocos2d::Scene* createScene();
	cocos2d::Sprite* Start;
	cocos2d::Sprite* logo;

	void gameStart();

	CREATE_FUNC(TitleScene);
};

#endif /*defined(__MyGame__TitleScene__*/