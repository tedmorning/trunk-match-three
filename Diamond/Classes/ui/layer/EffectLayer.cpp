#include "EffectLayer.h"
#include "DataPool.h"
#include "EffectData.h"
#include "CallbackData.h"
#include "SoundManager.h"

EffectLayer::EffectLayer()
: m_pDelegate(NULL)
{
}

EffectLayer::~EffectLayer()
{
}

void EffectLayer::iceMonsterCallback(CCNode* node , void* data)// 冰化了
{
	if (m_pDelegate)
	{
		m_pDelegate->onIceSendToMonsterSkill(true); //
	}
	if (node)
	{
		node->removeFromParent();
	}
}

void EffectLayer::MonsterWithIceCallback(CCNode* node, void* data)
{
	MonsterNode* monster = reinterpret_cast<MonsterNode*>(node);
	if (monster)
	{
		monster->moveAction(CCCallFuncND::create(monster, 
			callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
	}
}
void EffectLayer::runEffect(EffectData* data)  // 执行技能效果
{
	////////////////////////////////////////////////////////////////////////
	if (data->getActionType() == AT_SPIKEDSHIELD)
	{
		SoundManager::shareSoundManager().playEffect("protect.wav", false);
	}
	else if(data->getActionType() == AT_RECOVERY) // 回复
	{
		SoundManager::shareSoundManager().playEffect("getBlood.wav", false);
	}
	
	else if (data->getActionType() == AT_FIREBALL) // 火球
	{
		SoundManager::shareSoundManager().playEffect("ghostBomb.mp3", false);
	}
	else if (data->getActionType() == AT_ICE) // 冰
	{
		SoundManager::shareSoundManager().playEffect("ice.wav", false);

		u32 cool_time = getConfigSpriteSkill().getConfigSpriteSkillItem(15)->cool_time;
		// cool_time = 2;
		std::vector<MonsterNode*> monsterNodeVec = data->getBeAttackedMonster();
		for (int i=0; i<monsterNodeVec.size(); ++i) // 将所有的怪套上冰
		{
			if (monsterNodeVec[i]->getMonsterDataWithoutConst()->getSituationType() == STTUATION_SKILL)
			{
				if (m_pDelegate)
				{
					m_pDelegate->onIceSendToMonsterSkill(false);
				}
			}
			monsterNodeVec[i]->getMonsterDataWithoutConst()->setSituationType(SITUATION_ICE);

			std::vector<std::string> vecString = monsterNodeVec[i]->getMonsterDataWithoutConst()->getBeAttackedAnimateFrames();
			const char* image = vecString[0].c_str();
// 			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(image);
// 			CCSize contentSize = monsterNodeVec[i]->getContentSize();
// 			monsterNodeVec[i]->setTexture(texture);
// 			monsterNodeVec[i]->setTextureRect(CCRectMake(0, 0, contentSize.width, contentSize.height));
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(image);
			monsterNodeVec[i]->setDisplayFrame(frame); // 设置图片的方法

			monsterNodeVec[i]->stopAllActions();
			CCPoint pos = monsterNodeVec[i]->getPosition();
			CCSprite* iceSprite = CCSprite::create("icewithMonster.png");
			iceSprite->setAnchorPoint(CCPointZero);
			CCPoint monsterPosition = monsterNodeVec[i]->getPosition();
			iceSprite->setPosition(CCPointZero);
			iceSprite->setTag(0x123);
			CCDelayTime* delayTime = CCDelayTime::create(cool_time);
			CCCallFuncND* iceSpritecallback = CCCallFuncND::create(this, callfuncND_selector(
				EffectLayer::iceMonsterCallback), NULL);

			CCCallFuncND* monsterNodeVecCallback = CCCallFuncND::create(this, callfuncND_selector(
				EffectLayer::MonsterWithIceCallback), NULL);

			CCSequence* iceQueue = CCSequence::create(delayTime, iceSpritecallback, NULL);
			CCSequence* monsterQueue = CCSequence::create(delayTime, monsterNodeVecCallback, NULL);
		 	
			monsterNodeVec[i]->runAction(monsterQueue);
			iceSprite->runAction(iceQueue);
			if (monsterNodeVec[i]->getMonsterData()->isBoss())
			{
				CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("icewithMonsterWithBoss.png");
				iceSprite->setTexture(texture);
				iceSprite->setTextureRect(CCRectMake(0, 0, 280, 272));

			}

		 	monsterNodeVec[i]->addChild(iceSprite);
		}

	}
	////////////////////////////////////////////////////////////////////////
	CCActionInterval* action = getAction(data->getActionType()); // 获取是那种技能的攻击类型
	if (action && data->getDamage() != 0)
	{
		if (data->isGroup()) // 如果是群攻击
		{
			if (data->getActionType() == AT_FIREBALL )
			{
				std::vector<MonsterNode*> monsterNodeVec = data->getBeAttackedMonster();
				for (int i = 0; i< monsterNodeVec.size(); ++i)
				{
					CCPoint pt = ccp(monsterNodeVec[i]->getContentSize().width / 2, monsterNodeVec[i]->getContentSize().height / 2);
					CCSprite* sprite = createSkillSprite(monsterNodeVec[i], pt, data->getActionType());
					sprite->setScale(1.2);
					if (sprite)
					{
						// CCActionInterval* action = getAction(data->getActionType());
						CallbackData* callback_data = new CallbackData();
						callback_data->data1 = reinterpret_cast<void*>(data);
						if ((i+1) == monsterNodeVec.size())
						{
							CCCallFuncND* callback = CCCallFuncND::create(this, 
								callfuncND_selector(EffectLayer::cbAttackActionEnd), reinterpret_cast<void*>(callback_data));
							sprite->runAction((CCAction*)CCSequence::create(action, callback, NULL)->copy()->autorelease());
						}
						else
						{
							CCCallFuncND* callback = CCCallFuncND::create(this, 
								callfuncND_selector(EffectLayer::cvRemoveFireSprite), reinterpret_cast<void*>(callback_data));
							sprite->runAction((CCAction*)CCSequence::create(action, callback, NULL)->copy()->autorelease());
						}
					}
				}
			}
			else
			{
				CCSprite* sprite = createSkillSprite(this, data->getGroupPos(), data->getActionType());
				if (sprite)
				{
					sprite->setPosition(ccp(120,320));

					CallbackData* callback_data = new CallbackData();
					callback_data->data1 = reinterpret_cast<void*>(data);

					CCCallFuncND* callback = CCCallFuncND::create(this, 
						callfuncND_selector(EffectLayer::cbAttackActionEnd), reinterpret_cast<void*>(callback_data));
					sprite->runAction(CCSequence::create(action, callback, NULL));
				}
			}
		}
		else  // 不是群攻击
		{
			const std::vector<MonsterNode*>& refMonster = data->getBeAttackedMonster();
			std::vector<MonsterNode*>::const_iterator iter = refMonster.begin();
			for (; iter != refMonster.end(); ++iter)
			{
				MonsterNode* pMonster = *iter;
				if (pMonster == NULL)
				{
					continue;
				}

				CCPoint pt = ccp(pMonster->getContentSize().width / 2, pMonster->getContentSize().height / 2);
				CCSprite* sprite = createSkillSprite(pMonster, pt, data->getActionType());
				if (sprite)
				{
					CallbackData* callback_data = new CallbackData();
					callback_data->data1 = reinterpret_cast<void*>(data);
					callback_data->data2 = reinterpret_cast<void*>(pMonster);

					CCCallFuncND* callback = CCCallFuncND::create(this, 
						callfuncND_selector(EffectLayer::cbAttackActionEnd), reinterpret_cast<void*>(callback_data));
					sprite->runAction(CCSequence::create(action, callback, NULL));
				}
			}
		}
	}
	
	if (action && data->getRecovery() != 0)
	{
		CCSprite* sprite = getActionSprite(AT_RECOVERY);
		sprite->setPosition(ccp(700, 320));
		this->addChild(sprite, 10);
		
		CCActionInterval* animate = getAction(AT_RECOVERY);
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
			EffectLayer::cbRecoveryActionEnd), reinterpret_cast<void*>(data->getRecovery()));
		sprite->runAction(CCSequence::create(animate, callback, NULL));
	}

	if ((data->getDamage() == 0) && (!getMonsterStatus(data).empty()) 
		&& (data->getActionType() != AT_ICE) && (data->getActionType() != AT_THORNICE)
		&& (data->getActionType() != AT_SHIELD) && (data->getActionType() != AT_SPIKEDSHIELD))
	{
		const std::vector<MonsterNode*>& refMonster = data->getBeAttackedMonster();
		std::vector<MonsterNode*>::const_iterator iter = refMonster.begin();
		for (; iter != refMonster.end(); ++iter)
		{
			MonsterNode* pMonster = *iter;
			if (pMonster == NULL)
			{
				continue;
			}

			m_pDelegate->onAttackMonster(pMonster, data->getDamage(), getMonsterStatus(data));
		}
	}
	
	groupStatusAction(data);
}

void EffectLayer::cvRemoveFireSprite(CCNode* node, void* data)
{
	if (node)
	{
		node->removeFromParent();
	}
}

void EffectLayer::cbAttackActionEnd(CCNode* node, void* data)
{
	if (node)
	{
		node->removeFromParent();
	}

	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	EffectData* effect_data = reinterpret_cast<EffectData*>(callback_data->data1);
	if ((effect_data->getDamage() != 0) && m_pDelegate)
	{
		if (effect_data->isGroup())
		{
			const std::vector<MonsterNode*>& refMonster = effect_data->getBeAttackedMonster();
			std::vector<MonsterNode*>::const_iterator iter = refMonster.begin();
			for (; iter != refMonster.end(); ++iter)
			{
				MonsterNode* pMonster = *iter;
				if (pMonster == NULL)
				{
					continue;
				}

				m_pDelegate->onAttackMonster(pMonster, effect_data->getDamage(), getMonsterStatus(effect_data));
			}
		}
		else
		{
			MonsterNode* pMonster = reinterpret_cast<MonsterNode*>(callback_data->data2);
			if (pMonster)
			{
				m_pDelegate->onAttackMonster(pMonster, effect_data->getDamage(), getMonsterStatus(effect_data));
			}
		}
	}

	CC_SAFE_DELETE(callback_data);
}

void EffectLayer::cbRecoveryActionEnd(CCNode* node, void* data)
{
	if (node)
	{
		node->removeFromParent();
	}

	if (m_pDelegate)
	{
		m_pDelegate->onRecoveryHp(reinterpret_cast<int>(data));
	}
}

void EffectLayer::groupStatusAction(EffectData* data)
{
	std::vector<ConfigSpriteSkillStatusItem*> vSceneStatus = getSceneStatus(data);
	if (!vSceneStatus.empty())
	{
		if (m_pDelegate)
		{
			m_pDelegate->onSceneStatus(vSceneStatus);
		}
	}
}

std::vector<ConfigSpriteSkillStatusItem*> EffectLayer::getMonsterStatus(EffectData* data)
{
	std::vector<ConfigSpriteSkillStatusItem*> vStatus;
	if (!data->getSkillStatus().empty())
	{
		std::vector<ConfigSpriteSkillStatusItem*> refSkillStatus = data->getSkillStatus();
		std::vector<ConfigSpriteSkillStatusItem*>::iterator iter = refSkillStatus.begin();
		for (; iter != refSkillStatus.end(); ++iter)
		{
			if (((StatusType)(*iter)->type != ST_ICE) && ((StatusType)(*iter)->type != ST_DEFENCE))
			{
				vStatus.push_back((*iter));
			}
		}
	}
	return vStatus;
}

std::vector<ConfigSpriteSkillStatusItem*> EffectLayer::getSceneStatus(EffectData* data)
{
	std::vector<ConfigSpriteSkillStatusItem*> vStatus;
	if (!data->getSkillStatus().empty())
	{
		std::vector<ConfigSpriteSkillStatusItem*> refSkillStatus = data->getSkillStatus();
		std::vector<ConfigSpriteSkillStatusItem*>::iterator iter = refSkillStatus.begin();
		for (; iter != refSkillStatus.end(); ++iter)
		{
			if (((StatusType)(*iter)->type == ST_ICE) 
				|| ((StatusType)(*iter)->type == ST_DEFENCE) 
				|| ((StatusType)(*iter)->type == ST_DAMAGE))
			{
				vStatus.push_back((*iter));
			}
		}
	}
	return vStatus;
}

CCSprite* EffectLayer::getActionSprite(ActionType type)
{
	CCSprite* sprite = NULL;
	switch (type)
	{
	case AT_FIREBALL:
		{
			sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
		}
		break;

	case AT_METEOROLITE:
		{
			sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
			sprite->setScale(10);
		}
		break;

	case AT_RECOVERY:
		{
			sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
			sprite->setScale(6);
		}

	default:
		break;
	}

	return sprite;
}

CCSprite* EffectLayer::createSkillSprite(CCNode* target, const CCPoint& pt, ActionType type)
{
	CCSprite* sprite = getActionSprite(type); // 创建ActionType类型对应的技能精灵
	CCAssert(sprite, "");
	if (sprite == NULL)
	{
		return NULL;
	}
	sprite->setPosition(pt);
	target->addChild(sprite);
	return sprite;
}

CCActionInterval* EffectLayer::getAction(ActionType type)
{
	if (type == AT_RECOVERY)
	{
		return recoveryAnimateAction();
	}
	return fireAnimateAction();
}

CCActionInterval* EffectLayer::fireAnimateAction()
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 1; i <= 13; ++i)
	{
		std::string str = "fire000%d.png";
		if (i >= 10)
		{
			str = "fire00%d.png";
		}

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str.c_str(), i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.05f));
}

CCActionInterval* EffectLayer::recoveryAnimateAction()
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 1; i <= 13; ++i)
	{
		std::string str = "dark_0000%d.png";
		if (i >= 10)
		{
			str = "dark_000%d.png";
		}

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str.c_str(), i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.05f));
}