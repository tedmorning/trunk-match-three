#include "StatusData.h"

const float kDefaultStatusOverTime = 5.f;

StatusData::StatusData(const ConfigSpriteSkillStatusItem* pCfgStatusItem)
: m_bDeal(false)
, m_fDeltaTime(0.f)
, m_pCfgStatusItem(pCfgStatusItem)
{
	CCAssert(pCfgStatusItem, "");
}

StatusData::~StatusData()
{
}

StatusType StatusData::getStatusType() const
{
	if (m_pCfgStatusItem)
	{
		return (StatusType)m_pCfgStatusItem->type;
	}
	return ST_UNKNOWN;
}

bool StatusData::isOver() const
{
	if (m_pCfgStatusItem)
	{
		if (infinite())
		{
			return false;
		}
		return m_fDeltaTime >= (float)m_pCfgStatusItem->param1;
	}
	return m_fDeltaTime >= kDefaultStatusOverTime;
}

bool StatusData::infinite() const
{
	return m_pCfgStatusItem->param1 == -1;
}