#include "MenuLayer.h"
#include "DataPool.h"

const int kDefaultButtonPriority = -3;

const int kResumeButtonTag = 0x0001;
const int kRestartButtonTag = 0x0002;
const int kExitButtonTag = 0x0003;
const int kHelpButtonTag = 0x0004;
const int kStoreButtonTag = 0x0005;

MenuLayer::MenuLayer()
: m_pDelegate(NULL)
, m_pFrame(NULL)
, btn_resume(NULL)
{
}

MenuLayer::~MenuLayer()
{
}

bool MenuLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pFrame = CCSprite::create("kuang.png");
	m_pFrame->setPosition(ccp(size.width / 2 + 5, size.height / 2));
	this->addChild(m_pFrame);

	// resume
	btn_resume = Button::createWithFile("btn_resume_up.png");
	btn_resume->setPosition(ccp(162, 355));
	btn_resume->setDelegate(this);
	btn_resume->setTag(kResumeButtonTag);
	btn_resume->registerControlEvent(kDefaultButtonPriority);
	m_pFrame->addChild(btn_resume);

// 	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("btn_resume_down.png");
// 	btn_resume->setTexture(texture);

	// restart
	Button* btn_restart = Button::createWithFile("btn_restart_up.png");
	btn_restart->setPosition(ccp(435, 355));
	btn_restart->setDelegate(this);
	btn_restart->setTag(kRestartButtonTag);
	btn_restart->registerControlEvent(kDefaultButtonPriority);
	m_pFrame->addChild(btn_restart);

	// exit
	Button* btn_exit = Button::createWithFile("btn_exit_up.png");
	btn_exit->setPosition(ccp(m_pFrame->getContentSize().width / 2 - 8, 120));
	btn_exit->setDelegate(this);
	btn_exit->setTag(kExitButtonTag);
	btn_exit->registerControlEvent(kDefaultButtonPriority);
	m_pFrame->addChild(btn_exit);

	// help
	Button* btn_help = Button::createWithFile("btn_help_up.png");
	btn_help->setPosition(ccp(435, 232));
	btn_help->setDelegate(this);
	btn_help->setTag(kHelpButtonTag);
	btn_help->registerControlEvent(kDefaultButtonPriority);
	m_pFrame->addChild(btn_help);

	// store
	Button* btn_store = Button::createWithFile("btn_store_up.png");
	btn_store->setPosition(ccp(162, 232));
	btn_store->setDelegate(this);
	btn_store->setTag(kStoreButtonTag);
	btn_store->registerControlEvent(kDefaultButtonPriority);
	m_pFrame->addChild(btn_store);

	return true;
}

void MenuLayer::onTouchUp(Button* button)
{
	CCDirector::sharedDirector()->resume();
	
	if (m_pDelegate)
	{
		if (button->getTag() == kResumeButtonTag)
		{
			m_pDelegate->onMenuLayerResumeGame();
		}
		else if (button->getTag() == kRestartButtonTag)
		{
			m_pDelegate->onMenuLayerRestartGame();
		}
		else if (button->getTag() == kExitButtonTag)
		{
			m_pDelegate->onMenuLayerExitGame();
		}
		else if(button->getTag() ==kHelpButtonTag)
		{
			CCDirector::sharedDirector()->pause();
			m_pDelegate->onHelpMenuButtonClick();
			this->removeFromParent();
		}
		else if (button->getTag() == kStoreButtonTag)
		{
			CCDirector::sharedDirector()->pause();
			m_pDelegate->onStoreButtonClick();
			this->removeFromParent();
		}
	}
}

void MenuLayer::onTouchDown(Button* button)
{
	CCTexture2D *texture = NULL;
	if (button->getTag() == kResumeButtonTag)
	{
		texture = CCTextureCache::sharedTextureCache()->addImage("btn_resume_down.png");
		button->setTexture(texture);
	}
	else if (button->getTag() == kRestartButtonTag)
	{
		texture = CCTextureCache::sharedTextureCache()->addImage("btn_restart_down.png");
		button->setTexture(texture);
	}
	else if (button->getTag() == kExitButtonTag)
	{
		texture = CCTextureCache::sharedTextureCache()->addImage("btn_exit_down.png");
		button->setTexture(texture);
	}
	else if(button->getTag() ==kHelpButtonTag)
	{
		texture = CCTextureCache::sharedTextureCache()->addImage("btn_help_down.png");
		button->setTexture(texture);
	}
	else if (button->getTag() == kStoreButtonTag)
	{
		texture = CCTextureCache::sharedTextureCache()->addImage("btn_store_down.png");
		button->setTexture(texture);
	}
}