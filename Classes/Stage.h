#ifndef __STAGE_H__
#define __STAGE_H__

#include "vector"
#include "math\CCMath.h"
#include "cocos2d.h"
#include "AudioEngine.h"

#include "Player.h"
#include "Blocks.h"


class Stage : public cocos2d::Layer
{

protected:

	Stage();
	virtual ~Stage() override;
	//bool init() override;
	bool initWithLevel(int level);

	bool leftPressFlag;
	bool rightPressFlag;

	bool magicUse;
	bool moveFlag;

	bool upPressFlag;

	bool goalFlag;
	bool gameoverFlag;

	bool wallFlag;

	void playerMove();
	void jumpMethod();
	float playerX;
	float playerY;
	//次のステージ
	int nextLevel;

	//BlockGen
	int blockX;
	int blockY;
	int mapX;
	int mapY;
	int rectX;
	int rectY;
	int tileID;
	
	//反射ブロックを動かす変数
	float mirrorMove;

	//BGMのID
	int mainBgmID;



	enum class GameState
	{
		PLAYING,

		RESULT,

		GAMEOVER
	};


	cocos2d::Vector<Blocks*> _allBlocks;

	//現在たっているブロック用のガイド
	cocos2d::Vec2 testVec;

	cocos2d::Vec2 check;

	//ゴールの場所
	cocos2d::Vec2 goalVec;

	//プレイヤーの移動
	cocos2d::Vec2 _velocity;
	//プレイヤーの1フレーム前の座標
	cocos2d::Vec2 _prevPosition;

	//プレイヤーの現在の座標
	cocos2d::Vec2 _playerPosition;

	//現在たっているブロック用のガイド
	cocos2d::Vec2 playerMapVec;


	//現在接触しているブロックの座標
	cocos2d::Vec2 nowBlockPosition;
	//現在たっているブロックのマップ座標
	cocos2d::Vec2 _standBlockPosition;

	//現在接触しているブロックを格納する配列
	cocos2d::Vector<Blocks*> _neighborBlockPositions;

	//反射ブロックを格納する配列
	cocos2d::Vector<Blocks*> _mirrorAbleBlocks;

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

	//クリア後の表示
	cocos2d::Sprite* clearNext;
	cocos2d::Sprite* clearTitle;

	//Blocksクラスの変数_blockGen　ブロックの生成メソッドを代入
	Blocks* _blockGen;
	//最後に押されたキー


	//　void型の長い名前の関数の宣言
	void onEnterTransitionDidFinish() override;

public:

	enum TileType
	{	
		AIR = 0 << 0,
		BLOCKS = 1 << 0,
		PLAYER = 2 << 0,
		NONE = 3 << 0
	};

	void onResult();

	void update(float dt) override;

	//横反射のエフェクト用のメソッド
	Magic* sideMirrorEffect();
	//gidによってタイルを切り出し表示するメソッド
	Blocks* BlockGen(int gid);

	CC_SYNTHESIZE(bool, _jumpFlag, JumpFlag);

	CC_SYNTHESIZE_READONLY(int, _level, Level);
	CC_SYNTHESIZE_READONLY(cocos2d::EventKeyboard::KeyCode, _lastKeyCode, LastKeyCode);
	CC_SYNTHESIZE_RETAIN(Magic*, _sideMagic, SideMagic);
	CC_SYNTHESIZE_RETAIN(Magic*, _upDownMagic, UpDownMagic);

	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*, _tiledMap, TiledMap);
	CC_SYNTHESIZE_RETAIN(Player*, _player, Player);
	CC_SYNTHESIZE_RETAIN(Blocks*, _blocks, Blocks);
	CC_SYNTHESIZE(GameState, _state, State);
	static Stage* createWithLevel(int level);
	void moveBlockX(Blocks* mirrorBlock, cocos2d::Vec2 mirrorPosition);
	void moveBlockY(Blocks* mirrorBlock, cocos2d::Vec2 mirrorPosition);
	void checkStop();
};

#endif //__STAGE_H__
