#include "SceneStatusData.h"

SceneStatusData::SceneStatusData()
: m_fAttackDeltaTime(0.f)
{
}

SceneStatusData::~SceneStatusData()
{
}

StatusType SceneStatusData::getStatusType() const
{
	StatusData* pData = getFirstStatusData();
	if (pData)
	{
		return pData->getStatusType();
	}
	return ST_UNKNOWN;
}

StatusData* SceneStatusData::getStatus(StatusType type) const
{
	std::vector<StatusData*>::const_iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() == type)
		{
			return *iter;
		}
	}
	return NULL;
}

bool SceneStatusData::isExistAttackStatus() const
{
	return !!getStatus(ST_DAMAGE);
}

bool SceneStatusData::enableAttack() const
{
	return m_fAttackDeltaTime >= 1.0f;
}

void SceneStatusData::launchAttack()
{
	m_fAttackDeltaTime = 0.f;
}

void SceneStatusData::addAttackDeltaTime(float time)
{
	m_fAttackDeltaTime += time;
}

void SceneStatusData::addStatusDeltaTime(float time)
{
	StatusData* pData = getFirstStatusData();
	if (pData)
	{
		pData->addDeltaTime(time);
	}
}

bool SceneStatusData::isOver()
{
	StatusData* pData = getFirstStatusData();
	if (pData)
	{
		return pData->isOver();
	}
	return true;
}

bool SceneStatusData::isDeal()
{
	StatusData* pData = getFirstStatusData();
	if (pData)
	{
		return pData->isDeal();
	}
	return true;
}

void SceneStatusData::setDeal(bool deal)
{
	StatusData* pData = getFirstStatusData();
	if (pData)
	{
		pData->setDeal(deal);
	}
}

StatusData* SceneStatusData::getFirstStatusData() const
{
	std::vector<StatusData*>::const_iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() != ST_DAMAGE)
		{
			return *iter;
		}
	}
	return NULL;
}