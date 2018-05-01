#include "PropLayer.h"
#include "DataPool.h"

#define SKILL_ITEM_WIDTH             57.f
#define SKILL_ITEM_HEIGHT            64.f
#define SKILL_FIRST_ROW_HEIGHT       67.f

const int kForeverActionTag = 0x0001;
const int kPropFirstTag = 0x0002;
const int kPropRefreshTag = 0x0003;

PropLayer::PropLayer(int nCopyId)
: m_nCopyId(nCopyId)
, m_pDelegate(NULL)
, m_PropCount(0)
, m_isFirstPropRunAction(false)
, m_isShancreate(false)
, m_idThirdShanCanDelete(false)
{
}

PropLayer::~PropLayer()
{
}

PropLayer* PropLayer::create(int nCopyId)
{
	PropLayer* layer = new PropLayer(nCopyId);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool PropLayer::init()
{
	if (!CCLayer::init()) {
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("propShan.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("refreshprop.plist");

	int firstPropNum = 0;
	int secondPropNum = 0; 
	int thirdPropNum = 0;
	int fourPropNum = 0;
	std::vector<const ConfigPropItem*> propItemList = getConfigProp().getConfigPropItemList();
	std::vector<const ConfigPropItem*>::iterator propItemIter = propItemList.begin();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const std::map<u32, u32>& refPropList = getUserStoreData().getPropNumList();
	std::map<u32, u32>::const_iterator iter = refPropList.begin();

	for (; iter != refPropList.end(); ++iter) {
		const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(iter->first);
		if (pCfgPropItem == NULL) {
			continue;
		}

		PropData* pPropData = new PropData(iter->first, iter->second);
		m_vPropData.push_back(pPropData);

		switch(iter->first)
		{
		case 1:
			firstPropNum = iter->second;
			break;
		case 2:
			secondPropNum = iter->second;
			break;
		case 3:
			thirdPropNum = iter->second;
			break;
		case 4:
			fourPropNum = iter->second;
			break;

		default:
			break;
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////
	
	int UserPropNum = 0;
	for (; propItemIter!= propItemList.end(); ++propItemIter) // 循环道具表
	{
		const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem((*propItemIter)->id);
		if (pCfgPropItem == NULL) {
			continue;
		}
		switch ((*propItemIter)->id)
		{
		case 1:
			UserPropNum = firstPropNum;
			break;
		case 2:
			UserPropNum = secondPropNum;
			break;
		case 3:
			UserPropNum = thirdPropNum;
			break;
		case 4:
			UserPropNum = fourPropNum;
			break;
		default:
			break;
		}
		PropButton* button = PropButton::create((*propItemIter)->id,UserPropNum, pCfgPropItem->texture.c_str());
		if (button) {
			button->registerControlEvent();
			button->setDelegate(this);
			button->setAnchorPoint(CCPointZero);
			button->setPosition(getItemPoint());
			++m_PropCount;
			this->addChild(button, 2);
		}


	}
	///////////////////////////////////////////////////////////////////////////

	return true;
}

void PropLayer::loadUserPropData()
{
	m_vPropData.clear();

	const std::map<u32, u32>& refPropList = getUserStoreData().getPropNumList();
	std::map<u32, u32>::const_iterator iter = refPropList.begin();

	for (; iter != refPropList.end(); ++iter) {
		const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(iter->first);
		if (pCfgPropItem == NULL) {
			continue;
		}

		PropData* pPropData = new PropData(iter->first, iter->second);
		m_vPropData.push_back(pPropData);
	}
}

void PropLayer::onTouchUp(Button* button)
{
	PropButton* prop = dynamic_cast<PropButton*>(button);
	if (prop && m_pDelegate) {
		PropData* pData = getPropData(prop->getPropId());
		//if (pData) {
			if (prop->getChildByTag(0x100) == NULL)
			{
				if (m_pDelegate)
				{
					// 如果正在消除，那么放大镜就return掉
					if (m_pDelegate->onIsUserRemovegemNow() && prop->getPropId() == 1) 
					{
						return;
					}	
				}
				addUsedPropNum(prop->getPropId(), 1);
				createPropClickRest(prop);
				m_pDelegate->onPropClick(prop->getPropId());
				if (prop->getPropId() == 1 && m_isShancreate) // 如果是放大镜的话，并且有再闪烁
				{
					// 讲闪烁这个精灵关闭
					m_isFirstPropRunAction = true;
					if (m_pDelegate)
					{
						m_pDelegate->onFirstPropShanCloseSendToFight();
					}
				}
				if (((prop->getPropId() == 3) && (prop->getChildByTag(kPropRefreshTag) != NULL)) || 
						((prop->getPropId() == 4) && (getPropButton(3)->getChildByTag(kPropRefreshTag) != NULL)))
				{
					m_idThirdShanCanDelete = true;
				}
			}
		// }
	}
}

void PropLayer::createPropClickRest(PropButton* prop)
{
	CCProgressTo* to1 = CCProgressTo::create(2, 0); // 两秒时间走到0 
	CCProgressTimer* left = CCProgressTimer::create(CCSprite::create("skillProgress.png"));
	left->setType( kCCProgressTimerTypeRadial);
	left->setPercentage(99); // 初始量
	left->setTag(0x100);
	prop->addChild(left, 2);
	left->setAnchorPoint(CCPointZero);
	left->setPosition(ccp(0, 0));
	left->setReverseProgress(true);
	CCSequence* actionQueue = CCSequence::create(to1, CCCallFuncN::create(this, 
		callfuncN_selector(PropLayer::propClickRestCallback)), NULL);
	left->runAction(actionQueue);
	// this->schedule(schedule_selector(SkillLayer::updateLevelProgress1), 0.05f);
}
void PropLayer::propClickRestCallback(CCNode* node)
{
	if (node)
	{
		node->removeFromParent();
	}
}

CCPoint PropLayer::getItemPoint()
{
// 	return ccp((this->getChildrenCount() % 2) * SKILL_ITEM_WIDTH, 
// 		SKILL_FIRST_ROW_HEIGHT - ((this->getChildrenCount() / 2) * SKILL_ITEM_HEIGHT));
	CCPoint point;
	point.y = 5;
	point.x = m_PropCount* 100;
	return point;
}

void PropLayer::addUsedPropNum(int nPropId, int num)
{
	PropData* pData = getPropData(nPropId);
	PropButton* pButton = getPropButton(nPropId);
	if (pData && pButton) {
		// pData->addUsedPropNum(num);
		pButton->setPropNum(pData->getEnablePropNum());
	}
}

PropButton* PropLayer::getPropButton(int nPropId)
{
	for (unsigned int i = 0; i < getChildrenCount(); ++i) {
		PropButton* button = dynamic_cast<PropButton*>(getChildren()->objectAtIndex(i));
		if (button->getPropId() == nPropId) {
			return button;
		}
	}
	return NULL;
}

PropData* PropLayer::getPropData(int nPropId)
{
	std::vector<PropData*>::iterator iter = m_vPropData.begin();
	for (; iter != m_vPropData.end(); ++iter) {
		if ((*iter)->getId() == nPropId) {
			return *iter;
		}
	}
	return NULL;
}

void PropLayer::setPropNum()
{
	for (int i = 1; i < 5; ++i)
	{
		int num = getUserStoreData().getPropNum(i);
		PropButton* prop = this->getPropButton(i);
		prop->setPropNum(num);
	}
}

void PropLayer::setRefreshPropBlink()
{
	// 创建一个刷新卷轴闪烁的效果
	PropButton* prop = getPropButton(3);
	if (prop->getChildByTag(kPropRefreshTag))
	{
		return;
	}
	
	CCSprite* sprite = CCSprite::createWithSpriteFrameName("prop1.png");
	sprite->setPosition(ccp(40.5, 40));
	sprite->setTag(kPropRefreshTag);
	prop->addChild(sprite);

	CCActionInterval* animate = getProprefreshAnimate();
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(PropLayer::thirdPropActionCallback), NULL);
	CCSequence* queue = CCSequence::create(animate, callback, NULL);
	CCRepeatForever* action = CCRepeatForever::create(queue);
	sprite->runAction(action);

}

void PropLayer::setShowHintPropBlink()
{
	// 创建一个让放大镜闪烁的效果
	CCSprite* sprite = (CCSprite*)getChildByTag(kPropFirstTag);
	if (sprite)
	{
		return;
	}
	if (!m_isShancreate)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("propShan.plist");
		PropButton* prop = getPropButton(1);
		const char* frame = "prop2.png";
		CCSprite*  shan = CCSprite::createWithSpriteFrameName(frame);
		shan->setTag(kPropFirstTag);
		shan->setPosition(ccp(40.5, 40));
		prop->addChild(shan);

		CCActionInterval* animate = getPropFirstAnimate();
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(PropLayer::firstPropActionCallback), NULL);
		CCSequence* queue = CCSequence::create(animate, callback, NULL);
		CCRepeatForever* action = CCRepeatForever::create(queue);
		action->setTag(kForeverActionTag);
		shan->runAction(action);
		m_isShancreate = true;
	}
}

CCActionInterval* PropLayer::getPropFirstAnimate()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i <= 2; i++)
	{
		const char* str = CCString::createWithFormat("prop%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}

CCActionInterval* PropLayer::getProprefreshAnimate()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 3; i <= 4; i++)
	{
		const char* str = CCString::createWithFormat("prop%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}

	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}
void PropLayer::firstPropActionCallback(CCNode* node, void* data)
{
	if (m_isFirstPropRunAction)
	{
		node->stopAllActions();
		node->removeFromParent();
		m_isFirstPropRunAction = false;
		m_isShancreate = false;
	}
}

void PropLayer::setFirstPropShanDown(bool value)
{
	if (m_isShancreate)
	{
		m_isFirstPropRunAction = true;
	}
}

void PropLayer::setThirePropShanDown(bool value)
{
	m_idThirdShanCanDelete = true;
}

void PropLayer::thirdPropActionCallback(CCNode* node, void* data)
{
	if (m_idThirdShanCanDelete)
	{
		node->stopAllActions();
		if (node)
		{
			node->removeFromParent();
		}
		m_idThirdShanCanDelete = false;
	}
}
