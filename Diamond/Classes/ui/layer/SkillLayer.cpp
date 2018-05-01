#include "SkillLayer.h"
#include "SkillButton.h"
#include "DataPool.h"

#define SKILL_ITEM_WIDTH             57.f
#define SKILL_ITEM_HEIGHT            64.f
#define SKILL_FIRST_ROW_HEIGHT      139.f

SkillLayer::SkillLayer(int nCopyId)
: m_pDelegate(NULL)
, m_nCopyId(nCopyId)
, m_pro1(0)
, m_pro2(0)
, m_pLeft1(NULL)
, m_pLeft2(NULL)
, m_pSkill_button(NULL)
, m_SkillCount(0)
{
}

SkillLayer::~SkillLayer()
{
}

SkillLayer* SkillLayer::create(int nCopyId)
{
	SkillLayer* layer = new SkillLayer(nCopyId);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool SkillLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skillRotatesprite.plist");

	std::vector<const ConfigCopySkillItem*> vCopySkill = getConfigCopySkill().getConfigCopySkillItemList(m_nCopyId);
	std::vector<const ConfigCopySkillItem*>::iterator iter = vCopySkill.begin();
	for (; iter != vCopySkill.end(); ++iter) {
		SpriteSkillData* data = new SpriteSkillData((*iter)->skill_id);
		m_vSpriteSkill.push_back(data);
	}

	return true;
}

void SkillLayer::onEnter()
{
	CCLayer::onEnter();
	createSkillItem(); // 创建那些技能按钮的函数
}

void SkillLayer::onTouchUp(Button* button)
{
	SkillButton* skill_button = dynamic_cast<SkillButton*>(button);
	if (skill_button)
	{
		if (m_pDelegate)
		{
			const ConfigSpriteSkillItem* skill = getConfigSpriteSkill().getConfigSpriteSkillItem(skill_button->getSkillId());
			// if (skill && m_pDelegate->enableLaunch(skill->consume))
			if (skill && m_pDelegate->onGetEnabel(skill_button->getSkillId()))// 判断技能释放可以使用
			{	
// 				if (skill_button->getChildByTag(0x100) == NULL)
// 				{
					m_pDelegate->enableLaunch(skill->consume); // 使用技能减掉能量
// 					CCProgressTo* to1 = CCProgressTo::create(5, 100);
// 					CCProgressTimer* left = CCProgressTimer::create(CCSprite::create("skillProgress.png"));
// 					left->setType( kCCProgressTimerTypeRadial);
// 					left->setPercentage(100);
// 					left->setTag(0x100);
// 					skill_button->addChild(left, 1);
// 					left->setAnchorPoint(CCPointZero);
// 					left->setPosition(ccp(0, 0));
// 					left->setReverseProgress(true);
// 					CCSequence* actionQueue = CCSequence::create(to1, CCCallFuncN::create(this, 
// 						callfuncN_selector(SkillLayer::skillClickRestCallback)), NULL);
// 
// 					left->runAction(actionQueue);
					SpriteSkillResult* result = SpriteSkillManager::shareInstance().getSkillResult(skill_button->getSkillId());
					m_pDelegate->onSkillClick(result);
//				}
			}
		}
	}
}


void SkillLayer::countRunAnimateNum(int fullEnergy, int curEnergy) // 设置旋转的精灵显示几个
{
	int runActionNum = curEnergy/ 60;
	for (int i = 0; i < 4; i++)
	{
		m_pRotateSprite[i]->setVisible(false);
	}
	for (int  i = 0;  i < runActionNum;  i++)
	{
		m_pRotateSprite[i]->setVisible(true);
	}
}

void SkillLayer::skillClickRestCallback(CCNode* node)
{
	if (node)
	{
		node->removeFromParent();
	}
}



void SkillLayer::createSkillItem()
{
	std::vector<SpriteSkillData*>::iterator iter = m_vSpriteSkill.begin();
	for (int i = 0; (i < m_vSpriteSkill.size())||(iter != m_vSpriteSkill.end()); ++i, ++iter)
	{
		// 创建精灵
		SkillButton* button = SkillButton::createWithSpriteFrameName((*iter)->getTexture1());
		button->setDelegate(this);
		button->setSkillid((*iter)->getSkillId());
		button->registerControlEvent();
		button->setAnchorPoint(CCPointZero);
		button->setPosition(getItemPoint());
		++m_SkillCount;

		//旋转精灵
		m_pRotateSprite[i] = CCSprite::create("skillRotatesprite1.png");
		m_pRotateSprite[i]->setAnchorPoint(CCPointZero);
		m_pRotateSprite[i]->setPosition(ccp(0, -1));
		button->addChild(m_pRotateSprite[i]);
		m_pRotateSprite[i]->setVisible(false);
		CCAnimate* action = getSkillCanUseAnimateAction();
		CCRepeatForever* rotateForever = CCRepeatForever::create(action);
		m_pRotateSprite[i]->runAction(rotateForever);

		this->addChild(button);
		
	}
	// 之前的版本，没有配技能的话就上锁
// 	for (int i=m_vSpriteSkill.size(); i < 6; i++)
// 	{
// 		// SkillButton* button = SkillButton::createWithSpriteFrameName("skillLock.png");
// 		CCSprite* button = CCSprite::create("skillLock.png");
// 		button->setAnchorPoint(CCPointZero);
// 		button->setPosition(getItemPoint());
// 		this->addChild(button);
// 	}
}

CCPoint SkillLayer::getItemPoint()
{
// 	return ccp((this->getChildrenCount() % 2) * SKILL_ITEM_WIDTH, 
// 		SKILL_FIRST_ROW_HEIGHT - ((this->getChildrenCount() / 2) * SKILL_ITEM_HEIGHT));
 	CCPoint point;
	point.x = 0;
 	point.y = m_SkillCount*123;
 	return point;

}

CCAnimate*  SkillLayer::getSkillCanUseAnimateAction()
{
	CCArray* frames = CCArray::create();
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int i = 1; i < 9; ++i)
	{
		const char* str = CCString::createWithFormat("skillRotatesprite%d.png", i)->getCString();
		CCSpriteFrame* frame = pCache->spriteFrameByName(str);
		frames->addObject(frame);
	}
	return CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f));
}