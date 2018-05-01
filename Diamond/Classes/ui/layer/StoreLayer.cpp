#include "StoreLayer.h"
#include "CCTableViewEx.h"
#include "SoundManager.h"

#define BUTTON_CLINK_MUSIC  "buttonClink.mp3"
StoreLayer::StoreLayer():m_pDelegate(NULL)
	, m_pLabelGoldNum(NULL)
	, m_pDelegatewithFight(NULL)
	, m_thisStoreIsInFright(false)
{
	m_StoreList = getConfigStore().getConfigStoreItemList();
	// SoundManager::shareSoundManager().preloadBackgroundMusic(BUTTON_CLINK_MUSIC);
	
}

StoreLayer::~StoreLayer()
{

}

bool StoreLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* sprite = CCSprite::create("StoreMain.png");
	sprite->setPosition(ccp(winSize.width/2,winSize.height/2));
	this->addChild(sprite);

	CCGridView *tableView = CCGridView::create(this, CCSizeMake(960, 360));
	tableView->setDirection(GVD_HORIZONAL);
	tableView->setAnchorPoint(CCPointZero);
	tableView->setPosition(ccp(0, 100));
	tableView->setDelegate(this);
	this->addChild(tableView);
	tableView->reloadData();

	CCMenuItemImage* storeCancelItem = CCMenuItemImage::create("okItem1.png", "okItem2.png", 
		this, menu_selector(StoreLayer::storeCancelCallback));
	storeCancelItem->setPosition(CCPointZero);
	CCMenu* menu = CCMenu::create(storeCancelItem, NULL);
	menu->setPosition(ccp(480,88));
	this->addChild(menu);

	CCMenuItemImage* pFreeStoreItem = CCMenuItemImage::create("freeStore1.png", "freeStore2.png",
		this, menu_selector(StoreLayer::freeStoreCallback));
	pFreeStoreItem->setPosition(CCPointZero);
	CCMenu* pMenu = CCMenu::create(pFreeStoreItem, NULL);
	pMenu->setPosition(ccp(400, 466));
	sprite->addChild(pMenu);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	CCSprite* pSpriteGold = CCSprite::create("gold.png");
	pSpriteGold->setAnchorPoint(ccp(0, 1));
	pSpriteGold->setPosition(ccp(400, 630));
	this->addChild(pSpriteGold);

	const char* gold = CCString::createWithFormat("%d", getUserStoreData().getGold())->getCString();
	m_pLabelGoldNum = CCLabelAtlas::create(gold, "propNum.plist");
	m_pLabelGoldNum->setAnchorPoint(ccp(0, 1));
	m_pLabelGoldNum->setPosition(ccp(450, 625));
	this->addChild(m_pLabelGoldNum);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

u32 StoreLayer::numberOfGridCellsInTableCell(CCGridView* grid)
{
	return 2;
}

CCSize StoreLayer::cellSizeForGrid(CCGridView* grid)
{
	return CCSizeMake(300, 180);
}

CCGridViewCell* StoreLayer::gridCellAtIndex(CCGridView* grid, u32 idx)
{
	StoreTableViewCell* cell = NULL;
	if (cell == NULL)
	{
		u32 storeTable_id = m_StoreList[idx]->id;									  // 获取到商店表的id

		u32 price = getConfigStore().getConfigStoreItem(storeTable_id)->price;        // 获取到价格 
		const char* priceBuf = CCString::createWithFormat("%d", price)->getCString(); // 获取到价格子字符串 
		u32 num = getConfigStore().getConfigStoreItem(storeTable_id)->num;		 	  // 获取到数量 
		const char* numBuf = CCString::createWithFormat("%d", num)->getCString();     // 获取到数量字符串 
		u32 prop_id = getConfigStore().getConfigStoreItem(storeTable_id)->prop_id;    // 获取到道具id 
		std::string image = getConfigProp().getConfigPropItem(prop_id)->texture;      // 获取道具纹理

		cell = StoreTableViewCell::create(priceBuf, numBuf, image, storeTable_id);
		cell->setDelegate(this);
		// cell->autorelease(); 
	}
	return cell;
}

u32 StoreLayer::numberOfCellsInGridView(CCGridView* grid)
{
	return getConfigStore().getSize();
}

void StoreLayer::OnBuyItemChick(int propId) // 购买事件处理接口
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	u32 gold = getUserStoreData().getGold();
	int propNum = getUserStoreData().getPropNum(propId);
	const ConfigPropItem* propItem = getConfigProp().getConfigPropItem(propId);
	if(gold < propItem->price)
	{
		return;
	}
	gold = gold - (propItem->price);
	propNum +=1;
	m_pLabelGoldNum->setString(CCString::createWithFormat("%d", gold)->getCString());

	getUserStoreData().setGold(gold);
	getUserStoreData().setPropNum(propId, propNum);
	getUserStoreData().saveUserData();
}

void StoreLayer::storeCancelCallback(CCObject* pSender)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	this->removeFromParent();
	if (m_pDelegate)
	{
		m_pDelegate->OnStoreCancelClick();
	}
	if (m_pDelegatewithFight)
	{
		m_pDelegatewithFight->onStoreCancelCkickWithFight();
	}
}

void StoreLayer::freeStoreCallback(CCObject* pSender)
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	this->removeFromParent();

	if (m_pDelegate && !m_thisStoreIsInFright)
	{
		m_pDelegate->onFreeStoreClick();
	}
	else if (m_pDelegatewithFight && m_thisStoreIsInFright)
	{
		m_pDelegatewithFight->onStoreForFreeChlick();
	}
}
