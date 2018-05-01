#include "MusicLayer.h"
#include "SoundManager.h"

#define MusicOpenTag			0x01
#define MusicCloseTag			0x02
#define MusicEffectOpenTag	    0x03
#define MusicEffectCloseTag     0x04

MusicLayer::MusicLayer():m_musicIsOpenOrClose(true)
, m_musicIsBigOrLittle(true)
, m_pDelegate(NULL)
, m_pMusicOpen(NULL)
, m_pMusicClose(NULL)
, m_pMusicEffectOpen(NULL)
, m_pMusicEffectClose(NULL)
{
}

 MusicLayer::~MusicLayer()
 {
 }

bool MusicLayer::init()
{
	if(!BaseLayer::init())
	{
		return false;
	}

	m_WinSize = CCDirector::sharedDirector()->getWinSize();

	// ������Ч����������
	CCSprite* pMainSprite = CCSprite::create("MusicMain.png");
	pMainSprite->setPosition(ccp(m_WinSize.width/2, m_WinSize.height/2));
	this->addChild(pMainSprite, 0);

	m_MainSize = pMainSprite->getContentSize();

	m_MusicOpenCCP = ccp(m_WinSize.width/2 + 70, m_MainSize.height - 35);
	m_MusicCloseCCP = ccp(m_WinSize.width/2 + 170, m_MainSize.height - 35);
	m_MusicEffectopenCCP = ccp(m_WinSize.width/2 +70 , m_MainSize.height/2 + 50);
	m_MusicEffectCloseCCP = ccp(m_WinSize.width/2+170, m_MainSize.height/2 + 50);

// 	// ������������ͼƬ
// 	CCSprite* pBackgroundMusic = CCSprite::create("BackgroundMusic.png");
// 	pBackgroundMusic->setPosition(ccp(m_WinSize.width/2, m_MainSize.height-30));
// 	this->addChild(pBackgroundMusic);

// 	// ������Ч����ͼƬ
// 	CCSprite* pSoundSetting = CCSprite::create("SoundSetting.png");
// 	pSoundSetting->setPosition(ccp(m_WinSize.width/2, m_MainSize.height-160));
// 	this->addChild(pSoundSetting);

	// ����ȷ�ϰ�ť
	CCMenuItemImage* pMusicSure = CCMenuItemImage::create("MusicOk1.png", "MusicOk2.png", 
		this, menu_selector(MusicLayer::musicSureCallback));
	pMusicSure->setPosition(ccp(0, 0));
	CCMenu* menuMusicSure = CCMenu::create(pMusicSure, NULL);
	menuMusicSure->setPosition(ccp(m_WinSize.width/2, 180));
	this->addChild(menuMusicSure);

	// ������������ť
	createMusicOpenBtn(m_MusicOpenCCP);
	
	// ���������ذ�ť 
	createMusicCloseBtn(m_MusicCloseCCP);

	// ������Ч����ť 
	createMusicEffectOpenBtn(m_MusicEffectopenCCP);
	
	// ������Ч�ذ�ť 
	createMusicEffectCloseBtn(m_MusicEffectCloseCCP);
	
	return true;	
}

MusicLayer* MusicLayer::create()
{
	MusicLayer* pLayer = new MusicLayer();
	if(pLayer && pLayer->init())
	{
		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);
	return NULL;
}

void MusicLayer::createMusicOpenBtn(const CCPoint& Point)
{
	// ������������ť
	m_pMusicOpen = CCMenuItemImage::create("MusicOpen.png", "MusicOpenDown.png", "MusicOpenDown.png",
		this, menu_selector(MusicLayer::musicSetCallback)); 
	m_pMusicOpen->setTag(MusicOpenTag);
	m_pMusicOpen->setPosition(ccp(0, 0));
	if(!SoundManager::shareSoundManager().isBackgroundMusickStop())
	{
		m_pMusicOpen->setEnabled(false);
	}
	CCMenu* menuMusicOpen = CCMenu::create(m_pMusicOpen, NULL);
	menuMusicOpen->setPosition(Point);
	this->addChild(menuMusicOpen);
}

void MusicLayer::createMusicCloseBtn(const CCPoint& Point)
{
	// ���������ذ�ť
	m_pMusicClose = CCMenuItemImage::create("MusicClose.png", "MusicCloseDown.png", "MusicCloseDown.png",
		this, menu_selector(MusicLayer::musicSetCallback));
	m_pMusicClose->setTag(MusicCloseTag);
	m_pMusicClose->setPosition(ccp(0, 0));
	if (SoundManager::shareSoundManager().isBackgroundMusickStop())
	{
		m_pMusicClose->setEnabled(false);
	}
	CCMenu* menuMusicClose = CCMenu::create(m_pMusicClose, NULL);
	menuMusicClose->setPosition(Point);
	this->addChild(menuMusicClose);
}

void MusicLayer::createMusicEffectOpenBtn(const CCPoint& Point)
{
	// ������Ч��
	m_pMusicEffectOpen = CCMenuItemImage::create("MusicOpen.png", "MusicOpenDown.png", "MusicOpenDown.png",
		this, menu_selector(MusicLayer::musicSetCallback)); 
	m_pMusicEffectOpen->setTag(MusicEffectOpenTag);
	m_pMusicEffectOpen->setPosition(ccp(0, 0));
	if(!SoundManager::shareSoundManager().isEffectStop())
	{
		m_pMusicEffectOpen->setEnabled(false);
	}
	CCMenu* menuMusicEffectOpen = CCMenu::create(m_pMusicEffectOpen, NULL);
	menuMusicEffectOpen->setPosition(Point);
	this->addChild(menuMusicEffectOpen);
}

void MusicLayer::createMusicEffectCloseBtn(const CCPoint& Point)
{
	// ������Ч��
	m_pMusicEffectClose = CCMenuItemImage::create("MusicClose.png", "MusicCloseDown.png", "MusicCloseDown.png", 
		this, menu_selector(MusicLayer::musicSetCallback));
	m_pMusicEffectClose->setTag(MusicEffectCloseTag);
	m_pMusicEffectClose->setPosition(ccp(0, 0));
	if (SoundManager::shareSoundManager().isEffectStop())
	{
		m_pMusicEffectClose->setEnabled(false);
	}
	CCMenu* menuMusicEffectClose = CCMenu::create(m_pMusicEffectClose, NULL);
	menuMusicEffectClose->setPosition(Point);
	this->addChild(menuMusicEffectClose);
}

void MusicLayer::musicSureCallback(CCObject* pSender)
{
	if(m_pDelegate)
	{
		m_pDelegate->onMusicLayerButtonOk();
	}	
	this->removeFromParent();
}

void MusicLayer::musicSetCallback(CCObject* pSender)
{
	CCMenuItemImage* pMenuItem = dynamic_cast<CCMenuItemImage*>(pSender);
	if(pMenuItem->getTag() == MusicOpenTag)
	{
		SoundManager::shareSoundManager().resumeBackgroundMusic();

		m_pMusicOpen->setEnabled(false);
		m_pMusicClose->setEnabled(true);
	}
	else if (pMenuItem->getTag() == MusicCloseTag)
	{
		SoundManager::shareSoundManager().stopBackgroundMusick();

		m_pMusicClose->setEnabled(false);
		m_pMusicOpen->setEnabled(true);
	}
	else if(pMenuItem->getTag() == MusicEffectOpenTag)
	{
		SoundManager::shareSoundManager().resumeEffect();
		// SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
		m_pMusicEffectOpen->setEnabled(false);
		m_pMusicEffectClose->setEnabled(true);
	}
	else if(pMenuItem->getTag() == MusicEffectCloseTag)
	{
		SoundManager::shareSoundManager().stopEffect();

		m_pMusicEffectClose->setEnabled(false);
		m_pMusicEffectOpen->setEnabled(true);
	}
}
