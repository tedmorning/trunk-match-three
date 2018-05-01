#include "MainScene.h"
#include "MapLayer.h"
#include "DataPool.h"
#include "FightScene.h"
#include "delta_time.h"
#include "SoundManager.h"
#include "Callbackdata.h"

USING_NS_CC;

#define MAIN_BACKGROUND_MUSIC  "normal.mp3"
#define BUTTON_CLINK_MUSIC  "buttonClink.mp3"
#define GET_COIN_MUSIC "getcoin.mp3"
#define ENTER_GAME_MUSIC "entergame.mp3"

const int kVolumeBtnTag = 0x0001;
const int kPropBtnTag = 0x0002;
const int kStoreTag = 0x0003;
const int kMainMenuTag = 0x0004;
const int kReturnBeginGameTag = 0x0005;
const int kStoreAndVolumeItemTag = 0x0006;
const int kMapLightTag = 0x0007;

const int kXueJinglingTag = 0x0012;
const int kShiRenMoTag = 0x0013;
const int kHaiDaoTag = 0x0014;
const int kFaZhangTag = 0x0015;
const int kDaFaZhangTag = 0x0016;
const int kKnifeTag = 0x0017;
const int kLightNingTag = 0x0018;

MainScene::MainScene()
: m_pMapLayer(NULL)
, m_pCopyLayer(NULL)
, m_pBuyPropLayer(NULL)
, m_pMusicLayer(NULL)
, m_pStoreLayer(NULL)
, m_pMainSprite(NULL)
, m_pMuXinWangLuo(NULL)
, m_pMolaimoqu(NULL)
, m_pReturnBeginGame(NULL)
, m_pMoLaiMoQuItem(NULL)
, m_pDailyRewordLayer(NULL)
, m_pFreeGoldLayer(NULL)
{
    SoundManager::shareSoundManager().preloadBackgroundMusic(MAIN_BACKGROUND_MUSIC);
	SoundManager::shareSoundManager().preloadBackgroundMusic(GET_COIN_MUSIC);
	// SoundManager::shareSoundManager().preloadBackgroundMusic(BUTTON_CLINK_MUSIC);
	SoundManager::shareSoundManager().preloadBackgroundMusic(ENTER_GAME_MUSIC);
}

MainScene::~MainScene()
{
}

CCScene* MainScene::scene()
{
    CCScene* scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        MainScene *layer = MainScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool MainScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 用于创建公司LOGO显示
 	m_pMuXinWangLuo = CCSprite::create("Muxin.jpg");
 	m_pMuXinWangLuo->setPosition(ccp(winSize.width/2, winSize.height/2));
 	this->addChild(m_pMuXinWangLuo, 2);
 	CCFiniteTimeAction *actionF = CCCallFunc::create(this, 
 		callfunc_selector(MainScene::MuXinWangLuoCallback));
 	CCFiniteTimeAction*  action = CCSequence::create( CCDelayTime::create(2.f), actionF,NULL);	
	m_pMuXinWangLuo->runAction(action);

    return true;
}

void MainScene::onEnter()
{
    CCLayer::onEnter();

	if (m_pCopyLayer) {
		m_pCopyLayer->refresh();
	}

    SoundManager::shareSoundManager().playBackgroundMuisc(MAIN_BACKGROUND_MUSIC);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("buy_prop.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("props.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("lightning.plist");
}

void MainScene::onExit()
{
    CCLayer::onExit();

    SimpleAudioEngine::sharedEngine()->end();
}

void MainScene::onMapButtonClick(int nMapId)
{
	SoundManager::shareSoundManager().playEffect(ENTER_GAME_MUSIC); 

	this->removeChildByTag(kReturnBeginGameTag); 
	std::string copyTexture = getMapDataManager().getMapData(nMapId)->getCopyTexture(); 
	m_pMainSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(copyTexture.c_str())); 

	// 根据世界地图创建对应的副本地图 
	m_pCopyLayer = CopyLayer::create(nMapId); 
	m_pCopyLayer->setPosition(CCPointZero); 
	m_pCopyLayer->setDelegate(this); 
	this->addChild(m_pCopyLayer); 
	 
	m_pMapLayer->removeFromParent();
	m_pMapLayer = NULL; 
	this->removeChildByTag(kMapLightTag); 
	// this->removeChildByTag(0x105);// 1 2 3 go

	// 迷迪插屏广告
	 miDiSpotAd();
}

void MainScene::miDiSpotAd()
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
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID, NULL);
		CCLog("Jnihelper::getMethodInfo Success...");
	}

#endif
}

void MainScene::onCopyButtonClick(int nCopyId, bool isMineral)
{
	SoundManager::shareSoundManager().playEffect(ENTER_GAME_MUSIC);
	if (nCopyId == 1 && !getUserStoreData().getBeginnersGuide())
	{
// 		BeginnersGuideLayer* pBeginner = BeginnersGuideLayer::create();
// 		pBeginner->setAnchorPoint(CCPointZero);
// 		pBeginner->setPosition(CCPointZero);
// 		pBeginner->setDelegate(this);
// 		this->addChild(pBeginner, 3);
		// 初始用户赠送道具
		getUserStoreData().setPropNum(1, 15);
		getUserStoreData().setPropNum(2, 8);
		getUserStoreData().setPropNum(3, 2);
		getUserStoreData().setPropNum(4, 3);
		getUserStoreData().setBeginnersGuide(true);
		CCDirector::sharedDirector()->pushScene(FightScene::scene(nCopyId, isMineral, this)); 
	}
	else
	{
		CCDirector::sharedDirector()->pushScene(FightScene::scene(nCopyId, isMineral, this)); 
	}
}

void MainScene::OnBeginGuideOver()
{
	CCDirector::sharedDirector()->pushScene(FightScene::scene(1, this)); 
}

void MainScene::onPopCopyLayer()   // 返回主菜单按钮 
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC); 
	CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
	m_pCopyLayer->removeFromParent(); 
	m_pCopyLayer = NULL; 
	m_pMainSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("main_scene_bg.jpg")); 
	CCSprite* pLight = CCSprite::create("light.png"); 
	pLight->setPosition(ccp(winSize.width/2, winSize.height - 270)); 
	pLight->setTag(kMapLightTag); 

	m_pMapLayer = MapLayer::create(); 
	m_pMapLayer->setPosition(CCPointZero); 
	m_pMapLayer->setDelegate(this); 
	this->addChild(pLight); 
	this->addChild(m_pMapLayer); 
}

CCActionInterval* MainScene::getCopyItemSnakeAction()
{
	CCActionInterval* move11 = CCMoveBy::create(0.015f, ccp(-3, -5));
	CCActionInterval* move12 = CCMoveBy::create(0.03f, ccp(6, 10));
	CCActionInterval* move13 = CCMoveBy::create(0.03f, ccp(-6, -10));
	CCActionInterval* move14 = CCMoveBy::create(0.015f, ccp(3, 5));

	CCActionInterval* move21 = CCMoveBy::create(0.015f, ccp(3, -5));
	CCActionInterval* move22 = CCMoveBy::create(0.03f, ccp(-6, 10));
	CCActionInterval* move23 = CCMoveBy::create(0.03f, ccp(6, -10));
	CCActionInterval* move24 = CCMoveBy::create(0.015f, ccp(-3, 5));
	return CCRepeat::create(CCSequence::create(move11, move12, move13, move14, move21, move22, move23, move24, NULL), 3);
}

CCActionInterval* MainScene::getCopyItemWingAction()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("wingItemCCZ.plist");

	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 5; i++)
	{
		const char* str = CCString::createWithFormat("wingItem%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.15f));
}

CCActionInterval* MainScene::getCopyItemExplodeAction()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("copyitemanimate.plist");

	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 14; i++)
	{
		const char* str = CCString::createWithFormat("copyitemUnlock%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.07f));
}

CCActionInterval* MainScene::getCoptItemSnakeActionSecond()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("copyItemSnake.plist");

	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 14; i++)
	{
		const char* str = CCString::createWithFormat("animate%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.07f));
}

void MainScene::copyItemCallback(CCNode* node, void* data)
{
	int* buffer  = NULL;
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	if (callback_data && callback_data->data1)
	{
		buffer = reinterpret_cast<int*>(callback_data->data1);
	}
	int copyId  = -1;
	if (buffer)
	{
		copyId = (*buffer);
		//delete buffer; 
	}
	
	if (m_pCopyLayer)
	{
		m_pCopyLayer->resetCopyLayer(copyId);
	}
}

void MainScene::onFightExitForMenuLayer()
{
	if (m_pCopyLayer)
	{
		m_pCopyLayer->resetCopyLayer();
	}	
}

void MainScene::onExitFightScene(int nCopyId, bool isCopyUnlock, bool isMineral, bool isMineralNewUnlock)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(MainScene::copyItemCallback), NULL);
	if (m_pCopyLayer) 
	{
		if (isCopyUnlock)// 是true则说明这个关卡按钮之前已经被激活了无需再激活 
		{
			if (isMineral)
			{
				if (isMineralNewUnlock)
				{
					Button* curItem = m_pCopyLayer->getItemImage(nCopyId); 
					// CCSprite* sprite = CCSprite::createWithSpriteFrameName("wingItem1.png"); 
					CCDelayTime* time = CCDelayTime::create(1.f);
					CCActionInterval* animate = getCopyItemWingAction();
					CCSequence* queue = CCSequence::create(time, animate, callback, NULL);
					curItem->runAction(queue);
				}
			}
			else
			{
				m_pCopyLayer->resetCopyLayer();
			}
			return;
		}
		if (nCopyId != 1)
		{
			if (getConfigCopy().getConfigCopyItem(nCopyId-1)->infinite)
			{
				// 无限模式没有下一关了 
				return;
			}
		}
		
		Button* item = m_pCopyLayer->getItemImage(nCopyId+1); // 获取下一关的按钮 
		if (item == NULL) 
		{
			return;
		}
		CCPoint point = item->getPosition();
		CCSprite* animateSprite = CCSprite::create("copy_locked.png");
		animateSprite->setPosition(point);
		this->addChild(animateSprite, 2);
		item->setVisible(false);

		CallbackData* callbackDataForInt = new CallbackData();
		int *p = new int;
		*p = nCopyId+1;
		callbackDataForInt->data1 = reinterpret_cast<void*>(p);
		CCCallFuncND* callback3 = CCCallFuncND::create(this, callfuncND_selector(MainScene::copyItemCallback),
			reinterpret_cast<void*>(callbackDataForInt));
		CCCallFuncND* callback2 = CCCallFuncND::create(this, callfuncND_selector(MainScene::copyItemUnlockCallback), 
			reinterpret_cast<void*>(callbackDataForInt));
		CCActionInterval* snakeAction = getCopyItemSnakeAction();
		CCActionInterval* animateSecond = getCoptItemSnakeActionSecond();
		CCActionInterval* animate = getCopyItemExplodeAction();
		CCDelayTime* delay = CCDelayTime::create(0.5f);
		CCSequence* action = CCSequence::create(callback3, delay,snakeAction, animateSecond,callback2, NULL);
		animateSprite->runAction(action);
	}
}

void MainScene::copyItemUnlockCallback(CCNode* node, void* data)
{	
	if (node)
	{
		node->removeFromParent();
	}

	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	int* buffer = NULL;
	if (callback_data->data1)
	{
		buffer = reinterpret_cast<int*>(callback_data->data1);
	}	
	int copyId = *buffer;
	if (buffer)
	{
		m_pCopyLayer->getItemImage(copyId)->setVisible(true);
	}
	
// 	CCPoint point = m_pCopyLayer->getItemImage(copyId)->getPosition();
// 	CCSprite* sprite = CCSprite::create("copyitem.png");
// 	sprite->setPosition(ccp(point.x, 700));
// 	m_pCopyLayer->addChild(sprite, 2);
// 
// 	CCMoveTo* move = CCMoveTo::create(0.5, point);
// 	CCEaseExponentialIn* esae = CCEaseExponentialIn::create(move);
// 	CCCallFuncND* callback = CCCallFuncND::create(this, 
// 		callfuncND_selector(MainScene::copyItemMoveCallback),data);
// 	CCActionInterval* snake = getCopyItemSnakeAction();
// 
// 	CCSequence* queue = CCSequence::create(esae, callback, NULL);
// 
// 	sprite->runAction(queue);


}

void MainScene::copyItemMoveCallback(CCNode* node, void* data)
{
	if (node)
	{
		node->removeFromParent();
	}
	
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	int* buffer = NULL;
	if (callback_data->data1)
	{
		buffer = reinterpret_cast<int*>(callback_data->data1);
	}	
	int copyId = *buffer;
	if (buffer)
	{
		m_pCopyLayer->getItemImage(copyId)->setVisible(true);
	}
}

void MainScene::onNextCopy(int nCopyId)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
    if (getConfigCopy().isExistNextCopy(nCopyId)) 
	{
        u32 nNextCopyId = getConfigCopy().getNextCopyId(nCopyId);
        CCDirector::sharedDirector()->replaceScene(FightScene::scene(nNextCopyId, this));
    }
}

void MainScene::onRestartCopy(int nCopyId, bool isMineral)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
    CCDirector::sharedDirector()->replaceScene(FightScene::scene(nCopyId, isMineral, this));
}

void MainScene::onDailyRewordButtonClick()
{
	SoundManager::shareSoundManager().playEffect(GET_COIN_MUSIC);

    int gold = getUserStoreData().getGold();
    gold += getConst("DAILY_REWARD_GOLD");
    getUserStoreData().setGold(gold);
    getUserStoreData().setRewardTime(base::zeroTime());
    getUserStoreData().saveUserData();

	m_pDailyRewordLayer = NULL;
	m_pEnterGame->setEnabled(true);
}

void MainScene::onConfirmBuyProp()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
    m_pBuyPropLayer = NULL;
    getUserStoreData().saveUserData();
}

void MainScene::onMenuItemClick(CCObject* pSender)
{
	 if(m_pBuyPropLayer != NULL || m_pStoreLayer != NULL || m_pMusicLayer != NULL || m_pFreeGoldLayer)
	 {
		 return;
	 }
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);

    CCMenuItemImage* pMenuItem = dynamic_cast<CCMenuItemImage*>(pSender);
    if (pMenuItem) {
// 		if (pMenuItem->getTag() == kPropBtnTag) {
// 			if (m_pBuyPropLayer == NULL && m_pStoreLayer == NULL && m_pMusicLayer == NULL) {
// 				m_pBuyPropLayer = BuyPropLayer::create();
// 				m_pBuyPropLayer->setDelegate(this);
// 				this->addChild(m_pBuyPropLayer);
// 			}
// 		}
        if (pMenuItem->getTag() == kStoreTag){
			u32 userGold = getUserStoreData().getGold();
			if (userGold >= 1000)
			{
				m_pStoreLayer = StoreLayer::create();
				m_pStoreLayer->setDelegate(this);
				this->addChild(m_pStoreLayer, 3);
			}
			else
			{
				buyPropItemCallback(NULL);
			}
			
        }
        else if(pMenuItem->getTag() == kVolumeBtnTag ){
            m_pMusicLayer = MusicLayer::create();
            m_pMusicLayer->setDelegate(this);
            this->addChild(m_pMusicLayer, 3);

        }
		else if(pMenuItem->getTag() == kReturnBeginGameTag ){
			if(m_pMapLayer)
			{
				returnBeginCallback(NULL);
			}
			else
			{
				onPopCopyLayer();
			}
		}
    }
}

void MainScene::onMusicLayerButtonOk()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
    m_pMusicLayer = NULL;
    getUserStoreData().setBackgroundMusicStop(SoundManager::shareSoundManager().isBackgroundMusickStop());
    getUserStoreData().setEffectStop(SoundManager::shareSoundManager().isEffectStop());
    getUserStoreData().saveUserData();
}

void MainScene::OnStoreCancelClick()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
    m_pStoreLayer = NULL;
// 	m_pBuyPropLayer = NULL;
}

void MainScene::mainItemCallback(CCObject* pSender)   // 开始游戏点击事件
{
	SoundManager::shareSoundManager().playEffect(ENTER_GAME_MUSIC);
	if (m_pDailyRewordLayer)
	{
		return ;
	}
	
	m_pMolaimoqu->removeFromParent();
	m_pMainSprite->removeChildByTag(kXueJinglingTag);
	m_pMainSprite->removeChildByTag(kShiRenMoTag);
	m_pMainSprite->removeChildByTag(kHaiDaoTag);
	m_pMainSprite->removeChildByTag(kFaZhangTag);
	m_pMainSprite->removeChildByTag(kDaFaZhangTag);
	m_pMainSprite->removeChildByTag(kKnifeTag);
	m_pMainSprite->removeChildByTag(kLightNingTag);
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	this->removeChildByTag(kMainMenuTag);
	m_pMainSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("main_scene_bg.jpg"));

	CCSprite* pLight = CCSprite::create("light.png");
	pLight->setPosition(ccp(winSize.width/2, winSize.height - 297));
	pLight->setTag(kMapLightTag);

    m_pMapLayer = MapLayer::create();
    m_pMapLayer->setPosition(CCPointZero);
    m_pMapLayer->setDelegate(this);
	this->addChild(pLight);
    this->addChild(m_pMapLayer);

    // 创建商店菜单 
    CCMenuItemImage* pStoreItem = CCMenuItemImage::create(
        "prop_btn.png",
        "prop_btn.png",
        this,
        menu_selector(MainScene::onMenuItemClick));
    pStoreItem->setTag(kStoreTag);
    pStoreItem->setPosition(ccp(800, 600));
    ////////////////////////////////////////////////

    CCMenuItemImage* pVolumeItem = CCMenuItemImage::create(
        "volume_btn.png",
        "volume_btn.png",
        this,
        menu_selector(MainScene::onMenuItemClick));
    pVolumeItem->setTag(kVolumeBtnTag);
    pVolumeItem->setPosition(ccp(900, 600));
	////////////////////////////////////////////////

	CCMenuItemImage* pReturnBeginGame = CCMenuItemImage::create(
		"return_map.png",
		"return_map.png",
		this,
		menu_selector(MainScene::onMenuItemClick));
	pReturnBeginGame->setTag(kReturnBeginGameTag);
	pReturnBeginGame->setAnchorPoint(ccp(0, 1));
	pReturnBeginGame->setPosition(ccp(10, 630));

    CCMenu* pMenu = CCMenu::create( pVolumeItem, pStoreItem,pReturnBeginGame, NULL);
	pMenu->setTag(kStoreAndVolumeItemTag);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
   
// 	if(!getUserStoreData().getBeginnersGuide())
// 	{
// 		BeginnersGuideLayer* pBeginner = BeginnersGuideLayer::create();
// 		pBeginner->setAnchorPoint(CCPointZero);
// 		pBeginner->setPosition(CCPointZero);
// 		this->addChild(pBeginner, 3);
// 	}
}

void MainScene::MuXinWangLuoCallback() // 木心网络界面回调函数
{
	if (m_pMuXinWangLuo)
	{
		m_pMuXinWangLuo->removeFromParent();
	}

	createBeginGameView();

	// 每日奖励金币
	if (base::overOneDay(getUserStoreData().getRewardTime())) {
		m_pDailyRewordLayer = DailyRewordLayer::create();
		m_pDailyRewordLayer->setDelegate(this);
		this->addChild(m_pDailyRewordLayer, 1);
		m_pEnterGame->setEnabled(false);
	}
}

void MainScene::onFreeStoreClick()
{
	m_pStoreLayer = NULL;
    buyPropItemCallback(NULL);
}

void MainScene::buyPropItemCallback(CCObject* pSender)  // 在主场景点击免费金币按钮
{
	m_pFreeGoldLayer = FreeGoldLayer::create();
	m_pFreeGoldLayer->setMainFreeDelegate(this);
	m_pFreeGoldLayer->setAnchorPoint(CCPointZero);
	m_pFreeGoldLayer->setPosition(ccp(0, 0));
	this->addChild(m_pFreeGoldLayer, 3);


// 	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
//     m_pStoreLayer = NULL;
//     if (m_pBuyPropLayer == NULL && m_pStoreLayer == NULL && m_pMusicLayer == NULL) {
// 
//         m_pBuyPropLayer = BuyPropLayer::create();
//         m_pBuyPropLayer->setDelegate(this);
//         this->addChild(m_pBuyPropLayer, 3);
//     }

}

void MainScene::onFreeLayerToStoreLayer()
{
	// 从免费金币界面到商店界面
	if (m_pFreeGoldLayer)
	{
		m_pFreeGoldLayer = NULL;
	}
	
	m_pStoreLayer = StoreLayer::create();
	m_pStoreLayer->setDelegate(this);
	this->addChild(m_pStoreLayer, 3);
}

// 当免费金币界面关闭后，清空m_pFreeGoldLayer
void MainScene::onFreeGoldLayerCancel()
{
	if (m_pFreeGoldLayer)
	{
		m_pFreeGoldLayer = NULL;
	}
	if (m_pStoreLayer)
	{
		m_pStoreLayer = NULL;
	}
}

void MainScene::returnBeginCallback(CCObject* pSender) //  返回到开始游戏主界面
{
	if (m_pMusicLayer || m_pStoreLayer)
	{
		return;
	}
	
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	createBeginGameView();

	m_pMapLayer->removeFromParent();//
	m_pMapLayer = NULL;
	this->removeChildByTag(kReturnBeginGameTag);
	this->removeChildByTag(kStoreAndVolumeItemTag);
	this->removeChildByTag(kMapLightTag);
}

void MainScene::createBeginGameView()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main_All_png.plist");

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_pMainSprite = CCSprite::create("main_scene.png");
	m_pMainSprite->setPosition(ccp(winSize.width/2, winSize.height/2));

// 	// 旋转精灵
// 	CCSprite* rotateSprite = CCSprite::create("rotateSpriteFrame.jpg");
// 	rotateSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
// 	rotateSprite->setScale(1.2);
// 	rotateSprite->setTag(krotateSpriteTag);
// 	m_pMainSprite->addChild(rotateSprite);
// 	CCRotateBy* rotate = CCRotateBy::create(120.f, 360);
// 	CCRepeatForever* rotateForever = CCRepeatForever::create(rotate);
// 	rotateSprite->runAction(rotateForever);

// 	// 小刀
// 	CCSprite* pKnife = CCSprite::create("knife.png");
// 	pKnife->setPosition(ccp(631,373));
// 	pKnife->setTag(kKnifetag);
// 	m_pMainSprite->addChild(pKnife);
// 	CCMoveBy* move1 = CCMoveBy::create(0.2f, ccp(-5, -5));
// 	CCMoveBy* move2 = CCMoveBy::create(0.2f, ccp(10, 10));
// 	CCMoveBy* move3 = CCMoveBy::create(0.2f, ccp(-5, -5));
// 	CCDelayTime* delayTime = CCDelayTime::create(3.f);
// 	CCSequence* knifeAction = CCSequence::create(move1, move2, move3, delayTime, NULL);
// 	CCRepeatForever* knifeForever = CCRepeatForever::create(knifeAction);
// 	pKnife->runAction(knifeForever);

	// 魔来魔去
	m_pMolaimoqu = Button::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("molaimoqu.png"));
	m_pMolaimoqu->registerControlEvent(-1);
	m_pMolaimoqu->setDelegate(this);
	m_pMolaimoqu->setPosition(ccp(winSize.width/2, 500));
	m_pMainSprite->addChild(m_pMolaimoqu, 2);

	CCScaleTo* moveBig = CCScaleTo::create(1.f, 1.f);
	CCScaleTo* moveLittle = CCScaleTo::create(1.f, 0.9f);
	CCSequence* queue = CCSequence::create(moveLittle, moveBig, NULL);
	CCRepeatForever* moveForever = CCRepeatForever::create(queue);

	m_pMolaimoqu->runAction(moveForever);

	// 血精灵
	CCSprite* xue_jing_ling = CCSprite::createWithSpriteFrameName("xuejingling.png");
	xue_jing_ling->setPosition(ccp(winSize.width/2, winSize.height/2));
	xue_jing_ling->setTag(kXueJinglingTag);
	m_pMainSprite->addChild(xue_jing_ling, 1);

	// 食人魔
	CCSprite* shi_ren_mo = CCSprite::createWithSpriteFrameName("shiremo.png");
	shi_ren_mo->setPosition(ccp(winSize.width/2, winSize.height/2));
	shi_ren_mo->setTag(kShiRenMoTag);
	m_pMainSprite->addChild(shi_ren_mo, 3);

	// 海盗
	CCSprite* hai_dao = CCSprite::createWithSpriteFrameName("haidao.png");
	hai_dao->setPosition(ccp(winSize.width/2, winSize.height/2));
	hai_dao->setTag(kHaiDaoTag);
	m_pMainSprite->addChild(hai_dao, 1);

	// 血精灵法杖
	CCSprite* xue_fazhang = CCSprite::createWithSpriteFrameName("fazhang.png");
	xue_fazhang->setPosition(ccp(204, 132));
	xue_fazhang->setTag(kFaZhangTag);
	m_pMainSprite->addChild(xue_fazhang);

	CCMoveBy* move1 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCMoveBy* move2 = CCMoveBy::create(0.2f, ccp(10, 10));
	CCMoveBy* move3 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCDelayTime* delayTime1 = CCDelayTime::create(3.f);
	CCSequence* xue_fazhang_action = CCSequence::create(move1, move2, move3, delayTime1, NULL);
	CCRepeatForever* xue_fazhang_forever = CCRepeatForever::create(xue_fazhang_action);
	xue_fazhang->runAction(xue_fazhang_forever);

	// 食人魔法杖
	CCSprite* shi_fazhang = CCSprite::createWithSpriteFrameName("dafazhang.png");
	shi_fazhang->setPosition(ccp(378, 148));
	shi_fazhang->setTag(kDaFaZhangTag);
	m_pMainSprite->addChild(shi_fazhang, 2);

	CCMoveBy* move4 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCMoveBy* move5 = CCMoveBy::create(0.2f, ccp(10, 10));
	CCMoveBy* move6 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCDelayTime* delayTime2 = CCDelayTime::create(4.f);
	CCSequence* shi_fazhang_action = CCSequence::create(move4, move5, move6, delayTime2, NULL);
	CCRepeatForever* shi_fazhang_forever = CCRepeatForever::create(shi_fazhang_action);
	shi_fazhang->runAction(shi_fazhang_forever);


	// 海盗的刀
	CCSprite* hai_knafe = CCSprite::createWithSpriteFrameName("knife.png");
	hai_knafe->setPosition(ccp(778, 144));
	hai_knafe->setTag(kKnifeTag);
	m_pMainSprite->addChild(hai_knafe);

	CCMoveBy* move7 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCMoveBy* move8 = CCMoveBy::create(0.2f, ccp(10, 10));
	CCMoveBy* move9 = CCMoveBy::create(0.2f, ccp(-5, -5));
	CCDelayTime* delayTime3 = CCDelayTime::create(5.f);
	CCSequence* hai_knafe_action = CCSequence::create(move7, move8, move9, delayTime3, NULL);
	CCRepeatForever* hai_knafe_forever = CCRepeatForever::create(hai_knafe_action);
	hai_knafe->runAction(hai_knafe_forever);

	// 闪电
	CCSprite* light_ning = CCSprite::create("lightning1.png");
	light_ning->setPosition(ccp(winSize.width/2, winSize.height/2));
	light_ning->setTag(kLightNingTag);
	m_pMainSprite->addChild(light_ning, 1);

	CCActionInterval* animale1 = getLightNingLeftAction();
	CCActionInterval* animale2 = getLightNingRightAction();
	CCDelayTime* delayTime4 = CCDelayTime::create(0.3f);
	CCDelayTime* delayTime5 = CCDelayTime::create(3.f);
	CCSequence* light_ning_queue = CCSequence::create( animale1, delayTime4, animale2, delayTime5, NULL);
	CCRepeatForever* light_ning_forever = CCRepeatForever::create(light_ning_queue);
	light_ning->runAction(light_ning_forever);


	// 开始按钮
	m_pEnterGame = CCMenuItemImage::create("mainItemDown.png", "mainItemUp.png", 
		this, menu_selector(MainScene::mainItemCallback));
	m_pEnterGame->setPosition(CCPointZero);
	CCMenu* pMainMenu = CCMenu::create(m_pEnterGame, NULL);
	pMainMenu->setPosition(ccp(winSize.width/2, 40));
	pMainMenu->setTag(kMainMenuTag);
	this->addChild(m_pMainSprite);
	this->addChild(pMainMenu);
}


CCActionInterval* MainScene::getLightNingRightAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("lightning.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("lightning3.png");
	vFrames.push_back("lightning5.png");
	vFrames.push_back("lightning3.png");
	vFrames.push_back("lightning4.png");
	vFrames.push_back("lightning5.png");

	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.07f));
}

CCActionInterval* MainScene::getLightNingLeftAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("lightning.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("lightning1.png");
	vFrames.push_back("lightning5.png");
	vFrames.push_back("lightning1.png");
	vFrames.push_back("lightning2.png");
	vFrames.push_back("lightning5.png");

	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.07f));

}
void MainScene::onTouchUp(Button* button)
{
	if (!m_pDailyRewordLayer)
	{
		mainItemCallback(NULL);
	}
}

void MainScene::beginEventCount()
{
	CCActionInterval* animation = getEventCountAnimate();
	CCSprite* sprite = CCSprite::create("go1.png");
	sprite->setPosition(ccp(150, 150));
	sprite->setTag(0x105);
	this->addChild(sprite);
	CCSequence* action = CCSequence::create(animation, NULL);
	CCRepeatForever* foreveraction = CCRepeatForever::create(action);
	sprite->runAction(foreveraction);
	
}

CCActionInterval* MainScene::getEventCountAnimate()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("evencount.plist");
	
	std::vector<std::string> vFrames;
	vFrames.push_back("go1.png");
	vFrames.push_back("go2.png");
	vFrames.push_back("go3.png");
	vFrames.push_back("go.png");
 	
	CCAssert(vFrames.size() != 0, "");
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame,1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 1.f));
}


