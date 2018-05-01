#ifndef _DATAPOOL_EFFECT_DATA_H_
#define _DATAPOOL_EFFECT_DATA_H_

#include <vector>
#include "cocos2d.h"
#include "ActionType.h"
#include "StatusData.h"
#include "SpriteSkillManager.h"
#include "MonsterNode.h"

USING_NS_CC;

class EffectData
{
public:
	EffectData();
	virtual ~EffectData();

	void setResult(SpriteSkillResult* result) { m_pResult = result; }
	void setBeAttackedMonster(const std::vector<MonsterNode*>& vBeAttackedMonster) { m_vBeAttackedMonster = vBeAttackedMonster; }
	
	ActionType getActionType() const;
	bool isGroup() const;
	const CCPoint& getGroupPos() const;
	const std::vector<MonsterNode*>& getBeAttackedMonster() const;
	int getDamage() const;
	int getRecovery() const;
	bool statusEmpty() const;
	std::vector<ConfigSpriteSkillStatusItem*> getSkillStatus();

public:
	std::vector<MonsterNode*> m_vBeAttackedMonster;
	SpriteSkillResult* m_pResult;
};

#endif // _DATAPOOL_EFFECT_DATA_H_