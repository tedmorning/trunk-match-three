#include "SpriteSkillManager.h"
#include "DataPool.h"
#include "cocos2d.h"

SpriteSkillResult* SpriteSkillResult::create(ActionType type, bool group)
{
	SpriteSkillResult* result = new SpriteSkillResult();
	result->autorelease();
	result->retain();
	result->setActionType(type);
	result->setGroup(group);
	return result;
}

SpriteSkillResult* AttackSpriteSkill::getSpriteSkillResult(const ConfigSpriteSkillItem* item)
{
	SpriteSkillResult* result = SpriteSkillResult::create((ActionType)item->action_type, item->group);
	result->setDamage(item->power);
	// result->setDamage(0);
	return result;
}

SpriteSkillResult* RecoverySpriteSkill::getSpriteSkillResult(const ConfigSpriteSkillItem* item)
{
	SpriteSkillResult* result = SpriteSkillResult::create((ActionType)item->action_type, item->group);
	result->setRecovery(item->recovery);
	return result;
}

SpriteSkillResult* StatusSpriteSkill::getSpriteSkillResult(const ConfigSpriteSkillItem* item)
{
	SpriteSkillResult* result = SpriteSkillResult::create((ActionType)item->action_type, item->group);
	
	if (item->status1 != 0)
	{
		result->addSkillStatus(const_cast<ConfigSpriteSkillStatusItem*>(
			getConfigSpriteSkillStatus().getSpriteSkillStatusItem(item->status1)));
	}
	if (item->status2 != 0)
	{
		result->addSkillStatus(const_cast<ConfigSpriteSkillStatusItem*>(
			getConfigSpriteSkillStatus().getSpriteSkillStatusItem(item->status2)));
	}

	return result;
}

SpriteSkillResult* AttackStatusSpriteSkill::getSpriteSkillResult(const ConfigSpriteSkillItem* item)
{
	SpriteSkillResult* result = StatusSpriteSkill::getSpriteSkillResult(item);
	if (result && item)
	{
		result->setDamage(item->power);
	}
	return result;
}

SpriteSkillResult* SpriteSkillManager::getSkillResult(unsigned int skill_id)
{
	const ConfigSpriteSkillItem* item = getConfigSpriteSkill().getConfigSpriteSkillItem(skill_id);
	CCAssert(item, "");
	if (item == NULL)
	{
		return NULL;
	}

	SpriteSkill* sprite_skill = NULL;
	if (item->recovery != 0)
	{
		sprite_skill = new RecoverySpriteSkill();
		sprite_skill->autorelease();
	}
	else if (item->power != 0 && item->status1 == 0)
	{
		sprite_skill = new AttackSpriteSkill();
		sprite_skill->autorelease();
	}
	else if (item->power != 0 && item->status1 != 0)
	{
		sprite_skill = new AttackStatusSpriteSkill();
		sprite_skill->autorelease();
	}
	else if (item->power == 0 && item->status1 != 0)
	{
		sprite_skill = new StatusSpriteSkill();
		sprite_skill->autorelease();
	}

	if (sprite_skill)
	{
		return sprite_skill->getSpriteSkillResult(item);
	}

	return NULL;
}

SpriteSkillManager& SpriteSkillManager::shareInstance()
{
	static SpriteSkillManager skill_manager;
	return skill_manager;
}