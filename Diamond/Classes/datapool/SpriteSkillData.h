#ifndef _DATAPOOL_SPRITE_SKILL_DATA_H_
#define _DATAPOOL_SPRITE_SKILL_DATA_H_

#include "ConfigSpriteSkill.h"
#include "cocos2d.h"

USING_NS_CC;

class SpriteSkillData
{
public:
	SpriteSkillData(unsigned int skill_id);
	~SpriteSkillData();

	float getCoolTime() const { return m_fCoolTime; }
	void subTime(float time) { m_fCoolTime = MAX(0.f, m_fCoolTime - time); }

	const char* getTexture1() const;
	const char* getTexture2() const;

	// 标识技能是否可以发动
	bool enableLaunch() const { return (m_fCoolTime == 0.f); }
	void setLaunch();

	unsigned int getSkillId() const;

private:
	// 技能冷却时间（当减为0时，可以发动攻击）
	float m_fCoolTime;

	const ConfigSpriteSkillItem* m_pCfgSpriteSkillItem;
};

#endif // _DATAPOOL_SPRITE_SKILL_DATA_H_