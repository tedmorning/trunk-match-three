#include "DailyRewordLayer.h"
#include "DataPool.h"

const int kDefaultButtonPriority = -3;

DailyRewordLayer::DailyRewordLayer()
: m_pDelegate(NULL)
, m_pButton(NULL)
{
}

DailyRewordLayer::~DailyRewordLayer()
{
}

bool DailyRewordLayer::init()
{
	if (!BaseLayer::init()) {
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* sprite = CCSprite::create("reword.png");
	sprite->setPosition(ccp(size.width / 2, size.height / 2));
	this->addChild(sprite);

	m_pButton = Button::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("confirm.png"));
	m_pButton->registerControlEvent(kDefaultButtonPriority);
	m_pButton->setDelegate(this);
	m_pButton->setPosition(ccp(sprite->getContentSize().width / 2, 32));
	sprite->addChild(m_pButton, 3);

	this->setTouchEnabled(true);
	return true;
}

void DailyRewordLayer::onTouchUp(Button* button)
{
	// m_pButton->removeFromParent();
	if (m_pDelegate) {
		m_pDelegate->onDailyRewordButtonClick();
	}
	this->removeFromParent();
}


