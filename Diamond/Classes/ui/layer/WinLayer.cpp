#include "WinLayer.h"
#include "GainStar.h"
#include "DataPool.h"

const int kDefaultButtonPriority = -3;

WinLayer::WinLayer()
: m_pDelegate(NULL)
, m_pBtnExit(NULL)
, m_pBtnRestart(NULL)
, m_pBtnNext(NULL)
, m_pFrame(NULL)
, m_nGoldCount(0)
, m_nFlag(1)
, m_infinite(0)
, m_action(NULL)
{
}

WinLayer::~WinLayer()
{
}

WinLayer* WinLayer::create(u32 starNum, u32 gold, int infinite) // 星数，分数 
{
	WinLayer* layer = new WinLayer();
	if (layer && layer->init()) {
		layer->initWithScore(starNum, gold, infinite);
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

void WinLayer::initWithScore(u32 starNum, u32 gold, int infinite) // 星数，分数 
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_nStarNum = starNum;
	m_nGold = gold;
	m_infinite = infinite;

	CCSprite *winBG = CCSprite::create("winBG.png");
	winBG->setPosition(ccp(size.width/2, size.height/2));
	this->addChild(winBG);

	CCSprite *win = CCSprite::create("win.png");
	win->setPosition(ccp(size.width/2, 530));
	this->addChild(win);

	if (infinite)
	{

	}
	else
	{
		CCSprite *grayStar1 = CCSprite::create("grayStar1.png");
		grayStar1->setPosition(ccp(346, 400));
		this->addChild(grayStar1); 
		CCSprite *grayStar2 = CCSprite::create("grayStar2.png");
		grayStar2->setPosition(ccp(484, 412));
		this->addChild(grayStar2); 
		CCSprite *grayStar3 = CCSprite::create("grayStar3.png");
		grayStar3->setPosition(ccp(617, 395));
		this->addChild(grayStar3); 

		CCSprite *star1 = CCSprite::create("star1.png");
		star1->setScale(3.5);
		star1->setOpacity(0);
		star1->setPosition(ccp(146, 400));
		this->addChild(star1); 

		CCActionInterval *move = CCMoveTo::create(0.3f, ccp(346, 400));
		CCActionInterval *scale = CCScaleTo::create(0.3f, 1.0f);
		CCActionInterval *fade = CCFadeIn::create(0.3f);
		CCActionInterval *action = CCSpawn::create(move, scale, fade, NULL);
		CCActionInterval *delay = CCDelayTime::create(0.5f);
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(WinLayer::cbStar1MoveEnd), NULL);
		star1->runAction(CCSequence::create(delay, action, callback, NULL));

		// m_nMaxCount = gold*1.5f;
		// this->schedule(schedule_selector(WinLayer::cbGoldCount), 0.01f);
	}

	CCSprite *goldIcon = CCSprite::create("score.png");
	goldIcon->setPosition(ccp(397, 208));
	this->addChild(goldIcon); 

	CCActionInterval* move2 = CCMoveTo::create(0.3f, ccp(450, 190));
	CCActionInterval* scale2 = CCScaleTo::create(0.3, 1.f);
	CCActionInterval* fade2 = CCFadeIn::create(0.3f);		
	m_action = CCSpawn::create(move2, scale2, fade2, NULL);;
	CCString* str = CCString::createWithFormat("%d", gold);
	m_pScore = CCLabelAtlas::create(str->getCString(), "goldNum.plist");
	m_pScore->setPosition(100, 230);
	this->addChild(m_pScore, 0);
	m_pScore->runAction(m_action);

	m_pBtnExit = Button::createWithFile("a1.png");
	m_pBtnExit->setPosition(ccp(344, 100));
	m_pBtnExit->setDelegate(this);
	m_pBtnExit->registerControlEvent(kDefaultButtonPriority);
	this->addChild(m_pBtnExit);

	m_pBtnRestart = Button::createWithFile("a2.png");
	m_pBtnRestart->setPosition(ccp(480, 100));
	m_pBtnRestart->setDelegate(this);
	m_pBtnRestart->registerControlEvent(kDefaultButtonPriority);
	this->addChild(m_pBtnRestart);

	m_pBtnNext = Button::createWithFile("a3.png");
	m_pBtnNext->setPosition(ccp(620, 100));
	m_pBtnNext->setDelegate(this);
	m_pBtnNext->registerControlEvent(kDefaultButtonPriority);
	this->addChild(m_pBtnNext);
	
}

void WinLayer::onTouchUp(Button* button)
{

	if (m_pDelegate)
	{
		if (button == m_pBtnExit) {
			m_pDelegate->onExitCopyClick();
		}
		else if (button == m_pBtnNext) {
			m_pDelegate->onNextCopyClick();
		}
		else if (button == m_pBtnRestart) {
			m_pDelegate->onRestartCopyClick();
		}
	}
}

void WinLayer::cbGoldCount(float dt)
{
	m_nGoldCount += m_nFlag;
	if(m_nGoldCount >= m_nMaxCount)
	{
		m_nFlag = -1;
	}
	if(m_nGoldCount <= m_nGold && m_nFlag == -1)
	{
		this->unscheduleAllSelectors();
	}
	const char* strImage = CCString::createWithFormat("%4d", m_nGoldCount)->getCString();
	m_pScore->setString(strImage);
}


void WinLayer::cbStar1MoveEnd(CCNode* node, void* data)
{
	if(m_nStarNum < 2) return;

	CCSprite *star2 = CCSprite::create("star2.png");
	star2->setScale(3.5);
	star2->setOpacity(0);
	star2->setPosition(ccp(484, 412));
	this->addChild(star2); 

	CCActionInterval *scale = CCScaleTo::create(0.3f, 1.0f);
	CCActionInterval *fade = CCFadeIn::create(0.3f);
	CCActionInterval *action = CCSpawn::create(scale, fade, NULL);
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(WinLayer::cbStar2MoveEnd), NULL);
	star2->runAction(CCSequence::create(action, callback, NULL));
}

void WinLayer::cbStar2MoveEnd(CCNode* node, void* data)
{
	if(m_nStarNum < 3) return;

	CCSprite *star3 = CCSprite::create("star3.png");
	star3->setScale(3.5);
	star3->setOpacity(0);
	star3->setPosition(ccp(817, 395));
	this->addChild(star3); 

	CCActionInterval *move = CCMoveTo::create(0.3f, ccp(617, 395));
	CCActionInterval *scale = CCScaleTo::create(0.3f, 1.0f);
	CCActionInterval *fade = CCFadeIn::create(0.3f);
	CCActionInterval *action = CCSpawn::create(move, scale, fade, NULL);
	star3->runAction(action);
}