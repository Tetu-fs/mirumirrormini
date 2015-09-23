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


	//現在たっているブロック用のガイド
	cocos2d::Vec2 testVec;

	//プレイヤーの移動
	cocos2d::Vec2 _velocity;
	//プレイヤーの1フレーム前の座標
	cocos2d::Vec2 _prevPosition;

	//プレイヤーの現在の座標
	cocos2d::Vec2 _playerPosition;

	//現在接触しているブロックの座標
	cocos2d::Vec2 nowBlockPosition;
	//現在たっているブロックのマップ座標
	cocos2d::Vec2 _standBlockPosition;

	//現在接触しているブロックの座標を格納する配列
	std::vector<cocos2d::Vec2> _neighborBlockPositions;

	//反射ブロックを格納する配列
	std::vector<Blocks*> _mirrorAbleBlocks;

	//反射ブロックのマップ座標を格納する配列
	std::vector<cocos2d::Vec2> _mirrorAblePositions;

	//引数に渡されたStage上の座標をマップ座標に変換するメソッド
	cocos2d::Vec2 BlockVecConvert(cocos2d::Vec2 blockAncorVecs);
	//引数に渡されたマップ座標をStage上の座標に変換するメソッド
	cocos2d::Vec2 StageVecConvert(cocos2d::Vec2 blockAncorVecs);
	
	//引数に渡されたXのマップ座標をStage上のX座標に変換するメソッド
	float StageVecConvertX(float blockAncorVecs);
	//引数に渡されたYのマップ座標をStage上のY座標に変換するメソッド
	float StageVecConvertY(float blockAncorVecs);

	//現在たっているブロックのガイドで表示しているスプライト
	cocos2d::Sprite* testBlock;
	//Blocksクラスの変数_blockGen　ブロックの生成メソッドを代入
	Blocks* _blockGen;

public:

	enum TileType
	{
		BLOCKS = 1 << 0,
		GOAL = 1 << 1,
		PLAYER = 2 << 0
	};


	void update(float dt) override;

	//横反射のエフェクト用のメソッド
	Magic* sideMirrorEffect();
	//gidによってタイルを切り出し表示するメソッド
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