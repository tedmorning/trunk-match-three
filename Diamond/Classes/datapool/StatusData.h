#ifndef _DATAPOOL_STATUS_DATA_H_
#define _DATAPOOL_STATUS_DATA_H_

#include "cocos2d.h"
#include "ConfigSpriteSkillStatus.h"

USING_NS_CC;

enum StatusType
{
	ST_UNKNOWN = 0,
	ST_DAMAGE,			// �˺�
	ST_DIZZY,			// ��ѣ
	ST_EXCITING,		// �˷�
	ST_DEFENCE,			// ����
	ST_ICE,				// ��
	ST_RETURN,			// ��ͷ
	ST_MIXED,			// ����
};

class StatusData
{
public:
	StatusData(const ConfigSpriteSkillStatusItem* pCfgStatusItem);
	~StatusData();

public:
	StatusType getStatusType() const;
	
	// ״̬�Ƿ����
	bool isOver() const;
	bool infinite() const;

	void setDeal(bool deal) { m_bDeal = deal; }
	bool isDeal() const { return m_bDeal; }

	void addDeltaTime(float delta) { m_fDeltaTime += delta; }

private:
	bool m_bDeal;
	float m_fDeltaTime;				// ״̬�Ӵ��������ھ����˶೤ʱ��
	const ConfigSpriteSkillStatusItem* m_pCfgStatusItem;
};

#endif // _DATAPOOL_STATUS_DATA_H_