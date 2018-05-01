#include "TopLayer.h"


const int kMenuButtonTag = 0x0001;
const int kInfinitiTag = 7;

TopLayer::TopLayer(int wallHp, int waveCount, bool isIniiniti)
: m_pLabelHp(NULL)
, m_pDelegate(NULL)
, m_pLabelWave(NULL)
, m_nWallHp(wallHp)
, m_nWaveCount(waveCount)
, m_pLabelDeltaTime(NULL)
, m_fDeltaTime(0.f)
, m_pUserGoldLabel(NULL)
, m_pWallHp(NULL)
, m_isInitniti(isIniiniti)
{
}

TopLayer::~TopLayer()
{
}

TopLayer* TopLayer::create(int wallHp, int waveCount, bool isInitniti)
{
	TopLayer* layer = new TopLayer(wallHp, waveCount, isInitniti);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

void TopLayer::setHp(int start, int end, int total)
{
	if (m_pWallHp)
	{
		m_pWallHp->WallHpChange(start - end);
	}
}

void TopLayer::setWave(int cur, int total)
{
	if (m_pLabelWave)
	{
		m_pLabelWave->setWave(cur, total);
	}
}

void TopLayer::setUserGold(const int gold)  // 设置用户金币数量
{
	CCString* goldString = CCString::createWithFormat("%d", gold);
	const char* pUserGold = goldString->getCString();
	m_pUserGoldLabel->setString(pUserGold);  
	
}

bool TopLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSprite* sprite = CCSprite::create("top_bar1.png");
	sprite->setOpacity(0);
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointZero);
	this->addChild(sprite);

	//////////////////////////////////////////////////////////////////////////////////////// 
	// 在Top上创建用户的金币数量 
	CCSprite* pGoldSprite = CCSprite::create("gold.png");
	pGoldSprite->setPosition(ccp(30, -20));
	pGoldSprite->setScale(0.6f);
	sprite->addChild(pGoldSprite);

	int userGold = CCUserDefault::sharedUserDefault()->getIntegerForKey("user_gold", 0);
	CCPoint userGoldPoint;
	if (userGold >= 100000)
	{
		userGoldPoint = ccp(110, -20);
	}
	else
	{
		userGoldPoint = ccp(100, -20);
	}
	const char* userGoldBuf = CCString::createWithFormat("%d ",userGold) ->getCString();
	m_pUserGoldLabel = CCLabelBMFont::create(userGoldBuf, "top_hp_label.fnt");
	m_pUserGoldLabel->setScale(0.6f);

	m_pUserGoldLabel->setPosition(userGoldPoint);
	sprite->addChild(m_pUserGoldLabel,1);
	/////////////////////////////////////////////////////////////////////////////////////////

	// 创建波数
	CCSprite* pWaveSprite = CCSprite::create("wave.png");
	// pWaveSprite->setAnchorPoint(CCPointZero);
	pWaveSprite->setScale(0.6f);
	pWaveSprite->setPosition(ccp(190, -20));
	sprite->addChild(pWaveSprite);


	if (m_isInitniti)
	{
		// 创建“无限”文字
		CCSprite* initniti = CCSprite::create("initniti.png");
		initniti->setPosition(ccp(310, -23));
		initniti->setScale(0.8f);
		sprite->addChild(initniti, 2);

		m_pLabelWave = WaveLabel::create(m_nWaveCount, true);
		m_pLabelWave->setInfinitiTrue(true);
	}
	else
	{
		m_pLabelWave = WaveLabel::create(m_nWaveCount);
	}
	m_pLabelWave->setScale(0.6f);
	// m_pLabelWave->setAnchorPoint(CCPointZero);
	m_pLabelWave->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pLabelWave->setPosition(ccp(270, -23));
	sprite->addChild(m_pLabelWave, 1);


	Button* btn_menu = Button::createWithFile("menu.png");
	btn_menu->setPosition(ccp(sprite->getContentSize().width - 85, sprite->getContentSize().height / 2.f - 20));
	btn_menu->setDelegate(this);
	// btn_menu->setScale(0.8f);
	btn_menu->setTag(kMenuButtonTag);
	btn_menu->registerControlEvent();
	sprite->addChild(btn_menu);

	// 创建帮组按钮
// 	CCMenuItemImage* pHelpItem = CCMenuItemImage::create("help.png", "help.png", this, menu_selector(TopLayer::helpItemCallback));
// 	pHelpItem->setPosition(CCPointZero);
// 	CCMenu* pHelpMenu = CCMenu::create(pHelpItem, NULL);
// 	pHelpMenu->setPosition(ccp(sprite->getContentSize().width - 50, sprite->getContentSize().height / 2.f + 2));
// 	sprite->addChild(pHelpMenu);

	// 创建游戏时间
// 	m_pLabelDeltaTime = CCLabelBMFont::create("00:00", "top_hp_label.fnt");
// 	m_pLabelDeltaTime->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height / 2.f - 5));
// 	sprite->addChild(m_pLabelDeltaTime);
// 	
 	this->schedule(schedule_selector(TopLayer::update), 1.f);

	// 创建血量条
	m_pWallHp = WallHpLayer::create(m_nWallHp, m_nWallHp);
	m_pWallHp->setAnchorPoint(CCPointZero);
	m_pWallHp->setDelegate(this);
	m_pWallHp->setPosition(ccp(-5, sprite->getContentSize().height / 2.f - 40));
	sprite->addChild(m_pWallHp);

	return true;
}

void TopLayer::update(float dt)
{
	m_fDeltaTime += dt;
	// const char* time = CCString::createWithFormat("%2.2d:%2.2d", (int)m_fDeltaTime / 60, (int)m_fDeltaTime % 60)->getCString();
	// m_pLabelDeltaTime->setString(time);
}

void TopLayer::stopUpdate()
{
	this->unschedule(schedule_selector(TopLayer::update));
}

void TopLayer::onTouchUp(Button* button)
{
	if (m_pDelegate)
	{
		if (button->getTag() == kMenuButtonTag)
		{
			m_pDelegate->onMenuButtonClick();
		}
	}
}

void TopLayer::helpItemCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->pause();
	if (m_pDelegate)
	{
		m_pDelegate->onHelpMenuButtonClick();
	}
}

void TopLayer::onWallhpCHange(float hp )
 {
	 if (m_pDelegate)
	 {
		 m_pDelegate->onWallHpChange(hp);
	 }
 }