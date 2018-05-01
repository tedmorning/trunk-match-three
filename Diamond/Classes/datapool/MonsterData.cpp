#include "MonsterData.h"

const float kDefaultWeight = 5.0f;
const float kDefaultMoveVelocity = 20.f;
const int kMonsterDefaultHp = 5;
const int kMonsterDefaultAggress = 1;
const float kMonsterDefaultAttackInterval = 2.f;
const int kMonsterDefaultPriority = 1;

MonsterData::MonsterData(const ConfigMonsterItem* pCfgItem)
: m_nHp(kMonsterDefaultHp)
, m_nId(0)
, m_nRowIndex(0)
, m_nBeginHp(0)
, m_fAttackDeltaTime(0.f)
, m_pCfgMonster(pCfgItem)
, m_eSituationType(SITUATION_MOVE)
, m_bAttackEnabled(true)
{
	CCAssert(pCfgItem, "");
	if (m_pCfgMonster)
	{
		m_nHp = m_pCfgMonster->hp;
		m_nBeginHp = m_pCfgMonster->hp;
		m_nBackHp = m_nHp;
	}
}

MonsterData::~MonsterData()
{
}

float MonsterData::getWeight() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->weight;
	}
	return kDefaultWeight;
}

float MonsterData::getMoveVelocity() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->speed;
	}
	return kDefaultMoveVelocity;
}

float MonsterData::getLaunchPosPercent() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->atk_pos;
	}
	return 1.f;
}

bool MonsterData::isBoss() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->boss;
	}
	return false;
}

int MonsterData::getAggress() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->aggression;
	}
	return kMonsterDefaultAggress;
}

int MonsterData::getBeSkillAttackedPriority()
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->priority;
	}
	return kMonsterDefaultPriority;
}

bool MonsterData::isOwnUser() const
{
	return isExsitStatus(ST_MIXED);
}

bool MonsterData::isDizzy() const
{
	return isExsitStatus(ST_DIZZY);
}

void MonsterData::addStatusDeltaTime(float delta)
{
	std::vector<StatusData*>::iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		(*iter)->addDeltaTime(delta);
	}
}

std::vector<StatusData*> MonsterData::getOverStatus()
{
	std::vector<StatusData*> vOverStatus;
	std::vector<StatusData*>::iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		if ((*iter)->isOver())
		{
			vOverStatus.push_back(*iter);
		}
	}
	return vOverStatus;
}

std::vector<StatusData*> MonsterData::getNewStatus()
{
	std::vector<StatusData*> vNewStatus;
	std::vector<StatusData*>::iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		if (!(*iter)->isDeal())
		{
			vNewStatus.push_back(*iter);
		}
	}
	return vNewStatus;
}

void MonsterData::clearOverStatus()
{
	std::vector<StatusData*>::iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); )
	{
		bool bNext = true;
		if ((*iter)->isOver())
		{
			bNext = false;

			iter = m_vStatus.erase(iter);
		}

		if (bNext)
		{
			++iter;
		}
	}
}

void MonsterData::addStatus(ConfigSpriteSkillStatusItem* pCfgItem)
{
	if (!isExsitStatus((StatusType)pCfgItem->type))
	{
		StatusData* status = new StatusData(pCfgItem);
		m_vStatus.push_back(status);
	}
}

u32 MonsterData::getSkillId() const
{
	if (m_pCfgMonster)
	{
		return m_pCfgMonster->skill_id;
	}
	return 0;
}

bool MonsterData::enableLaunch() const
{
	if (m_pCfgMonster)
	{
		return m_fAttackDeltaTime >= m_pCfgMonster->interval;
	}
	return m_fAttackDeltaTime >= kMonsterDefaultAttackInterval;
}

std::vector<std::string> MonsterData::getMoveAnimateFrames() // 获取移动动画纹理
{
	std::vector<std::string> vFrames;
	vFrames.push_back(m_pCfgMonster->texture);
	vFrames.push_back(m_pCfgMonster->texture2);
	vFrames.push_back(m_pCfgMonster->texture3);
	return vFrames;
}

std::vector<std::string> MonsterData::getAttackAnimateFrames() // 获取攻击动画纹理 
{
	std::vector<std::string> vFrames;
	vFrames.push_back(m_pCfgMonster->atk_tex1);
	vFrames.push_back(m_pCfgMonster->atk_tex2);
	if(m_pCfgMonster->atk_tex3 != "")
	{
		vFrames.push_back(m_pCfgMonster->atk_tex3);
	}
	return vFrames;
}

std::vector<std::string> MonsterData::getBeAttackedAnimateFrames()
{
	std::vector<std::string> vFrames;
	vFrames.push_back(m_pCfgMonster->be_atked_tex1);
	return vFrames;
}

std::vector<std::string> MonsterData::getRestAnimateFrames()
{
	std::vector<std::string> vFrames;
	vFrames.push_back(m_pCfgMonster->rest_tex1);
	vFrames.push_back(m_pCfgMonster->rest_tex2);
	return vFrames;
}

bool MonsterData::isExsitStatus(StatusType type) const
{
	std::vector<StatusData*>::const_iterator iter = m_vStatus.begin();
	for (; iter != m_vStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() == type)
		{
			return true;
		}
	}
	return false;
}