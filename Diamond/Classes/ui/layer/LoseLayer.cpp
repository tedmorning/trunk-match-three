#include "LoseLayer.h"
#include "SoundManager.h"
const int kDefaultButtonPriority = -3;
#define BUTTON_CLINK_MUSIC  "buttonClink.mp3"
LoseLayer::LoseLayer()
: m_pDelegate(NULL)
, m_pBtnRestart(NULL)
, m_pBtnExit(NULL)
{
	// SoundManager::shareSoundManager().preloadBackgroundMusic(BUTTON_CLINK_MUSIC);
}

LoseLayer::~LoseLayer()
{
}

bool LoseLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();	
	CCSprite* loseBG = CCSprite::create("loseBG.png");
	loseBG->setPosition(ccp(size.width / 2, size.height / 2));
	this->addChild(loseBG);
	

	CCSprite* loseBg2 = CCSprite::create("loseBg2.png");
	loseBg2->setPosition(ccp(460, 390));
	this->addChild(loseBg2);

	CCSprite* lose1 = CCSprite::create("shi.png");
	lose1->setPosition(ccp(403, 710));
	this->addChild(lose1);

	CCActionInterval *move = CCEaseExponentialIn::create(CCMoveBy::create(0.5f, ccp(0.0f,-400.0f)));
	CCActionInterval *jump1 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 15.0f, 1);
	CCActionInterval *jump2 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 8.0f, 1);
	CCActionInterval *jump3 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 4.0f, 1);
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(LoseLayer::cbMoveEnd), NULL);
	lose1->runAction(CCSequence::create(move, callback, jump1, jump2, jump3, NULL));
	
	CCSprite* loseShadow1 = CCSprite::create("shiShadow.png");
	loseShadow1->setPosition(ccp(397, 215));
	loseShadow1->setScale(1.5);
	this->addChild(loseShadow1);

	CCActionInterval *scale = CCEaseExponentialIn::create(CCScaleTo::create(0.5f, 1.0f));
	loseShadow1->runAction(scale);

	m_pBtnExit = Button::createWithFile("a1.png");
	m_pBtnExit->setPosition(ccp(419, 100));
	m_pBtnExit->setDelegate(this);
	m_pBtnExit->registerControlEvent(kDefaultButtonPriority);
	this->addChild(m_pBtnExit);

	m_pBtnRestart = Button::createWithFile("a2.png");
	m_pBtnRestart->setPosition(ccp(555, 100));
	m_pBtnRestart->setDelegate(this);
	m_pBtnRestart->registerControlEvent(kDefaultButtonPriority);
	this->addChild(m_pBtnRestart);

	return true;
}

void LoseLayer::onTouchUp(Button* button)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	if (m_pDelegate)
	{
		if (button == m_pBtnExit) {
			m_pDelegate->onLoseLyaerExitCopy();
		}
		else if (button == m_pBtnRestart) {
			m_pDelegate->onLoseLayerRestartCopy();
		}
	}
}

void LoseLayer::cbMoveEnd(CCNode* node, void* data)
{
	CCSprite* lose2 = CCSprite::create("bai.png");
	lose2->setPosition(ccp(556, 710));
	this->addChild(lose2);

	CCActionInterval *move = CCEaseExponentialIn::create(CCMoveBy::create(0.5f, ccp(0.0f,-400.0f)));
	CCActionInterval *jump1 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 15.0f, 1);
	CCActionInterval *jump2 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 8.0f, 1);
	CCActionInterval *jump3 = CCJumpBy::create(0.2f, ccp(0.0f,0.0f), 4.0f, 1);
	lose2->runAction(CCSequence::create(move, jump1, jump2, jump3, NULL));

	CCSprite* loseShadow2 = CCSprite::create("baiShadow.png");
	loseShadow2->setPosition(ccp(563, 215));
	loseShadow2->setScale(1.5);
	this->addChild(loseShadow2);

	CCActionInterval *scale = CCEaseExponentialIn::create(CCScaleTo::create(0.5f, 1.0f));
	loseShadow2->runAction(scale);
}