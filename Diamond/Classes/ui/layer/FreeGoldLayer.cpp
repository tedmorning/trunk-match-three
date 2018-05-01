#include "FreeGoldLayer.h"

FreeGoldLayer::FreeGoldLayer()
: m_pFreeFrame(NULL)
, m_IsThisLayerInFight(NULL)
, m_pFightDelegate(NULL)
, m_pMainDelegate(NULL)
{

}
FreeGoldLayer::~FreeGoldLayer()
{

}

bool FreeGoldLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}
	m_pFreeFrame = CCSprite::create("freeGoldBG.png");
	m_pFreeFrame->setPosition(ccp(480, 320));
	this->addChild(m_pFreeFrame, 2);

	CCMenuItemImage* item1 = CCMenuItemImage::create("fenxiang1.png", "fenxiang2.png", this, menu_selector(FreeGoldLayer::onItemFirstClick));
	item1->setPosition(CCPointZero);
	CCMenu* menu1 = CCMenu::create(item1, NULL);
	menu1->setPosition(ccp(290, 165));
	m_pFreeFrame->addChild(menu1);

	CCMenuItemImage* item2 = CCMenuItemImage::create("jifengqing1.png", "jifengqing2.png", this, menu_selector(FreeGoldLayer::onItemSecondClick));
	item2->setPosition(CCPointZero);
	CCMenu* menu2 = CCMenu::create(item2, NULL);
	menu2->setPosition(ccp(290, 340));
	m_pFreeFrame->addChild(menu2);

	CCMenuItemImage* item3 = CCMenuItemImage::create("okItem1.png", "okItem2.png", this, menu_selector(FreeGoldLayer::onItemThirdClick));
	item3->setPosition(CCPointZero);
	CCMenu* menu3 = CCMenu::create(item3, NULL);
	menu3->setPosition(ccp(295, 17));
	m_pFreeFrame->addChild(menu3);

	CCMenuItemImage* item4 = CCMenuItemImage::create("storeItem1.png", "storeItem2.png", 
		this, menu_selector(FreeGoldLayer::onFromFreeCreateStoreLayer));
	item4->setPosition(CCPointZero);
	CCMenu* menu4 = CCMenu::create(item4, NULL);
	// menu4->setPosition(ccp(165, 465));
	menu4->setPosition(ccp(186, 466));
	m_pFreeFrame->addChild(menu4, 2);
	
	return true;
}

void FreeGoldLayer::onItemFirstClick(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// C++调用java 
	JniMethodInfo minfo;   
	jboolean bSuccess;   
	bool b = JniHelper::getStaticMethodInfo(minfo,    
		"net/miidi/wall/AdWall",  //类路径   
		"showAppOffers",   //静态方法名   
		"(Lnet/miidi/wall/IAdWallShowAppsNotifier;)Z");   //括号里的是参数，后面的是返回值。   
	if (!b) {   
		CCLog("JniHelper::getStaticMethodInfo error...");   
	}else{   
		// 返回值是上面注册的那个函数的返回值，注册的是什么类型返回值的函数，就对应的使用配套的minfo.env函数
		bSuccess = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, NULL);
		CCLog("JniHelper::getStaticMethodInfo success...");
	} 
#endif
}

void FreeGoldLayer::onItemSecondClick(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	bool bo = JniHelper::getStaticMethodInfo(minfo,
		"com/lansey/diamond/Diamond",
		"showYouMiAd",
		"()V");   
	if (!bo)
	{
		CCLog("Jnihelper::getMethodInfo error...");
	}
	else
	{
		jobject obj;
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, NULL);
		CCLog("Jnihelper::getMethodInfo Success...");
	}

#endif
}

void FreeGoldLayer::onItemThirdClick(CCObject* pSender)
{
	this->removeFromParent();
	CCDirector::sharedDirector()->resume();
	if (m_pMainDelegate && !m_IsThisLayerInFight)
	{
		m_pMainDelegate->onFreeGoldLayerCancel();
	}
}

void FreeGoldLayer::onFromFreeCreateStoreLayer(CCObject* pSender)
{
	// 从免费道具界面到商店界面
	this->removeFromParent();
	if (m_pMainDelegate && !m_IsThisLayerInFight)
	{
		m_pMainDelegate->onFreeLayerToStoreLayer();
	}

	else if (m_pFightDelegate && m_IsThisLayerInFight)
	{
		m_pFightDelegate->onFreeLayerToStoreLayer();
	}

}