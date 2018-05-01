#ifndef _MONSTER_DATA_H_
#define _MONSTER_DATA_H_

#include <vector>
#include <string>
#include "cocos2d.h"
#include "StatusData.h"
#include "ConfigMonster.h"
#include "ConfigSpriteSkillStatus.h"
#include "MarcoDefine.h"

USING_NS_CC;

enum SituationType
{
	SITUATION_MOVE,
	//SITUATION_RETURN,
	SITUATION_ICE,
	SITUATION_ATTACK,
	STTUATION_SKILL,
	STTUATION_UNKNOW,
	STTUATION_SPECIAL,
};

class MonsterData
{
public:
	MonsterData(const ConfigMonsterItem* pCfgItem);
	~MonsterData();

	float getLaunchPosPercent() const;
	float getWeight() const;
	float getMoveVelocity() const;
	bool isBoss() const;

	void subHp(int hp) { m_nHp = MAX(0, m_nHp - hp); }
	int getHp() const { return m_nHp; }
	bool isDead() const { return (m_nHp <= 0); }
	u32 getSkillId() const;
	int getBeginHp()const { return m_nBeginHp; }

	void subBackHp(int hp) { m_nBackHp = MAX(0, m_nBackHp - hp); }
	//是否可攻击该怪物
	bool isAttackEnabled(){return (m_nBackHp != 0);}

	// 怪物攻击的间隔时间
	bool enableLaunch() const;
	void addAttackDeltaTime(float delta) { m_fAttackDeltaTime += delta; }
	void launchAttack() { m_fAttackDeltaTime = 0.f; }

	void setSituationType(SituationType type) { m_eSituationType = type; }
	SituationType getSituationType() const { return m_eSituationType; }

	void setRowIndex(int index) { m_nRowIndex = index; }
	int getRowIndex() const { return m_nRowIndex; }

	void setId(int id) { m_nId = id; }
	int getId() const { return m_nId; }
	
	int getAggress() const;
	// 被技能攻击的优先级
	int getBeSkillAttackedPriority();

	bool isOwnUser() const;
	bool isDizzy() const;
	void addStatusDeltaTime(float delta);
	std::vector<StatusData*> getOverStatus();
	std::vector<StatusData*> getNewStatus();
	void clearOverStatus();
	void addStatus(ConfigSpriteSkillStatusItem* pCfgItem);

	std::vector<std::string> getMoveAnimateFrames();
	std::vector<std::string> getAttackAnimateFrames();
	std::vector<std::string> getBeAttackedAnimateFrames();
	std::vector<std::string> getRestAnimateFrames();

	bool isExsitStatus(StatusType type) const;

private:
	int m_nId;
	int m_nHp;
	int m_nBackHp;
	int m_nBeginHp;
	float m_fAttackDeltaTime;
	int m_nRowIndex;
	bool m_bAttackEnabled;
	SituationType m_eSituationType;
	std::vector<StatusData*> m_vStatus;
	const ConfigMonsterItem* m_pCfgMonster;
};

#endif // _MONSTER_DATA_H_