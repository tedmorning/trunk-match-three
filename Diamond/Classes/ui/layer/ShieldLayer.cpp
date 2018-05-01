#include "ShieldLayer.h"

const float kShieldUpdateInterval = 0.1f;

ShieldLayer::ShieldLayer(SceneStatusData* status)
: m_pDelegate(NULL)
, m_pStatusData(status)
{
}

ShieldLayer::~ShieldLayer()
{
}

void ShieldLayer::startListener()
{
	schedule(schedule_selector(ShieldLayer::update), kShieldUpdateInterval);
}

bool ShieldLayer::enableAttack()
{
	return m_pStatusData->isExistAttackStatus();
}

ShieldLayer* ShieldLayer::create(SceneStatusData* status, ShieldDelegate* delegate)
{
	ShieldLayer* shield = new ShieldLayer(status);
	if (shield && shield->init())
	{
		shield->setDelegate(delegate);
		shield->startListener();
		shield->autorelease();

		if (delegate)
		{
			delegate->onShieldAppear(shield);
		}

		return shield;
	}
	CC_SAFE_DELETE(shield);
	return NULL;
}

bool ShieldLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shield.plist");

	for (int i = 0; i < 4; i++)
	{
		m_pSpriteShield[i] = CCSprite::createWithSpriteFrameName("shieldBeAttack2.png");
		m_pSpriteShield[i]->setAnchorPoint(CCPointZero);
		m_pSpriteShield[i]->setPosition(ccp(277, 8 + 128*i));
		this->addChild(m_pSpriteShield[i], 4);
	}
	return true;
}

void ShieldLayer::update(float time)
{
	// ÊÇ·ñËÀÍö
	// ÊÇ·ñÓÐ¸½´ø¹¥»÷
	// Ìí¼ÓdeltaTime
	if (m_pStatusData->isOver())
	{
		if (m_pDelegate)
		{
			m_pDelegate->onShieldDisappear(this);
		}
	
		this->removeFromParent();
		
		return;
	}

	if (m_pStatusData->isExistAttackStatus())
	{
		if (m_pStatusData->enableAttack())
		{
			if (m_pDelegate)
			{
				// TODO: 
				m_pDelegate->onShieldAttack(2);
			}

			m_pStatusData->launchAttack();
		}
		else
		{
			m_pStatusData->addAttackDeltaTime(time);
		}
	}

	m_pStatusData->addStatusDeltaTime(time);
}

CCAnimate* ShieldLayer::getBeAttackAnimateAction()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 2; i++)
	{
		const char* str = CCString::createWithFormat("shieldBeAttack%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.2f));
}

void ShieldLayer::beAttacked(int rowIndex, int rowIndex2)
{
	if(rowIndex>=0 && rowIndex<=3)
	{
		m_pSpriteShield[rowIndex]->runAction(this->getBeAttackAnimateAction());
	}
	if(rowIndex2>=0 && rowIndex2<=3)
	{
		m_pSpriteShield[rowIndex2]->runAction(this->getBeAttackAnimateAction());
	}
}
