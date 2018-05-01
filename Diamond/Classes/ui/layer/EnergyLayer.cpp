#include "EnergyLayer.h"

const int kDefaultFullEnergy = 240;
const int kDefualtOneFullEnergy = 60;

const int kBlueTag = 0x0001;
const int kRedTag = 0x0002;
const int kGoldTag = 0x0003;

const float kEnergyChangedTime = 0.2f;

EnergyLayer::EnergyLayer()
: m_pEnergyBar(NULL)
, m_nEnergyFull(kDefaultFullEnergy)
, m_nOneEnergyFull(kDefualtOneFullEnergy)
, m_nCur(0)
, m_pDelegate(NULL)
{
	for (int i = 0; i < 4; i++)
	{
		m_pEnergyBarFull[i] = NULL;
	}
}

EnergyLayer::~EnergyLayer()
{
}

void EnergyLayer::addEnergy(int energy)
{
	if (energy <= 0)
	{
		return;
	}

	changeEnergy(energy);
}

bool EnergyLayer::subEnergy(int energy)
{
	if (energy <= 0)
	{
		return false;
	}

	if (m_nCur < energy)
	{
		return false;
	}

	changeEnergy(-energy);
	return true;
}


bool EnergyLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mpFull.plist");

	CCSprite* pEnergyBackGround = CCSprite::create("mpEdging.png");
	pEnergyBackGround->setAnchorPoint(CCPointZero);
	pEnergyBackGround->setPosition(ccp(45,-280));
	this->addChild(pEnergyBackGround);

	
	CCSprite* sprite = CCSprite::create("mp.png");
	sprite->setTag(kBlueTag);
	sprite->setAnchorPoint(CCPointZero);
	m_pEnergyBar = CCProgressTimer::create(sprite);
	m_pEnergyBar->setMidpoint(CCPointZero);
	m_pEnergyBar->setBarChangeRate(ccp(0, 1));
	m_pEnergyBar->setType(kCCProgressTimerTypeBar);
	// m_pEnergyBar->setType(kCCProgressTimerTypeRadial);
	m_pEnergyBar->setAnchorPoint(CCPointZero);
	m_pEnergyBar->setPosition(ccp(0, 0));
	pEnergyBackGround->addChild(m_pEnergyBar, -2);

	for(int i = 0; i < 4; i++)  // 创建满了的那个动作无限在执行
	{
		m_pEnergyBarFull[i] = CCSprite::createWithSpriteFrameName("mpFull1.png");
		m_pEnergyBarFull[i]->setAnchorPoint(CCPointZero);
		m_pEnergyBarFull[i]->setPosition(ccp(9, 9 + i*124.5));
		m_pEnergyBarFull[i]->setVisible(false);
		pEnergyBackGround->addChild(m_pEnergyBarFull[i], -1);

		CCActionInterval* animation = this->getFullAnimateAction();
		CCRepeatForever* repeat = CCRepeatForever::create(animation);
		m_pEnergyBarFull[i]->runAction(repeat);
	}

	return true;
}

void EnergyLayer::changeEnergy(int energy)
{
	m_nCur = MIN(m_nEnergyFull, m_nCur + energy);
	m_nCur = MAX(0, m_nCur);

	setEnergyBarValue();

	if (m_pDelegate)
	{
		m_pDelegate->onEnergyChanged(m_nEnergyFull ,m_nCur);
	}
}

void EnergyLayer::setEnergyBarValue() // 设置能量槽进度条
{
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(EnergyLayer::cbProgressEnd), NULL);
	CCProgressFromTo* pTo = CCProgressFromTo::create(kEnergyChangedTime, m_pEnergyBar->getPercentage(), 
			(float)m_nCur / m_nEnergyFull * 100);

	m_pEnergyBar->runAction(CCSequence::create(pTo, callback, NULL));
}

CCAnimate* EnergyLayer::getFullAnimateAction()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 4; i++)
	{
		const char* str = CCString::createWithFormat("mpFull%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}

void EnergyLayer::cbProgressEnd(CCNode* node, void* data) // 能量加减完后，要调用这个方法来设置发亮的动作是否显示
{
	for (int i = 0; i < 4; i++)
	{
		m_pEnergyBarFull[i]->setVisible(false);
	}
	int nNum = m_nCur/m_nOneEnergyFull; 
	for (int i = 0; i < nNum; i++)
	{
		m_pEnergyBarFull[i]->setVisible(true);	
	}
}