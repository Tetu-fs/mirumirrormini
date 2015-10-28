#include "Player.h"
#include "Stage.h"

USING_NS_CC;
const int FRAME_COUNT = 3;
Player::Player()
	:indexCheck(-1)
	, magicFlag(false)
	, upFlag(false)
	, rightFlag(true)
	, downFlag(false)
	, stopL(false)
	, stopR(false)
	, LRMagicPosition(0,0)
	, UDMagicPosition(0, 0)
{
}

void Player::update(float dt)
{

}

bool Player::init()
{
	if (!Sprite::initWithFile("graphics/luk_sprite.png"))
	{
		return false;
	}
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	cocos2d::Point PBox[8]{cocos2d::Point(-2, -12), cocos2d::Point(-4, -10), cocos2d::Point(-4, -2), cocos2d::Point(-2, 0), cocos2d::Point(2, 0), cocos2d::Point(4, -2), cocos2d::Point(4, -10), cocos2d::Point(2, -12)};
	playerRect = Rect();
	auto body = PhysicsBody::createPolygon(PBox, 8);
	body->setEnable(true);
	body->setDynamic(true);
	body->setGravityEnable(true);
	body->setRotationEnable(false);
	body->setVelocityLimit(30.0);
	body->setCategoryBitmask(static_cast<int>(Stage::TileType::PLAYER));
	//BLOCKS�Ƃ̐ڐG�����ON
	body->setCollisionBitmask(static_cast<int>(Stage::TileType::EMPTY));
	body->setContactTestBitmask(static_cast<int>(Stage::TileType::BLOCKS));

	//body->setContactTestBitmask(INT_MAX);

	this->setPhysicsBody(body);

	this->scheduleUpdate();
	return true;
}

// 0~2�̃A�j���[�V�������Đ����郁�\�b�h(0�ʏ�A1��A2��)
//void�^��Player::playAnimation(int index)�֐����쐬
void Player::playAnimation(int index)
{

	//�������ϐ�indexCheck�ƕϐ�index��������������
	if (indexCheck == index) {
		//�����Ŋ֐��̏������I������
		return;
	}

	// �A�j���[�V�����p�̃^�O���Œ�l�Œ�`
	const int ACTION_TAG = 999; // �D���Ȑ�

	// ���ɃA�j���[�V�����������Ă�����~�߂�
	// �A�j���[�V�����ɂ͑S��999�̃^�O���t���Ă�͂��Ȃ̂ŁA�d�����Ȃ��悤�Ɏ~�߂�
	this->stopActionByTag(ACTION_TAG);

	// cocos2d::Size�^��frameSize�ϐ��ŕ\�����邩�킸����̃X�v���C�g�̃T�C�Y���w��H
	//Size(frameSize.width, frameSize.height)�ɂ��ꂼ��16.0�������Ă���B�^��float
	auto frameSize = cocos2d::Size(16.0, 24.0);

	//�H
	//�X�v���C�g��X���W0�AY���W0(����)����A16x16��؂�o���H
	//Rect��x,y,width,height��4���Ƃ��B��`�Ƃ����Ӗ�
	this->setTextureRect(cocos2d::Rect(0, 0, frameSize.width, frameSize.height));

	//SpriteFrame*�Ƃ����e���v���[�g��frames�Ƃ����z���錾
	Vector<SpriteFrame*> frames;

	//�ϐ�i��錾�A0�ŏ�������i��FRAME_COUNT�����ł��鎞�A�ϐ�i��1�����Z��{}���̏������s�����[�v
	//i��FRAME_COUNT�����łȂ��Ȃ������Ƀ��[�v�𔲂���
	for (int i = 0; i < FRAME_COUNT; ++i)
	{
		// index�̒l�ɂ����y���W��ς���
		//cocos2d::SpriteFrame�^�̃|�C���g�ϐ�frame��錾
		//�摜kawaz_shooting.png��ǂ݁AframeSize.width��i������2�R�}�A�j���[�V�����̃��[�v
		//index��16(frameSize.height�̒l)�������A�A�j���[�V������؂�ւ�
		//frameSize.width��frameSize.height�ŕ\������摜�̑傫�����w��H
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width * i, index * frameSize.height, frameSize.width, frameSize.height));

		//�z��frames�̏I����frame�̒l��}������
		frames.pushBack(frame);
	}
	auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width, index * frameSize.height, frameSize.width, frameSize.height));
	frames.pushBack(frame);

	if (index == 2)
	{
		frames.clear();
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width, 2 * frameSize.height, frameSize.width, frameSize.height));
		frames.pushBack(frame);
	}
	else if (index == 3)
	{
		frames.clear();
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width * 2, 2 * frameSize.height, frameSize.width, frameSize.height));
		frames.pushBack(frame);
	}
	else if (index == 4)
	{
		frames.clear();
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(0, 3 * frameSize.height, frameSize.width, frameSize.height));
		frames.pushBack(frame);
	}
	else if (index == 5)
	{
		frames.clear();
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width, 3 * frameSize.height, frameSize.width, frameSize.height));
		frames.pushBack(frame);
	}
	else if (index == 6)
	{
		frames.clear();
		auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width * 2, 3 * frameSize.height, frameSize.width, frameSize.height));
		frames.pushBack(frame);
	}
	else if (index == 7)
	{
		frames.clear();
		for (int i = 0; i <= FRAME_COUNT; ++i)
		{
			auto frame = SpriteFrame::create("graphics/luk_sprite.png", cocos2d::Rect(frameSize.width * i, 96, frameSize.width, frameSize.height));
			frames.pushBack(frame);
		}
	}

	//cocos2d::Animation�^�̃|�C���^�ϐ�animation��錾
	//�z��frames�������Ă���ۂ�����With�Ȃ񂿂�炪�悭�킩��Ȃ�
	auto animation = Animation::createWithSpriteFrames(frames);
	if (index == 0)
	{
		animation->setDelayPerUnit(0.2);
	}
	else if (index == 7)
	{
		animation->setDelayPerUnit(0.3);
	}
	else
	{
		animation->setDelayPerUnit(0.1);
	}
	if (index != 7)
	{
		//cocos2d::RepeatForever�^�̃|�C���^�ϐ�animate��錾�@�Ȃ񂩈ł��ۂ�
		//Animate::create(animation)��animation�Ő錾���ꂽ2�R�}�A�j���𐶐����Ă�Ƃ�����
		//RepeatForever�͐��������A�j���[�V�������𖳌��ɌJ��Ԃ�
		auto animate = RepeatForever::create(Animate::create(animation));

		//�|�C���^�ϐ�animate��ACTION_TAG�Ƃ����^�O��ݒ�
		animate->setTag(ACTION_TAG);

		//animate�Ƃ����A�N�V�����𑖂点��
		this->runAction(animate);
	}
	else
	{
		auto clearAnimate = Animate::create(animation);

		clearAnimate->setTag(ACTION_TAG);
		this->runAction(clearAnimate);
	}

	//indexCheck��index�̒l��������
	indexCheck = index;

}
