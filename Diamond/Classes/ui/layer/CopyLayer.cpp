#include "CopyLayer.h"
#include "DataPool.h"
#include "Button.h"

const int kCopyItemSelectTag = 0x00123;
const int kSmokeTag = 0x00124;
const int kFishTag = 0x00125;
const int kSmoke2Tag = 0x00126;

CopyLayer::CopyLayer(int nMapId)
: m_pDelegate(NULL)
, m_nMapId(nMapId)
, m_pButtonReturn(NULL)
, m_pGridView(NULL)
, m_copyData(NULL)
{
}

CopyLayer::~CopyLayer()
{
	m_vCopyData.clear();
}

void CopyLayer::refresh()
{
	if (m_pGridView) {
		m_pGridView->reloadData();
	}
}

CopyLayer* CopyLayer::create(int nMapId)
{
	CopyLayer* layer = new CopyLayer(nMapId);
	if (layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool CopyLayer::init()
{
	if (!CCLayer::init()) {
		return false;
	}
	initData();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("copyItemCCZ.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CopyMapAnimate.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bossFanAnimate.plist");
	resetCopyLayer();
//  m_pGridView = CCGridView::create(this, CCSizeMake(960, 500), 0);
//  m_pGridView->setDirection(GVD_HORIZONAL);
// 	m_pGridView->setAnchorPoint(CCPointZero);
// 	m_pGridView->setPosition(ccp(75, 60));
// 	m_pGridView->setTouchMoved(false);
// 	this->addChild(m_pGridView);
//  m_pGridView->reloadData();
	this->setTouchEnabled(true);
	return true; 
}

// u32 CopyLayer::numberOfCellsInGridView(CCGridView* grid)
// {
// 	// int n = m_vCopyData.size();
// 	return m_vCopyData.size();
// }
// 
// u32 CopyLayer::numberOfGridCellsInTableCell(CCGridView* grid)
// {
// 	return 6;
// }
// 
// CCSize CopyLayer::cellSizeForGrid(CCGridView* grid)
// {
// 	return CCSizeMake(150, 150);
// }
// 
// CCGridViewCell* CopyLayer::gridCellAtIndex(CCGridView* grid, u32 idx)
// {
// 	return CopyGridViewCell::create(this, getCopyItem(idx), idx);
// }

void CopyLayer::onCopyItemClick(CopyGridViewCell* cell, CopyData* data, Button* button)
{
	if (m_pDelegate) {
		if (data && data->isLocked()) {
			return;
		}
		///////////////////////////////////////////////////////////////////////////////////////
		if (this->getChildByTag(kCopyItemSelectTag))
		{
			this->getChildByTag(kCopyItemSelectTag)->removeFromParent();
		}
		CCActionInterval* animate = getCopyItemAnimate();
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(CopyLayer::copyItemBGCaallback), NULL);

		CCSprite* sprite = CCSprite::createWithSpriteFrameName("copyItemBg1.png");
		sprite->setPosition(ccp(button->getPosition().x, button->getPosition().y));
		sprite->setTag(kCopyItemSelectTag);
		this->addChild(sprite, 5);
		//sprite->setScale(1.5f);
		sprite->runAction(CCSequence::create(animate, callback, NULL));

		//////////////////////////////////////////////////////////////////////////////////////
		m_copyData = data;
		//m_pDelegate->onCopyButtonClick(data->getId());
		//m_pButtonReturn->unregisterControlEvent();
	}
}

void CopyLayer::copyItemBGCaallback(CCNode* node, void* data)
{
	CCMenuItemImage* item1 = CCMenuItemImage::create("normalItem.png", "normalItem.png", 
		this, menu_selector(CopyLayer::onUserClickNormalCopyItem));
	item1->setPosition(CCPointZero);
	CCMenu* menu1 = CCMenu::create(item1, NULL);
	menu1->setPosition(ccp(27, 40));
	node->addChild(menu1, 2);
	int starNum = m_copyData->getScore();
	if (starNum >=3)
	{
		CCMenuItemImage* item2 = CCMenuItemImage::create("mineralItem.png", "mineralItem.png", 
			this, menu_selector(CopyLayer::onUserClickMineralCopyItem));
		item2->setPosition(CCPointZero);
		CCMenu* menu2 = CCMenu::create(item2, NULL);
		menu2->setPosition(ccp(122, 40));
		node->addChild(menu2, 2);
	}
	else
	{
		CCMenuItemImage* item2 = CCMenuItemImage::create("mineralItemLock.png", "mineralItemLock.png", 
			this, menu_selector(CopyLayer::onUserClickLockCopyItem));
		item2->setPosition(CCPointZero);
		CCMenu* menu2 = CCMenu::create(item2, NULL);
		menu2->setPosition(ccp(122, 40));
		node->addChild(menu2, 2);
	}
}

void CopyLayer::ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent)
{
	if (this->getChildByTag(kCopyItemSelectTag))
	{
		this->getChildByTag(kCopyItemSelectTag)->removeFromParent();
	}
}

void CopyLayer::onUserClickNormalCopyItem(CCObject* pSender)
{ 
	// 用户选择进入正常关卡
	m_pDelegate->onCopyButtonClick(m_copyData->getId());
	m_pButtonReturn->unregisterControlEvent();

	// 点击按钮之后就删除
	if (this->getChildByTag(kCopyItemSelectTag))
	{
		this->getChildByTag(kCopyItemSelectTag)->removeFromParent();
	}
}
void CopyLayer::onUserClickMineralCopyItem(CCObject* pSender)
{
	// 进入矿石模式
	m_pDelegate->onCopyButtonClick(m_copyData->getId(),true);
	m_pButtonReturn->unregisterControlEvent();

	// 点击按钮之后就删除
	if (this->getChildByTag(kCopyItemSelectTag))
	{
		this->getChildByTag(kCopyItemSelectTag)->removeFromParent();
	}
}

void CopyLayer::onUserClickLockCopyItem(CCObject* pSender)
{
	return;
}

void CopyLayer::initData()
{
	const MapData* pMapData = getMapDataManager().getMapData(m_nMapId);
	if (pMapData) 
	{
		m_vCopyData = pMapData->getCopyDataList();
	}
}

const CopyData* CopyLayer::getCopyItem(int index)
{
	if (index >= m_vCopyData.size()) {
		return NULL;
	}
	return m_vCopyData[index];
}

int CopyLayer::getIndexByCopyId(int id)
{
	for (unsigned int i = 0; i < m_vCopyData.size(); ++i) {
		if (m_vCopyData[i]->getId() == id) {
			return i;
		}
	}
	return -1;
}

CCActionInterval* CopyLayer::getCopyItemAnimate()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 7; i++)
	{
		const char* str = CCString::createWithFormat("copyItemBg%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.01f));
}

CCActionInterval* CopyLayer::getFishAnimate()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fishCCZ.plist");
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 6; i++)
	{
		const char* str = CCString::createWithFormat("fish%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}

CCActionInterval* CopyLayer::getSmokeAnimate()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("smoke3CCZ.plist");
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 5; i++)
	{
		const char* str = CCString::createWithFormat("smoke%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.2f));
}

CCActionInterval* CopyLayer::getBossRunAcimate(int bossId)
{
	CCArray* frames = CCArray::create();
	float time = 0.f;
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 3; i++)
	{
		char* buffer = new char(20);
		switch (bossId)
		{
		case 18:
			time = 0.2f;
			strcpy(buffer, "shirenmo");
			break;
		case 21:
			time = 0.25f;
			strcpy(buffer, "pojia");
			break;
		case 22:
			time = 0.3f;
			strcpy(buffer, "lang");
			break;
		case 23:
			time = 0.35f;
			strcpy(buffer, "sishen");
			break;
		default:
			break;
		}

		const char* str = CCString::createWithFormat("%d.png", i)->getCString();
		strcat(buffer, str);
		CCSpriteFrame* frame = pCache->spriteFrameByName(buffer);
		buffer = NULL;
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, time));
}

CCActionInterval* CopyLayer::geBossRunFanAnimate(int bossId)
{
	CCArray* frames = CCArray::create();
	float time = 0.f;
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 3; i++)
	{
		char* buffer = new char(20);
		switch (bossId)
		{
		case 18:
			time = 0.2f;
			strcpy(buffer, "shirenmofan");
			break;
		case 21:
			time = 0.25f;
			strcpy(buffer, "pojiafan");
			break;
		case 22:
			time = 0.3f;
			strcpy(buffer, "langfan");
			break;
		case 23:
			time = 0.35f;
			strcpy(buffer, "sishenfan");
			break;
		default:
			break;
		}

		const char* str = CCString::createWithFormat("%d.png", i)->getCString();
		strcat(buffer, str);
		CCSpriteFrame* frame = pCache->spriteFrameByName(buffer);
		buffer = NULL;
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, time));
}

void CopyLayer::createAnimationWithCopyLayer()
{
	if (this->getChildByTag(kSmokeTag))
	{
		this->getChildByTag(kSmokeTag)->stopAllActions();
		this->getChildByTag(kSmokeTag)->removeFromParent();
	}

	if (this->getChildByTag(kFishTag))
	{
		this->getChildByTag(kFishTag)->stopAllActions();
		this->getChildByTag(kFishTag)->removeFromParent();
	}

	if (this->getChildByTag(kSmoke2Tag))
	{
		this->getChildByTag(kSmoke2Tag)->stopAllActions();
		this->getChildByTag(kSmoke2Tag)->removeFromParent();
	}

	CCActionInterval* animateForSmoke = getSmokeAnimate();
	CCSprite* smoke = CCSprite::createWithSpriteFrameName("smoke1.png");
	smoke->setPosition(ccp(350, 357));
	smoke->setTag(kSmokeTag);
	this->addChild(smoke, 1);
	CCSequence* smokeQueue = CCSequence::create(animateForSmoke, CCDelayTime::create(2.f), NULL);
	CCRepeatForever* action = CCRepeatForever::create(smokeQueue);
	smoke->runAction(action);

	CCActionInterval* animateForSmoke2 = getSmokeAnimate();
	CCSprite* smoke2 = CCSprite::createWithSpriteFrameName("smoke1.png");
	smoke2->setPosition(ccp(360, 350));
	smoke2->setTag(kSmoke2Tag);
	this->addChild(smoke2, 1);
	CCSequence* smokeQueue2 = CCSequence::create(animateForSmoke2, CCDelayTime::create(2.f), NULL);
	CCRepeatForever* action2 = CCRepeatForever::create(smokeQueue2);
	smoke2->runAction(action2);

	CCActionInterval* animateForFish = getFishAnimate();
	CCSprite* fish = CCSprite::createWithSpriteFrameName("fish1.png");
	fish->setPosition(ccp(395, 245));
	fish->setTag(kFishTag);
	fish->setVisible(false);
	this->addChild(fish, 5);
	CCCallFuncND* callback1 = CCCallFuncND::create(this, callfuncND_selector(CopyLayer::fishActionVisibleCallback), NULL);
	CCCallFuncND* callback2 = CCCallFuncND::create(this, callfuncND_selector(CopyLayer::fishActionCallback), NULL);
	CCDelayTime* delay = CCDelayTime::create(2.f);
	CCSequence* queue = CCSequence::create(delay, callback1, animateForFish, callback2, NULL);
	CCRepeatForever* actionForFish = CCRepeatForever::create(queue);
	fish->runAction(actionForFish);
}

void CopyLayer::resetCopyLayer(int wantToSetVisebleId)
{
	this->removeAllChildren();
	m_Button.clear();

	createAnimationWithCopyLayer();


	std::vector<CopyData*>::iterator iter = m_vCopyData.begin();
	for (; iter!= m_vCopyData.end(); ++iter)
	{
		Button* item = new Button();
		
		if ((*iter)->isLocked())
		{
			item = Button::createWithFile("copy_locked.png");
		}
		else
		{
			if ((*iter)->getMineralValue())
			{
				item = Button::createWithFile("copy_mineral.png");
			}
			else
			{
				item =Button::createWithFile(((*iter)->getTexture().c_str())); 
			}
		}
		item->setPosition(ccp((*iter)->getcopyButtonX(), (*iter)->getcopyButtonY()));
		item->setTag((*iter)->getId());		item->registerControlEvent(-3);
		item->setDelegate(this);
		this->addChild(item);
		if ((wantToSetVisebleId != -1) && ((*iter)->getId() == wantToSetVisebleId) )
		{
			item->setVisible(false);
		}

		u32 starNum = (*iter)->getScore();
		if (starNum)
		{
			if (!(*iter)->isInfinite()) // 如果不是无限模式就显示星星
			{
				CopyStar* star = CopyStar::create(starNum);
				star->setPosition(ccp(20, 75));
				star->setScale(0.25f);
				item->addChild(star, 1);
			}
		}
		CCActionInterval* animate = NULL;
		CCActionInterval* animateFan = NULL;
		CCSprite * sprite = NULL;
		CCRepeatForever* action = NULL;
		CCCallFuncND* callback = NULL;
		CCSequence* queue = NULL;
		CCRepeat* repeat = NULL;
		CCRepeat* repeatFan = NULL;
		CCMoveBy* move1 = NULL;
		CCMoveBy* move2 = NULL;
		CCSpawn* pawn1 = NULL;
		CCSpawn* pawn2 = NULL;
		CCDelayTime* time = NULL;
		CCSprite* boss = NULL;
		switch ((*iter)->getId())
		{
		case 6:
			if (starNum) // 关卡打过了 
			{
				boss = CCSprite::createWithSpriteFrameName("shirenmo4.png");
				boss->setPosition(ccp(655, 490));
				this->addChild(boss, 2);
				boss->setScale(0.7f);
			}
			else
			{
				move1 = CCMoveBy::create(1.8f,ccp(-90, 0));
				move2 = CCMoveBy::create(1.8f,ccp(90, 0));
				sprite = getBossSprite("shirenmo1.png");
				sprite->setPosition(ccp(707, 510));
				this->addChild(sprite, 2);
				sprite->setScale(0.5f);
				animate = getBossRunAcimate(18);
				animateFan = geBossRunFanAnimate(18);
				repeat = CCRepeat::create(animate, 3);
				repeatFan = CCRepeat::create(animateFan, 3);
				pawn1 = CCSpawn::create(repeat, move1, NULL);
				pawn2 = CCSpawn::create(repeatFan, move2, NULL);
				queue = CCSequence::create(pawn1, pawn2, NULL);
				action = CCRepeatForever::create(queue);
				sprite->runAction(action);
			}
		
			break;
		case 12:
			if (starNum)
			{
				boss = CCSprite::createWithSpriteFrameName("pojia4.png");
				boss->setPosition(ccp(237, 240));
				this->addChild(boss, 2);
				boss->setScale(0.7f);
			}
			else
			{
				move1 = CCMoveBy::create(2.25f,ccp(-90, 0));
				move2 = CCMoveBy::create(2.25f,ccp(90, 0));
				sprite = getBossSprite("pojia1.png");
				sprite->setPosition(ccp(237, 240));
				this->addChild(sprite, 2);
				sprite->setScale(0.5f);
				animate = getBossRunAcimate(21);
				animateFan = geBossRunFanAnimate(21);
				repeat = CCRepeat::create(animate, 3);
				repeatFan = CCRepeat::create(animateFan, 3);
				pawn1 = CCSpawn::create(repeat, move1, NULL);
				pawn2 = CCSpawn::create(repeatFan, move2, NULL);
				queue = CCSequence::create(pawn1, pawn2, NULL);
				action = CCRepeatForever::create(queue);
				sprite->runAction(action);
			}
			break;
		case 18:
			if (starNum)
			{
				boss = CCSprite::createWithSpriteFrameName("lang4.png");
				boss->setPosition(ccp(631, 210));
				this->addChild(boss, 2);
				boss->setScale(0.7f);
			}
			else
			{
				move1 = CCMoveBy::create(2.7f,ccp(-90, 0));
				move2 = CCMoveBy::create(2.7f,ccp(90, 0));
				sprite = getBossSprite("lang1.png");
				sprite->setPosition(ccp(631, 210));
				this->addChild(sprite, 2);
				sprite->setScale(0.5f);
				animate = getBossRunAcimate(22);
				animateFan = geBossRunFanAnimate(22);
				repeat = CCRepeat::create(animate, 3);
				repeatFan = CCRepeat::create(animateFan, 3);
				pawn1 = CCSpawn::create(repeat, move1, NULL);
				pawn2 = CCSpawn::create(repeatFan, move2, NULL);
				queue = CCSequence::create(pawn1, pawn2, NULL);
				action = CCRepeatForever::create(queue);
				sprite->runAction(action);
			}
		
			break;
		case 24:
			if (starNum)
			{
				boss = CCSprite::createWithSpriteFrameName("sishen4.png");
				boss->setPosition(ccp(724, 350));
				this->addChild(boss, 2);
				boss->setScale(0.7f);
			}
			else
			{
				move1 = CCMoveBy::create(3.3f,ccp(-90, 0));
				move2 = CCMoveBy::create(3.3f,ccp(90, 0));
				sprite = getBossSprite("sishen1.png");
				sprite->setPosition(ccp(724, 350));
				this->addChild(sprite, 2);
				sprite->setScale(0.5f);
				animate = getBossRunAcimate(23);
				animateFan = geBossRunFanAnimate(23);
				repeat = CCRepeat::create(animate, 3);
				repeatFan = CCRepeat::create(animateFan, 3);
				pawn1 = CCSpawn::create(repeat, move1, NULL);
				pawn2 = CCSpawn::create(repeatFan, move2, NULL);
				queue = CCSequence::create(pawn1, pawn2, NULL);
				action = CCRepeatForever::create(queue);
				sprite->runAction(action);
			}
			break;
		default:
			break;
		}
		m_Button.push_back(item);
	}
}

void CopyLayer::fishActionCallback(CCNode* node, void* data)
{
	if (node)
	{
		node->setVisible(false);
	}
}
void CopyLayer::fishActionVisibleCallback(CCNode* node, void* data)
{
	if (node)
	{
		node->setVisible(true);
	}
}

CCSprite*  CopyLayer::getBossSprite(const char* texture)
{
	CCSprite* sprite = CCSprite::createWithSpriteFrameName(texture);
	return sprite;
}

Button* CopyLayer::getItemImage(int copyId)
{
	CopyData* data = getcopyDataforId(copyId);
	if (!data->isLocked())
	{
		return m_Button[copyId - 1];
	}
	return NULL;
}

void CopyLayer::onCopyItemCallBack(CCObject* pSender)
{
// 	CCMenuItemImage* pImageItem = dynamic_cast<CCMenuItemImage*>(pSender);
// 	int id = pImageItem->getTag();
// 	CopyData* data = getcopyDataforId(id);
// 
// 	onCopyItemClick(NULL, data);
}

CopyData* CopyLayer::getcopyDataforId(int id)
{
	std::vector<CopyData*>::iterator iter = m_vCopyData.begin();
	for (; iter != m_vCopyData.end(); ++iter)
	{
		if (id == (*iter)->getId())
		{
			return (*iter);
		}
	}
	return NULL;
}

void CopyLayer::onTouchUp(Button* button)
{
	int id = button->getTag();
	CopyData* data = getcopyDataforId(id);

	onCopyItemClick(NULL, data, button);
}
