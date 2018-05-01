#include "WallLayer.h"

const char* kDefaultSpriteWallImage = "defence.png";

const int kFirstWall = 0;
const int kSecondWall = 1;
const int kThirdWall = 2;
const int kFourWall = 3;

WallLayer::WallLayer(int wallHp)
: m_pSpriteWall1(NULL)
, m_pSpriteWall2(NULL)
, m_pSpriteWall3(NULL)
, m_pSpriteWall4(NULL)
, m_nCurHp(wallHp)
, m_nTotalHp(wallHp)
, m_pDelegate(NULL)
, m_nWallHp(wallHp)
{
}

WallLayer::~WallLayer()
{
}

void WallLayer::beAttacked(int reduceHp, int rowIndex, int rowIndex2)
{
	if (reduceHp <= 0) {
		return;
	}

	if (m_nCurHp >= m_nWallHp/2)
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallFirst.png") ;
		setWallImage(ptext);
	}
	else if (m_nCurHp <= m_nWallHp/2 && m_nCurHp >=600)
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallSecond.png") ;
		setWallImage(ptext);
	}
	else
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallThird.png") ;
		setWallImage(ptext);
	}

	int cur = m_nCurHp;
	m_nCurHp = MAX(0, m_nCurHp - reduceHp);

	if (m_pDelegate) {
		m_pDelegate->onWallHpChanged(cur, m_nCurHp, m_nTotalHp);   // 修改TopLayer上的血量
		
		if (m_nCurHp == 0) {
			m_pDelegate->onOver();
		}
	}

	CCSprite* pSpriteWall = getWallByRowIndex(rowIndex);
	if (pSpriteWall) {
		pSpriteWall->runAction(getWallBeAttackAction());
		CCSprite* animate = createBeAttackSprite();
		if (animate) {
			animate->setPosition(ccp(pSpriteWall->getContentSize().width / 2, pSpriteWall->getContentSize().height / 2));
			pSpriteWall->addChild(animate);
		}
	}

	pSpriteWall = getWallByRowIndex(rowIndex2);
	if (pSpriteWall) {
		pSpriteWall->runAction(getWallBeAttackAction());
		CCSprite* animate = createBeAttackSprite();
		if (animate) {
			animate->setPosition(ccp(pSpriteWall->getContentSize().width / 2, pSpriteWall->getContentSize().height / 2));
			pSpriteWall->addChild(animate);
		}
	}
}

void WallLayer::hpChangeForWallTexture(float hp)
{
	int HP = hp;
	if (HP >= m_nWallHp/2)
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallFirst.png") ;
		setWallImage(ptext);
	}
	else if (HP <= m_nWallHp/2 && HP >=600)
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallSecond.png") ;
		setWallImage(ptext);
	}
	else
	{
		CCTexture2D* ptext = CCTextureCache::sharedTextureCache()->addImage("wallThird.png") ;
		setWallImage(ptext);
	}
}

void WallLayer::recovery(int recoveryHp)
{
	if (recoveryHp <= 0)
	{
		return;
	}

	int cur = m_nCurHp;
	m_nCurHp = MIN(m_nTotalHp, m_nCurHp + recoveryHp);

	if (m_pDelegate)
	{
		m_pDelegate->onWallHpChanged(cur, m_nCurHp, m_nTotalHp);
	}
}

WallLayer* WallLayer::create(int waveHp)
{
	WallLayer* layer = new WallLayer(waveHp);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_RELEASE(layer);
	return NULL;
}

bool WallLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	m_pSpriteWall1 = CCSprite::create("wallFirst.png");
	m_pSpriteWall1->setAnchorPoint(CCPointZero);
	m_pSpriteWall1->setPosition(ccp(-40, 0));
	this->addChild(m_pSpriteWall1, 4);

	m_pSpriteWall2 = CCSprite::create("wallFirst.png");
	m_pSpriteWall2->setAnchorPoint(CCPointZero);
	m_pSpriteWall2->setPosition(ccp(-40, 128));
	this->addChild(m_pSpriteWall2, 3);

	m_pSpriteWall3 = CCSprite::create("wallFirst.png");
	m_pSpriteWall3->setAnchorPoint(CCPointZero);
	m_pSpriteWall3->setPosition(ccp(-40, 256));
	this->addChild(m_pSpriteWall3, 2);

	m_pSpriteWall4 = CCSprite::create("wallFirst.png");
	m_pSpriteWall4->setAnchorPoint(CCPointZero);
	m_pSpriteWall4->setPosition(ccp(-40, 382));
	this->addChild(m_pSpriteWall4);

	return true;
}

CCSprite* WallLayer::getWallByRowIndex(int rowIndex)
{
	if (rowIndex == kFirstWall) {
		return m_pSpriteWall1;
	}
	else if (rowIndex == kSecondWall) {
		return m_pSpriteWall2;
	}
	else if (rowIndex == kThirdWall) {
		return m_pSpriteWall3;
	}
	else if (rowIndex == kFourWall) {
		return m_pSpriteWall4;
	}
	return NULL;
}

CCActionInterval* WallLayer::getWallBeAttackAction()
{
	CCActionInterval* move11 = CCMoveBy::create(0.015f, ccp(-3, -5));
	CCActionInterval* move12 = CCMoveBy::create(0.03f, ccp(6, 10));
	CCActionInterval* move13 = CCMoveBy::create(0.03f, ccp(-6, -10));
	CCActionInterval* move14 = CCMoveBy::create(0.015f, ccp(3, +5));

	CCActionInterval* move21 = CCMoveBy::create(0.015f, ccp(3, -5));
	CCActionInterval* move22 = CCMoveBy::create(0.03f, ccp(-6, 10));
	CCActionInterval* move23 = CCMoveBy::create(0.03f, ccp(6, -10));
	CCActionInterval* move24 = CCMoveBy::create(0.015f, ccp(-3, +5));
	return CCSequence::create(move11, move12, move13, move14, move21, move22, move23, move24, NULL);
}

CCSprite* WallLayer::createBeAttackSprite()
{
	CCSprite* sprite = CCSprite::createWithSpriteFrameName("wall_be_attacked (1).png");
	
	CCArray* frames = new CCArray();
	frames->autorelease();
	const char* str = "wall_be_attacked (%d).png";
	for (int i = 1; i <= 11; ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str, i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	CCActionInterval* animate = CCAnimate::create(CCAnimation::create(frames, 0.08f));
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(WallLayer::cbRemoveNode), NULL);
	sprite->runAction(CCSequence::create(animate, callback, NULL));

	return sprite;
}

void WallLayer::cbRemoveNode(CCNode* node, void* data)
{
	if (node) {
		node->removeFromParent();
	}
}

void WallLayer::setWallImage(CCTexture2D* text)
{
		m_pSpriteWall1->setTexture(text);
		m_pSpriteWall2->setTexture(text);
		m_pSpriteWall3->setTexture(text);
		m_pSpriteWall4->setTexture(text);
}