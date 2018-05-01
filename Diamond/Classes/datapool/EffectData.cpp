#include "EffectData.h"
#include <algorithm>

EffectData::EffectData()
: m_pResult(NULL)
{
}

EffectData::~EffectData()
{
}

ActionType EffectData::getActionType() const
{
	if (m_pResult)
	{
		return m_pResult->getActionType();
	}
	return AT_UNKNOWN;
}

bool EffectData::isGroup() const
{
	if (m_pResult)
	{
		return m_pResult->isGroup();
	}
	return false;
}

const CCPoint& EffectData::getGroupPos() const
{
	if (m_pResult)
	{
		return m_pResult->getGroupSkillPos();
	}
	return CCPointZero;
}

const std::vector<MonsterNode*>& EffectData::getBeAttackedMonster() const
{
	return m_vBeAttackedMonster;
}

int EffectData::getDamage() const
{
	if (m_pResult)
	{
		return m_pResult->getDamage();
	}
	return 0;	
}

int EffectData::getRecovery() const
{
	if (m_pResult)
	{
		return m_pResult->getRecovery();
	}
	return 0;	
}

bool EffectData::statusEmpty() const
{
	if (m_pResult)
	{
		return m_pResult->getSkillStatus().empty();
	}
	return false;
}

std::vector<ConfigSpriteSkillStatusItem*> EffectData::getSkillStatus()
{
	std::vector<ConfigSpriteSkillStatusItem*> vSkillStatus;
	if (m_pResult)
	{
		return m_pResult->getSkillStatus();
	}
	return vSkillStatus;
}