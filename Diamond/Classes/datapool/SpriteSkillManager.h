#ifndef _DATAPOOL_SPRITE_MANAGER_H_
#define _DATAPOOL_SPRITE_MANAGER_H_

#include "ConfigSpriteSkill.h"
#include "ConfigSpriteSkillStatus.h"
#include "cocos2d.h"
#include "ActionType.h"

USING_NS_CC;

// 精灵技能类型
enum SpriteSkillType
{
	SPRITE_SKILL_ATTACK,    // 攻击
	SPRITE_SKILL_RECOVERY,  // 回血
	SPRITE_SKILL_DEFENCE,   // 防御
	SPRITE_SKILL_STATUS,    // 状态
	SPRITE_SKILL_ICE,       // 冰
};

// 释放精灵技能的返回结果
class SpriteSkillResult : public CCObject
{
public:
	SpriteSkillResult() : m_nDamage(0), m_nRecovery(0), m_eActionType(AT_UNKNOWN) {}
	~SpriteSkillResult() {}

public:
	static SpriteSkillResult* create(ActionType type, bool group);

public:
	void setActionType(ActionType type) { m_eActionType = type; }
	ActionType getActionType() const {  return m_eActionType; }

	void setGroup(bool group) { m_bGroup = group; }
	bool isGroup() const { return m_bGroup; }

	void setDamage(unsigned int damage) { m_nDamage = damage; }
	unsigned int getDamage() const { return m_nDamage; }

	void setRecovery(unsigned int recovery) { m_nRecovery = recovery; }
	unsigned int getRecovery() const { return m_nRecovery; }

	bool skillStatusEmpty() const { return m_vSkillStatus.empty(); }
	const std::vector<ConfigSpriteSkillStatusItem*>& getSkillStatus() const { return m_vSkillStatus; }
	void addSkillStatus(ConfigSpriteSkillStatusItem* item) { if (item) { m_vSkillStatus.push_back(item); } }

	void setGroupSkillPos(const CCPoint& pt) { m_ptSkillPoint = pt; }
	const CCPoint& getGroupSkillPos() const { return m_ptSkillPoint; }

private:
	// 动作类型
	ActionType m_eActionType;

	// 技能造成的伤害
	unsigned int m_nDamage;

	// 技能回血
	unsigned int m_nRecovery;

	// 群体攻击技能的位置
	CCPoint m_ptSkillPoint;

	// 是否群体攻击
	bool m_bGroup;

	// 状态
	std::vector<ConfigSpriteSkillStatusItem*> m_vSkillStatus;
};

// 技能基类
class SpriteSkill : public CCObject
{
public:
	SpriteSkill() {}
	virtual ~SpriteSkill() {}

	// 返回释放该技能的结果
	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item) = 0;
};

// 攻击技能
class AttackSpriteSkill : public SpriteSkill
{
public:
	AttackSpriteSkill() {}
	virtual ~AttackSpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// 回血技能
class RecoverySpriteSkill : public SpriteSkill
{
public:
	RecoverySpriteSkill() {}
	virtual ~RecoverySpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// 带状态技能
class StatusSpriteSkill : public SpriteSkill
{
public:
	StatusSpriteSkill() {}
	virtual ~StatusSpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// 攻击且带状态技能
class AttackStatusSpriteSkill : public StatusSpriteSkill
{
public:
	AttackStatusSpriteSkill() {}
	~AttackStatusSpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

class SpriteSkillManager
{
private:
	SpriteSkillManager() {}
	~SpriteSkillManager() {}

public:
	SpriteSkillResult* getSkillResult(unsigned int skill_id);

	static SpriteSkillManager& shareInstance();
};

#endif // _DATAPOOL_SPRITE_DATA_H_
