#ifndef _DATAPOOL_STATUS_DATA_H_
#define _DATAPOOL_STATUS_DATA_H_

#include "cocos2d.h"
#include "ConfigSpriteSkillStatus.h"

USING_NS_CC;

enum StatusType
{
	ST_UNKNOWN = 0,
	ST_DAMAGE,			// 伤害
	ST_DIZZY,			// 晕眩
	ST_EXCITING,		// 兴奋
	ST_DEFENCE,			// 防御
	ST_ICE,				// 冰
	ST_RETURN,			// 回头
	ST_MIXED,			// 混乱
};

class StatusData
{
public:
	StatusData(const ConfigSpriteSkillStatusItem* pCfgStatusItem);
	~StatusData();

public:
	StatusType getStatusType() const;
	
	// 状态是否结束
	bool isOver() const;
	bool infinite() const;

	void setDeal(bool deal) { m_bDeal = deal; }
	bool isDeal() const { return m_bDeal; }

	void addDeltaTime(float delta) { m_fDeltaTime += delta; }

private:
	bool m_bDeal;
	float m_fDeltaTime;				// 状态从创建到现在经过了多长时间
	const ConfigSpriteSkillStatusItem* m_pCfgStatusItem;
};

#endif // _DATAPOOL_STATUS_DATA_H_