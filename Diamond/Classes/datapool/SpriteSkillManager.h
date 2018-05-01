#ifndef _DATAPOOL_SPRITE_MANAGER_H_
#define _DATAPOOL_SPRITE_MANAGER_H_

#include "ConfigSpriteSkill.h"
#include "ConfigSpriteSkillStatus.h"
#include "cocos2d.h"
#include "ActionType.h"

USING_NS_CC;

// ���鼼������
enum SpriteSkillType
{
	SPRITE_SKILL_ATTACK,    // ����
	SPRITE_SKILL_RECOVERY,  // ��Ѫ
	SPRITE_SKILL_DEFENCE,   // ����
	SPRITE_SKILL_STATUS,    // ״̬
	SPRITE_SKILL_ICE,       // ��
};

// �ͷž��鼼�ܵķ��ؽ��
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
	// ��������
	ActionType m_eActionType;

	// ������ɵ��˺�
	unsigned int m_nDamage;

	// ���ܻ�Ѫ
	unsigned int m_nRecovery;

	// Ⱥ�幥�����ܵ�λ��
	CCPoint m_ptSkillPoint;

	// �Ƿ�Ⱥ�幥��
	bool m_bGroup;

	// ״̬
	std::vector<ConfigSpriteSkillStatusItem*> m_vSkillStatus;
};

// ���ܻ���
class SpriteSkill : public CCObject
{
public:
	SpriteSkill() {}
	virtual ~SpriteSkill() {}

	// �����ͷŸü��ܵĽ��
	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item) = 0;
};

// ��������
class AttackSpriteSkill : public SpriteSkill
{
public:
	AttackSpriteSkill() {}
	virtual ~AttackSpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// ��Ѫ����
class RecoverySpriteSkill : public SpriteSkill
{
public:
	RecoverySpriteSkill() {}
	virtual ~RecoverySpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// ��״̬����
class StatusSpriteSkill : public SpriteSkill
{
public:
	StatusSpriteSkill() {}
	virtual ~StatusSpriteSkill() {}

	virtual SpriteSkillResult* getSpriteSkillResult(const ConfigSpriteSkillItem* item);
};

// �����Ҵ�״̬����
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
