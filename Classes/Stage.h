#ifndef __STAGE_H__
#define __STAGE_H__
#include "vector"
#include "math\CCMath.h"
#include "cocos2d.h"
#include "Player.h"
#include "Blocks.h"
class Stage : public cocos2d::Layer
{

protected:

	Stage();
	virtual ~Stage() override;
	bool init() override;

	bool leftPressFlag;
	bool rightPressFlag;

	bool upPressFlag;

	bool goalFlag;

	void playerMove();
	void jumpMethod();
	float testX;
	float testY;
	
	//BlockGen
	int blockX;
	int blockY;
	int mapX;
	int mapY;
	int rectX;
	int rectY;
	int tileID;

	enum class GameState
	{
		PLAYING,

		RESULT
	};


	//���݂����Ă���u���b�N�p�̃K�C�h
	cocos2d::Vec2 testVec;

	//�v���C���[�̈ړ�
	cocos2d::Vec2 _velocity;
	//�v���C���[��1�t���[���O�̍��W
	cocos2d::Vec2 _prevPosition;

	//�v���C���[�̌��݂̍��W
	cocos2d::Vec2 _playerPosition;

	//���ݐڐG���Ă���u���b�N�̍��W
	cocos2d::Vec2 nowBlockPosition;
	//���݂����Ă���u���b�N�̃}�b�v���W
	cocos2d::Vec2 _standBlockPosition;

	//���ݐڐG���Ă���u���b�N�̍��W���i�[����z��
	std::vector<cocos2d::Vec2> _neighborBlockPositions;

	//���˃u���b�N���i�[����z��
	std::vector<Blocks*> _mirrorAbleBlocks;

	//���˃u���b�N�̃}�b�v���W���i�[����z��
	std::vector<cocos2d::Vec2> _mirrorAblePositions;

	//�����ɓn���ꂽStage��̍��W���}�b�v���W�ɕϊ����郁�\�b�h
	cocos2d::Vec2 BlockVecConvert(cocos2d::Vec2 blockAncorVecs);
	//�����ɓn���ꂽ�}�b�v���W��Stage��̍��W�ɕϊ����郁�\�b�h
	cocos2d::Vec2 StageVecConvert(cocos2d::Vec2 blockAncorVecs);
	
	//�����ɓn���ꂽX�̃}�b�v���W��Stage���X���W�ɕϊ����郁�\�b�h
	float StageVecConvertX(float blockAncorVecs);
	//�����ɓn���ꂽY�̃}�b�v���W��Stage���Y���W�ɕϊ����郁�\�b�h
	float StageVecConvertY(float blockAncorVecs);

	//���݂����Ă���u���b�N�̃K�C�h�ŕ\�����Ă���X�v���C�g
	cocos2d::Sprite* testBlock;
	//Blocks�N���X�̕ϐ�_blockGen�@�u���b�N�̐������\�b�h����
	Blocks* _blockGen;

public:

	enum TileType
	{
		BLOCKS = 1 << 0,
		GOAL = 1 << 1,
		PLAYER = 2 << 0
	};


	void update(float dt) override;

	//�����˂̃G�t�F�N�g�p�̃��\�b�h
	Magic* sideMirrorEffect();
	//gid�ɂ���ă^�C����؂�o���\�����郁�\�b�h
	Blocks* BlockGen(int gid);

	CC_SYNTHESIZE(bool, _jumpFlag, JumpFlag);

	CC_SYNTHESIZE_RETAIN(Magic*, _sideMagic, SideMagic);
	CC_SYNTHESIZE_RETAIN(Magic*, _upDownMagic, UpDownMagic);

	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*, _tiledMap, TiledMap);
	CC_SYNTHESIZE_RETAIN(Player*, _player, Player);
	CC_SYNTHESIZE_RETAIN(Blocks*, _blocks, Blocks);
	CC_SYNTHESIZE(GameState, _state, State);
	CREATE_FUNC(Stage);


private:
	//cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer*layer, cocos2d::Vec2&coodinate);

};

#endif //__STAGE_H__