//����define����Ă��Ȃ����
#ifndef __MyGame__clearScene__
//�R���p�C�����ɒu��������@�C���N���[�h�K�[�h�Ƃ����炵���B
//ifndef��endif�̊Ԃɋ��ނƃG���[���o�Ȃ��Ȃ邨�܂��Ȃ��B
#define __MyGame___clearScene__

#include "cocos2d.h"
#include "AudioEngine.h"

class clearScene :public cocos2d::Layer
{
protected:
	clearScene();
	virtual ~clearScene();
	bool init() override;
	//�@void�^�̒������O�̊֐��̐錾
	void onEnterTransitionDidFinish() override;

public:
	static cocos2d::Scene* createScene();
	int clearJingle;
	void gameStart();

	CREATE_FUNC(clearScene);
};

#endif /*defined(__MyGame__clearScene__*/
