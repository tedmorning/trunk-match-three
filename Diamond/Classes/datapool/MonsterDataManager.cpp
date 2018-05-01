#include "MonsterDataManager.h"

MonsterDataManager::MonsterDataManager()
: m_fAppearTime(0.f)
, m_bComming(false)
{
}

MonsterDataManager::~MonsterDataManager()
{
}

bool MonsterDataManager::isOver() const
{
	std::vector<MonsterData*>::const_iterator iter = m_vMonsterData.begin();
	for (; iter != m_vMonsterData.end(); ++iter)
	{
		if (!(*iter)->isDead())
		{
			return false;
		}
	}
	return true;
}
