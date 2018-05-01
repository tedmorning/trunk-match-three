#include "TimeLayer.h"

const char* kTimeSpriteImage = "time.png";

TimeLayer::TimeLayer()
: m_pTimeBar(NULL)
{
}

TimeLayer::~TimeLayer()
{
}

bool TimeLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSprite* sprite = CCSprite::create(kTimeSpriteImage);
	sprite->setAnchorPoint(CCPointZero);
	m_pTimeBar = CCProgressTimer::create(sprite);
	m_pTimeBar->setMidpoint(CCPointZero);
	m_pTimeBar->setBarChangeRate(ccp(1, 0));
	m_pTimeBar->setType(kCCProgressTimerTypeBar);
	m_pTimeBar->setAnchorPoint(CCPointZero);
	m_pTimeBar->setPosition(CCPointZero);
	this->addChild(m_pTimeBar);

	return true;
}