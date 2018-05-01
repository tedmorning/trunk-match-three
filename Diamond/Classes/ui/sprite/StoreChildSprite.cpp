#include "StoreChildSprite.h"
#include "DataPool.h"

StoreChildSprite::StoreChildSprite(): m_pUserPropNum(NULL)
{
}

StoreChildSprite::StoreChildSprite(const char* priceBuf,const char* numBuf, const std::string& image,unsigned int idx)
{
	m_priceBuf = priceBuf;
	// m_priceBuf.insert(0,"RMB ");
	m_numBuf = numBuf;
	m_image = image;
	m_index = idx;
}

StoreChildSprite::~StoreChildSprite()
{
}

StoreChildSprite* StoreChildSprite::create(const char* priceBuf,const char* numBuf, const std::string& image, unsigned int idx)
{
	StoreChildSprite* sprite = new StoreChildSprite(priceBuf, numBuf, image,idx);
	if (sprite && sprite->init())
	{
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

bool StoreChildSprite::init()
{
	if (!CCNode::init())
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("props.plist");

	CCSprite* sprite = CCSprite::create("cellBackGround.png"); // 创建背景图 283*156
	sprite->setPosition(ccp(324, 100));

	// 创建购买按钮 
	CCMenuItemImage* pBuyItem = CCMenuItemImage::create("buy1.png", "buy2.png", this,
		menu_selector(StoreChildSprite::buyItemCallback));
	pBuyItem->setTag(m_index);
	pBuyItem->setPosition(ccp(0 ,0));
	CCMenu* pMenu = CCMenu::create(pBuyItem,NULL);
	pMenu->setPosition(ccp(160,45));
	sprite->addChild(pMenu);

	// 创建价格 
	CCSprite* pRMBbackGround = CCSprite::create("RMBBackGround.png");
	pRMBbackGround->setPosition(ccp(162,105));

	CCLabelTTF* pRMB = CCLabelTTF::create(m_priceBuf.c_str(), "Arial", 24);
	pRMB->setPosition(ccp(52,23));
	pRMBbackGround->addChild(pRMB);
	sprite->addChild(pRMBbackGround);
	
	// 创建技能背景图 
	CCSprite* pPropBackGround = CCSprite::create("PropBackGround.png");
	pPropBackGround->setPosition(ccp(55, 70));
	sprite->addChild(pPropBackGround);

	CCSprite* prop = CCSprite::createWithSpriteFrameName(m_image.c_str());
	prop->setPosition(ccp(40, 55));
	pPropBackGround->addChild(prop, 2);

	// 用户道具数量
	u32 propNum = getUserStoreData().getPropNum(m_index);
	CCString* strPropNum = CCString::createWithFormat("%d", propNum);
	m_pUserPropNum = CCLabelTTF::create(strPropNum->getCString(), "Arial", 24);
	m_pUserPropNum->setPosition(ccp(60, 85));
	prop->addChild(m_pUserPropNum);


	if(strcmp(m_image.c_str(), "prop_mushroom.png") == 0)
	{
		CCSprite* spr = CCSprite::create("shengnneng.png");
		spr->setPosition(ccp(50, 90));
		sprite->addChild(spr, 1);

		CCParticleSystem* pParticle = CCParticleSystemQuad::create("sheng.plist");
		pParticle->setPosition(ccp(50, 90));
		sprite->addChild(pParticle, 1);
	}
// 	CCLabelTTF* numBuf = CCLabelTTF::create(m_numBuf.c_str());
// 	numBuf->setPosition(ccp(40, 20));
// 	pPropBackGround->addChild(numBuf);

	this->addChild(sprite);

	return true;
}

void StoreChildSprite::buyItemCallback(CCObject* pSender) // 购买事件处理
{
	CCMenuItemImage* pImageItem = dynamic_cast<CCMenuItemImage*>(pSender);

	const ConfigPropItem* propItem = getConfigProp().getConfigPropItem(m_index);
	u32 gold = getUserStoreData().getGold();

	if(gold < propItem->price)
	{
		return;
	}
	const char* propStr = m_pUserPropNum->getString();
	int propnum = atoi(propStr);
	propnum += 1;
	CCString* str = CCString::createWithFormat("%d",propnum);
	m_pUserPropNum->setString(str->getCString());
	if (m_pDelegate && pImageItem)
	{
		m_pDelegate->OnBuyItemChick(pImageItem->getTag()); // 传递给StoreTableView
	}
}

