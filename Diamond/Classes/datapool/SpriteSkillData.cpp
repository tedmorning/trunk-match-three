#include "SpriteSkillData.h"
#include "DataPool.h"

SpriteSkillData::SpriteSkillData(unsigned int skill_id)
{
	m_pCfgSpriteSkillItem = getConfigSpriteSkill().getConfigSpriteSkillItem(skill_id);
	if (m_pCfgSpriteSkillItem)
	{
		m_fCoolTime = m_pCfgSpriteSkillItem->cool_time;
	}
}

SpriteSkillData::~SpriteSkillData()
{
}

void SpriteSkillData::setLaunch()
{
	if (m_pCfgSpriteSkillItem)
	{
		m_fCoolTime = m_pCfgSpriteSkillItem->cool_time;
	}
}

unsigned int SpriteSkillData::getSkillId() const
{
	if (m_pCfgSpriteSkillItem)
	{
		return m_pCfgSpriteSkillItem->id;
	}
	return 0;
}

const char* SpriteSkillData::getTexture1() const
{
	if (m_pCfgSpriteSkillItem)
	{
		return m_pCfgSpriteSkillItem->tex1.c_str();
	}
	return NULL;
}

const char* SpriteSkillData::getTexture2() const
{
	if (m_pCfgSpriteSkillItem)
	{
		return m_pCfgSpriteSkillItem->tex2.c_str();
	}
	return NULL;
}